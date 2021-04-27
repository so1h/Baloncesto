/* ----------------------------------------------------------------------- */
/*                               plotear.c                                 */
/* ----------------------------------------------------------------------- */
/*             funcion plotear (con salida en formato binario)             */
/* ----------------------------------------------------------------------- */

#include "kernel.h"                   /* proc.h necesita esas definiciones */
#include "proc.h"                             /* proc, campos p_nr, p_name */
#include <signal.h>                                               /* _NSIG */
#include "../include/minix/com.h"                                   /* ANY */
#include "../include/minix/ipc.h"                               /* message */
#include "../servers/pm/mproc.h"                                  /* mproc */

#include "ll_s_plot.h"                /* PLOT_ON, PLOT_PID, PLOT_PRIO, ... */

#include "plotear.h"     /* opENQUEUE, opDEQUEUE, opSCHED, opPICKPROC, ... */

#include "sys_call_plot.h"     /* plot_gen, plot_mem, plot_fin, plot_bufer */

#include "e9_system.h"                                        /* e9_printf */

int clock_ints = -1 ;

void mostrar_procesos ( struct mproc * ptr_mproc ) ;

void mostrar_preparados ( struct mproc * ptr_mproc ) ;

void mostrar_cola ( struct mproc * ptr_mproc ) ;

void mostrar_PID ( struct proc * rp ) ;

void mostrar_nombre ( struct proc * rp ) ;

void mostrar_llamada ( struct proc * rp_par, unsigned op_par) ;

void mostrar_bloque_llamada ( struct proc * rp_par, unsigned op_par ) ;

void mostrar_endequeue ( struct proc * rp_par ) ;

void mostrar_status ( struct proc * rp ) ;

void inic_cola_sc ( void ) ;

/* !!!!!!!!! desplazamientos de las variables: mproc y realtime (A CAPON)  */
/* !!!!!!!!! mproc podria cambiar si se modifica el gestor de procesos pm  */
/* !!!!!!!!! realtime cambia si se modifica el el kernel (ver Makefile)    */

#define MPROC    0x0000ba68      /* nm /usr/src/servers/pm/pm | grep mproc */ /* BSS  */

#define REALTIME (*((long *)0x1EA0)) /* nm kernel | grep realtim -> 0x1EA0 */ /* puede no ser necesario */
                                                                              /* get_uptime */
static struct mproc * ptr_mproc ;
static struct mproc * pmproc_addr [ NR_PROCS ] ;             /* eficiencia */
static int contador ;                           /* contador de operaciones */

static unsigned kernel_D_CLICK ;                /* click del DS del kernel */

/* init_plot(acum, sched) inicializa las siguientes variables:             */
/*                                                                         */
/* contador         : 0                                                    */
/* plot_acum        : acum                                                 */
/* plot_sched       : sched                                                */
/* plot_bufer       : 0x2000000 (32 M) - (kernel_D_CLICK << CLICK_SHIFT)   */
/* plot_count       : 0                                                    */
/* plot_in          : plot_bufer                                           */
/* plot_out         : plot_bufer                                           */
/* plot_gen         : plot_acum & _PLOT_ON                                 */
/* plot_mem         : plot_acum & _PLOT_MEM                                */
/* ptr_mproc        : puntero a mproc (tabla de procesos de pm)            */
/* pmproc_addr      : tabla de punteros &ptr_mproc[i]                      */
/*                                                                         */

void init_plot ( unsigned acum,                  /* por defecto 0x00000000 */
                 unsigned sched )                /* por defecto 0x00000000 */
{
    unsigned pm_D_CLICK ;                       /* click del DS del pm     */
    int i ;
    contador = 0 ;

    plot_acum = acum ;                       /* debe tener activo _PLOT_ON */
    plot_sched = sched ;

/*  kernel_D_CLICK = proc[NR_TASKS + KERNEL].p_memmap[D].mem_phys ;        */
    kernel_D_CLICK = kinfo.data_base >> CLICK_SHIFT ;           /* start.c */
                   /* kernel.h, <minix/type.h>, mpx386.s, start.c y main.c */

    plot_bufer = (char *)0x2000000 -  (kernel_D_CLICK << CLICK_SHIFT) ;
    plot_count = 0 ;
    plot_in = plot_bufer ;
    plot_out = plot_bufer ;

    plot_gen = plot_acum & PLOT_ON ;                                  /* 1 */
    plot_mem = plot_acum & PLOT_MEM ;

/*  if (proc[NR_TASKS + PM_PROC_NR].p_memmap[D].mem_phys == 0x00000000) */
    if (pproc_addr[NR_TASKS + PM_PROC_NR]->p_memmap[D].mem_phys == 0x00000000)
    {
        kprintf("\n ERROR inic_plot (todavia no existe pm) ") ;
        for ( ; ; ) { } ;
    }

/*  pm_D_CLICK = proc[NR_TASKS + PM_PROC_NR].p_memmap[D].mem_phys -        */
    pm_D_CLICK = pproc_addr[NR_TASKS + PM_PROC_NR]->p_memmap[D].mem_phys ;

    ptr_mproc = (struct mproc *)
        (MPROC + ((pm_D_CLICK - kernel_D_CLICK) << CLICK_SHIFT)) ;

    for ( i = 0 ; i < NR_PROCS ; i++ )
        pmproc_addr[i] = &ptr_mproc[i] ;          /* para mayor eficiencia */

#if 0                                                   /* para depuracion */
        e9_printf("\n kernel_D_CLICK = %08X ", (unsigned)kernel_D_CLICK) ;
        e9_printf("\n pm_D_CLICK     = %08X ", (unsigned)pm_D_CLICK) ;
        e9_printf("\n ptr_mproc      = %08X ", (unsigned)ptr_mproc) ;
        e9_printf("\n prueba         = %08X ", (unsigned)0x12345678) ;
        e9_printf("\n PM_PROC_NR     = %u ", PM_PROC_NR) ;
        e9_printf("\n NR_TASKS + PM_PROC_NR = %u ", NR_TASKS + PM_PROC_NR) ;
        e9_printf("\n proc[NR_TASKS + PM_PROC_NR].p_memmap[D].mem_phys = %08X ",
                      proc[NR_TASKS + PM_PROC_NR].p_memmap[D].mem_phys) ;
        e9_printf("\n proc[NR_TASKS + KERNEL].p_memmap[D].mem_phys = %08X ",
                      proc[NR_TASKS + KERNEL].p_memmap[D].mem_phys) ;
        for ( ; ; ) ;
#endif

    mostrar_procesos(ptr_mproc) ;
/*  mostrar_preparados(ptr_mproc) ; */
    mostrar_cola(ptr_mproc) ;
	
	inic_cola_sc() ;                /* vaciamos la cola de codigos de scan */

}

