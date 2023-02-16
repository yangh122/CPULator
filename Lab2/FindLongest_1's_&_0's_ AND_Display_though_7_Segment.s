.global _start
_start:
	
	/* Program that counts consecutive 1's */

          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R3, #TEST_NUM   // load the data word ...
		  MOV     R4, #0
		  
LOOP1:	  LDR     R1, [R3]
		  ADD     R3,#4
		  CMP     R1, #0
		  BEQ     RE_SET1
		  MOV     R0, #0
		  BL      ZEROS
		  CMP     R4, R0
		  BGE     LOOP1
		  MOV	  R4, R0          // R4 will hold the biggest 1;
		  B       LOOP1
		  
RE_SET1:  MOV     R3, #TEST_NUM   // load the data word ...
		  MOV     R6, R4
		  MOV     R4, #0
		  
LOOP2:	  LDR     R1, [R3]
		  ADD     R3,#4
		  CMP     R1, #0
		  BEQ     RE_SET2
		  MOV     R0, #0
		  BL      ONES
		  CMP     R4, R0
		  BGE     LOOP2
		  MOV	  R4, R0          // R4 will hold the biggest 1;
		  B       LOOP2
		  
RE_SET2:  MOV     R3, #TEST_NUM   // load the data word ...
		  MOV     R5, R4
		  MOV     R4, #0
		  
LOOP3:    LDR     R1, [R3]
		  ADD     R3,#4
		  CMP     R1, #0
		  BEQ     DONE
		  MOV     R0, #0
		  BL      XOR
		  BL      ONES
		  CMP     R4, R0
		  BGE     LOOP3
		  MOV	  R4, R0          // R4 will hold the biggest 1;
		  B       LOOP3
		  
		  
		  
DONE:     ADD     R4, #1
          MOV     R7,R4
		  
DISPLAY:    MOV     R0, #0
            
			LDR     R8, =0xFF200020 // base address of HEX3-HEX0
            MOV     R2, R5          // display R5 on HEX1-0
            BL      DIVIDE10          // tens in R1
            BL      DIVIDE1          // ones digit will be in R0;               
            MOV     R9, R1          // save the tens digit
            BL      SEG7_CODE       
            MOV     R4, R0          // save bit code
            MOV     R0, R9          // retrieve the tens digit, get bit
                                    // code
            BL      SEG7_CODE       
            LSL     R0, #8
            ORR     R4, R0
			
			STR     R4, [R8] 
			
			
			
			
			MOV     R0, #0
			MOV     R1, #0
			MOV     R2, #0
            
			LDR     R8, =0xFF200020 // base address of HEX3-HEX0
            MOV     R2, R6          // display R5 on HEX1-0
            BL      DIVIDE10          // tens in R1
            BL      DIVIDE1          // ones digit will be in R0;               
            MOV     R9, R1          // save the tens digit
            BL      SEG7_CODE       
            MOV     R4, R0          // save bit code
            MOV     R0, R9          // retrieve the tens digit, get bit
                                    // code
            BL      SEG7_CODE       
            LSL     R0, #8
            ORR     R4, R0
			
			LSL     R4, #16
			LDR     R0, [R8]
			ORR     R4, R0
			STR     R4, [R8]

			
			
			
			
			
			
			MOV     R0, #0
			MOV     R1, #0
			MOV     R2, #0
            
			LDR     R8, =0xFF200030 // base address of HEX3-HEX0
            MOV     R2, R7          // display R5 on HEX1-0
            BL      DIVIDE10          // tens in R1
            BL      DIVIDE1          // ones digit will be in R0;               
            MOV     R9, R1          // save the tens digit
            BL      SEG7_CODE       
            MOV     R4, R0          // save bit code
            MOV     R0, R9          // retrieve the tens digit, get bit
                                    // code
            BL      SEG7_CODE       
            LSL     R0, #8
            ORR     R4, R0
			STR     R4, [R8] 
			
				  
		 	  
END:      B       END  



		  
		  



ZEROS:    CMP     R1, #4294967295         // loop until the data contains no more 1's
          BEQ     RETURN    
          LSL     R2, R1, #1      // perform SHIFT, followed by AND
		  ADD     R2,#1
          ORR     R1, R1, R2      
          ADD     R0, #1          // count the string length so far
          B       ZEROS 
		  
ONES:     CMP     R1, #0          // loop until the data contains no more 1's
          BEQ     RETURN             
          LSR     R2, R1, #1      // perform SHIFT, followed by AND
          AND     R1, R1, R2      
          ADD     R0, #1          // count the string length so far
          B       ONES 
		  
XOR:      ASR     R2, R1, #1      // perform SHIFT, followed by AND
          EOR     R1, R1, R2      
          B       RETURN 
		  		  



SEG7_CODE:  MOV     R1, #BIT_CODES  
            ADD     R1, R0         // index into the BIT_CODES "array"
            LDRB    R0, [R1]       // load the bit pattern (to be returned)
            MOV     PC, LR
			
 
DIVIDE10:   CMP    R2, #10
	        BLT    RETURN
            SUB    R2, #10
            ADD    R1, #1
            B      DIVIDE10
			
DIVIDE1:    CMP    R2, #1
	        BLT    RETURN
            SUB    R2, #1
            ADD    R0, #1
            B      DIVIDE1


RETURN:   MOV	  pc, lr






TEST_NUM: .word   0x12345678, 0x1234567c, 0x1234567e, 0x123456fe, 0x123457fe, 0xfffffeff, 0xfffffcff, 0xfffff8ff, 0xfffff0ff, 0xff09e0ff, 0x55555555, 0x00000001, 0  

BIT_CODES:.byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110 
          .byte 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
		  
		  .end