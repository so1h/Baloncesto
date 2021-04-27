/* ----------------------------------------------------------------------- */
/*                             sys_call_plot.h                             */
/* ----------------------------------------------------------------------- */
/*   manejador de la llamada al sistema ll_s_plot (INT21 ECX = B1OC0FF)    */
/* ----------------------------------------------------------------------- */

#ifndef SYS_CALL_PLOT_H
#define SYS_CALL_PLOT_H

extern unsigned plot_flags ;       /* ultimos flags enviados por ll_s_plot */
extern unsigned plot_acum ;           /* flags activos para generar trazas */
extern unsigned plot_sched ;   /* planificacion seleccionada por ll_s_plot */

extern char   * plot_bufer ;        /* bufer car. pendientes de envio a e9 */
extern char   * plot_in ;            /* puntero al primer hueco disponible */
extern char   * plot_out ;          /* puntero al primer caracter a enviar */
extern unsigned plot_count ;        /* numero de car. pendientes de enviar */

extern unsigned plot_gen ;     /* switch para indicar generacion de trazas */
extern unsigned plot_mem ;     /* switch para indicar el uso del bufer     */
extern unsigned plot_fin ;     /* switch para indicar el vaciado del bufer */

int sys_call_plot ( unsigned call_nr, 
                    unsigned flags, 
                    unsigned sched, 
					unsigned reg_edx ) ;

#endif /* SYS_CALL_PLOT_H */
