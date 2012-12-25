org     100h
lxi     sp, 150h
lxi     b, 2524h
push    b
lxi     b, 2322h
push    b
lxi     b, 2100h
push    b
pop     d
mov     a, d
pop     d
add     e
sta     0170h
add     d
sta     0171h
pop     d
sub     e
sta     0172h
sub     d
sta     0173h
rar
sta     0174h
ani     07fh
rar
sta     0175h
cpi     0
hlt
