       ;
       ; TEST OF 40 COLUMN MODE
       ;

.segment "CODE"
       ; clear the screen
       ldx #0
:      lda #32
       sta $0400,x
       sta $0500,x
       sta $0600,x
       sta $0700,x
       dex
       bne :-
       ; set text color to white
       ldx #0
:      lda #1
       sta $D800,x
       sta $D900,x
       sta $DA00,x
       sta $DB00,x
       dex
       bne :-
       ; set characters to a-z
       ldx #1
       ldy #0
       lda #1
       sta $0400
:      lda $0400, y
       sta $0400, x
       inc $0400, x
       inx
       iny
       lda $0400, y
       cmp #26
       bne :-
       ; set characters to a-z
       lda #1
       iny
       inx
       sta $0400, y
:      lda $0400, y
       sta $0400, x
       inc $0400, x
       inx
       iny
       lda $0400, y
       cmp #26
       bne :-
       ; set character at end of screen RAM
       lda #0
       sta $07e7
