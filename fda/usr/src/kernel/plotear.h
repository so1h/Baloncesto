/* ----------------------------------------------------------------------- */
/*                               plotear.h                                 */
/* ----------------------------------------------------------------------- */
/*                            funcion plotear                              */
/* ----------------------------------------------------------------------- */

/* se llama a esta funcion desde las funciones correspondientes de proc.c  */
/* sched, enqueue, dequeue, pick_proc, sys_call (SEND, RECEIVE, SENDREC)   */
/* desde la funcion intr_handle en i8259.c (interrupciones) y desde la     */
/* funcion exception en exception.c. (y desde algún sitio mas)             */  

#ifndef PLOTEAR_H
#define PLOTEAR_H

#define opMIN        opIRQ_00
#define opMAX        opSTATUS   
/* */                                  /* /usr/src/kernel/mpx386.s         */
/* */                                  /* /usr/src/include/ibm/interrupt.h */
#define opIRQ_00      0                /* CLOCK_IRQ     */
#define opIRQ_01      1                /* KEYBOARD_IRQ  */ 
#define opIRQ_02      2                /* CASCADE_IRQ   */
#define opIRQ_03      3                /* ETHER_IRQ     */ /* SECONDARY_IRQ */
#define opIRQ_04      4                /* RS232_IRQ     */
#define opIRQ_05      5                /* XT_WINI_IRQ   */  
#define opIRQ_06      6                /* FLOPPY_IRQ    */
#define opIRQ_07      7                /* PRINTER_IRQ   */
#define opIRQ_08      8
#define opIRQ_09      9
#define opIRQ_10     10
#define opIRQ_11     11
#define opIRQ_12     12                /* KBD_AUX_IRQ   */  
#define opIRQ_13     13
#define opIRQ_14     14                /* AT_WINI_0_IRQ */
#define opIRQ_15     15                /* AT_WINI_1_IRQ */

/* */                                  /* /usr/src/kernel/mpx386.s         */
/* */                                  /* /usr/src/include/ibm/interrupt.h */
#define opEXC_00     16                /* 16 + DIVIDE_VECTOR       */
#define opEXC_01     17                /* 16 + DEBUG_VECTOR        */
#define opEXC_02     18                /* 16 + NMI_VECTOR          */
#define opEXC_03     19                /* 16 + BREAKPOINT_VECTOR   */
#define opEXC_04     20                /* 16 + OVERFLOW_VECTOR     */
/* */                                  /* /usr/src/kernel/protect.h */
#define opEXC_05     21                /* 16 + BOUNDS_VECTOR       */
#define opEXC_06     22                /* 16 + INVAL_OP_VECTOR     */
#define opEXC_07     23                /* 16 + COPROC_NOT_VECTOR   */
#define opEXC_08     24                /* 16 + DOUBLE_FAULT_VECTOR */
#define opEXC_09     25                /* 16 + COPROC_SEG_VECTOR   */
#define opEXC_10     26                /* 16 + INVAL_TSS_VECTOR    */
#define opEXC_11     27                /* 16 + SEG_NOT_VECTOR      */
#define opEXC_12     28                /* 16 + STACK_FAULT_VECTOR  */
#define opEXC_13     29                /* 16 + PROTECTION_VECTOR   */
#define opEXC_14     30                /* 16 + PAGE_FAULT_VECTOR   */
#define opEXC_15     31                 
#define opEXC_16     32                /* 16 + COPROC_ERR_VECTOR   */

/* */                                  /* /usr/src/kernel/ipc.h             */
#define opSVC_00     33                /* 33 + 00                  */ 
#define opSVC_01     34                /* 33 + 01 SEND             */
#define opSVC_02     35                /* 33 + 02 RECEIVE          */
#define opSVC_03     36                /* 33 + 03 SENDREC          */
#define opSVC_04     37                /* 33 + 04 NOTIFY           */
#define opSVC_05     38                /* 33 + 05 IPC_REQUEST      */
#define opSVC_06     39                /* 33 + 06 IPC_REPLY        */
#define opSVC_07     40                /* 33 + 07 IPC_NOTIFY       */
#define opSVC_08     41                /* 33 + 08 ECHO             */
#define opSVC_09     42                /* 33 + 09 IPC_RECEIVE      */

#define opMAPKBD     43   /* proporciona el scancode y el caracter ASCII */ 
/* */                     /* /usr/src/drivers/tty/keyboard.c (map_key) */	
						  
#define opIDE        44   /* debe proporcionar el sector y si es lectura/escritura */ 
                          /* /usr/src/drivers/at_wini/atwini.c (do_transfer) */
                          /* opcode, sector, count */	/* w_intr_wait */					  

/* no usados de momento */

#define opINTHND     45                /* /usr/src/kernel/i8259.c          */ /* interrupcion       */
#define opEXCHND     46                /* /usr/src/kernel/exception.c      */ /* excepcion          */

#define opENQUEUE    47                /* /usr/src/kernel/proc.c           */
#define opDEQUEUE    48                /* /usr/src/kernel/proc.c           */
#define opSCHED      49                /* /usr/src/kernel/proc.c           */ /* decide el lugar del proceso en la cola de preparados */    
#define opPICKPROC   50                /* /usr/src/kernel/proc.c           */ /* decide el siguiente proceso a ejecutar (next_ptr) */

#define opSEND       51                /* /usr/src/kernel/proc.c           */ /* los 5 tipos de llamada al sistema (SVC) */
#define opRECEIVE    52                /* /usr/src/kernel/proc.c           */
#define opSENDREC    53                /* /usr/src/kernel/proc.c           */
#define opNOTIFY     54                /* /usr/src/kernel/proc.c           */
#define opECHO       55                /* /usr/src/kernel/proc.c           */

#define opL_ENQUEUE  56                /* /usr/src/kernel/proc.c           */ /* invocada por interrupciones o tareas */
#define opL_DEQUEUE  57                /* /usr/src/kernel/proc.c           */ /* invocada por interrupciones o tareas */

#define opL_SEND     58                /* /usr/src/kernel/proc.c           */ /* invocada por interrupciones o tareas */
#define opL_NOTIFY   59                /* /usr/src/kernel/proc.c           */ /* invocada por interrupciones o tareas */

#define opDO_FORK    60                /* /usr/src/kernel/system/do_fork.c */ /* se crea un proceso hijo */
#define opDO_EXIT    61                /* /usr/src/kernel/system/do_exit.c */ /* se destruye un proceso: SLOT_FREE */

#define opBALANCE    62                /* /usr/src/kernel/proc.c           */ /* invocada por /usr/src/kernel/clock.c */ 

#define opSTATUS     63                /* /usr/src/kernel/proc.c           */  


#define id_DEVIO      0                      /* /usr/src/kernel/do_devio.c */
#define id_PLOTEAR    1

extern int vacia_sc ( int id ) ;

extern void encolar_sc ( int id, unsigned char sc ) ; 

extern unsigned char desencolar_sc ( int id ) ;


extern int clock_ints ;


void init_plot ( unsigned acum,                  /* por defecto 0x00000000 */
                 unsigned sched ) ;              /* por defecto 0x00000000 */

void plotear ( struct proc * rp, unsigned op ) ;

#endif /* PLOTEAR_H */
