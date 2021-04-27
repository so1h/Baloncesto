/* ----------------------------------------------------------------------- */
/*                               e9_printf.c                               */
/* ----------------------------------------------------------------------- */
/*                 implementacion modulo/unidad/biblioteca                 */
/* ----------------------------------------------------------------------- */

#include "e9_system.h"                                       /* e9_hackstr */

#include <stdarg.h>

extern int vsprintf ( char * buf, const char * fmt, va_list args ) ;

static char bufer [256] ;

int e9_printf ( const char * format, ... )
{
    va_list aptr ;
    int ret ;

    va_start(aptr, format) ;
    ret = vsprintf((char *)&bufer, format, aptr) ;
    va_end(aptr) ;

    e9_str(bufer) ;

    return(ret) ;
}

