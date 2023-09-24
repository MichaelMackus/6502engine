.segment "ZEROPAGE"

zp1: .res 2

.segment "CODE"

    lda #1         ; 1  = file number
    ldx #8         ; 8  = disk drive
    ldy #$FF       ; FF = no command
    jsr $FFBA      ; SETLFS
    lda #8         ; filename size
    ldx #<filename ; low byte of addr
    ldy #>filename ; high byte of addr
    jsr $FFBD      ; SETNAM
    lda #<start
    sta zp1
    lda #>start
    sta zp1+1
    ldx #<end
    ldy #>end
    lda #<zp1
    jsr $FFD8      ; SAVE

    rts

filename: .asciiz "@0:hello"

start: .byte 42
end:
