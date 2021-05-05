//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
//  Menus
	TMainMenu *MainMenu1;        /* Menu */
	TMenuItem *Fichero1;         /* Pestaña Fichero */   
	TMenuItem *Abrir1;           /* abre el fichero de trazas (Y:\log_e9.bin) */
	TMenuItem *Cerrar1;          /* cierra el fichero de trazas */
	TMenuItem *Salir1;           /* sale del programa */
	TMenuItem *Seguimiento1;     /* Pestaña seguimiento */
	TMenuItem *Inicio1;          /* muestra los primeros segundos */
	TMenuItem *SigOp1;           /* muestra la siguiente operacion */
	TMenuItem *SigTick1;         /* muestra el siguiente tick */
	TMenuItem *SigSeg1;          /* muestra el siguiente segundo */ 
	TMenuItem *AntSeg1;          /* muestra el anterior segundo */ 
	TMenuItem *StartStop1;       /* arranca/para la visualizacion continua */ 
	TMenuItem *Final1;           /* muestra los últimos segundos */
	TMenuItem *Parado1;          /* para la visualizacion continua (0 ticks/seg) */ 
	TMenuItem *Frecuencia15Hz1;  /* visualizacion continua a  15 ticks/seg */
	TMenuItem *Frecuencia30Hz1;  /* visualizacion continua a  30 ticks/seg */
	TMenuItem *Frecuencia45Hz1;  /* visualizacion continua a  45 ticks/seg */
	TMenuItem *Frecuencia60Hz1;  /* visualizacion continua a  60 ticks/seg */
	TMenuItem *Frecuencia90Hz1;  /* visualizacion continua a  90 ticks/seg */
	TMenuItem *Frecuencia120Hz1; /* visualizacion continua a 120 ticks/seg */
	TMenuItem *Frecuencia240Hz1; /* visualizacion continua a 240 ticks/seg */
	TMenuItem *Frecuencia480Hz1; /* visualizacion continua a 480 ticks/seg */
	TMenuItem *Frecuencia960Hz1; /* visualizacion continua a 960 ticks/seg */
	TMenuItem *Continuo1;        /* visualizacion continua a lo mas que se pueda */
	TMenuItem *Ayuda1;           /* Pestaña Ayuda */
	TMenuItem *Ayuda2;           /* Codigo de Minix */ 
	TMenuItem *usrsrcmpx386s1;             /* /usr/src/kernel/mpx386.s */
	TMenuItem *usrsrcmpx386s2;             /* /usr/src/kernel/i8259.c */
	TMenuItem *usrsrckernelexceptionc1;    /* /usr/src/kernel/mpx386 */
	TMenuItem *usrsrcdriversttykeyboardc1; /* /usr/src/kernel/mpx386 */
	TMenuItem *usrsrcdriversttykeyboardc2; /* /usr/src/kernel/mpx386 */ 
	TMenuItem *Sobre1;                     /* Sobre... (About) */ 
	
//  Dialogo	Abrir
	TOpenDialog *OpenDialog1;    /* dialogo para abrir el fichero de trazas */  
//
	TImage *Image1;              /* imagen para pintar el cronograma */
//
	TTimer *Timer1;              /* timer para la visualizacion de ticks */
//
	TTimer *TimerIndice;         /* timer para la creacion del indice de ticks */  
	TMenuItem *Ver1;
	TMenuItem *Ir_a1;            /* para ir a un determinado segundo */  
	TMenuItem *ScrollSeg1;       /* barra de desplazamiento para situarse */ 
	TPanel *Panel1;              /* para seleccionar un cierto tick */ 
	TPanel *Panel2;              /* para contener StatusBar1 y RichEdit1 */  
	TStatusBar *StatusBar1;      /* indica el numero de segundo y de tick seleccionado */ 
	TRichEdit *RichEdit1;
	TMenuItem *nOpsTick1;        /* detalla las operaciones dentro del tick seleccionado */
//  Menus Clicks
	void __fastcall Abrir1Click(TObject *Sender);
	void __fastcall Cerrar1Click(TObject *Sender);
	void __fastcall Salir1Click(TObject *Sender);
	void __fastcall Inicio1Click(TObject *Sender);
	void __fastcall SigOp1Click(TObject *Sender);
	void __fastcall SigTick1Click(TObject *Sender);
	void __fastcall AntTick1Click(TObject *Sender);
	void __fastcall SigSeg1Click(TObject *Sender);
	void __fastcall AntSeg1Click(TObject *Sender);
	void __fastcall StartStop1Click(TObject *Sender);
	void __fastcall Final1Click(TObject *Sender);
	void __fastcall Parado1Click(TObject *Sender);
	void __fastcall Frecuencia15Hz1Click(TObject *Sender);
	void __fastcall Frecuencia30Hz1Click(TObject *Sender);
	void __fastcall Frecuencia45Hz1Click(TObject *Sender);
	void __fastcall Frecuencia90Hz1Click(TObject *Sender);
	void __fastcall Frecuencia60Hz1Click(TObject *Sender);
	void __fastcall Frecuencia120Hz1Click(TObject *Sender);
	void __fastcall Frecuencia240Hz1Click(TObject *Sender);
	void __fastcall Frecuencia480Hz1Click(TObject *Sender);
	void __fastcall Frecuencia960Hz1Click(TObject *Sender);
	void __fastcall Continuo1Click(TObject *Sender);
	void __fastcall Ir_a1Click(TObject *Sender);
	void __fastcall ScrollSeg1Click(TObject *Sender);