/* ----------------------------------------------------------------------- */
/* plotear ( rp_par, op_h, op_m, op_l ) ;                                  */
/* ----------------------------------------------------------------------- */
/*
    op          origen            significado
    =========== ================= ========================================
    opIRQ_00    mpx386.s          interrupcion         00 (0x00) interrupciones inhibidas
    opIRQ_15                                           15 (0x0F) interrupciones inhibidas

    opEXC_00    mpx386.s          excepcion            16 (0x10) interrupciones inhibidas
    opEXC_16                                           32 (0x20) interrupciones inhibidas

    opINT       i8259.c           interrupt handler    plotear(NIL_PROC, (hook->id << 16) | (hook->irq << 8) | opINT) ; (proc_ptr)
    opEXC       exception.c       exception handler    plotear(NIL_PROC, (vec_nr << 16) | opEXC) ; (proc_ptr)

    opENQUEUE   proc.c                                                                   plotear(rp, opENQUEUE) ; (proc_ptr)
    opDEQUEUE   proc.c                                                                   plotear(rp, opDEQUEUE) ; (proc_ptr)
    opSCHED     proc.c            decide el lugar del proceso en la cola de preparados   plotear(NIL_PROC, (queue << 16) | (front << 8) | opSCHED) ;
    opPICKPROC  proc.c            decide el siguiente proceso a ejecutar (next_ptr)      plotear(NIL_PROC, opPICKPROC) ; (next_ptr)

    opSEND      proc.c            los 5 tipos de llamada al sistema (SVC)  plotear((struct proc *)m_ptr, (src_dst_e << 16) | opSEND) ;
    opRECEIVE   proc.c                                                     plotear((struct proc *)m_ptr, (src_dst_e << 16) | opRECEIVE) ;
    opSENDREC   proc.c                                                     plotear((struct proc *)m_ptr, (src_dst_e << 16) | opSENDREC) ;
    opNOTIFY    proc.c                                                     plotear((struct proc *)m_ptr, (src_dst_e << 16) | opNOTIFY) ;
    opECHO      proc.c                                                     plotear((struct proc *)m_ptr, (src_dst_e << 16) | opECHO) ;

    opL_ENQUEUE proc.c            invocada por interrupciones o tareas     plotear(rp, opL_ENQUEUE) ; (proc_ptr)
    opL_DEQUEUE proc.c            invocada por interrupciones o tareas     plotear(rp, opDEQUEUE) ; (proc_ptr)

    opL_SEND    proc.c            invocada por interrupciones o tareas     plotear((struct proc *)m_ptr, (dst_e << 16) | opL_SEND) ;
    opL_NOTIFY  proc.c            invocada por interrupciones o tareas     plotear(NIL_PROC, (dst << 16) | opL_NOTIFY) ;

    opDO_FORK   system/do_fork.c  rpp crea un proceso hijo rpc             plotear(rpp, (rpc->p_endpoint << 16) | (rpc->p_nr << 8) | opDO_FORK) ;
    opDO_EXIT   system/do_exit.c  se destruye un proceso: SLOT_FREE        plotear(pproc_addr(exit_e), opDO_EXIT) ;
                                                                           plotear(pproc_addr(who_p), opDO_EXIT) ;

    opBALANCE   proc.c            se aumenta la priodidad de un proceso    plotear(rp, opBALANCE) ;
                                  invocada por clock.c

    opSTATUS    proc.c            para informar de un cambio de estado     plotear(rp, opSTATUS) ;
                                  que queda por reportar
*/
/* ----------------------------------------------------------------------- */

#define cola_sc_TAM 16

struct cola_sc {
    unsigned char sc [ cola_sc_TAM ] ;
    int in ;
    int out ;
    int num ;
} ;

struct cola_sc cola_sc [ 2 ] =        /* id: 0 -> id_DEVIO, 1-> id_PLOTEAR */
{
    { { 0 }, 0, 0, 0 },
    { { 0 }, 0, 0, 0 }
} ;

void inic_cola_sc ( void )
{
    int id ;
    for ( id = 0 ; id < 2 ; id++ ) 
    {
        cola_sc[id].in  = 0 ;
        cola_sc[id].out = 0 ;
        cola_sc[id].num = 0 ;
    }

/*  ñapa para que el primer sc que se encole con encolar_sc(id_PLOTEAR,sc) */
/*  realmente no se encole, ya que corresponde a la liberacion de la tecla */
/*  Enter pulsada tras el comando "plot on" con sc = 0x9C (0x1C | 0x80).   */  

    cola_sc[id_PLOTEAR].in = cola_sc_TAM - 1 ; 
    cola_sc[id_PLOTEAR].num = - 1 ; 
	
}

