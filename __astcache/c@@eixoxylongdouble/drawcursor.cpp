//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DrawCursor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFDrawCursor *FDrawCursor;
//---------------------------------------------------------------------------
__fastcall TFDrawCursor::TFDrawCursor(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
