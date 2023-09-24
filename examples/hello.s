;*********************************************************
;* HELLOWORLD using .BYTE, CHROUT and loops	         *
;*********************************************************

;set up some helpful labels
CHROUT = $FFD2
SETLFS = $FFBA
SETNAM = $FFBD
OPEN   = $FFC0
CLRCHN = $FFCC

        ; TODO the following doesn't work when STARTADDRESS = $0801
        LDA MSG+2
        JSR CHROUT

		LDX	#0	  ;put 0 in X
		
        ; TODO this works...
        ; LDA MSG
        ; JSR CHROUT
		; LDX	#0	  ;put 0 in X

READ:
        LDA MSG,X	;put MSG[X] in A
		BEQ END		;end if true
		
		JSR CHROUT 	;output A
		
		INX
		JMP READ	;loop


END:
        RTS

MSG:
        .asciiz "hello world!"