int vacia_sc ( int id )
{
    return(cola_sc[id].num == 0) ;
}

void encolar_sc ( int id, unsigned char sc )      /* ultimos 16 scan codes */
{
    int in ;
    in = cola_sc[id].in ;
    cola_sc[id].sc[in] = sc ;
    cola_sc[id].in = (in + 1) % cola_sc_TAM ;
    if (cola_sc[id].num < cola_sc_TAM)     
		cola_sc[id].num++ ;
	else 
/*      cola_sc[id].out = (cola_sc[id].out + 1) % cola_sc_TAM ; */	
        cola_sc[id].out = cola_sc[id].in ;	
}

unsigned char desencolar_sc ( int id )
{
    int out ;
    unsigned char sc ;
    if (cola_sc[id].num == 0) return '\0' ;
    out = cola_sc[id].out ;
    sc = cola_sc[id].sc[out] ;
    cola_sc[id].out = (out + 1) % cola_sc_TAM ;
    cola_sc[id].num-- ;
    return sc ;
}

unsigned char devio_scan_code ;

void plotear ( struct proc * rp_par, unsigned op_par )
{
    unsigned op_h = (op_par >> 16) ;                 /* src_dst_e, hook_id */
    unsigned op_m = (op_par >> 8) & 0x000000FF ;                    /* irq */
    unsigned op_l = (op_par & 0x000000FF) ;                   /* operacion */

    unsigned long tsc_high, tsc_low ;                 /* RDTSC instruction */
    unsigned clock_realtime ;
    unsigned clock_counter ;

    int irq ;              /* numero de la pata de interrupcion solicitada */
    int hook_id ;
	int hook_ok ;
    int exc ;                                    /* numero de la excepcion */

    int i ;

/*  if (!(plot_acum & PLOT_ON)) return ; */                 /* ¿ plot on ? */
    if (!plot_gen) return ;                                 /* ¿ plot on ? */

/*  if ((op_l < opMIN) || (opMAX < op_l)) return ; */   /* ¿ op_l válida ? */
    if (opMAX < op_l) return ;                          /* ¿ op_l válida ? */

/*  if (machine.processor > 486)                                           */
/*  {                                                                      */
/*      read_tsc(&tsc_high, &tsc_low) ; */                    /* klib386.s */
/*      e9_dword(tsc_high) ; */                                      /* >> */
/*      e9_dword(tsc_low) ; */                                       /* >> */
/*  }                                                                      */

    clock_counter = read_clock() ;             /* mas urgente que realtime */
/*  clock_realtime = REALTIME ; */ /* !!!!!!!! tomado a capon (ver define) */
/*  clock_realtime = get_uptime() ; */ /* no necesario contando opIRQ_00   */
    e9_byte(op_l) ;                                                  /* >> */
/*  e9_dword(clock_realtime) ; */      /* no necesario contando opIRQ_00   */
    e9_word(clock_counter) ;                                         /* >> */

    switch (op_l) {

        case opINTHND    : irq = ((int)op_m) & 0x0000000F ;
		                   hook_ok = (((int)op_m) & 0x000000F0) >> 4 ; 
                           hook_id = (int)op_h ;
                           e9_byte(irq) ;                            /* >> */
                           e9_byte(hook_id) ;                        /* >> */
                    	   if (irq == 1)    /* TECLADO */
						   {
                               e9_byte(0xAB) ;                       /* >> */ 
						   } 	   
#if 0
                           {
                               devio_scan_code = desencolar_sc(id_PLOTEAR) ;						   
                               e9_byte(devio_scan_code) ;            /* >> */
                           }
#elif 0							   
                           {
                               if (vacia_sc(id_PLOTEAR)) {
/* */                                   /* /usr/src/lib/i386/misc/io_inb.s */
                                   devio_scan_code = inb(0x60) ;
                                   encolar_sc(id_DEVIO, devio_scan_code) ;
                               }
                               else {
                                   devio_scan_code = desencolar_sc(id_PLOTEAR) ;
                               }
                               e9_byte(devio_scan_code) ;            /* >> */
                           }
#endif						   
                           return ;

        case opEXCHND    : exc = (int)op_h ;
                           e9_byte(exc) ;                            /* >> */
                           return ;

        case opENQUEUE   : ;
        case opDEQUEUE   : mostrar_endequeue(rp_par) ;
                           return ;

        case opSCHED     : mostrar_PID(rp_par) ;
                           e9_byte(op_h) ; /* q */
                           e9_byte(op_m) ; /* front */
                           return ;

        case opPICKPROC  : mostrar_PID(next_ptr) ;
                           mostrar_cola(ptr_mproc) ; /* PP para depurar */
                           return ;

        case opSEND      : ;
        case opRECEIVE   : ;
        case opSENDREC   : ;
        case opNOTIFY    : ;
        case opECHO      : mostrar_bloque_llamada(rp_par, op_par) ;
                           return ;

        case opL_ENQUEUE : ;
        case opL_DEQUEUE : mostrar_endequeue(rp_par) ;
                           return ;

        case opL_SEND    : ;
        case opL_NOTIFY  : mostrar_bloque_llamada(rp_par, op_par) ;
                           return ;

        case opDO_FORK   : mostrar_PID(proc_ptr) ; /* proceso en ejecucion (pm) */
                           mostrar_PID(rp_par) ;   /* proceso que crea (FORK) */
                           e9_word(op_h) ;         /* mp_pid del proceso que se crea */
                           e9_byte(op_m) ;         /* p_nr del proceso que se crea */
                           return ;

        case opDO_EXIT   : mostrar_PID(proc_ptr) ; /* proceso en ejecucion (pm) */
                           mostrar_PID(rp_par) ;   /* proceso que termina (EXIT) */
                           return ;

        case opBALANCE   : mostrar_endequeue(rp_par) ;
                           return ;

        case opSTATUS    : mostrar_status(rp_par) ; /* cambio de estado de rp_par */
                           return ;

        default          : ;
    }

}

