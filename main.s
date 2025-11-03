.intel_syntax noprefix
.global _start

.section .data
AF_INET:
	.long 2

SOCK_STREAM:
	.long 1

IPPROTO_IP:
	.long 0	

SOCKADDR_IN:
	.2byte 0x02
	.byte 0
	.byte 80
	.4byte 0x0
	.8byte 0x0

ADDR_LEN:
	.long 16

.section .text

_start:
	socket:
		mov edi, dword ptr [AF_INET]
		mov esi, dword ptr [SOCK_STREAM]
		mov edx, dword ptr [IPPROTO_IP]
		mov rax, 41
		syscall
	
	#check socket exist
	cmp	rax, 0x0
	jbe exit_failure

	bind:
		mov	rdi, rax
		mov	rsi, offset SOCKADDR_IN
		mov	edx, dword ptr [ADDR_LEN]
		mov	rax, 49	
		syscall
	
	#check bind sucess
	cmp	rax, 0x0
	jb exit_failure

	exit_success:
		mov rdi, 0
		mov rax, 60
		syscall

	exit_failure:
		mov rdi, 1
		mov rax, 60
		syscall
