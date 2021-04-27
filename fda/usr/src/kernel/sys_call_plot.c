/* ----------------------------------------------------------------------- */
/*                             sys_call_plot.c                             */
/* ----------------------------------------------------------------------- */
/*   manejador de la llamada al sistema ll_s_plot (INT21 ECX = B1OC0FF)    */
/* ----------------------------------------------------------------------- */

/* #include "proc.h" */                    /* proc (descriptor de proceso) */
                                               /* proc (tabla de procesos) */
                       /* RECEIVING (campo p_rts_flags, proceso bloqueado) */
                              /* p_nextready (campo para enlazar procesos) */
                                     /* NIL_PROC (fin de cola de procesos) */
/* ----------------------------------------------------------------------- */
                                 /* enqueue(rp): funcion privada de proc.c */
                                 /*     (mete rp en la cola de preparados) */
                                 /* dequeue(rp): funcion privada de proc.c */
                                 /*     (saca rp de la cola de preparados) */

/* #include "glo.h" */                  /* proc_ptr (proceso en ejecucion) */

/* #include "errno.h" */                                       /* EBADCALL */

#include "kernel.h" 

#include "ll_s_plot.h"                                         /* PLOT_ON_ */

#include "e9_system.h"                                        /* e9_printf */

#include "plotear.h"                                          /* init_plot */

/* En plot_flags se usan los bits pares e impares. los pares activan y los */
/* impares desactivan. En plot_acum solo se usan los bits pares            */
/* 1 -> activo, 0 -> inactivo.                                             */

unsigned plot_flags = 0x00000000 ; /* ultimos flags enviados por ll_s_plot */
unsigned plot_acum =  0x00000000 ;    /* flags activos para generar trazas */
unsigned plot_sched = 0 ;      /* planificacion seleccionada por ll_s_plot */

char   * plot_bufer ;           /* bufer de car. pendientes de enviar a e9 */
char   * plot_in ;                   /* puntero al primer hueco disponible */
char   * plot_out ;                 /* puntero al primer caracter a enviar */
unsigned plot_count = 0 ;           /* numero de car. pendientes de enviar */

unsigned plot_gen = 0 ;        /* switch para indicar generacion de trazas */
unsigned plot_mem = 0 ;        /* switch para indicar el uso del bufer     */
unsigned plot_fin = 0 ;        /* switch para indicar el vaciado del bufer */

void mostrar_flags ( unsigned flags, 
                     unsigned acum, 
                     unsigned sched,
                     unsigned gen,
                     unsigned mem,
                     unsigned fin ) 
{
    e9_printf(
        "\n"
        "\n"
        "  flags = %08X  acum = %08X  sched = %08X  gen = %d  mem = %d  fin = %d \n",
        flags, acum, sched, gen, mem, fin
    ) ; 
}

/* orden de las llamadas ll_s_plot:                                        */
/* plot sched x          ll_s_plot(0x00000000, x) ;                        */
/* plot on mem           ll_s_plot(0x00000005, x) ;                        */
/* plot off       repite ll_s_plot(0x00000002, _) ; hasta obtener 0        */

int sys_call_plot ( unsigned call_nr, 
                    unsigned flags, 
                    unsigned sched, 
                    unsigned reg_edx ) 
{                                                 
                                        /* call_nr = reg_ecx == 0xB1OC00FF */

/*  struct proc * rp ; */         /* runnable process (proceso ejecutable) */
    int nbytes ;
    plot_flags = flags ;
    plot_sched = sched ;
    
#if 0
    mostrar_flags(plot_flags, plot_acum, plot_sched, 
                  plot_gen, plot_mem, plot_fin) ;   
#endif

    if (plot_fin) 
    {       
        if (plot_count < 512) 
        {   
            nbytes = plot_count ;
            plot_fin = 0 ;
            plot_mem = 0 ;
        }   
        else    
            nbytes = 512 ;
        
        e9_raw(plot_out, nbytes) ;
        plot_out = plot_out + nbytes ; 
		plot_count = plot_count - nbytes ; 
        return plot_count ;
    }       
    else 
    {
        plot_acum |=   (plot_flags & 0x55555555) ;
        plot_acum &= ~((plot_flags & 0xAAAAAAA8) >> 1) ;
        
        if (!(plot_gen) && (plot_flags & PLOT_ON))              /* plot on */
        {
			init_plot(plot_acum, plot_sched) ;
/* borrar:			
            plot_gen = 1 ;
            if (plot_flags & PLOT_MEM) 
            {
                plot_mem = 1 ;
                plot_count = 0 ;
                plot_in = plot_bufer ;
                plot_out = plot_bufer ;
            } 
*/			
        }   
        else if (plot_flags & PLOT_ON_)                        /* plot off */
        {
            plot_gen = 0 ;
            if (plot_mem) 
			{	
                plot_fin = 1 ;
				return plot_count ;
			}	
        }       
    }

#if 0
    mostrar_flags(plot_flags, plot_acum, plot_sched, 
                  plot_gen, plot_mem, plot_fin) ;   
#endif

/*  return EBADCALL ; */
    return 0 ;
}
