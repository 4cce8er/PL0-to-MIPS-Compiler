	.data
var_f1:	.word	0
var_f2:	.word	0
var_f:	.word	0
var_i:	.word	0
	.text
	.globl main
main:
	li $t0, 1
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	sw $t0, var_f1
	addi $sp, $sp, -4
	li $t0, 1
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	sw $t0, var_f2
	addi $sp, $sp, -4
	li $t0, 1
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	sw $t0, var_i
	addi $sp, $sp, -4
label_0:
	lw $t0, var_i
	sw $t0, ($sp)
	addi $sp, $sp, 4
	li $t0, 20
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	lw $t1, -8($sp)
	addi $sp, $sp, -8
	bgt $t1, $t0, label_1
	lw $t0, var_f1
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	move $a0, $t0
	addi $sp, $sp, -4
	li $v0, 1
	syscall
	lw $t0, var_f1
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, var_f2
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	lw $t1, -8($sp)
	add $t2, $t1, $t0
	addi $sp, $sp, -4
	sw $t2, -4($sp)
	lw $t0, -4($sp)
	sw $t0, var_f
	addi $sp, $sp, -4
	lw $t0, var_f2
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	sw $t0, var_f1
	addi $sp, $sp, -4
	lw $t0, var_f
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	sw $t0, var_f2
	addi $sp, $sp, -4
	lw $t0, var_i
	sw $t0, ($sp)
	addi $sp, $sp, 4
	li $t0, 1
	sw $t0, ($sp)
	addi $sp, $sp, 4
	lw $t0, -4($sp)
	lw $t1, -8($sp)
	add $t2, $t1, $t0
	addi $sp, $sp, -4
	sw $t2, -4($sp)
	lw $t0, -4($sp)
	sw $t0, var_i
	addi $sp, $sp, -4
	j label_0
label_1:
	jr $ra
