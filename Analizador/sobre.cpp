//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "sobre.h"
#include "cmd.h"                                         /* mostrar_enlace */

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject *Sender)
{
	 Form4->Hide() ;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button2Click(TObject *Sender)
{
	Form4->Hide() ;
//  system("start /B minixview.chm") ;
//	system("start minixview.chm") ;
	mostrar_fichero_chm("minixview.chm") ;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Image1Click(TObject *Sender)
{
	mostrar_enlace("http://www.minix3.org") ;
}
//---------------------------------------------------------------------------

void __fastcall TForm4::StaticText1Click(TObject *Sender)
{
	mostrar_enlace("http://www.etsisi.upm.es/escuela/dptos/si/docencia/asigs/sistemas-operativos-2014") ;
}
//---------------------------------------------------------------------------

