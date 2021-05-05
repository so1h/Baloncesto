/* ----------------------------------------------------------------------- */
/*                              sys_call_e9.c                              */
/* ----------------------------------------------------------------------- */
/*     manejador de las llamadas al sistema e9 (INT21 ECX = E900000FF)     */
/* ----------------------------------------------------------------------- */

#include "kernel.h"                   /* proc.h necesita esas definiciones */
#include "proc.h"                                  /* proc, campo p_memmap */

#include "e9_system.h"        /* e9_car, e9_word, e9_dword, e9_str, e9_raw */

#if 1                                                 /* tomado de clock.c */

/* Clock parameters. */
#define COUNTER_FREQ (2*TIMER_FREQ) /* counter frequency using square wave */
#define LATCH_COUNT     0x00  /* cc00xxxx, c = channel, x = any */
#define SQUARE_WAVE     0x36  /* ccaammmb, a = access, m = mode, b = BCD */
                              /*   11x11, 11 = LSB then MSB, x11 = sq wave */
#define TIMER_COUNT ((unsigned) (TIMER_FREQ/HZ)) /* initial value for counter*/
#define TIMER_FREQ  1193182L     /* clock frequency for timer in PC and AT */

#define CLOCK_ACK_BIT	0x80       /* PS/2 clock interrupt acknowledge bit */

#endif

/* TIMER_MODE (<ibm/ports.h>) */        /* I/O port for timer mode control */

int sys_call_e9 ( unsigned call_nr,                             /* reg_ecx */     
                  unsigned reg_eax, 
                  unsigned reg_ebx, 
                  unsigned reg_edx ) 
{                                                 
                                        /* call_nr = reg_ecx == 0xE90000FF */
	struct proc * rp = proc_ptr ;
	
	char * ptr ;
	
    switch (reg_ebx) {
    case 0 : e9_car  ((char )(reg_eax & 0x000000FF)) ; break ;
    case 1 : e9_word ((short)(reg_eax & 0x0000FFFF)) ; break ;
    case 2 : e9_dword(        reg_eax              ) ; break ;

	case 3 : ptr = (char *)((rp->p_memmap[D].mem_phys << CLICK_SHIFT) 
	                          + reg_eax - kinfo.data_base) ;
	         e9_str(ptr) ; 
			 break ;  
			 
	case 4 : ptr = (char *)((rp->p_memmap[D].mem_phys << CLICK_SHIFT) 
	                          + reg_eax - kinfo.data_base) ;
	         e9_raw(ptr, reg_edx) ; 
			 break ;  
	case 5 : ptr = (char *)((rp->p_memmap[D].mem_phys << CLICK_SHIFT) 
	                          + reg_eax - kinfo.data_base) ;
	         e9_raw(ptr, reg_edx) ; 
			 outb(TIMER_MODE, LATCH_COUNT) ;      /* read_clock en clock.c */
			 outb(0xE9, inb(TIMER0)) ;
			 outb(0xE9, inb(TIMER0)) ;
			 break ;  
    default : ;
    }
	
    return 0 ;
}
