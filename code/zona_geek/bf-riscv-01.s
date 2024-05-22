	.text
	.global _start
	.equ MEM_SIZE, 4096
_start:
	# Lee datos desde entrada standard
	la a1, prog
	move t1, zero
bucle01:	
	li a7, 63
	li a0, 0
	li a2, 1
	ecall
	blez a0, cont01
	
	lb t0, 0(a1)
	add t1, t1, 1
	add a1, a1, 1
	j bucle01
cont01:
	# Muestra la cadena leida por pantalla
	li a7, 64
	la a1, prog
	move a2, t1
	ecall
	# a1 apunta al programa
	# t1 contiene longitud del programa
	
	
exit:
	li a7, 93
	# Exit	
	li a0, 0
	ecall
	
	.data
input:
	.fill 1024
	input_end = . -1
prog:
	.fill MEM_SIZE
data:
	.fill MEM_SIZE
	
