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

BUFFER1:
	.space	1024

BUFFER_LEN:
	.long	1024

O_RDONLY:
	.long  0

.section .text

_start:
	#create space to save variable
	push rbp
	mov rbp, rsp
	sub	rsp, 0x20

	socket:
		mov edi, dword ptr [AF_INET]
		mov esi, dword ptr [SOCK_STREAM]
		mov edx, dword ptr [IPPROTO_IP]
		mov rax, 41
		syscall
	
	#check socket exist
	cmp	rax, 0x0
	jle exit_failure

	#store socket fd
	mov	dword ptr [rsp], eax   #socket fd

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
		mov	dword ptr [rsp + 0x8], eax	 #client fd	
	
	get_request:
		#set arg for read_request
		mov edi, dword ptr [rsp + 0x8]
		lea rsi, [rip + BUFFER]
		mov edx, dword ptr [BUFFER_LEN]
		call read_request
		#get read size
		mov dword ptr [rsp + 0x10], eax
		#get path
		call get_path
		cmp rax, 0x0
		jle exit_failure
		#open path
		mov rdi, rax
		mov esi, dword ptr [rip + O_RDONLY]
		call open_path
		#check open
		cmp rax, 0x0
		jle exit_failure
	
	read_path_content:
		mov rdi, rax
		lea rsi, [rip + BUFFER1] 
		mov edx, dword ptr [rip + BUFFER_LEN] 
		call read_request

	send_response:
		#set arg for write_response
		lea	rsi, [rip + BUFFER1]
		mov rdi, rsi
		call strlen
		mov rdx, 3
		mov edi, dword ptr [rsp + 0x8]
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
	
	open_path:
		mov	rax, 2
		syscall
		ret
	
	strlen:
		cmp rdi, 0x0
		jle done
		mov rax, 0
		loop:
			mov dl, byte ptr [rdi + rax]
			cmp dl, 0x0
			je	done
			inc rax
			jmp loop

		done:
			nop
		ret

	get_path:
		#check read length
		mov eax, dword ptr [rsp + 0x10]
		cmp rax, 0x0
		jle exit_get_path

		#init register
		mov	rcx, 0
		lea rdi, [rip + BUFFER]
		mov edx, dword ptr [rsp + 0x10]  #max len

		#get first space
		remove_first_space:
			mov dl, byte ptr [rdi + rcx]
			cmp rcx, rdx
			ja	done_trim
			inc rcx
			cmp dl, 0x20 #is_space
			jne remove_first_space

		lea rdi, [rdi + rcx]
		mov qword ptr [rsp + 0x18], rdi
		mov rcx, 0
		remove_second_space:
			mov dl, byte ptr [rdi + rcx]
			cmp rcx, rdx
			ja	done_trim
			inc rcx
			cmp dl, 0x20 #is_space
			jne remove_second_space
			dec rcx
			mov byte ptr [rdi + rcx], 0x0
		
		exit_get_path:
			nop
		done_trim:
			mov rax, rdi
		ret
