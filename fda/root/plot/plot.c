/* ----------------------------------------------------------------------- */
/*                                  plot.c                                 */
/* ----------------------------------------------------------------------- */
/*                               comando plot                              */
/* ----------------------------------------------------------------------- */

#include <stdio.h>                                      /* printf, getchar */
#include <string.h>                                              /* strcmp */
#include "ll_s_plot.h"                     /* ll_s_plot, PLOT_ON, PLOT_MEM */

void formato ( void )
{
    printf(
"                                                                          \n"
"       formato: plot [ -h | { on | off | mem | e9 | sched num } ]         \n"
    ) ;
    getchar() ;
}

void help ( void )
{
    printf(
"                                                                          \n"
"NOMBRE                                                                    \n"
"     plot  Controla la generacion de trazas de Minix                      \n"
"                                                                          \n"
"SYNOPSIS                                                                  \n"
"     plot [ -h | { on | mem | e9 | off | sched num } ]                    \n"
"                                                                          \n"
"DESCRIPCION                                                               \n"
"     el comando plot on pone en marcha el volcado continuo de informacion \n"
"     sobre el estado de Minix. El comando plot off detiene ese volcado    \n"
"     inmediatamente. La informacion se envia por defecto directamente al  \n"
"     puerto E9. También puede enviarse primero a un bufer en memoria      \n"
"     utilizando el comando plot mem. En ese caso, tras desactivarse la    \n"
"     generacion de trazas, el bufer se copia al puerto E9. La opcion      \n"
"     sched de plot permite solicitar al sistema el cambio a una politica  \n"
"     de planificacion diferente a la actual en Minix (proc.c).            \n"
"                                                                          \n"
"OPCIONES                                                                  \n"
"     -h      muestra este help (por la salida estandar)                   \n"
"     on      comienza la visualizacion de la informacion por el puerto E9 \n"
"     mem     envia las trazas primero a un bufer en memoria               \n"
"     sched   solicita la politica de planificacion num indicada           \n"
"     off     termina la visualizacion de la informacion por el puerto E9  \n"
    ) ;
    getchar() ;
    printf(
"EJEMPLOS:    1) plot on   2) plot on mem   3) plot sched 1   4) plot off  \n"
"                                                                          \n"
"AUTORA:      (C) 2021 Cristina Ruiz Puerto                                \n"
    ) ;
    getchar() ;	
}

#define f flags                               /* simplemente para abreviar */

int main ( int argc, char * argv [ ] )
{
    unsigned flags = 0 ;        /* para configurar la generacion de trazas */
    unsigned sched_num = 0 ;      /* para cambiar el tipo de planificacion */
    unsigned nbytes ;           /* numero de bytes enviados al puerto 0xE9 */
    unsigned total ;                    /* numero total de bytes generados */
    int i ;

    if (argc == 1)
    {
        formato() ;
        return 0 ;
    }

    if (strcmp(argv[1], "-h") == 0)
    {
        help() ;
        return 0 ;
    }

    for ( i = 1 ; i < argc ; i++ ) 
    {
        if      (strcmp(argv[i], "on" ) == 0) f = (f| PLOT_ON  ) & ~PLOT_ON_ ; 
        else if (strcmp(argv[i], "off") == 0) f = (f| PLOT_ON_ ) & ~PLOT_ON  ;
        else if (strcmp(argv[i], "mem") == 0) f = (f| PLOT_MEM ) & ~PLOT_MEM_; 
        else if (strcmp(argv[i], "e9" ) == 0) f = (f| PLOT_MEM_) & ~PLOT_MEM ;
        else if (strcmp(argv[i], "sched") == 0) 
        {
            if ((i >= (argc-1)) || 
                (sscanf(argv[i+1], "%u", &sched_num) != 1)) 
            {
                printf(
                    "\n falta el argumento de sched o no es un numero \n\n"
                ) ;
                return(1) ;
            }   
            else 
            {
                i = i + 1 ;                      /* argv[i+1] ya procesado */
            }               
        }                   
        else
        {
            printf("\n argv[%d] = \"%s\" erroneo \n\n", i, argv[i]) ;
            return(2) ;
        }
    }
    
    if (f & PLOT_ON_) { 
        total = 0 ; 
        while ((nbytes = ll_s_plot(flags, sched_num)) != 0x00000000) 
        { 
            total = total + nbytes ;  
        } ;
        printf("\n numero total de bytes generados = %d \n\n", total) ;
    }     
    else {
        ll_s_plot(flags, sched_num) ;
    }   
    return(0) ;
}