//  Raton	
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
          TPoint &MousePos, bool &Handled);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Panel1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall StatusBar1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall StatusBar1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall StatusBar1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
//
	void __fastcall Timer1Timer(TObject *Sender);
//
	void __fastcall TimerIndiceTimer(TObject *Sender);
	void __fastcall Sobre1Click(TObject *Sender);
	void __fastcall Ayuda2Click(TObject *Sender);
	void __fastcall usrsrcmpx386s1Click(TObject *Sender);
	void __fastcall usrsrcmpx386s2Click(TObject *Sender);
	void __fastcall usrsrckernelexceptionc1Click(TObject *Sender);
	void __fastcall usrsrcdriversttykeyboardc1Click(TObject *Sender);
	void __fastcall usrsrcdriversttykeyboardc2Click(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall nOpsTick1Click(TObject *Sender);

private:	// User declarations

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
//
	void __fastcall irAlSegundo ( int seg ) ;
//
	void __fastcall actualizarIndice ( void ) ;
//
	void __fastcall borrar ( void ) ;
	void __fastcall pintarSegundos ( void ) ;
	void __fastcall pintarLinea ( void ) ;
	void __fastcall pintarIntReloj ( void ) ;
	void __fastcall pintarTecla ( void ) ;
	void __fastcall pintarNumOpsTick ( void ) ;
	void __fastcall saltarOperacion ( unsigned op ) ;

	int __fastcall parse ( void ) ;

	int __fastcall microsegs ( int numTickSel, int clock_counter ) ;

	int __fastcall analizarTick ( int numTickSel ) ;
//
#if 0
	int __fastcall posTick ( int num ) ; /* en Image1 */
	int __fastcall posTickSel ( void ) ; /* en Image1 */
	int __fastcall numTickEn ( int X ) ; /* X relativa a Form1 */
	bool __fastcall tickVisible ( int num ) ;
	bool __fastcall tickSelVisible ( void ) ;
	void __fastcall mostrarTick ( int num ) ;
	void __fastcall mostrarTickSel ( void ) ;
	void __fastcall ocultarTickSel ( void ) ;
#endif
//
	AnsiString FileName ;                             /* ruta del fichero  */
	int df_1 ;                                        /* df para analizar  */
	int Contador ;                                    /* Contador de ticks */
	int nOpsTick ;                                    /* num Ops Tick      */
	int yActual ;                          /* coordenada y de la op actual */
	int parserEntry ;      /* punto de entrada, siguiente llamada a parser */
	bool paso ;                     /* para procesar operacion a operacion */
//
	int df_size ;                      /* df para obtener el tamaño        */
	int df_off ;                       /* df para indice off_seg           */
	int num_segs ;                     /* numero de segundos               */
	int off_seg [ 60*60 ] ;            /* indice del fichero FileName df_1 */
	int num_ticks ;                    /* numero de ticks                  */
	int off_tick [ 60*60*60 ] ;        /* indice del fichero FileName df_1 */

/*  off_seg [s] = desplazamiento del comienzo del segundo s en el fichero  */
/*  off_tick[t] = desplazamiento del comienzo del tick    t en el fichero  */

	int numTickSel ;          /* numero (-1, 0, ...) del tick seleccionado */

/*
 *  Los siguientes campos no se modifican tras la creacion del formulario
 */

    int margenDerecho ;             /* margenDerecho         <-----------> */
    int shift ;                     /* desplazamiento        <----->       */
    int ancho ;                     /* achura de la columna  <--->         */
    int alto ;                      /* altura del renglon                  */

    int xActual ;                   /* coordenada x punto de escritura     */

    int yLine ;                     /* coordenada y de la Linea            */
    int yFin ;                      /* coordenada y del fin de la linea    */  
    int yActual_0 ;                 /* coordenada y de comienzo de ops     */
    
	TRect Origen ;                        /* para desplazar a la izquierda */
	TRect Destino;
	TImage* I ;                                      /* solo para abreviar */
	TCanvas* C ;                                     /* solo para abreviar */
	
	bool actualizarScrollSeg ;     /* parser (pintarSegundos), irAlSegundo */

	bool mostrarNumOpsTick ;

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
