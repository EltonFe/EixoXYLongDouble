//---------------------------------------------------------------------------

#ifndef UnitConstrucaoLineH
#define UnitConstrucaoLineH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TConstrucaoLines : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdtYo;
	TEdit *EdtXo;
	TEdit *EdtZo;
	TButton *BConstruirLine;
	TPanel *Panel1;
	TLabel *LMPonto;
	TLabel *LLPonto;
	TColorBox *Cores;
	TLabel *LXo;
	TLabel *LYo;
	TLabel *LZo;
	TLabel *LTitulo;
	void __fastcall BConstruirLineClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TConstrucaoLines(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TConstrucaoLines *ConstrucaoLines;
//---------------------------------------------------------------------------
#endif
