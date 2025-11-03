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
	.2byte 0x02   			#AF_INET
	.2byte 0x5000 			#port=80
	.4byte 0x0    			#address=0.0.0.0 
	.8byte 0x0	  			#null padding

ADDR_LEN:
	.long 16

BACKLOG:
	.long 0

BASIC_RESPONSE:
	.ascii "HTTP/1.0 200 OK\r\n\r\n"
	BASIC_RESPONSE_LEN = . - BASIC_RESPONSE

BUFFER:
	.space	1024

BUFFER_LEN:
	.long	1024

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
		mov	dword ptr [rsp + 0x8], eax		
	
	get_request:
		#set arg for read_request
		mov edi, dword ptr [rsp + 0x8]
		lea rsi, [rip + BUFFER]
		mov edx, dword ptr [BUFFER_LEN]
		call read_request
		#check read done

	send_response:
		#set arg for write_response
		mov	edi, dword ptr [rsp + 0x8]
		lea	rsi, [rip + BASIC_RESPONSE]
		mov edx, BASIC_RESPONSE_LEN
		call write_response
		#check write done

		mov edi, dword ptr [rsp + 0x8]
		call close_fd

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
	
	read_request:
		mov	rax, 0
		syscall
		ret
	
	write_response:
		mov rax, 1
		syscall
		ret

	close_fd:
		mov rax, 3
		syscall
		ret
		