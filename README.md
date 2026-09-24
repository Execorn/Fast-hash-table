# Fast-hash-table

A cache-aligned closed-addressing hash table in C optimized with 256-bit AVX2 SIMD vector instructions and hardware-accelerated CRC32 intrinsics. The engine processes 5,659,100 token lookups across a 30.8 MB corpus (Leo Tolstoy's *War and Peace*) in 3.5 seconds, achieving a 2.5x overall runtime reduction and 1.8x instruction reduction over standard library implementations.

*Note: The original Russian academic research report is preserved in [README.MD](README.MD).*

## Microarchitectural Design

```
 Input String Key (32-Byte Aligned Buffer)
                   │
                   ▼
 ┌────────────────────────────────────────────────────────┐
 │ Hardware CRC32 Hash Calculation                        │
 │ • 4 iterations of _mm_crc32_u64 across 8-byte chunks   │
 │ • Unrolled loop: 32 bytes hashed in <10 CPU cycles     │
 └─────────────────────────┬──────────────────────────────┘
                           │
                           ▼ Bucket Index: hash % capacity
 ┌────────────────────────────────────────────────────────┐
 │ Bucket Array Lookup (Closed Addressing Chaining)       │
 │ • Capacity scaled by Golden Ratio phi ≈ 1.618          │
 │ • Fill factor threshold = 0.72                         │
 └─────────────────────────┬──────────────────────────────┘
                           │
                           ▼
 ┌────────────────────────────────────────────────────────┐
 │ Linked List Node Traversal                             │
 │ • For each node: compare search key against node key   │
 └─────────────────────────┬──────────────────────────────┘
                           │
                           ▼
 ┌────────────────────────────────────────────────────────┐
 │ 256-Bit AVX2 Vector Key Comparison (avx_strcmp)       │
 │ 1. _mm256_load_si256   -> Load 32 bytes from key A     │
 │ 2. _mm256_load_si256   -> Load 32 bytes from key B     │
 │ 3. _mm256_cmpeq_epi8   -> Parallel byte equality test  │
 │ 4. _mm256_movemask_epi8-> Extract 32-bit bitmask       │
 │ 5. return ~mask        -> 0 if all 32 bytes match      │
 └────────────────────────────────────────────────────────┘
```

### 1. Vectorized String Equality (`avx_strcmp`)

Standard libc `strcmp` iterates character by character with data-dependent branches, which triggers branch mispredictions on short string lookups.

By padding word buffers to 32 bytes and enforcing 32-byte alignment (`__attribute__((aligned(32)))`), key comparisons execute in a single AVX2 vector operation in `src/asm_optimise/types_config.h`:

```c
static inline int avx_strcmp(const char* a, const char* b) {
    __m256i first   = _mm256_load_si256((const __m256i*) a);
    __m256i second  = _mm256_load_si256((const __m256i*) b);
    __m256i compare = _mm256_cmpeq_epi8(first, second);
    int mask        = _mm256_movemask_epi8(compare);
    return ~mask; // 0 if all 32 bytes match
}
```

`_mm256_cmpeq_epi8` compares all 32 bytes simultaneously. `_mm256_movemask_epi8` collapses the comparison result into a 32-bit integer mask. If all bytes match, the mask equals `0xFFFFFFFF`, and `~mask` evaluates to 0. This replaces loops and pointer chasing with 4 vector instructions.

### 2. Hardware-Accelerated CRC32 Hashing

Software polynomial hash routines require dozens of arithmetic operations per word. The default hash implementation (`HASH_6` in `src/asm_optimise/hash_table.c`) uses the Intel SSE4.2 hardware CRC32 instruction:

```c
uint64_t hash = 0;
for (unsigned i = 0; i < 4; ++i) {
    hash += _mm_crc32_u64(hash, *((const uint64_t*) key));
    key += 8;
}
```

Because every key buffer occupies exactly 32 bytes, the loop unrolls into four 64-bit hardware CRC32 calculations without residual byte handling.

### 3. Sizing Constants & Golden Ratio Growth

The hash table structure uses closed addressing with singly linked list chains (`node_t`):

- **Fill Factor**: Set to `0.72f`. This matches the default load factor in C# `Dictionary` and balances table density against bucket chain length.
- **Growth Factor**: Set to $\phi \approx 1.6180339887$ (Golden Ratio: $1 + \frac{\sqrt{5}-1}{2}$). Expanding capacity by $\phi$ rather than $2.0$ allows memory allocators to reuse previously freed memory blocks from earlier resize cycles, minimizing heap fragmentation.

### 4. Hash Collision Distribution Analysis

Six hash algorithms were benchmarked across the vocabulary of *War and Peace* (29,954 unique words distributed across 2,500 test buckets):

| Algorithm | Implementation Mechanism | Collision Distribution | Maximum Bucket Chain |
|---|---|---|---|
| `HASH_1` | Constant return `1` | Degenerate $O(N)$ single bucket | 29,954 |
| `HASH_2` | ASCII code of first character `s[0]` | Severe clustering around letters | ~2,500 |
| `HASH_3` | Inline assembly string length `strlen` | Bounded by word length ($\le 22$) | ~3,200 |
| `HASH_4` | Sum of ASCII character values | Bell curve clustering in mid-range | ~800 |
| `HASH_5` | Bitwise rotate polynomial: $H_i = \text{rol}(H_{i-1}) + s[i]$ | Uniform spread across buckets | $\le 40$ |
| `HASH_6` | Hardware CRC32 (`_mm_crc32_u64`) | Uniform distribution, lowest variance | $\le 40$ |

`HASH_6` generates the most uniform bucket distribution, keeping average chain length below 20 elements on production table configurations (~21,000 buckets).

## Empirical Benchmark & Profiling Results

### Workload Specification

- **Corpus**: Leo Tolstoy's *War and Peace* (`war_and_peace.txt`), 30.8 MB text file.
- **Total Scanned Token Usages**: 5,659,100 word operations.
- **Unique Vocabulary Entries**: 29,954 words inserted and tracked.
- **Bucket Count**: ~21,000 linked list heads.

### Callgrind Profiling Comparison

Profiling via Valgrind Callgrind measured instruction counts and relative CPU time across optimization phases:

| Optimization Phase | Bottleneck Function | Optimization Applied | Speedup Effect |
|---|---|---|---|
| **Baseline (Unoptimized)** | Software hash function | Naive byte-by-byte hash | CRC32 computation consumed 52% of total execution cycles |
| **Phase 1: Hardware Hash** | Hash calculation | Unrolled `_mm_crc32_u64` | Hash calculation time dropped by an order of magnitude |
| **Phase 2: SIMD String Compare** | String equality | 256-bit AVX2 `avx_strcmp` | String comparison time dropped by an order of magnitude |
| **Phase 3: Macro Guard Stripping** | Assert checks | Disabled runtime type asserts | Minimal latency impact (<2%) |
| **Final Profile** | Bucket traversal | Hardware CRC32 + AVX2 compare | **2.5x total program speedup**, execution time dropped to 43% of baseline |

After Phase 2, string equality checks and hash evaluations accounted for less than 15% of total runtime, shifting the remaining cost to memory access latency during linked list pointer dereferences.

## Build and Run on Arch Linux

### Prerequisites

```bash
sudo pacman -S gcc valgrind make
```

Ensure the host CPU supports AVX2 and SSE4.2 instructions:

```bash
grep -E '(avx2|sse4_2)' /proc/cpuinfo
```

### Compilation

```bash
mkdir -p build

gcc -mavx2 -O3 -ggdb3 \
    src/asm_optimise/hash_table.c \
    src/asm_optimise/linked_list.c \
    src/asm_optimise/ht_test_asm_optimize.c \
    -o build/ht_test
```

### Running the Benchmark

```bash
./build/ht_test txt_files/war_and_peace.txt /tmp/ht_output.txt
```

Expected terminal output:

```text
Scanned 5659100 usages.
Printed 29954 nodes to '/tmp/ht_output.txt'.
```

### Profiling with Callgrind

```bash
valgrind --tool=callgrind --callgrind-out-file=/tmp/callgrind.out \
    ./build/ht_test txt_files/war_and_peace.txt /tmp/ht_output.txt

callgrind_annotate /tmp/callgrind.out | head -n 30
```

## Engineering Trade-offs

- **Closed Addressing (Chaining) vs Open Addressing (SwissTable / Robin Hood)**: Chaining with singly linked lists allows node pointers to remain stable during resizes, avoiding tombstone markers and large contiguous reallocations. But traversing linked list nodes requires pointer chasing across heap memory, which causes cache misses compared to flat open-addressing arrays like SwissTable.
- **Fixed 32-Byte Padding vs Variable-Length Strings**: Allocating fixed 32-byte buffers simplifies vectorization by allowing unconditional 256-bit aligned loads (`_mm256_load_si256`) without bounds checking. And because natural vocabulary words in English and Russian rarely exceed 22 characters, the memory overhead per node remains small (32 bytes). For arbitrary documents containing 500-character tokens, this design would require an external overflow table.