void mostrar_procesos ( struct mproc * ptr_mproc )
{
    int i ;
    struct proc * rp ;
    struct mproc * mrp ;
    short p_rts_flags ;
    unsigned numProcesos ;

    e9_byte(proc_ptr->p_nr) ;                                        /* >> */

    numProcesos = 0 ;
    for ( i = 0 ; i < (NR_TASKS + NR_PROCS) ; i++ )
        if (pproc_addr[i]->p_rts_flags != SLOT_FREE)
            numProcesos++ ;

    e9_byte(numProcesos) ;                                           /* >> */

    for ( i = 0 ; i < (NR_TASKS + NR_PROCS) ; i++ )
    {
/*      rp  = (struct proc *)&proc[i] ; */
        rp  = pproc_addr[i] ;                 /* mas eficiente, ver proc.h */
        if (rp->p_rts_flags != SLOT_FREE)
        {
            mostrar_endequeue(rp) ;                                  /* >> */
            if (i < NR_TASKS)
                e9_byte(0) ;                                       /* PPID */
            else
                e9_byte(pmproc_addr[i-NR_TASKS]->mp_parent) ;      /* PPID */
        }
    }
}

#if 0

void mostrar_preparados ( struct mproc * ptr_mproc )
{
    int q ;
    struct proc * rp ;
    struct mproc * mrp ;

    e9_printf("\n\n Procesos preparados: \n") ;

    for ( q = 0 ; q < NR_SCHED_QUEUES ; q++ )
    {
        rp = rdy_head[q] ;
        while (rp != NIL_PROC)
        {
            e9_printf("\n") ;
            if (rp->p_nr >= 0)                          /* (i >= NR_TASKS) */
            {
                mrp = (struct mproc *) &ptr_mproc[rp->p_nr] ;
                e9_printf(" %3i  ""%2u ", mrp->mp_pid, rp->p_nr) ;  /* PID y p_nr (indice) */
            }
            else
                e9_printf(" [%2i] ""   ", rp->p_nr) ;               /* [p_nr] y nada */
            e9_printf("%2u (%2u) ", rp->p_priority, rp->p_max_priority) ;
            e9_printf("%2u ", rp->p_ticks_left) ;
            e9_printf("%2u ", rp->p_quantum_size) ;
            e9_printf("%-8s", rp->p_name) ;
            if (rp->p_nr >= 0)
                e9_printf("%-16s", mrp->mp_name) ;
            else
                e9_printf("%-16s", "") ;

            rp = rp->p_nextready ;
        }
    }
    e9_printf("\n") ;
}

#endif

void mostrar_cola ( struct mproc * ptr_mproc )
{
    int q ;
    struct proc * rp ;

    unsigned numProcesosPreparados ;
    numProcesosPreparados = 0 ;
    for ( q = (NR_SCHED_QUEUES-1) ; q >= 0 ; q-- )
    {
        rp = rdy_head[q] ;
        while (rp != NIL_PROC)
        {
            numProcesosPreparados++ ;
            rp = rp->p_nextready ;
        }
    }
    e9_byte(numProcesosPreparados) ;                                 /* >> */

    for ( q = (NR_SCHED_QUEUES-1) ; q >= 0 ; q-- )
    {
        rp = rdy_head[q] ;
        while (rp != NIL_PROC)
        {
            e9_byte(q) ;                                             /* >> */
            e9_byte(rp->p_nr) ;                                      /* >> */
            rp = rp->p_nextready ;
        }
    }
}

void mostrar_PID ( struct proc * rp )
{
    if (rp->p_nr >= 0)
    {
        e9_word(pmproc_addr[rp->p_nr]->mp_pid) ;    /* PID y p_nr (indice) */
        e9_byte(rp->p_nr) ;
    }
    else
    {
        e9_word(rp->p_nr) ;                               /* [p_nr] y nada */
        e9_byte(rp->p_nr) ;
    }
}

void mostrar_nombre ( struct proc * rp )
{
#if 0
    if (rp->p_nr >= 0)
        e9_printf("%-8s%-16s", rp->p_name, pmproc_addr[rp->p_nr]->mp_name) ;
    else
        e9_printf("%-8s%-16s", rp->p_name, "") ;
#else
    char * ptr ;
    int i ;
    i = 0 ;
    ptr = rp->p_name ;
    while ((i++ < 8) && (*ptr > ' '))
        e9_car(*ptr++) ;
    e9_car('\0') ;
    if (rp->p_nr >= 0)
    {
        i = 0 ;
        ptr = pmproc_addr[rp->p_nr]->mp_name ;
        while ((i++ < 16) && (*ptr > ' '))
            e9_car(*ptr++) ;
    }
    e9_car('\0') ;
#endif
}

