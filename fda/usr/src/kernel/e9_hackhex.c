/* ----------------------------------------------------------------------- */
/*                              e9_hackhex.c                               */
/* ----------------------------------------------------------------------- */
/*                 implementacion modulo/unidad/biblioteca                 */
/* ----------------------------------------------------------------------- */

#include "e9_system.h"                                          /* e9_hack */

static char dighex [ 17 ] = "0123456789ABCDEF" ; 

void e9_hackhex ( unsigned n, unsigned ancho )      /* escribe n en hexad. */
{
    
    int despl ;
    
    if (ancho < 1) return ;                    /* una posicion como minimo */
    
    if (ancho > 8) return ;                    /* 8 posiciones como maximo */
    
    despl = ancho * 4 ;
    
    do 
    {
        despl = despl - 4 ; 
        e9_hack(dighex[(n >> despl) & 0x0000000F]) ;
    }
    while (despl > 0) ;
    
}
