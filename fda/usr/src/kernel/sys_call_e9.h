/* ----------------------------------------------------------------------- */
/*                              sys_call_e9.h                              */
/* ----------------------------------------------------------------------- */
/*     manejador de las llamadas al sistema e9 (INT21 ECX = E900000FF)     */
/* ----------------------------------------------------------------------- */

#ifndef SYS_CALL_E9_H
#define SYS_CALL_E9_H

int sys_call_e9 ( unsigned call_nr,                             /* reg_ecx */     
                  unsigned reg_eax, 
                  unsigned reg_ebx, 
                  unsigned reg_edx ) ;
				  
#endif /* SYS_CALL_E9_H */