void mostrar_llamada ( struct proc * rp_par, unsigned op_par)
{
    unsigned op_h = (op_par >> 16) ;                 /* src_dst_e, hook_id */
    unsigned op_m = (op_par >> 8) & 0x000000FF ;                    /* irq */
    unsigned op_l = (op_par & 0x000000FF) ;                   /* operacion */

    message * m_ptr_usr ;
    message * m_ptr ;
    int syscallnr ;
    int src_dst_e ;

/*  precondicion:
    switch ( op_l ) {
        case opSEND      : ;
        case opRECEIVE   : ;
        case opSENDREC   : ;
        case opNOTIFY    : ;
        case opECHO      : ;
        case opL_SEND    : ;
        case opL_NOTIFY  : break ;
        default          : return ;
    }
*/

/*  en este caso el parametro rp_par es el mensaje */
    m_ptr_usr = (message *)rp_par ; /* puntero en el espacio del usuario */
    m_ptr = (message *)
/*      (((proc[NR_TASKS + proc_ptr->p_nr].p_memmap[D].mem_phys - */
        (((pproc_addr[NR_TASKS + proc_ptr->p_nr]->p_memmap[D].mem_phys -
            kernel_D_CLICK
          ) << CLICK_SHIFT /* 12 */
         ) + (unsigned)m_ptr_usr
        ) ;

    syscallnr = m_ptr->m_type ;
    e9_word(syscallnr) ;
    src_dst_e = (int)((short int)op_h) ;
    e9_word(src_dst_e) ;

}

void mostrar_bloque_llamada ( struct proc * rp, unsigned op )
{
    mostrar_endequeue(proc_ptr) ; /* proceso en ejecucion */
    mostrar_llamada(rp, op) ;
}

void mostrar_endequeue ( struct proc * rp )
{
    mostrar_PID(rp) ;
#if 0
    e9_dword(*((unsigned *)&rp->p_priority)) ;
#else
    e9_byte(rp->p_priority) ;
    e9_byte(rp->p_max_priority) ;
    e9_byte(rp->p_ticks_left) ;
    e9_byte(rp->p_quantum_size) ;
#endif
    e9_byte(rp->p_rts_flags & 0x000000FF) ;
    mostrar_nombre(rp) ;
}

void mostrar_status ( struct proc * rp )
{
    short p_rts_flags = rp->p_rts_flags & 0x000000FF ;
    mostrar_PID(rp) ;
    e9_byte(p_rts_flags) ;
}

#if 0 /* HASTA FIN */

#include "../include/minix/callnr.h"                /* llamadas al sistema */
#include "../include/minix/com.h"                /* llamadas de las tareas */

#include "tabla_1.c"                  /* nombre de las llamadas al sistema */
#include "tabla_2.c"                  /* nombre de las llamadas al sistema */

#include "e9_system.h"       /* e9_hack, e9_hackstr, e9_hackint, e9_printf */

#define e9_byte( x )  e9_hackraw((char *)&(x), 1) ;
#define e9_word( x )  e9_hackraw((char *)&(x), 2) ;
#define e9_dword( x ) e9_hackraw((char *)&(x), 4) ;

#include "ll_s_plot.h"                /* PLOT_ON, PLOT_PID, PLOT_PRIO, ... */

#if 0
char strOP [ ] [10] = {       /* no se usa en la implementacion compactada */
   "---------", /*  0 */
   "SVC",       /*  1 */
   "INT",       /*  2 */
   "EXC",       /*  3 */

   "ENQUEUE",   /*  4 */
   "DEQUEUE",   /*  5 */
   "SCHED",     /*  6 */
   "PICKPROC",  /*  7 */

   "SEND",      /*  8 */ /* ver kernel/ipc.h SEND    1 0001 */
   "RECEIVE",   /*  9 */ /* ver kernel/ipc.h RECEIVE 2 0010 */
   "SENDREC",   /* 10 */ /* ver kernel/ipc.h SENDREC 3 0011 */
   "NOTIFY",    /* 11 */ /* ver kernel/ipc.h NOTIFY  4 0100 */
   "ECHO",      /* 12 */ /* ver kernel/ipc.h ECHO    8 1000 */

   "L_ENQUEUE", /* 13 */
   "L_DEQUEUE", /* 14 */

   "L_SEND",    /* 15 */
   "L_NOTIFY",  /* 16 */

   "DO_FORK",   /* 17 */
   "DO_EXIT",   /* 18 */

   "BALANCE"    /* 19 */
} ;
#endif

/* !!!!!!!!! desplazamientos de las variables: mproc y realtime (A CAPON)  */
/* !!!!!!!!! podrian cambiar si se modifica el nucleo de Minix             */

#define MPROC    0x0000ba68  /* nm /usr/src/servers/pm/pm    | grep mproc  */ /* BSS  */

#define REALTIME (*((long *)0x1EA0))   /* nm kernel | realtim ----> 0x1EA0 */

extern unsigned char devio_scan_code ;       /* /usr/src/kernel/do_devio.c */

void mostrar_procesos ( struct mproc * ptr_mproc ) ;

void mostrar_preparados ( struct mproc * ptr_mproc ) ;

void mostrar_cola ( struct mproc * ptr_mproc ) ;

void mostrar_PID ( struct proc * rp ) ;

void mostrar_nombre ( struct proc * rp ) ;

void mostrar_llamada ( struct proc * rp_par, unsigned op_par) ;

void mostrar_bloque_llamada ( struct proc * rp_par, unsigned op_par ) ;

void mostrar_endequeue ( struct proc * rp_par ) ;

void mostrar_status ( struct proc * rp ) ;

