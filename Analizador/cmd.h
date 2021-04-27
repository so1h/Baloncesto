//---------------------------------------------------------------------------

#ifndef cmdH
#define cmdH
//---------------------------------------------------------------------------

#include <system.hpp>                                        /* AnsiString */
#include <vcl.h>                       /* FileCreate, FileWrite, FileClose */

long SetScreenResolution ( int width, int height ) ;

bool CrearCmdWait ( wchar_t * cmd,
					wchar_t * dir,
					wchar_t * env,
					wchar_t * titulo,
					bool visible,
					int  width,
					int  height,
					bool esperarTerminacion ) ; 
					
bool CrearCmd ( wchar_t * cmd,
				wchar_t * dir,
				wchar_t * env,
				wchar_t * titulo,
				bool      visible,
				int       width,
				int       height ) ;
				
				
/* ------------------------------------------------------------------------*/
/* void obtener_ejecutable_firefox ( )                                     */
/* ------------------------------------------------------------------------*/

void obtener_ejecutable_firefox ( void ) ;

/* ------------------------------------------------------------------------*/
/* void mostrar_enlace ( enlace )                                          */
/* ------------------------------------------------------------------------*/

void mostrar_enlace ( AnsiString enlace ) ;

/* ------------------------------------------------------------------------*/
/* borrar_enlace ( enlace )                                                */
/* ------------------------------------------------------------------------*/

void borrar_enlace ( void ) ;


/* ------------------------------------------------------------------------*/
/* void obtener_ejecutable_excel ( )                                       */
/* ------------------------------------------------------------------------*/

void obtener_ejecutable_excel ( void ) ;
				
/* ------------------------------------------------------------------------*/
/* void mostrar_fichero_excel ( FileName )                                 */
/* ------------------------------------------------------------------------*/
				
void mostrar_fichero_excel ( AnsiString FileName ) ;				
				
				
/* ------------------------------------------------------------------------*/
/* void obtener_ejecutable_chm ( )                                         */
/* ------------------------------------------------------------------------*/

void obtener_ejecutable_chm ( void ) ;

/* ------------------------------------------------------------------------*/
/* void mostrar_fichero_chm ( FileName )                                   */
/* ------------------------------------------------------------------------*/

void mostrar_fichero_chm ( AnsiString FileName ) ;


/* ------------------------------------------------------------------------*/
/* void mostrar_fichero_pdf ( FileName, pagina )                           */
/* ------------------------------------------------------------------------*/

void mostrar_fichero_pdf ( AnsiString FileName, int pagina ) ;
								
#endif /* cmdH */
