; ================================
; Microprocessor system for digital signal processing (ADC -> differential equation -> DAC)
; Copyright (c) Yuriy Guts, 2008
; ================================

; ---------------------------
; RST 7 interrupt handler
; ---------------------------
Interrupt7Handler:

    ; Remembering the registers in the stack
    push    psw
    push    b
    push    d
    push    h

    ; Disabling interrupts
    di

    ; Reading Xn from ADC
    in      64h
    mov     l, a
    in      65h
    mov     h, a

    ; Loading the read data into memory
    shld    0305h

    ; Restoring registers
    pop     h
    pop     d
    pop     b
    pop     psw

    ; Interrupt return
    ret

; ------------------------------
; MPS initialization subroutine
; ------------------------------
Initialize:
    org     0100h

    ; Initializing stack pointer
    lxi     sp, 03ffh

    ; Loading the control word into Parallel Interface 1
    mvi     a, 10110110b
    out     67h

    ; Loading the control word into Parallel Interface 2
    mvi     a, 10100100b
    out     77h

    ; Proceeding to the main program
    jmp     Main


; -----------------
; Main program
; -----------------
Main:
    ; Resetting the iteration counter
    mvi     a, 0
    sta     030dh

Main_PerformNextIteration:
    ; Generating the control signal for ADC
    mvi     a, 1
    out     66h
    hlt

    ; Calculating the expression according to received Xn
    call    CalculateExpression
    ; Output the result to DAC
    call    OutputResult

    ; Incrementing the iteration counter
    lda     030dh
    inr     a
    sta     030dh

    ; Checking if the required sample size is reached
    cpi     0ffh

    ; Yes - shutting down
    jnz     Main_StopProcessing

    ; No - allowing interrupts and proceeding to the next iteration
    ei
    jmp     Main_PerformNextIteration

Main_StopProcessing:
    hlt

; ----------------------------------------
; Expression calculation subroutine
;   Result is loaded to 0309,030ah
; ----------------------------------------
CalculateExpression:
    ; Loading Xn from memory
    lhld    0305h
    ; 1st multiplier (X_n)
    mov     d, h
    mov     e, l
    ; 2nd multiplier ('a' coefficient)
    lda     0300h
    mov     b, a
    ; Multiplying the numbers
    call    MultiplySignedBin12
    ; Pushing a * X_n to stack
    push    h
    
    ; Loading X_n-1 from memory
    lhld    0307h
    ; 1st multiplier (X_n-1)
    mov     d, h
    mov     e, l
    ; 2nd multiplier ('b' coefficient)
    lda     0301h
    mov     b, a
    ; Multiplying the numbers
    call    MultiplySignedBin12
    ; Pushing b * X_n-1 to stack
    push    h

    ; Loading Y_n-1 from memory
    lhld    030bh
    ; 1st multiplier (Y_n-1)
    mov     d, h
    mov     e, l
    ; 2nd multiplier ('c' coefficient)
    lda 0302h
    mov     b, a
    ; Multiplying the numbers
    call    MultiplySignedBin12
    ; Pushing c * Y_n-1 to stack
    push    h

    ; Adding (b * X_n-1) + (c * Y_n-1)
    pop     d
    pop     b
    call    AddSignedBin12
    ; Remembering the result in stack
    push    h

    ; Adding (a * X_n) + (b * X_n-1) + (c * Y_n-1)
    pop     d
    pop     b
    call    AddSignedBin12

    ; Loading the calculated value of Y_n into memory
    shld    0309h
    ret

; ----------------------------------------
; DAC output subroutine
; ----------------------------------------
OutputResult:
    ; Sending the result bytes to Parallel Interface 2
    lda     0309h
    out     74h
    lda     030ah
    out     75h
    ret

; ----------------------------------------------------
; Підпрограма додавання двох 12-бітних знакових чисел 
;    Результат: HL = BС + DE
; ----------------------------------------------------
AddSignedBin12:
    mov     h, d
    mov     e, l
    dad     b
    mov     a, h
    ani     00010000b
    cpi     00010000b
    jnz     AddSignedBin12_Finalize
    stc
AddSignedBin12_Finalize:
    mov     a, h
    ani     00001111b   
    mov     h, a
    ret

; -------------------------------------------------------------------
; 12-bit signed * 8-bit signed multiplication subroutine
;    Result: HL = B * DE
; -------------------------------------------------------------------
MultiplySignedBin12:
    push    psw
    push    b
    push    d

    ; Initializing the temp variable
    mvi     a, 0
    sta     030eh

    ; Checking the sign of the 8-bit argument
    mov     a, b
    ani     10000000b
    cpi     10000000b
    ; Negative - remembering the sign in 030eh
    jz      MultiplySignedBin12_RememberSign
    jmp     MultiplySignedBin12_Begin

MultiplySignedBin12_RememberSign:
    mvi     a, 1
    sta     030eh
    dcr     a
    cma

MultiplySignedBin12_Begin:
    mov     b, a
    
    ; Initializing the result register pair
    mvi     h, 0
    mvi     l, 0

MultiplySignedBin12_NextIteration:
    ; Accumulating the result
    dad     d
    dcr     b
    ; Checking the loop termination condition
    mov     a, b
    cpi     0
    jz      MultiplySignedBin12_EndMultiply
    jmp     MultiplySignedBin12_NextIteration

MultiplySignedBin12_EndMultiply:
    ; Checking the sign remembered earlier
    lda     030eh
    cpi     a, 1
    ; Negative - flipping the sign of multiplication result
    jz      MultiplySignedBin12_InvertSign
    jmp     MultiplySignedBin12_Finalize

MultiplySignedBin12_InvertSign:
    mov     a, h
    cma
    mov     h, a
    mov     a, l
    cma
    mov     l, a
    inx     h

MultiplySignedBin12_Finalize:
    ; Adjusting the result to 12-bit form
    mov     a, h
    ani     00001111b
    mov     h, a
    ; Restoring the registers
    pop     d
    pop     b
    pop     psw
    ret
