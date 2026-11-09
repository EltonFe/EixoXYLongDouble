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
	TCheckBox *ChkEditarVertices;
	TCheckBox *ChkEncaixe;
	TCheckBox *ChkSelecionar;
	TButton *BtnLimparSelecao;
	TButton *BtnDesfazer;
	TButton *BtnRefazer;
	TLabel *LblExtrusao;
	TEdit *EdtExtrusao;
	TButton *BtnExtrudar;
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
	void __fastcall ChkEditarVerticesClick(TObject *Sender);
	void __fastcall BtnDesfazerClick(TObject *Sender);
	void __fastcall BtnRefazerClick(TObject *Sender);
	void __fastcall BtnExtrudarClick(TObject *Sender);
	void __fastcall ChkSelecionarClick(TObject *Sender);
	void __fastcall BtnLimparSelecaoClick(TObject *Sender);
private:	// User declarations

	// um vertice que pode ser arrastado: a figura (Tipo, Indice) e qual ponto dela.
	// Linha: 0=M, 1=L. Quadrilatero: 0=M, 1=L, 2=M2, 3=L2.
	// Elipse: 0=centro (move a elipse), 1=ponta do eixo A, 2=ponta do eixo B.
	struct VerticeFigura{
		int Tipo;
		int Indice;
		int Ponto;
	};
	bool Arrastando;					// botao esquerdo arrastando um vertice
	VerticeFigura VerticeAtivo;			// o vertice arrastado
	long double ProfArraste;			// profundidade dele na tela (fica fixa no arraste)
	long double InicioArraste[3];		// onde ele estava, para o Esc desfazer
	bool TemVerticeSobMouse;
	VerticeFigura VerticeSobMouse;		// vertice debaixo do mouse (cursor de mao)

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
	std::vector<ItemFigura> Colecao;	// figuras selecionadas para a extrusao

	// copia de todas as figuras, para o Desfazer e o Refazer (a visao nao entra)
	struct LinhaGuardada{
		std::array<PontoPolar,2> P;
		COLORREF Cor;
		long double Alpha,Beta,Teta;
	};
	struct QuadGuardado{
		std::array<PontoPolar,4> C;
		COLORREF Cor;
	};
	struct ElipseGuardada{
		long double A,B,AZo,BZo,CX,CY,CZ,AnguloX,AnguloY,AnguloZ;
		int Tipo;
		COLORREF Cor;
	};
	struct EstadoDesenho{
		std::vector<LinhaGuardada> Linhas;
		std::vector<QuadGuardado> Quads;
		std::vector<ElipseGuardada> Elipses;
		Duplicador Duplicacao;
		bool ComecoDuplicacao;
	};
	std::vector<EstadoDesenho> PilhaDesfazer;
	std::vector<EstadoDesenho> PilhaRefazer;
	EstadoDesenho EstadoAntesArraste;	// o desenho antes do arraste de um vertice
	bool ArrasteMoveu;					// o arraste mudou algo: entra no Desfazer ao soltar

	// ponto na tela da visao 2D, ja em unidades do desenho (sem a escala do zoom)
	struct PontoTela{
		long double h;		// eixo horizontal da visao
		long double v;		// eixo vertical da visao
		bool TemMundo;		// encaixou num vertice: vale o ponto Mundo, com a profundidade dele
		long double Mundo[3];
	};

	HWND JanelaGL;			// janela em que o contexto foi criado
	HDC  hdc;
	HGLRC hrc;

	Name_Arquivos::Objetos *Desenho;	// desenho proprio (new Objetos[1]())
	long double Escala;					// pixels por unidade do desenho (zoom)

	std::vector<PontoTela> Cliques;		// pontos da figura que esta sendo desenhada
	PontoTela Cursor;					// posicao do mouse, para a previa
	bool SelecionandoRet;				// botao esquerdo arrastando o retangulo de selecao
	PontoTela RetInicio,RetFim;			// cantos do retangulo de selecao
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

	bool EditandoVertices();
	void ListaVertices(std::vector<VerticeFigura> &Vertices);
	void PosicaoVertice(const VerticeFigura &V,long double xyz[3]);
	void MoveVertice(const VerticeFigura &V,const long double xyz[3]);
	void ParaTela(const long double xyz[3],long double s[3]);
	void DaTela(long double h,long double v,long double Prof,long double xyz[3]);
	void ListaVerticesSelecionados(std::vector<VerticeFigura> &Vertices);
	bool AchaVertice(int X,int Y,VerticeFigura &V,const VerticeFigura *Ignora=NULL,bool SoSelecionadas=false);
	PontoTela PontoComEncaixe(int X,int Y,const VerticeFigura *Ignora=NULL);
	void DesenhaEncaixe();

	EstadoDesenho CapturaEstado();
	void RestauraEstado(const EstadoDesenho &E);
	void EmpilhaDesfazer(const EstadoDesenho &Antes);
	void Desfazer();
	void Refazer();
	void AtualizaBotoesDesfazer();

	void AdicionaQuad(const long double P[4][3],COLORREF Cor);
	void AdicionaLateral(const long double A[3],const long double B[3],const long double d[3],COLORREF Cor);
	bool ExtrudaLinha(int i,long double H,const long double Tela[3]);
	int ExtrudaQuads(const std::vector<int> &Indices,long double H,const long double Tela[3]);
	void ExtrudaElipse(int i,long double H,const long double Tela[3]);
	void Extruda();

	bool FiguraExiste(const ItemFigura &Item);
	void ContornoFigura(const ItemFigura &Item,std::vector<std::array<long double,3> > &Pontos,bool &Fechado);
	int PosicaoNaColecao(const ItemFigura &Item);
	void AlternaNaColecao(const ItemFigura &Item);
	void AtualizaColecao();
	bool AchaFigura(int X,int Y,ItemFigura &Achada);
	void TerminaSelecao(int X,int Y);
	void DesenhaRetanguloSelecao();
	bool MesmoVertice(const VerticeFigura &A,const VerticeFigura &B);
	UnicodeString NomeVertice(const VerticeFigura &V);
	void SelecionaFigura(int Tipo,int Indice);
	void CursorDoMouse(TCursor Novo);
	void DesenhaVertices();

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
