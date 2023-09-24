       ;
       ; TEST OF 80 COLUMN MODE
       ; NOTE: this doesn't work but is an example of something that
       ; could be cool if it was ever implemented
       ;

       ; set custom status flag
       lda #%00000001
       sta $02
       ; clear the screen
       ldx #0
:      lda #32
       sta $0400,x
       sta $0500,x
       sta $0600,x
       sta $0700,x
       dex
       bne :-
       ; set characters in entire screen memory
       ldx #1
       ldy #0
       lda #1
       sta $0400
:      lda $0400, y
       sta $0400, x
       inc $0400, x
       inx
       iny
       cpx #$FF
       beq :+
       lda $0400, y
       cmp #26
       bne :-
       lda #1
       iny
       inx
       sta $0400, y
       cpx #$FF
       bne :-
:
       ; set characters to a-z
       ldx #1
       ldy #0
       lda #1
       iny
       inx
       sta $0800, y
:      lda $0800, y
       sta $0800, x
       inc $0800, x
       inx
       iny
       cpx #$FF
       beq :+
       lda $0800, y
       cmp #26
       bne :-
       lda #1
       iny
       inx
       sta $0800, y
       cpx #$FF
       bne :-
:
       ; set character at end of screen RAM
       lda #0
       sta $0FE7
