! ------------------------------------------------------------------------- !
!                                 e9_raw.s                                  !
! ------------------------------------------------------------------------- !
!                                                                           !
! ------------------------------------------------------------------------- !

.sect .text; .sect .rom; .sect .data; .sect .bss
.extern _e9_raw
.sect .text

! void e9_raw ( char * ptr, unsigned l ) ;     /* escribe l bytes por 0xE9 */

_e9_raw:

    push ebp
    mov ebp,esp
    push esi         ! salvamos esi
    push ecx         ! salvamos ecx 
    mov esi,8(ebp)
    mov ecx,12(ebp)
    
    test ecx,ecx  ! es cero el contador
    jz fin
    mov dx,0xE9   ! puerto de E/S (outb 0xE9)
    cld           ! sentido ascendente DS:ESI se incrementa 
    rep outsb     ! ECX se decrementa
fin:

!bucle:
!   test ecx,ecx
!   jz fin
!   movb al,(esi)    ! realmente (ds:esi)
!   outb 0xE9  ! escribe el byte contenido en el registro AL en el puerto 0xE9 
!              ! AL (8 bits de menor peso del registro acumulador AX o EAX)
!   dec ecx 			   
!   inc 
!fin: 	

    pop ecx          ! restauramos ecx
    pop esi          ! restauramos esi
    leave
    ret
