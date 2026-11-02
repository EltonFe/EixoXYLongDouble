//---------------------------------------------------------------------------

#ifndef DrawCursorH
#define DrawCursorH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFDrawCursor : public TForm
{
__published:	// IDE-managed Components
	TLabel *DrawCursor;
private:	// User declarations
public:		// User declarations
	__fastcall TFDrawCursor(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFDrawCursor *FDrawCursor;
//---------------------------------------------------------------------------
#endif