void plotear ( struct proc * rp_par, unsigned op_par )
{
    unsigned op_h = (op_par >> 16) ;                 /* src_dst_e, hook_id */
    unsigned op_m = (op_par >> 8) & 0x000000FF ;                    /* irq */
    unsigned op_l = (op_par & 0x000000FF) ;                   /* operacion */

    int irq ;              /* numero de la pata de interrupcion solicitada */
    int hook_id ;
    int exc ;                                    /* numero de la excepcion */

    int i ;

    unsigned clock_realtime ;
    unsigned clock_counter ;

/* ----------------------------------------------------------------------- */

    if (plot_flags != 0x00000000)                 /* procesamos plot_flags */
    {                                                   /* ver ll_s_plot.h */
/*
        e9_printf(
            "\n"
            "\n"
            "  plot_flags = %08X  plot_acum = %08X  plot_sched = %08X \n",
            plot_flags, plot_acum, plot_sched
        ) ;
*/
        if (plot_flags & PLOT_ON_) plot_acum = 0x00000000 ;
        else
        {
            plot_acum |=   (plot_flags & 0x55555555) ;
            plot_acum &= ~((plot_flags & 0xAAAAAAA8) >> 1) ;
        }

        plot_flags = 0x00000000 ;                      /* borro plot_flags */
/*
        e9_printf(
            "  plot_flags = %08X  plot_acum = %08X  plot_sched = %08X \n",
            plot_flags, plot_acum, plot_sched
        ) ;
*/
    }

/* ----------------------------------------------------------------------- */

    if ((plot_acum & PLOT_ON) == 0x00000000) return ;

    if ((op_l < opMIN) || (opMAX < op_l)) return ;

/* ----------------------------------------------------------------------- */

    if (ptr_mproc == 0x00000000)                   /* calculamos ptr_mproc */
    {
/*      if (rp->p_nr >= 0) */                 /* con esto tambien funciona */
/*      if (proc[NR_TASKS + PM_PROC_NR].p_memmap[D].mem_phys == 0x00000000)*/
        if (pproc_addr[NR_TASKS + PM_PROC_NR]->p_memmap[D].mem_phys == 0x00000000)
            return ;                            /* debe estar inicializado */
                                                /* solo se ejecuta una vez */
/*      kinfo.data_base = 0x8000 ; *//* valor normal en el kernel original */
/*      kernel_D_CLICK = 8 ; */      /* valor normal en el kernel original */
/*      kernel_D_CLICK = proc[NR_TASKS + KERNEL].p_memmap[D].mem_phys ;    */
        kernel_D_CLICK = kinfo.data_base >> CLICK_SHIFT ;       /* start.c */
                   /* kernel.h, <minix/type.h>, mpx386.s, start.c y main.c */
        ptr_mproc = (struct mproc *)
/*          (((proc[NR_TASKS + PM_PROC_NR].p_memmap[D].mem_phys -          */
            (((pproc_addr[NR_TASKS + PM_PROC_NR]->p_memmap[D].mem_phys -
               kernel_D_CLICK
              ) << CLICK_SHIFT /* 12 */
             ) + MPROC
            ) ;

        for ( i = 0 ; i < NR_PROCS ; i++ )
            pmproc_addr[i] = &ptr_mproc[i] ;      /* para mayor eficiencia */

        contador = 0 ;                          /* contador de operaciones */

#if 0                                                   /* para depuracion */
        e9_printf("%08X ", (unsigned)ptr_mproc) ;
        e9_printf("\n kernel_D_CLICK = %08X ", (unsigned)kernel_D_CLICK) ;
        e9_printf("\n ptr_mproc = %08X ", (unsigned)ptr_mproc) ;
        e9_printf("\n prueba = %08X ", (unsigned)0x12345678) ;
        e9_printf("\n PM_PROC_NR = %u ", PM_PROC_NR) ;
        e9_printf("\n NR_TASKS + PM_PROC_NR = %u ", NR_TASKS + PM_PROC_NR) ;
        e9_printf("\n proc[NR_TASKS + PM_PROC_NR].p_memmap[D].mem_phys = %08X ",
                      proc[NR_TASKS + PM_PROC_NR].p_memmap[D].mem_phys) ;
        e9_printf("\n proc[NR_TASKS + KERNEL].p_memmap[D].mem_phys = %08X ",
                      proc[NR_TASKS + KERNEL].p_memmap[D].mem_phys) ;
        for ( ; ; ) ;
#endif
    }

/* ----------------------------------------------------------------------- */
/* plotear ( rp_par, op_h, op_m, op_l ) ;                                  */
/* ----------------------------------------------------------------------- */
/*
    op          origen            significado
    =========== ================= ========================================
    opSVC       proc.c            llamada al sistema   plotear((struct proc *)m_ptr, (src_dst_e << 16) | opSEND + function - 1) ; (proc_ptr)
    opINT       i8259.c           interrupcion         plotear(NIL_PROC, (hook->id << 16) | (hook->irq << 8) | opINT) ; (proc_ptr)
    opEXC       exception.c       excepcion            plotear(NIL_PROC, (vec_nr << 16) | opEXC) ; (proc_ptr)

    opENQUEUE   proc.c                                                                   plotear(rp, opENQUEUE) ; (proc_ptr)
    opDEQUEUE   proc.c                                                                   plotear(rp, opDEQUEUE) ; (proc_ptr)
    opSCHED     proc.c            decide el lugar del proceso en la cola de preparados   plotear(NIL_PROC, (queue << 16) | (front << 8) | opSCHED) ;
    opPICKPROC  proc.c            decide el siguiente proceso a ejecutar (next_ptr)      plotear(NIL_PROC, opPICKPROC) ; (next_ptr)

    opSEND      proc.c            los 5 tipos de llamada al sistema (SVC)  plotear((struct proc *)m_ptr, (src_dst_e << 16) | opSEND) ;
    opRECEIVE   proc.c                                                     plotear((struct proc *)m_ptr, (src_dst_e << 16) | opRECEIVE) ;
    opSENDREC   proc.c                                                     plotear((struct proc *)m_ptr, (src_dst_e << 16) | opSENDREC) ;
    opNOTIFY    proc.c                                                     plotear((struct proc *)m_ptr, (src_dst_e << 16) | opNOTIFY) ;
    opECHO      proc.c                                                     plotear((struct proc *)m_ptr, (src_dst_e << 16) | opECHO) ;

    opL_ENQUEUE proc.c            invocada por interrupciones o tareas     plotear(rp, opL_ENQUEUE) ; (proc_ptr)
    opL_DEQUEUE proc.c            invocada por interrupciones o tareas     plotear(rp, opDEQUEUE) ; (proc_ptr)

    opL_SEND    proc.c            invocada por interrupciones o tareas     plotear((struct proc *)m_ptr, (dst_e << 16) | opL_SEND) ;
    opL_NOTIFY  proc.c            invocada por interrupciones o tareas     plotear(NIL_PROC, (dst << 16) | opL_NOTIFY) ;

    opDO_FORK   system/do_fork.c  rpp crea un proceso hijo rpc             plotear(rpp, (rpc->p_endpoint << 16) | (rpc->p_nr << 8) | opDO_FORK) ;
    opDO_EXIT   system/do_exit.c  se destruye un proceso: SLOT_FREE        plotear(pproc_addr(exit_e), opDO_EXIT) ;
                                                                           plotear(pproc_addr(who_p), opDO_EXIT) ;

    opBALANCE   proc.c            se aumenta la priodidad de un proceso    plotear(rp, opBALANCE) ;
                                  invocada por clock.c

    opSTATUS    proc.c            para informar de un cambio de estado     plotear(rp, opSTATUS) ;
                                  que queda por reportar
*/
/* ----------------------------------------------------------------------- */

    if (contador == 0)
    {
        mostrar_procesos(ptr_mproc) ;
/*      mostrar_preparados(ptr_mproc) ; */
        mostrar_cola(ptr_mproc) ;
    }
    contador++ ; /* no se usa mas */

    clock_realtime = REALTIME ;    /* !!!!!!!! tomado a capon (ver define) */
    e9_dword(clock_realtime) ;
    clock_counter = read_clock() ;
    e9_word(clock_counter) ;
    e9_byte(op_l) ;

    switch (op_l) {

        case opSVC       : return ;

        case opINT       : irq = (int)op_m ;
                           hook_id = (int)op_h ;
                           e9_byte(irq) ;
                           e9_byte(hook_id) ;
                           if (irq == 1)    /* TECLADO */
                           {
                               if (devio_scan_code != 0xFF) {
                                   e9_byte(devio_scan_code) ;
                                   devio_scan_code = 0xFF ;
                               }
                               else
                               {        /* /usr/src/lib/i386/misc/io_inb.s */
                                   devio_scan_code = inb(0x60) ;
                                   e9_byte(devio_scan_code) ;
                               }
                           }
                           return ;

        case opEXC       : exc = (int)op_h ;
                           e9_byte(exc) ;
                           return ;

        case opENQUEUE   : ;
        case opDEQUEUE   : mostrar_endequeue(rp_par) ;
                           return ;

        case opSCHED     : mostrar_PID(rp_par) ;
                           e9_byte(op_h) ; /* q */
                           e9_byte(op_m) ; /* front */
                           return ;

        case opPICKPROC  : mostrar_PID(next_ptr) ;
                           mostrar_cola(ptr_mproc) ; /* PP para depurar */
                           return ;

        case opSEND      : ;
        case opRECEIVE   : ;
        case opSENDREC   : ;
        case opNOTIFY    : ;
        case opECHO      : mostrar_bloque_llamada(rp_par, op_par) ;
                           return ;

        case opL_ENQUEUE : ;
        case opL_DEQUEUE : mostrar_endequeue(rp_par) ;
                           return ;

        case opL_SEND    : ;
        case opL_NOTIFY  : mostrar_bloque_llamada(rp_par, op_par) ;
                           return ;

        case opDO_FORK   : mostrar_PID(proc_ptr) ; /* proceso en ejecucion (pm) */
                           mostrar_PID(rp_par) ;   /* proceso que crea (FORK) */
                           e9_word(op_h) ;         /* mp_pid del proceso que se crea */
                           e9_byte(op_m) ;         /* p_nr del proceso que se crea */
                           return ;

        case opDO_EXIT   : mostrar_PID(proc_ptr) ; /* proceso en ejecucion (pm) */
                           mostrar_PID(rp_par) ;   /* proceso que termina (EXIT) */
                           return ;

        case opBALANCE   : mostrar_endequeue(rp_par) ;
                           return ;

        case opSTATUS    : mostrar_status(rp_par) ; /* cambio de estado de rp_par */
                           return ;

        default          : ;
    }

}

