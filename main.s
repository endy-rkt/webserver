.intel_syntax noprefix
.global _start

.section .data
	

.section .text

_start:
	create_socket:
		mov rdi, 2
		mov rsi, 1
		mov rdx, 0
		mov rax, 41
		syscall
	exit:
		mov rdi, 0
		mov rax, 60
		syscall