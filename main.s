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
	.2byte 0x02   #AF_INET
	.2byte 0x5000 #port=80
	.4byte 0x0    #address=0.0.0.0 
	.8byte 0x0	  #null padding

ADDR_LEN:
	.long 16

BACKLOG:
	.long 0

.section .text

_start:
	#create space to save variable
	push rbp
	mov rbp, rsp
	sub	rsp, 0x18

	socket:
		mov edi, dword ptr [AF_INET]
		mov esi, dword ptr [SOCK_STREAM]
		mov edx, dword ptr [IPPROTO_IP]
		mov rax, 41
		syscall
	
	#check socket exist
	cmp	rax, 0x0
	jbe exit_failure

	#store socket fd
	mov	dword ptr [rsp], eax

	bind:
		mov	edi, dword ptr [rsp]
		mov	rsi, offset SOCKADDR_IN
		mov	edx, dword ptr [ADDR_LEN]
		mov	rax, 49	
		syscall
	
	#check bind sucess
	cmp	rax, 0x0
	jb exit_failure

	listen:
		mov edi, dword ptr [rsp]
		mov esi, dword ptr [BACKLOG]
		mov rax, 50 
		syscall

	#check listen error
	cmp rax, 0x0
	jne exit_failure

	accept:
		mov edi, dword ptr [rsp]
		mov rsi, 0
		mov rdx, 0
		mov rax, 43
		syscall

	#check accept error
	cmp rax, 0x0
	jb not_storing_socket

	store_client_socket:
		mov	dword ptr [rsp + 8], eax

	not_storing_socket:
		nop

	exit_success:
		mov	rsp, rbp
		mov rdi, 0
		mov rax, 60
		syscall

	exit_failure:
		mov	rsp, rbp
		mov rdi, 1
		mov rax, 60
		syscall
