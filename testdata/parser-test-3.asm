Code:
org 0100h
mvi e, -121 ; 87h
mvi a, 89 ; 59h
call bmult ; hl=e*a
hlt

bmult: mov d, a
mvi b, 00h
mov a, e
ani 80h
jm m3 ; jump minus
jmp m7 ; jump
m3: mov a, e ; a=e
cma ; invert a
inr a ; a=a+1
mov e, a ; e=a
mov a, b ; a=b
xri 01h ; a=a xor 1
mov b, a ; b=a

m7: mov a, d ; a=d
ani 80h ;
jm m4
jmp m8
m4: mov a, d ; a=d
cma ; invert a
inr a ; a=a+1
mov d, a ; d=a
mov a, b ; a=b
xri 01h
mov b, a
m8: mov a, d

m5: lxi h, 00h
mvi d, 00h ; d=0
mvi c, 08h ; c=8
m1: dad h
rlc
jnc m2
dad d
m2: dcr c ; c=c-1
jnz m1

mov a, b
cpi 00h
jz m6
mov a, h
cma
mov h, a
mov a, l
cma
mov l, a
inx h
m6: ret