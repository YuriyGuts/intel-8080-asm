org     100h
lxi     sp, 160h
mvi     b, 09h
mvi     a, 08h

mvi     h, 01h
mvi     l, 70h

NextIteration:
ana     a
jz      EndLoop

push    psw
ani     01h
cpi     01h
jz      odd
jmp     PrepareNextIteration

Odd:
mov     m, b
inx     h

PrepareNextIteration:
mov     a, b
ral
ani     0feh
mov     b, a
pop     psw
rrc
ani     07fh
jmp     NextIteration


EndLoop:
dcx     h
xra     a

ReadNextPartialSum:
push    psw
mov     a, l
cpi     6fh
jz      Finalize
pop     psw
mov     b, m
add     b
dcr     l
jmp     ReadNextPartialSum

Finalize:
pop     psw
mvi     h, 0
mov     l, a

nop
hlt