void mostrar_procesos ( struct mproc * ptr_mproc )
{
    int i ;
    struct proc * rp ;
    struct mproc * mrp ;
    short p_rts_flags ;
    unsigned numProcesos ;

    e9_byte(proc_ptr->p_nr) ;

    numProcesos = 0 ;
    for ( i = 0 ; i < (NR_TASKS + NR_PROCS) ; i++ )
        if (pproc_addr[i]->p_rts_flags != SLOT_FREE)
            numProcesos++ ;

    e9_byte(numProcesos) ;

    for ( i = 0 ; i < (NR_TASKS + NR_PROCS) ; i++ )
    {
/*      rp  = (struct proc *)&proc[i] ; */
        rp  = pproc_addr[i] ;                 /* mas eficiente, ver proc.h */
        if (rp->p_rts_flags != SLOT_FREE)
        {
            mostrar_endequeue(rp) ;
            if (i < NR_TASKS)
                e9_hack('\0') ;
            else
                e9_byte(pmproc_addr[i-NR_TASKS]->mp_parent) ;    /* PPID */
        }
    }
}

#if 0

void mostrar_preparados ( struct mproc * ptr_mproc )
{
    int q ;
    struct proc * rp ;
    struct mproc * mrp ;

    e9_printf("\n\n Procesos preparados: \n") ;

    for ( q = 0 ; q < NR_SCHED_QUEUES ; q++ )
    {
        rp = rdy_head[q] ;
        while (rp != NIL_PROC)
        {
            e9_printf("\n") ;
            if (rp->p_nr >= 0)                          /* (i >= NR_TASKS) */
            {
                mrp = (struct mproc *) &ptr_mproc[rp->p_nr] ;
                e9_printf(" %3i  ""%2u ", mrp->mp_pid, rp->p_nr) ;  /* PID y p_nr (indice) */
            }
            else
                e9_printf(" [%2i] ""   ", rp->p_nr) ;               /* [p_nr] y nada */
            e9_printf("%2u (%2u) ", rp->p_priority, rp->p_max_priority) ;
            e9_printf("%2u ", rp->p_ticks_left) ;
            e9_printf("%2u ", rp->p_quantum_size) ;
            e9_printf("%-8s", rp->p_name) ;
            if (rp->p_nr >= 0)
                e9_printf("%-16s", mrp->mp_name) ;
            else
                e9_printf("%-16s", "") ;

            rp = rp->p_nextready ;
        }
    }
    e9_printf("\n") ;
}

