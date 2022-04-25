global avx_strcmp
section .text


; ! Using C x86 calling conventions
avx_strcmp:                             ; ? RAX  avx_strCMP(RDI, RSI)
            XOR R12, R12                ; ? MOV R12, 0
            MOV RAX, 0x00007fffffffde50

            SHR RAX, 44
            MOV R8, RDI

            SHR R8,  44
            CMP R8, RAX

            JL  loop2
loop1:
            MOV R13b, [RDI + R12]

            CMP byte [RDI + R12], 0
            JZ exit1

            CMP byte [RSI + R12], 0
            JZ exit1

            CMP R13b, byte [RSI + R12]
            JNZ exit1

            INC R12
            JMP loop1

exit1:
            SUB R13b, [RSI + R12]
            MOVSX RAX, R13b
            
            RET
loop2:
            MOV R13b, [RDI + R12]

            CMP byte [RDI + R12], 0
            JZ  exit2

            CMP byte [RSI + R12], 0
            JZ  exit2

            CMP R13b, byte [RSI + R12]
            JNZ exit2

            INC R12
            JMP loop2

exit2:
            CMP R13b, byte [RSI + R12]
            JL if_smaller
            JG if_bigger
if_equal:
            XOR RAX, RAX ; ? MOV RAX, 0
            RET
if_bigger:
            MOV RAX, 1
            RET
if_smaller:
            MOV RAX, -1
            RET