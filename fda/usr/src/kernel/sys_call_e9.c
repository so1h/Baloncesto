/* ----------------------------------------------------------------------- */
/*                              sys_call_e9.c                              */
/* ----------------------------------------------------------------------- */
/*     manejador de las llamadas al sistema e9 (INT21 ECX = E900000FF)     */
/* ----------------------------------------------------------------------- */

#include "kernel.h"                   /* proc.h necesita esas definiciones */
#include "proc.h"                                  /* proc, campo p_memmap */

#include "e9_system.h"        /* e9_car, e9_word, e9_dword, e9_str, e9_raw */

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
    default : ;
    }
	
    return 0 ;
}