#endif

void mostrar_cola ( struct mproc * ptr_mproc )
{
    int q ;
    struct proc * rp ;

    unsigned numProcesosPreparados ;
    numProcesosPreparados = 0 ;
    for ( q = (NR_SCHED_QUEUES-1) ; q >= 0 ; q-- )
    {
        rp = rdy_head[q] ;
        while (rp != NIL_PROC)
        {
            numProcesosPreparados++ ;
            rp = rp->p_nextready ;
        }
    }
    e9_byte(numProcesosPreparados) ;

    for ( q = (NR_SCHED_QUEUES-1) ; q >= 0 ; q-- )
    {
        rp = rdy_head[q] ;
        while (rp != NIL_PROC)
        {
            e9_byte(q) ;
            e9_byte(rp->p_nr) ;
            rp = rp->p_nextready ;
        }
    }
}

void mostrar_PID ( struct proc * rp )
{
    if (rp->p_nr >= 0)
    {
        e9_word(pmproc_addr[rp->p_nr]->mp_pid) ;    /* PID y p_nr (indice) */
        e9_byte(rp->p_nr) ;
    }
    else
    {
        e9_word(rp->p_nr) ;                               /* [p_nr] y nada */
        e9_byte(rp->p_nr) ;
    }
}

void mostrar_nombre ( struct proc * rp )
{
#if 0
    if (rp->p_nr >= 0)
        e9_printf("%-8s%-16s", rp->p_name, pmproc_addr[rp->p_nr]->mp_name) ;
    else
        e9_printf("%-8s%-16s", rp->p_name, "") ;
#else
    char * ptr ;
    int i ;
    i = 0 ;
    ptr = rp->p_name ;
    while ((i++ < 8) && (*ptr > ' '))
        e9_hack(*ptr++) ;
    e9_hack('\0') ;
    if (rp->p_nr >= 0)
    {
        i = 0 ;
        ptr = pmproc_addr[rp->p_nr]->mp_name ;
        while ((i++ < 16) && (*ptr > ' '))
            e9_hack(*ptr++) ;
    }
    e9_hack('\0') ;
#endif
}

void mostrar_llamada ( struct proc * rp_par, unsigned op_par)
{
    unsigned op_h = (op_par >> 16) ;                 /* src_dst_e, hook_id */
    unsigned op_m = (op_par >> 8) & 0x000000FF ;                    /* irq */
    unsigned op_l = (op_par & 0x000000FF) ;                   /* operacion */

    message * m_ptr_usr ;
    message * m_ptr ;
    int syscallnr ;
    int src_dst_e ;

/*  precondicion:
    switch ( op_l ) {
        case opSEND      : ;
        case opRECEIVE   : ;
        case opSENDREC   : ;
        case opNOTIFY    : ;
        case opECHO      : ;
        case opL_SEND    : ;
        case opL_NOTIFY  : break ;
        default          : return ;
    }
*/

/*  en este caso el parametro rp_par es el mensaje */
    m_ptr_usr = (message *)rp_par ; /* puntero en el espacio del usuario */
    m_ptr = (message *)
/*      (((proc[NR_TASKS + proc_ptr->p_nr].p_memmap[D].mem_phys - */
        (((pproc_addr[NR_TASKS + proc_ptr->p_nr]->p_memmap[D].mem_phys -
            kernel_D_CLICK
          ) << CLICK_SHIFT /* 12 */
         ) + (unsigned)m_ptr_usr
        ) ;

    syscallnr = m_ptr->m_type ;
    e9_word(syscallnr) ;
    src_dst_e = (int)((short int)op_h) ;
    e9_word(src_dst_e) ;

}

void mostrar_bloque_llamada ( struct proc * rp, unsigned op )
{
    mostrar_endequeue(proc_ptr) ; /* proceso en ejecucion */
    mostrar_llamada(rp, op) ;
}

void mostrar_endequeue ( struct proc * rp )
{
    short p_rts_flags ;
    mostrar_PID(rp) ;
    e9_byte(rp->p_priority) ;
    e9_byte(rp->p_max_priority) ;
    e9_byte(rp->p_ticks_left) ;
    e9_byte(rp->p_quantum_size) ;
    p_rts_flags = rp->p_rts_flags & 0x000000FF ;
    e9_byte(p_rts_flags) ;
    mostrar_nombre(rp) ;
}

void mostrar_status ( struct proc * rp )
{
    short p_rts_flags = rp->p_rts_flags & 0x000000FF ;
    mostrar_PID(rp) ;
    e9_byte(p_rts_flags) ;
}

#endif /* HASTA FIN */
