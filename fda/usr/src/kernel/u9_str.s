! ------------------------------------------------------------------------- !
!                                 u9_str.s                                  !
! ------------------------------------------------------------------------- !
!                                                                           !
! ------------------------------------------------------------------------- !

.sect .text; .sect .rom; .sect .data; .sect .bss
.extern _u9_str
.extern _u9_raw
.sect .text


! void u9_str ( char * str ) ;            /* escribe str por el puerto 0xE9 */

_u9_str:

    push ebp
    mov ebp,esp
    mov eax,8(ebp)     ! str

    mov ecx,0xE90000FF ! identifica call_e9
    mov ebx,0x00000003 ! identifica la operacion u9_dword
    int 0x21           ! trap a MINIX

    leave
    ret


! void u9_raw ( char * ptr, unsigned l ) ;      /* escribe l bytes por 0xE9 */

_u9_raw:

    push ebp
    mov ebp,esp
    mov eax,8(ebp)     ! ptr 
	mov edx,12(ebp)    ! l

    mov ecx,0xE90000FF ! identifica call_e9
    mov ebx,0x00000004 ! identifica la operacion u9_dword
    int 0x21           ! trap a MINIX

    leave
    ret


