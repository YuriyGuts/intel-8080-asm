org     100h
lxi     sp, 0150h
lxi     b, 0b0ch
push    b
lxi     b, 0d0eh
push    b
lxi     b, 0f10h
push    b
mvi     b, 03h
mvi     h, 01h
mvi     l, 80h

analyzepair:
pop     d
mov     a, e
ani     10h
cpi     10h
jz      addvalues
jmp     subtractvalues

addvalues:
mov     a, e
add     d
mov     m, a
jmp     checkloop

subtractvalues:
mov     a, e
sub     d
mov     m, a

checkloop:
inr     l
dcr     b
jnz     analyzepair

lda     0180h
mov     c, a
lda     0181h
mov     h, a
lda     0182h
mov     l, a
hlt
