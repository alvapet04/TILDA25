# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# Choose the macro syntax for RARS or DTEK-V board.         #
# The syntax for RARS is probably due to its MIPS heritage. #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp) 
#.end_macro

#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp) 
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x0000
timstr:	.asciz 	"text more text lots of text\0"
	.text
	.globl timetemplate, tick, time2string

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 1000		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hex2asc, delay.#
#########################################################

  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	.globl hex2asc

hex2asc:
	li	a0, 12		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
hexasc:
	andi 	a0,a0, 0xF
	
	addi	a0, a0, 0x30
	li 	t1, 0x39
	
	bgt	a0, t1, is_letter
	jr	ra
	
is_letter:	
	addi	a0, a0, 0x07
	li	t1, 0x46
	jr	ra



time2string:
	PUSH 	ra
	PUSH	s0
	PUSH	s1
	PUSH	s2
	PUSH	s3
	PUSH	a0
	
	add	a0, zero, a1
	jal	hexasc 
	add	s0, zero, a0 # borde bli sista siffran 8
	
	srli	a0, a1, 4
	jal	hexasc 
	add	s1, zero, a0 # borde bli näst sista siffran 5
	
	srli	a0, a1, 8
	jal	hexasc 
	add	s2, zero, a0 # borde bli näst första siffran 9
	
	srli	a0, a1, 12
	jal	hexasc 
	add	s3, zero, a0 # borde bli fösta siffran 5
	
	addi	t0, zero, 0x3A
	
	POP	a0
	
	sb	s3, 0(a0)
	sb	s2, 1(a0)
	sb	t0, 2(a0)
	sb	s1, 3(a0)
	sb	s0, 4(a0)
	sb	zero, 5(a0)
		
	
	POP	s3
	POP	s2
	POP	s1
	POP	s0
	POP	ra
	
	jr	ra
	
delay:
	addi	a0, a0, -1
	
	addi	a1, zero, 0

for_loop:
	addi	a1, a1, 1
	li 	t0, 8000
	
	blt	a1, t0, for_loop
	
	bgt	a0, zero, delay
	
	
	
	jr ra
	
