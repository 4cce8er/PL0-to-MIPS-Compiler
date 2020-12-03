	.data
var_m:	.word	0
var_n:	.word	0
var_r:	.word	0
var_q:	.word	0
	.text
	.globl main
proc_gcd:
label_0:
	lw $t0, var_r
	sw $t0, ($sp)
	addi $sp, $sp, 4
	li $t0, 0
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	lw $t1, -8($sp)
	addi $sp, $sp, -8
	beq $t1, $t0, label_1
	lw $t0, var_m
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, var_n
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	lw $t1, -8($sp)
	div $t1, $t0
	mflo $t2
	addi $sp, $sp, -4
	sw $t2, -4($sp)
	lw $t0, -4($sp)
	sw $t0, var_q
	addi $sp, $sp, -4
	lw $t0, var_m
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, var_q
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, var_n
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	lw $t1, -8($sp)
	mult $t1, $t0
	mflo $t2
	addi $sp, $sp, -4
	sw $t2, -4($sp)
	lw $t0, -4($sp)
	lw $t1, -8($sp)
	sub $t2, $t1, $t0
	addi $sp, $sp, -4
	sw $t2, -4($sp)
	lw $t0, -4($sp)
	sw $t0, var_r
	addi $sp, $sp, -4
	lw $t0, var_n
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	sw $t0, var_m
	addi $sp, $sp, -4
	lw $t0, var_r
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	sw $t0, var_n
	addi $sp, $sp, -4
	j label_0
label_1:
	jr $ra
main:
label_2:
	li $t0, 1
	sw $t0, ($sp)
	addi $sp, $sp, 4
	li $t0, 1
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	lw $t1, -8($sp)
	addi $sp, $sp, -8
	bne $t1, $t0, label_3
	li $v0, 5
	syscall
	sw $v0, var_m
	li $v0, 5
	syscall
	sw $v0, var_n
	lw $t0, var_m
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, var_n
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	lw $t1, -8($sp)
	addi $sp, $sp, -8
	bge $t1, $t0, label_4
	lw $t0, var_m
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	sw $t0, var_r
	addi $sp, $sp, -4
	lw $t0, var_n
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	sw $t0, var_m
	addi $sp, $sp, -4
	lw $t0, var_r
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	sw $t0, var_n
	addi $sp, $sp, -4
label_4:
	li $t0, 1
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	sw $t0, var_r
	addi $sp, $sp, -4
	sw $ra, ($sp)
	addi $sp, $sp, 4
	jal proc_gcd
	lw $ra, -4($sp)
	addi $sp, $sp, -4
	lw $t0, var_m
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	move $a0, $t0
	addi $sp, $sp, -4
	li $v0, 1
	syscall
	j label_2
label_3:
	jr $ra
