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



