global my_strcmp

section .text

; ! Using x86 calling conventions (cdecl), first 2 parameters are passed as RDI and RSI
; todo: save and restore R12, which is preserver register
my_strcmp: ; ? RAX my_strcmp(RDI, RSI)
                xor R12, R12  
cmp_loop:
                ; * 'B in the end of register name means ONE BYTE (8 bits) 
                mov R13B, [RDI + R12]
                cmp byte [RDI + R12], 0 ; < checking for end of string (\0)
                jz return

                cmp byte [RSI + R12], 0 ; < again \0 of string 2 check 
                jz return

                cmp R13B, byte [RSI + R12]
                jnz return ; * jnz works if R13B != byte[RSI + R12]

                inc R12  ; ? move to the next symbol 
                jmp cmp_loop ; repeat loop

return:
                SUB R13B, [RSI + R12] ; ? get the answer, for example: 'a' - 's'
                MOVSX RAX, R13B ; ! Using MOVSX allows to keep answer signed, which is important for 'strcmp'
                ret 