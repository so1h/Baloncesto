/* ----------------------------------------------------------------------- */
/*                              e9_hackint.c                               */
/* ----------------------------------------------------------------------- */
/*                 implementacion modulo/unidad/biblioteca                 */
/* ----------------------------------------------------------------------- */

#include "e9_system.h"                                          /* e9_hack */

void e9_hackint ( int n )
{
    int i ;
    int pot ;
    int n_div_10 ;

    if (n < 0) 
    { 
        e9_hack('-') ; 
        n = -n ; 
    }
    
    if (n < 10) e9_hack('0' + n) ;
    else
    {
        pot = 10 ;
        n_div_10 = n / 10 ;
        while (pot <= n_div_10) pot = 10 * pot ;
        
        while (pot > 0)
        {
            e9_hack('0' + n/pot) ;
            n = n % pot ;
            pot = pot / 10 ;
        }
    }
}
