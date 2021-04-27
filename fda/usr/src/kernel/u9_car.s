! ----------------------------------------------------------------------- !
!                                u9_car.s                                 !
! ----------------------------------------------------------------------- !
!                                                                         !
! ----------------------------------------------------------------------- !

.sect .text; .sect .rom; .sect .data; .sect .bss
.extern _u9_car
.extern _u9_byte
.extern _u9_word
.extern _u9_dword
.sect .text

! void u9_car ( char car ) ;              /* escribe car por el puerto 0xE9 */
! void u9_byte ( char b ) ;                 /* escribe b por el puerto 0xE9 */

_u9_car:
_u9_byte:

    push ebp
    mov ebp,esp
    movb al,8(ebp)     ! car

    mov ecx,0xE90000FF ! identifica call_e9
    mov ebx,0x00000000 ! identifica la operacion u9_car
    int 0x21           ! trap a MINIX

               ! escribe el byte contenido en el registro AL por el puerto 0xE9 
               ! AL (8 bits de menor peso del registro acumulador AX o EAX)

    leave
    ret


! void u9_word ( short w ) ;                /* escribe w por el puerto 0xE9 */

_u9_word:

    push ebp
    mov ebp,esp
    mov ax,8(ebp)      ! w

    mov ecx,0xE90000FF ! identifica call_e9
    mov ebx,0x00000001 ! identifica la operacion u9_word
    int 0x21           ! trap a MINIX
	
    leave
    ret


! void u9_dword ( unsigned dw ) ;          /* escribe dw por el puerto 0xE9 */

_u9_dword:

    push ebp
    mov ebp,esp
    mov eax,8(ebp)     ! dw

    mov ecx,0xE90000FF ! identifica call_e9
    mov ebx,0x00000002 ! identifica la operacion u9_dword
    int 0x21           ! trap a MINIX

    leave
    ret



