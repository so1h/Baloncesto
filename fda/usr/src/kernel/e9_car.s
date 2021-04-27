! ----------------------------------------------------------------------- !
!                                e9_car.s                                 !
! ----------------------------------------------------------------------- !
!                                                                         !
! ----------------------------------------------------------------------- !

.sect .text; .sect .rom; .sect .data; .sect .bss
.extern _e9_car, _e9_byte, _e9_word, _e9_dword
.sect .text

_e9_car:
_e9_byte:

    push ebp
    mov ebp,esp
    movb al,8(ebp)

    outb 0xE9  ! escribe el byte contenido en el registro AL por el puerto 0xE9 
               ! AL (8 bits de menor peso del registro acumulador AX o EAX)

    leave
    ret

_e9_word:

    push ebp
    mov ebp,esp
    mov ax,8(ebp)
	
    outb 0xE9  
	shr ax,8
    outb 0xE9  
	
    leave
    ret

_e9_dword:

    push ebp
    mov ebp,esp
    mov eax,8(ebp)

    outb 0xE9  
	shr eax,8
    outb 0xE9  
	shr eax,8
    outb 0xE9  
	shr eax,8
    outb 0xE9  

    leave
    ret


