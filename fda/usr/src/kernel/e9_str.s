! ------------------------------------------------------------------------- !
!                                 e9_str.s                                  !
! ------------------------------------------------------------------------- !
!                                                                           !
! ------------------------------------------------------------------------- !

.sect .text; .sect .rom; .sect .data; .sect .bss
.extern _e9_str
.sect .text

_e9_str:

    push ebp
    mov ebp,esp
	push esi         ! guardamos esi
    mov esi,8(ebp)

    mov dx,0xE9      ! puerto de E/S (outb 0xE9)
bucle:
    movb al,(esi)    ! realmente (ds:esi)
    testb al,al
    jz fin
    outb dx    ! escribe el byte contenido en el registro AL en el puerto 0xE9 
               ! AL (8 bits de menor peso del registro acumulador AX o EAX)
	inc esi
	jmp bucle 
fin: 	

    pop esi          ! restauramos esi
    leave
    ret
