//---------------------------------------------------------------------------

#ifndef UnitEixoXYH
#define UnitEixoXYH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "UnitConstrucaoLine.h"
#include "UnitElipse.h"
#include "Arquivo.h"
#include <string.h>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>
#include "Matriz.h"
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
using Name_Arquivos::Objetos;
using Name_Arquivos::Line;
using Name_Arquivos::Arquivo;
using Name_Arquivos::ExportGLB;
//---------------------------------------------------------------------------
class TEixoXY : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TTimer *Tempo_Eixo;
	TTimer *Tempo_Draw;
	TColorBox *Cores;
	TPanel *Panel3;
	TButton *ButtonEixo_Zo;
	TButton *BEixoYo;
	TButton *BEixoXo;
	TPanel *Panel4;
	TButton *BDiminuir;
	TPanel *Panel5;
	TButton *BAumentar;
	TEdit *EdtXo;
	TEdit *EdtYo;
	TEdit *EdtZo;
	TPopupMenu *PMEixoXY;
	TMenuItem *ConstruirLinha1;
	TSaveDialog *DlgSalvar;
	TMenuItem *PElipse;
	TOpenDialog *DlgAbrir;
	TMenuItem *Limpa_Desenho;
	TMenuItem *Volta_Linha;
	TButton *BtnLancar;
	TLabel *LblDivisao;
	TEdit *EdtVelocidadeInicial;
	TEdit *EdtAngulo;
	TLabel *LablVelocidadeInicial;
	TLabel *LbelAngulo;
	TTimer *TimerLancar;
	TButton *Button1;
	TButton *BtnDupTeta;
	TButton *BtnDupAlpha;
	TMainMenu *MainMenu1;
	TMenuItem *Arquivo1;
	TMenuItem *Salvar1;
	TMenuItem *Abrir1;
	TMenuItem *Viso1;
	TMenuItem *RBVisaoXoY;
	TMenuItem *RBVisaoXoZ;
	TMenuItem *RBVisaoYoZ;
	TMenuItem *Desenhos1;
	TMenuItem *Cancela_Line;
	TMenuItem *Line;
	TMenuItem *RBQuadrilatero;
	TMenuItem *RBElipse;
	TMenuItem *Export1;
	TMenuItem *ExportarGLB1;
	TMenuItem *ImportarGLB1;
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Tempo_DrawTimer(TObject *Sender);
	void __fastcall ButtonEixo_ZoClick(TObject *Sender);
	void __fastcall Tempo_EixoTimer(TObject *Sender);
	void __fastcall LineClick(TObject *Sender);
	void __fastcall RBVisaoXoZClick(TObject *Sender);
    void __fastcall RBVisaoXoYClick(TObject *Sender);
	void __fastcall BEixoYoClick(TObject *Sender);
	void __fastcall BEixoXoClick(TObject *Sender);
	void __fastcall RBVisaoYoZClick(TObject *Sender);
	void __fastcall BAumentarClick(TObject *Sender);
	void __fastcall BDiminuirClick(TObject *Sender);
	void __fastcall ConstruirLinha1Click(TObject *Sender);

	void __fastcall PElipseClick(TObject *Sender);

	void __fastcall Limpa_DesenhoClick(TObject *Sender);
	void __fastcall Volta_LinhaClick(TObject *Sender);
	void __fastcall BtnLancarClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall BtnDupTetaClick(TObject *Sender);
	void __fastcall BtnDupAlphaClick(TObject *Sender);
	void __fastcall Salvar1Click(TObject *Sender);
	void __fastcall Abrir1Click(TObject *Sender);
	void __fastcall Cancela_LineClick(TObject *Sender);
	void __fastcall RBQuadrilateroClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ExportarGLB1Click(TObject *Sender);
	void __fastcall ImportarGLB1Click(TObject *Sender);

private:	// User declarations
public:		// User declarations
	bool Eixo_Lancar;
	bool Lancar;
 	Objetos *Objetos_Atr;
    Objetos *ObjEixos;
	int Contador_Line;
	int Contador_Circle;
	int **FullMLK;
	int **FullMLK2;
	int **FullMLK3;
	TPoint **P_Circle;
	TPoint **MP1;
	float Kx1;
	float Ky1;
	TPoint **MP2;
	TPoint **MP3;
	// memoria (std::vector) dos ponteiros acima; FullMLK=BufFullMLK.Redimensiona(n,4) etc.
	Matriz<int> BufFullMLK;
	Matriz<int> BufFullMLK2;
	Matriz<int> BufFullMLK3;
	Matriz<TPoint> BufP_Circle;
	Matriz<TPoint> BufMP1;
	Matriz<TPoint> BufMP2;
	Matriz<TPoint> BufMP3;
	bool Faz;
	TPoint **Draw_Elipse(TCanvas *Plano,Objetos *);
	void Draw_ElipseDirect(TCanvas *Plano,Objetos *,TPoint **);
	long double ABT[3];
	void Draw_Lines_Direct(TCanvas *Plano,Objetos *Objetos_Atr, int **FullMLK);
	// desenha os quadrilateros com o TPoint** devolvido pelo Draw_Quadrilatero (5 pontos cada)
	void DrawQuadrilineDirect(TCanvas *Plano,Objetos *Objetos_Atr,TPoint **MLP_Quadrilane);
	TPoint **Draw_Lines(TCanvas *Plano,Objetos *,int **);
	TPoint **Draw_Quadrilatero(TCanvas *Plano,Objetos *,int **,int **);
	// apaga os quadrilateros (MLP_Quadrilatero = TPoint** do Draw_Quadrilatero) com PolyPolyline
	void LimpaQuadrilatero(TCanvas *Plano,TPoint **MLP_Quadrilatero,int ContadorQuadrilatero);
	void Grava_Arquivo(Objetos *Objetos_Atr,char *Nome_Arquivo);
	void Ler_Arquivo(Objetos *Objetos_Atr,char *Nome_Arquivo);
	void Limpa_Elipses(TCanvas *,TPoint **,int);
	void Lancamento();
	void DuplicaValoresTeta(Objetos *,long double *ABT);
    void DuplicaValoresAlpha(Objetos *);
	__fastcall TEixoXY(TComponent* Owner);
	__fastcall ~TEixoXY();
};
//---------------------------------------------------------------------------
extern PACKAGE TEixoXY *EixoXY;

// Ponto i (0 a 50) da elipse j no espaco 3D, antes da rotacao da vista
void PontoElipse3D(Name_Arquivos::Elipse *Elipse_Atr,int j,int i,long double xyz[3]);
//---------------------------------------------------------------------------
#endif
