//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Ir_a.h"

#include "Main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
	Form1->Ir_a1->Checked = false ;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{

	static int valorAnterior = 0 ;

	int seg ;

	if (Key == VK_RETURN)
	{
		seg = StrToInt(Edit1->Text) ;
		if ((seg < 0) || (seg >= Form1->num_segs))
			seg = valorAnterior ;
		else
			valorAnterior = seg ;
		Edit1->Text = IntToStr(seg) ;
		Form1->irAlSegundo(seg) ;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Edit1KeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == VK_RETURN)
	Key = 0 ;
}
//---------------------------------------------------------------------------


