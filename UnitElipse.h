//---------------------------------------------------------------------------

#ifndef UnitElipseH
#define UnitElipseH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TFElipse : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TRadioGroup *RGElipses;
	TRadioButton *RBEixoXY;
	TRadioButton *RBEixoXZ;
	TRadioButton *RBEixoYZ;
	TEdit *EdtA;
	TEdit *EdtB;
	TButton *BtnElipse;
	TEdit *EdtAZo;
	TEdit *EdtBZo;
	TEdit *EdtCentroX;
	TEdit *EdtCentroY;
	TEdit *EdtCentroZ;
	TLabel *LA;
	TLabel *LB;
	TLabel *LCentroX;
	TGroupBox *GBCentroXYZ;
	TLabel *LCentroY;
	TLabel *LCentroZ;
	TLabel *LPosicaoA;
	TLabel *LPosicaoB;
	TColorBox *Cores;
	TLabel *LAngulo;
	TEdit *EdtAngulo;
	void __fastcall BtnElipseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations

	__fastcall TFElipse(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFElipse *FElipse;
//---------------------------------------------------------------------------
#endif
