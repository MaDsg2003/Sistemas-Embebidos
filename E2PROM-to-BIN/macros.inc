
;*****************************************
.MACRO INICIA_PILA
   	LDI 	R25,LOW(ramend)
	OUT 	SPL,R25
	LDI 	R25,HIGH(ramend)
	OUT 	SPH,R25
.ENDMACRO
;*****************************************
.MACRO SETX
	LDI		XL,LOW(@0)
	LDI		XH,HIGH(@0)
.ENDMACRO
;******************************************
.MACRO SETY
	LDI		YL,LOW(@0)
	LDI		YH,HIGH(@0)
.ENDMACRO
;******************************************
.MACRO SETZ
	LDI		ZL,LOW(@0)
	LDI		ZH,HIGH(@0)
.ENDMACRO	




//**************************************************
; INR  Rn,[RAM] ************************************
//**************************************************
.MACRO INR
	.if(@1<=0x3F)
		IN		@0,@1	
	.else 
		LDS		@0,@1	
	.endif
.ENDMACRO
//**************************************************
; OUTR [RAM],Rn ************************************
//**************************************************
.MACRO OUTR
	.if(@0<=0x3F)
		OUT		@0,@1	
	.else 
		STS		@0,@1	
	.endif
.ENDMACRO
//**************************************************
; SKBRS	[RAM],bit **********************************
//**************************************************
.MACRO SKBS
	.if((@1<=7)&&(@1>=0))
		.if(@0>0x3F)
			LDS		R8,@0
			SBRS	R8,@1	
		.elif(@0>0x1F) 
			IN		R8,@0
			SBRS	R8,@1
		.else
			SBIS		@0,@1		
		.endif
	.else
		.error"Bit debe estar [0-7]"
 	.endif
.ENDMACRO
//**************************************************
; SKBRC	[RAM],bit **********************************
//**************************************************
.MACRO SKBC
	.if((@1<=7)&&(@1>=0))
		.if(@0>0x3F)
			LDS		R8,@0
			SBRC	R8,@1	
		.elif(@0>0x1F) 
			IN		R8,@0
			SBRC	R8,@1
		.else
			SBIC		@0,@1		
		.endif
	.else
		.error"Bit debe estar [0-7]"
 	.endif
.ENDMACRO
//**************************************************
; SB	[RAM],bit **********************************
//**************************************************
.MACRO SB
	.if((@1<=7)&&(@1>=0))
		.if(@0>0x3F)
			LDS		R25,@0
			SBR		R25,(1<<@1)
			STS		@0,R25	
		.elif(@0>0x1F) 
			IN		R25,@0
			SBR		R25,(1<<@1)
			OUT		@0,R25
		.else
			SBI		@0,@1		
		.endif
	.else
		.error"Bit debe estar [0-7]"
 	.endif
.ENDMACRO
//**************************************************
; CB	[RAM],bit **********************************
//**************************************************
.MACRO CB
.if((@1<=7)&&(@1>=0))
		.if(@0>0x3F)
			LDS		R25,@0
			CBR		R25,(1<<@1)
			STS		@0,R25
		.elif(@0>0x1F) 
			IN		R25,@0
			CBR		R25,(1<<@1)
			OUT		@0,R25
		.else
			CBI		@0,@1
		.endif
	.else
		.error"Bit debe estar [0-7]"
 	.endif
.ENDMACRO
//**************************************************
; WBS	[RAM],bit **********************************
//**************************************************
.MACRO WBS	// ESPERA A QUE BIT SEA 1
	.if((@1<=7)&&(@1>=0))
		.if(@0>0x3F)
			LDS		R9,@0
			SBRS	R9,@1
			RJMP	PC-3	
		.elif(@0>0x1F)  
			IN		R9,@0
			SBRS	R9,@1
			RJMP	PC-2
		.else
			SBIS	@0,@1		
			RJMP	PC-1
		.endif
	.else
		.error"Bit debe estar [0-7]"
 	.endif
.ENDMACRO
//**************************************************
; WBS	[RAM],bit **********************************
//**************************************************
.MACRO WBC	// ESPERA A QUE BIT SEA 0
	.if((@1<=7)&&(@1>=0))
		.if(@0>0x3F)
			LDS		R8,@0
			SBRC	R8,@1
			RJMP	PC-3	
		.elif(@0>0x1F)  
			IN		R8,@0
			SBRC	R8,@1
			RJMP	PC-2
		.else
			SBIC	@0,@1		
			RJMP	PC-1
		.endif
	.else
		.error"Bit debe estar [0-7]"
 	.endif
.ENDMACRO
