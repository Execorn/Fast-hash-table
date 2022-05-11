#define label(label_name) label_name:;

#define jump goto;

#define checkpoint fprintf(stderr, "CHECKPOINT REACHED @ %s:%i\n", __FILE__, __LINE__);

static const unsigned long long DEFAULT_HT_CAPACITY = 100;
// ! 32 bytes = 256 bits, which is required for AVX2 instructions to work properly 
static const unsigned long long ALIGNED_TOKEN_SIZE  =  32; 
