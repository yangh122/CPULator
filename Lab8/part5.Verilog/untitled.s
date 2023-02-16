.define LED_ADDRESS 0X10
.define SWITCH_ADDRESS 0X30

MAIN: 
	  mv  r2, #0	  
	  mv r3, #0x30
	  mvt r4,#SWITCH_ADDRESS
	  mvt r0,#LED_ADDRESS
	  mvt r5, #0x01
	  mv  r6, #0xFF
	  add r5,r6
	  
CHECK_COUNTER:
	 mv r1, r2
	 sub  r1,r5
	 beq  RESET
COUNTER:
	 sub  r3, #1 
	 beq  ADD_COUNTER
	 b	  COUNTER

ADD_COUNTER:
	 add r2, #1
	 //MoV  R3, #DATA
	 mv  r3, #0x30
	 st  r2, [r0]
CHECK_CHANGE:
	 ld   r1, [r4] 
	 and  r1, #0X2 
	 mv   r6, r1
	 sub  r6, #1         
	 bpl  INCREASE
         
	 ld   r1, [r4] 
	 and  r1,#0X1  
	 mv   r6, r1
	 sub  r6, #1
	 beq  DECREASE
	 b    CHECK_COUNTER
RESET:
	mv r2,#0
	b  CHECK_COUNTER

INCREASE:
	add r3, #0x20
	b  CHECK_COUNTER
DECREASE:
	sub r3, #0x20
	b  CHECK_COUNTER

