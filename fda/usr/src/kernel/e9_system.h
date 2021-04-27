/* ----------------------------------------------------------------------- */
/*                               e9_system.h                               */  
/* ----------------------------------------------------------------------- */
/*                   definicion modulo/unidad/biblioteca                   */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* [Qemu-devel] [PATCH] Port E9 hack, for debugging purposes:              */
/*                                                                         */
/*   https://lists.gnu.org/archive/html/qemu-devel/2005-01/msg00169.html   */
/*                                                                         */

#ifndef E9_SYSTEM_H
#define E9_SYSTEM_H

/* funciones que solo pueden invocarse desde el kernel (usan in y out)     */

void e9_car ( char car ) ;               /* escribe car por el puerto 0xE9 */

void e9_byte ( char b ) ;                  /* escribe b por el puerto 0xE9 */

void e9_word ( short w ) ;                 /* escribe w por el puerto 0xE9 */

void e9_dword ( unsigned dw ) ;           /* escribe dw por el puerto 0xE9 */

void e9_str ( char * str ) ;             /* escribe str por el puerto 0xE9 */

void e9_raw ( char * ptr, unsigned l ) ;       /* escribe l bytes por 0xE9 */

void e9_int ( int n ) ;                    /* escribe n por el puerto 0xE9 */

void e9_hex ( unsigned n, unsigned ancho ) ;        /* escribe n en hexad. */

int  e9_printf ( const char * format, ... ) ;           /* printf por 0xE9 */

/* llamadas al sistema añadidas para ser invocadas por el usuario          */
/* Se asegura que todos los bytes enviados en la llamada se escriben de    */
/* manera consecutiva por el puerto 0xE9                                   */ 

void u9_car ( char car ) ;               /* escribe car por el puerto 0xE9 */

void u9_byte ( char b ) ;                  /* escribe b por el puerto 0xE9 */

void u9_word ( short w ) ;                 /* escribe w por el puerto 0xE9 */

void u9_dword ( unsigned dw ) ;           /* escribe dw por el puerto 0xE9 */

void u9_str ( char * str ) ;             /* escribe str por el puerto 0xE9 */

void u9_raw ( char * ptr, unsigned l ) ;       /* escribe l bytes por 0xE9 */

void u9_int ( int n ) ;                    /* escribe n por el puerto 0xE9 */

void u9_hex ( unsigned n, unsigned ancho ) ;        /* escribe n en hexad. */

int  u9_printf ( const char * format, ... ) ;           /* printf por 0xE9 */

#endif /* E9_SYSTEM_H */
