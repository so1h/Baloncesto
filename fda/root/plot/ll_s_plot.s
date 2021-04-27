! ------------------------------------------------------------------------- !
!                                ll_s_plot.s                                !
! ------------------------------------------------------------------------- !
!   implementacion de la rutina de interfaz de la llamada al sistema plot   !
! ------------------------------------------------------------------------- !

! sections

.sect .text; .sect .rom; .sect .data; .sect .bss

.define _ll_s_plot

.sect .text

! int ll_s_plot ( unsigned flags, unsigned sched )             

    .align 16

_ll_s_plot:
    push ebp
    mov ebp,esp

    mov eax,8(ebp)     ! eax = flags
    mov ebx,12(ebp)    ! ebx = sched
    mov ecx,0xB10C00FF ! identifica la operacion plot
    int 0x21           ! trap a MINIX
 
    leave
    ret
