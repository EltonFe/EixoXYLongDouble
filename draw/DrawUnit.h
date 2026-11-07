//---------------------------------------------------------------------------

#ifndef DrawUnitH
#define DrawUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
#include <vector>
#include "../Arquivo.h"
#include "../Geometria.h"
//---------------------------------------------------------------------------
// DrawOpenGL: desenha linhas, quadrilateros e elipses com o mouse, como o EixoXY,
// mas mostra tudo em OpenGL. O desenho e guardado nas mesmas classes do EixoXY
// (Objetos, Line, Elipse e TQuadrilatero), em coordenadas polares, e e proprio
// deste form: nao mexe no desenho nem nos contadores do EixoXY.
//
// Visao 2D (projecao ortografica): cada clique e um ponto da tela (h,v) mais a
// profundidade digitada, e vai para o mundo pela matriz da visao:
//     XoY: (h, v, Zo)     XoZ: (h, Yo, v)     YoZ: (Xo, h, v)
// girada pelos angulos Alpha, Beta e Teta do desenho (R = Rx*Ry*Rz, a mesma do EixoXY).
// Na visao 3D (perspectiva) so da para girar e olhar; o mouse nao desenha.
//---------------------------------------------------------------------------
class TDrawOpenGL : public TForm
{
__published:	// IDE-managed Components
	TPanel *PnlFerramentas;
	TRadioGroup *RGFigura;
	TRadioGroup *RGVisao;
	TLabel *LblProfundidade;
	TEdit *EdtProfundidade;
	TColorBox *CBCor;
	TButton *BtnGiraXo;
	TButton *BtnGiraYo;
	TButton *BtnGiraZo;
	TCheckBox *ChkPerspectiva;
	TButton *BtnLimpar;
	TButton *BtnSalvarDEN;
	TButton *BtnAbrirGLB;
	TButton *BtnExportarGLB;
	TButton *BtnAbrirDEN;
	TButton *BtnDupTeta;
	TButton *BtnDupAlpha;
	TLabel *LblStatus;
	TLabel *LblFiguras;
	TComboBox *CBFiguras;
	TPanel *PnlEdicao;
	TLabel *LblEdicao;
	TValueListEditor *VLEdicao;
	TButton *BtnAplicar;
	TButton *BtnRecarregar;
	TSaveDialog *SaveDialog1;
	TOpenDialog *OpenDialog1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RGFiguraClick(TObject *Sender);
	void __fastcall RGVisaoClick(TObject *Sender);
	void __fastcall BtnGiraXoClick(TObject *Sender);
	void __fastcall BtnGiraYoClick(TObject *Sender);
	void __fastcall BtnGiraZoClick(TObject *Sender);
	void __fastcall ChkPerspectivaClick(TObject *Sender);
	void __fastcall BtnLimparClick(TObject *Sender);
	void __fastcall BtnSalvarDENClick(TObject *Sender);
	void __fastcall BtnAbrirGLBClick(TObject *Sender);
	void __fastcall BtnExportarGLBClick(TObject *Sender);
	void __fastcall BtnAbrirDENClick(TObject *Sender);
	void __fastcall BtnDupTetaClick(TObject *Sender);
	void __fastcall BtnDupAlphaClick(TObject *Sender);
	void __fastcall CBFigurasChange(TObject *Sender);
	void __fastcall BtnAplicarClick(TObject *Sender);
	void __fastcall BtnRecarregarClick(TObject *Sender);
private:	// User declarations

	// valores como foram carregados no VLEdicao: o Aplicar compara com eles para
	// saber se um ponto foi mudado pelo Vetor/Alpha (polar) ou pelo Xo/Yo
	TStringList *ValoresCarregados;

	// uma figura do desenho, como aparece no CBFiguras: o tipo (FiguraLinha,
	// FiguraQuadrilatero ou FiguraElipse) e o indice dela na sua classe
	struct ItemFigura{
		int Tipo;
		int Indice;
	};
	std::vector<ItemFigura> Figuras;	// Figuras[k] e o item k do CBFiguras

	// ponto na tela da visao 2D, ja em unidades do desenho (sem a escala do zoom)
	struct PontoTela{
		long double h;		// eixo horizontal da visao
		long double v;		// eixo vertical da visao
	};

	HWND JanelaGL;			// janela em que o contexto foi criado
	HDC  hdc;
	HGLRC hrc;

	Name_Arquivos::Objetos *Desenho;	// desenho proprio (new Objetos[1]())
	long double Escala;					// pixels por unidade do desenho (zoom)

	std::vector<PontoTela> Cliques;		// pontos da figura que esta sendo desenhada
	PontoTela Cursor;					// posicao do mouse, para a previa
	bool TemCursor;

	Duplicador Duplicacao;				// originais, plano e copias deste desenho (Dup. Teta / Dup. Alpha)
	bool ComecoDuplicacao;				// a proxima duplicacao comeca do desenho de agora

	bool Girando;						// botao direito arrastando: gira a visao
	int UltimoX,UltimoY;

	bool IniciaGL();
	void FinalizaGL();
	void Desenha();

	int AlturaFerramentas();
	bool NaAreaGL(int Y);
	PontoTela TelaDoMouse(int X,int Y);
	void MatrizDaVisao(long double M[3][3]);
	void TelaParaMundo(const PontoTela &P,long double xyz[3]);
	long double Profundidade();
	TipoVisao Visao();
	PlanoDoDesenho PlanoAtual();

	void GravaLinha();
	void GravaQuadrilatero();
	void GravaElipse();
	void CancelaFigura();
	void Duplica(TipoGiro Tipo);
	void Gira(int Eixo,long double Angulo);
	void NovoDesenho(Name_Arquivos::Objetos *Novo);
	void Status(const UnicodeString &Texto);

	void DesenhaEixos();
	void DesenhaLinhas();
	void DesenhaQuadrilateros();
	void DesenhaElipses();
	void DesenhaPrevia();
	void DesenhaReferencia();
	void DesenhaSelecionada();

	void AtualizaListaFiguras();
	bool FiguraSelecionada(ItemFigura &Item);

	int LarguraEdicao();
	void CarregaEdicao();
	void PoeCampo(const UnicodeString &Campo,long double Valor);
	void PoePonto(const UnicodeString &Nome,const PontoPolar &P);
	bool LeCampo(const UnicodeString &Campo,long double &Valor);
	bool Mudou(const UnicodeString &Campo);
	bool LePonto(const UnicodeString &Nome,PontoPolar &P);
	bool AplicaEdicao();

	void __fastcall FormMouseLeave(TObject *Sender);
	void __fastcall RGVisaoBotaoMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void ReiniciaVisao();

protected:
	virtual void __fastcall CreateParams(Vcl::Controls::TCreateParams &Params);

public:		// User declarations
	__fastcall TDrawOpenGL(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDrawOpenGL *DrawOpenGL;
//---------------------------------------------------------------------------
#endif
