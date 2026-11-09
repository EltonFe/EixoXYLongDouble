//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DrawUnit.h"
#include "../Cores.h"
#include "../Geometria.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <cmath>
#include <array>
#include <algorithm>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDrawOpenGL *DrawOpenGL;

using Name_Arquivos::Objetos;
using Name_Arquivos::Line;
using Name_Arquivos::Elipse;

// figuras do RGFigura (mesma ordem dos Items); as visoes do RGVisao seguem o TipoVisao
enum { FiguraLinha, FiguraQuadrilatero, FiguraElipse };

static const long double PassoGiro=M_PI/80;		// mesmo passo do ButtonEixo_Zo do EixoXY
static const int PontosElipse=51;				// mesmo numero de pontos do EixoXY

//---------------------------------------------------------------------------
// Cor das classes (COLORREF) para o OpenGL. Como o fundo e escuro, o preto vira
// branco, igual ao Render.
static void CorGL(COLORREF Cor){
	COLORREF RGBCor=TColorParaCOLORREF(Cor);
	if(RGBCor==RGB(0,0,0))
		RGBCor=RGB(255,255,255);
	glColor3f(GetRValue(RGBCor)/255.0f,GetGValue(RGBCor)/255.0f,GetBValue(RGBCor)/255.0f);
}

// Giro proprio de uma linha, como no Draw_Lines do EixoXY: primeiro
// Teta em volta de Zo, depois Alpha em volta de Xo.
static void GiraFigura(long double xyz[3],long double Teta,long double Alpha){
	const long double x=xyz[0],y=xyz[1],z=xyz[2];
	const long double y1=x*std::sin(Teta)+y*std::cos(Teta);
	xyz[0]=x*std::cos(Teta)-y*std::sin(Teta);
	xyz[1]=y1*std::cos(Alpha)-z*std::sin(Alpha);
	xyz[2]=y1*std::sin(Alpha)+z*std::cos(Alpha);
}

// o contrario do GiraFigura: desfaz o Alpha (em volta de Xo) e depois o Teta (Zo)
static void DesfazGiroFigura(long double xyz[3],long double Teta,long double Alpha){
	const long double x=xyz[0],y=xyz[1],z=xyz[2];
	const long double y1=y*std::cos(Alpha)+z*std::sin(Alpha);
	xyz[2]=-y*std::sin(Alpha)+z*std::cos(Alpha);
	xyz[0]=x*std::cos(Teta)+y1*std::sin(Teta);
	xyz[1]=-x*std::sin(Teta)+y1*std::cos(Teta);
}

// eixos do plano da elipse em x,y,z (0,1,2): U recebe o A, V o B e N a inclinacao
static void EixosDaElipse(int Tipo,int &U,int &V,int &N){
	switch(Tipo){
		case 1:		U=0; V=2; N=1; break;		// plano XZ
		case 2:		U=1; V=2; N=0; break;		// plano YZ
		default:	U=0; V=1; N=2; break;		// plano XY
	}
}

// n = (B-A) x d
static void Produto(const long double B[3],const long double A[3],const long double d[3],long double n[3]){
	const long double e[3]={B[0]-A[0],B[1]-A[1],B[2]-A[2]};
	n[0]=e[1]*d[2]-e[2]*d[1];
	n[1]=e[2]*d[0]-e[0]*d[2];
	n[2]=e[0]*d[1]-e[1]*d[0];
}

static long double Modulo(const long double v[3]){
	return std::hypot(std::hypot(v[0],v[1]),v[2]);
}

// a cor escurecida conforme a direcao n da face: sem isso um solido de uma cor so
// viraria uma mancha, porque o desenho nao usa iluminacao do OpenGL
static COLORREF CorSombreada(COLORREF Cor,const long double n[3]){
	COLORREF RGBCor=TColorParaCOLORREF(Cor);
	if(RGBCor==RGB(0,0,0))
		RGBCor=RGB(255,255,255);			// o preto aparece branco na tela (CorGL)
	const long double Tam=Modulo(n);
	const long double Luz=(Tam>0)? std::fabs(n[0]*-0.42L+n[1]*0.51L+n[2]*0.75L)/Tam : 1;
	const long double f=0.45L+0.55L*std::min(Luz,1.0L);
	return RGB((int)(GetRValue(RGBCor)*f),(int)(GetGValue(RGBCor)*f),(int)(GetBValue(RGBCor)*f));
}

// aceita ponto ou virgula como separador decimal
static bool LeNumero(const UnicodeString &Original,double &Valor){
	const UnicodeString Separador=FormatSettings.DecimalSeparator;
	UnicodeString Texto=Trim(Original);
	Texto=StringReplace(Texto,".",Separador,TReplaceFlags()<<rfReplaceAll);
	Texto=StringReplace(Texto,",",Separador,TReplaceFlags()<<rfReplaceAll);
	return TryStrToFloat(Texto,Valor);
}

// ponto guardado em polar (Alpha,Vetor) com a altura Zo -> x,y,z
static void Polar3D(long double Alpha,long double Vetor,long double Zo,long double xyz[3]){
	xyz[0]=std::cos(Alpha)*Vetor;
	xyz[1]=std::sin(Alpha)*Vetor;
	xyz[2]=Zo;
}

static void Vertice(const long double xyz[3]){
	glVertex3d((double)xyz[0],(double)xyz[1],(double)xyz[2]);
}

static double Graus(long double Radianos){
	return (double)(Radianos*180/M_PI);
}

// um ponto polar no texto do CBFiguras: Nome(Xo; Yo; Zo) Vetor e Alpha (em graus)
static UnicodeString TextoPonto(const wchar_t *Nome,const PontoPolar &P){
	return Format(L"%s(%.2f; %.2f; %.2f) V=%.2f A=%.1f\u00B0",
		ARRAYOFCONST((UnicodeString(Nome),(double)P.Xo,(double)P.Yo,(double)P.Zo,(double)P.Vetor,Graus(P.Alpha))));
}

//---------------------------------------------------------------------------
__fastcall TDrawOpenGL::TDrawOpenGL(TComponent* Owner)
	: TForm(Owner)
{
	JanelaGL=NULL;
	hdc=NULL;
	hrc=NULL;
	Desenho=new Objetos[1]();
	Escala=1;
	TemCursor=false;
	Girando=false;
	UltimoX=UltimoY=0;
	ComecoDuplicacao=true;
	ValoresCarregados=new TStringList();
	Arrastando=false;
	TemVerticeSobMouse=false;
	VerticeAtivo=VerticeSobMouse=VerticeFigura{-1,-1,-1};
	ProfArraste=0;
	InicioArraste[0]=InicioArraste[1]=InicioArraste[2]=0;
	ArrasteMoveu=false;
	Cursor.h=Cursor.v=0;
	Cursor.TemMundo=false;
	EstadoAntesArraste.ComecoDuplicacao=true;
	SelecionandoRet=false;
	RetInicio=RetFim=Cursor;
}
//---------------------------------------------------------------------------

// a area do OpenGL e o form todo: o painel de ferramentas nao pode ser apagado por ela
void __fastcall TDrawOpenGL::CreateParams(Vcl::Controls::TCreateParams &Params){
	TForm::CreateParams(Params);
	Params.Style=Params.Style|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
}

void __fastcall TDrawOpenGL::FormCreate(TObject *Sender)
{
	KeyPreview=true;
	OnMouseLeave=FormMouseLeave;		// apaga as linhas de referencia ao sair da janela

	// cada botao do RGVisao reinicia os eixos, mesmo clicando na visao ja marcada
	for(int i=0;i<RGVisao->ControlCount;i++){
		TRadioButton *Botao=dynamic_cast<TRadioButton*>(RGVisao->Controls[i]);
		if(Botao!=NULL)
			Botao->OnMouseUp=RGVisaoBotaoMouseUp;
	}
	AtualizaListaFiguras();
	AtualizaBotoesDesfazer();
	Status("Linha: clique no inicio e no fim. Botao direito arrastando gira a visao.");
}

void __fastcall TDrawOpenGL::FormDestroy(TObject *Sender)
{
	FinalizaGL();
	delete[] Desenho;			// criado com new Objetos[1]()
	Desenho=NULL;
	delete ValoresCarregados;
	ValoresCarregados=NULL;
}

//---------------------------------------------------------------------------
// Contexto OpenGL
//---------------------------------------------------------------------------

bool TDrawOpenGL::IniciaGL(){

	// a janela do form pode ser recriada pela VCL: o contexto vai junto
	if((hrc!=NULL)&&(JanelaGL==Handle))
		return true;
	FinalizaGL();

	JanelaGL=Handle;
	hdc=GetDC(JanelaGL);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd,sizeof(pfd));
	pfd.nSize=sizeof(pfd);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=24;
	pfd.cDepthBits=24;
	pfd.iLayerType=PFD_MAIN_PLANE;

	const int Formato=ChoosePixelFormat(hdc,&pfd);
	if((Formato==0)||(!SetPixelFormat(hdc,Formato,&pfd))){
		Status("Nao foi possivel escolher o formato de pixel do OpenGL");
		FinalizaGL();
		return false;
	}

	hrc=wglCreateContext(hdc);
	if(hrc==NULL){
		Status("Nao foi possivel criar o contexto OpenGL");
		FinalizaGL();
		return false;
	}
	return true;
}

void TDrawOpenGL::FinalizaGL(){
	if(hrc!=NULL){
		if(wglGetCurrentContext()==hrc)
			wglMakeCurrent(NULL,NULL);
		wglDeleteContext(hrc);
		hrc=NULL;
	}
	if((hdc!=NULL)&&(JanelaGL!=NULL))
		ReleaseDC(JanelaGL,hdc);
	hdc=NULL;
	JanelaGL=NULL;
}

//---------------------------------------------------------------------------
// Tela e mundo
//---------------------------------------------------------------------------

int TDrawOpenGL::AlturaFerramentas(){
	return PnlFerramentas->Visible? PnlFerramentas->Height : 0;
}

bool TDrawOpenGL::NaAreaGL(int Y){
	return Y>=AlturaFerramentas();
}

// mouse -> ponto da tela em unidades do desenho, com a origem no centro da area GL
TDrawOpenGL::PontoTela TDrawOpenGL::TelaDoMouse(int X,int Y){
	const int Topo=AlturaFerramentas();
	const long double Cx=(ClientWidth-LarguraEdicao())/2.0L;		// a area GL termina no painel de edicao
	const long double Cy=Topo+(ClientHeight-Topo)/2.0L;
	PontoTela P;
	P.h=(X-Cx)/Escala;
	P.v=(Cy-Y)/Escala;			// na tela o y cresce para baixo
	P.TemMundo=false;
	return P;
}

// o valor fixo da visao: Zo na XoY, Yo na XoZ e Xo na YoZ
long double TDrawOpenGL::Profundidade(){
	return StrToFloatDef(EdtProfundidade->Text,0);
}

// visao escolhida no RGVisao (os Items estao na ordem do TipoVisao)
TipoVisao TDrawOpenGL::Visao(){
	switch(RGVisao->ItemIndex){
		case VisaoXoZ:	return VisaoXoZ;
		case VisaoYoZ:	return VisaoYoZ;
		default:		return VisaoXoY;
	}
}

// plano da visao de agora, para uma figura que acabou de ser desenhada
PlanoDoDesenho TDrawOpenGL::PlanoAtual(){
	const long double *ABT=Desenho[0].GetAlpha_B_T();
	PlanoDoDesenho P;
	P.Guardado=true;
	P.ABT[0]=ABT[0];
	P.ABT[1]=ABT[1];
	P.ABT[2]=ABT[2];
	P.Visao=Visao();
	return P;
}

// M = V*R: R gira o desenho (Alpha, Beta, Teta, as mesmas formulas do EixoXY) e V
// escolhe os eixos da visao. Linhas de M: horizontal, vertical e profundidade.
void TDrawOpenGL::MatrizDaVisao(long double M[3][3]){

	const long double *ABT=Desenho[0].GetAlpha_B_T();
	const long double ca=std::cos(ABT[0]),sa=std::sin(ABT[0]);
	const long double cb=std::cos(ABT[1]),sb=std::sin(ABT[1]);
	const long double ct=std::cos(ABT[2]),st=std::sin(ABT[2]);

	const long double R[3][3]={
		{ cb*ct,				-cb*st,				 sb		},
		{ sa*sb*ct+ca*st,		-sa*sb*st+ca*ct,	-sa*cb	},
		{-ca*sb*ct+sa*st,		 ca*sb*st+sa*ct,	 ca*cb	}
	};

	// V: cada linha diz qual eixo do mundo aparece em cada eixo da tela
	static const long double V[3][3][3]={
		{{1,0,0},{0,1,0},{0,0,1}},		// XoY: h=x, v=y, profundidade=z
		{{1,0,0},{0,0,1},{0,-1,0}},		// XoZ: h=x, v=z, profundidade=-y
		{{0,1,0},{0,0,1},{1,0,0}}		// YoZ: h=y, v=z, profundidade=x
	};

	for(int l=0;l<3;l++)
		for(int c=0;c<3;c++)
			M[l][c]=V[Visao()][l][0]*R[0][c]+V[Visao()][l][1]*R[1][c]+V[Visao()][l][2]*R[2][c];
}

// ponto da tela + profundidade -> mundo: p = Mt*s (M e ortogonal)
void TDrawOpenGL::TelaParaMundo(const PontoTela &P,long double xyz[3]){

	if(P.TemMundo){				// encaixado num vertice: e exatamente aquele ponto
		xyz[0]=P.Mundo[0];
		xyz[1]=P.Mundo[1];
		xyz[2]=P.Mundo[2];
		return;
	}

	long double M[3][3];
	MatrizDaVisao(M);

	// na XoZ a profundidade da tela e -Yo
	const long double Prof=(Visao()==VisaoXoZ)? -Profundidade() : Profundidade();
	const long double s[3]={P.h,P.v,Prof};

	for(int k=0;k<3;k++)
		xyz[k]=M[0][k]*s[0]+M[1][k]*s[1]+M[2][k]*s[2];
}

//---------------------------------------------------------------------------
// Gravacao das figuras nas classes do EixoXY (sempre em polar, como o FormMouseDown)
//---------------------------------------------------------------------------

void TDrawOpenGL::GravaLinha(){

	Line *Lines=Desenho[0].Lines_Atr;
	const int Novo=Lines->GetContador_Line();
	long double M[3],L[3],Alpha,Vetor;

	TelaParaMundo(Cliques[0],M);
	TelaParaMundo(Cliques[1],L);

	Lines->Alocando_Line(Novo+1);

	EixoXo(M[0],M[1],0,&Alpha,&Vetor);
	Lines->SetMAlpha_Atr(Alpha,Novo);
	Lines->SetMVetor_Atr(Vetor,Novo);
	EixoXo(L[0],L[1],0,&Alpha,&Vetor);
	Lines->SetLAlpha_Atr(Alpha,Novo);
	Lines->SetLVetor_Atr(Vetor,Novo);

	Lines->SetMXo_Atr(M[0],Novo);
	Lines->SetMYo_Atr(M[1],Novo);
	Lines->SetMZo_Atr(M[2],Novo);
	Lines->SetLXo_Atr(L[0],Novo);
	Lines->SetLYo_Atr(L[1],Novo);
	Lines->SetLZo_Atr(L[2],Novo);

	Lines->SetCor_Line(CBCor->Selected,Novo);
	Lines->Set_AngluloTeta(0,Novo);			// linha nova sem giro proprio
	Lines->Set_AngluloAlpha(0,Novo);

	Lines->SetContador_Line(Novo+1);
	Duplicacao.GuardaPlanoLine(Novo,PlanoAtual());
}

void TDrawOpenGL::GravaQuadrilatero(){

	TQuadrilatero *Quad=Desenho[0].Quadrilatero;
	const int Novo=Quad->GetContadorQuadrilatero();
	long double P[4][3],Alpha[4],Vetor[4];

	// os cantos na ordem do contorno: M, L, M2 e L2
	for(int k=0;k<4;k++){
		TelaParaMundo(Cliques[k],P[k]);
		EixoXo(P[k][0],P[k][1],0,&Alpha[k],&Vetor[k]);
	}

	Quad->AlocarQuadrilatero(Novo+1);

	Quad->SetMAlpha(Alpha[0],Novo);		Quad->SetMVetor(Vetor[0],Novo);
	Quad->SetLAlpha(Alpha[1],Novo);		Quad->SetLVetor(Vetor[1],Novo);
	Quad->SetMAlpha2(Alpha[2],Novo);	Quad->SetMVetor2(Vetor[2],Novo);
	Quad->SetLAlpha2(Alpha[3],Novo);	Quad->SetLVetor2(Vetor[3],Novo);

	Quad->SetMXo(P[0][0],Novo);		Quad->SetMYo(P[0][1],Novo);		Quad->SetMZo(P[0][2],Novo);
	Quad->SetLXo(P[1][0],Novo);		Quad->SetLYo(P[1][1],Novo);		Quad->SetLZo(P[1][2],Novo);
	Quad->SetMXo2(P[2][0],Novo);	Quad->SetMYo2(P[2][1],Novo);	Quad->SetMZo2(P[2][2],Novo);
	Quad->SetLXo2(P[3][0],Novo);	Quad->SetLYo2(P[3][1],Novo);	Quad->SetLZo2(P[3][2],Novo);

	Quad->SetCor(CBCor->Selected,Novo);
	Quad->SetContadorQuadrilatero(Novo+1);
	Duplicacao.GuardaPlanoQuad(Novo,PlanoAtual());
}

// circulo (A=B) no plano da visao: o primeiro clique e o centro, o segundo da o raio
void TDrawOpenGL::GravaElipse(){

	Elipse *Elipses=Desenho[0].Elipse_Atr;
	const int Novo=Elipses->GetContadorCircle_Atr();
	long double Centro[3];

	TelaParaMundo(Cliques[0],Centro);
	const long double A=std::hypot(Cliques[1].h-Cliques[0].h,Cliques[1].v-Cliques[0].v);

	Elipses->AlocaElipse_Atr(Novo+1);

	Elipses->SetA_Atr(A,Novo);
	Elipses->SetB_Atr(A,Novo);
	Elipses->SetAZo_Atr(0,Novo);
	Elipses->SetBZo_Atr(0,Novo);

	// o circulo fica no plano da tela: o centro e guardado antes do giro da visao (R*p,
	// no plano do TipoElipse) e o giro proprio da elipse e Rt (AnguloY=-Alpha,
	// AnguloZ=-Beta e AnguloX=-Teta; veja GiroDaElipse)
	const long double *ABT=Desenho[0].GetAlpha_B_T();
	const VisaoTela T=MontaVisaoTela(ABT[0],ABT[1],ABT[2],Visao());
	long double Local[3];
	for(int l=0;l<3;l++)
		Local[l]=T.R[l][0]*Centro[0]+T.R[l][1]*Centro[1]+T.R[l][2]*Centro[2];

	// tipo 0 = plano XY, 1 = XZ, 2 = YZ: o plano da visao em que foi desenhada
	Elipses->SetTipoElipse_Atr(Visao(),Novo);
	Elipses->SetCX(Local[0],Novo);
	Elipses->SetCY(Local[1],Novo);
	Elipses->SetCZ(Local[2],Novo);
	Elipses->SetCenter_Atr(Local[0],Local[1],Local[2],Novo);

	Elipses->Set_AnguloX(-ABT[2],Novo);
	Elipses->Set_AnguloY(-ABT[0],Novo);
	Elipses->Set_AnguloZ(-ABT[1],Novo);
	Elipses->SetColor_Circle_Atr(CBCor->Selected,Novo);

	Elipses->SetContadorCircle_Atr(Novo+1);
	Duplicacao.GuardaPlanoCircle(Novo,PlanoAtual());
}

void TDrawOpenGL::CancelaFigura(){
	Cliques.clear();
	Desenha();
}

// mais uma copia girada de cada figura, como o Duplica Teta / Duplica Alpha do EixoXY
void TDrawOpenGL::Duplica(TipoGiro Tipo){
	EmpilhaDesfazer(CapturaEstado());
	Duplicacao.Duplica(Desenho,Desenho[0].GetAlpha_B_T(),Visao(),ComecoDuplicacao,Tipo,PassoGiro);
	ComecoDuplicacao=false;
	AtualizaListaFiguras();
	CancelaFigura();
}

// Eixo 0: Alpha (Xo), 1: Beta (Yo), 2: Teta (Zo)
void TDrawOpenGL::Gira(int Eixo,long double Angulo){
	long double ABT[3];
	Desenho[0].GetAlpha_B_T(ABT);
	ABT[Eixo]+=Angulo;
	Desenho[0].SetAlpha_B_T(ABT);
	Desenha();
}

// troca o desenho (limpar ou abrir): o anterior e liberado
void TDrawOpenGL::NovoDesenho(Objetos *Novo){
	{
		const EstadoDesenho Antes=CapturaEstado();
		if((!Antes.Linhas.empty())||(!Antes.Quads.empty())||(!Antes.Elipses.empty()))
			EmpilhaDesfazer(Antes);			// o Desfazer traz de volta o desenho limpo ou trocado
	}
	delete[] Desenho;
	Desenho=Novo;
	Duplicacao.EsquecePlanos();			// figuras novas: nenhuma tem plano guardado
	ComecoDuplicacao=true;
	Cliques.clear();
	CBFiguras->ItemIndex=-1;			// os indices antigos nao valem para o desenho novo
	Colecao.clear();
	AtualizaListaFiguras();
	Desenha();
}

void TDrawOpenGL::Status(const UnicodeString &Texto){
	LblStatus->Caption=Texto;
}

//---------------------------------------------------------------------------
// Lista das figuras (CBFiguras): todas as linhas, quadrilateros e elipses do
// desenho, com as coordenadas, vetores e angulos, para depois serem alteradas
//---------------------------------------------------------------------------

// refaz a lista a partir das classes; a figura que estava escolhida continua escolhida
void TDrawOpenGL::AtualizaListaFiguras(){

	ItemFigura Anterior;
	const bool TinhaSelecao=FiguraSelecionada(Anterior);

	// o Desfazer pode ter apagado figuras da colecao
	for(size_t k=Colecao.size();k-->0;)
		if(!FiguraExiste(Colecao[k]))
			Colecao.erase(Colecao.begin()+k);
	AtualizaColecao();

	Figuras.clear();
	CBFiguras->Items->BeginUpdate();
	CBFiguras->Items->Clear();

	Line *Lines=Desenho[0].Lines_Atr;
	for(int i=0;i<Lines->GetContador_Line();i++){
		const std::array<PontoPolar,2> P=LerPontas(Lines,i);
		CBFiguras->Items->Add(Format(L"Linha %d   ",ARRAYOFCONST((i+1)))
			+TextoPonto(L"M",P[0])+L"   "+TextoPonto(L"L",P[1])
			+Format(L"   Giro: Teta=%.1f\u00B0 Alpha=%.1f\u00B0",
				ARRAYOFCONST((Graus(Lines->GetAnguloTeta(i)),Graus(Lines->GetAnguloAlpha(i))))));
		Figuras.push_back(ItemFigura{FiguraLinha,i});
	}

	TQuadrilatero *Quad=Desenho[0].Quadrilatero;
	static const wchar_t *Canto[4]={L"M",L"L",L"M2",L"L2"};
	for(int i=0;i<Quad->GetContadorQuadrilatero();i++){
		const std::array<PontoPolar,4> C=LerCantos(Quad,i);
		UnicodeString Texto=Format(L"Quadril\u00E1tero %d",ARRAYOFCONST((i+1)));
		for(int k=0;k<4;k++)
			Texto+=L"   "+TextoPonto(Canto[k],C[k]);
		CBFiguras->Items->Add(Texto);
		Figuras.push_back(ItemFigura{FiguraQuadrilatero,i});
	}

	Elipse *Elipses=Desenho[0].Elipse_Atr;
	static const wchar_t *Plano[3]={L"XY",L"XZ",L"YZ"};
	for(int i=0;i<Elipses->GetContadorCircle_Atr();i++){
		const int Tipo=Elipses->GetTipoElipse_Atr(i);
		const UnicodeString NomePlano=((Tipo>=0)&&(Tipo<3))? UnicodeString(Plano[Tipo]) : UnicodeString(L"?");
		CBFiguras->Items->Add(Format(
			L"Elipse %d   Centro(%.2f; %.2f; %.2f)   A=%.2f B=%.2f   Plano %s   Giro: X=%.1f\u00B0 Y=%.1f\u00B0 Z=%.1f\u00B0",
			ARRAYOFCONST((i+1,
				(double)Elipses->GetCX(i),(double)Elipses->GetCY(i),(double)Elipses->GetCZ(i),
				(double)Elipses->GetA_Atr(i),(double)Elipses->GetB_Atr(i),
				NomePlano,
				Graus(Elipses->GetAnguloX(i)),Graus(Elipses->GetAnguloY(i)),Graus(Elipses->GetAnguloZ(i))))));
		Figuras.push_back(ItemFigura{FiguraElipse,i});
	}

	int Selecao=-1;
	if(TinhaSelecao)
		for(size_t k=0;k<Figuras.size();k++)
			if((Figuras[k].Tipo==Anterior.Tipo)&&(Figuras[k].Indice==Anterior.Indice)){
				Selecao=(int)k;
				break;
			}

	CBFiguras->Items->EndUpdate();
	CBFiguras->ItemIndex=Selecao;
	CarregaEdicao();				// os valores da figura escolhida (ou nada)
}

// figura escolhida no CBFiguras (false se nenhuma ou se ela nao existe mais)
bool TDrawOpenGL::FiguraSelecionada(ItemFigura &Item){

	const int k=CBFiguras->ItemIndex;
	if((k<0)||(k>=(int)Figuras.size()))
		return false;
	Item=Figuras[k];
	return FiguraExiste(Item);
}

// o indice ainda vale no desenho atual?
bool TDrawOpenGL::FiguraExiste(const ItemFigura &Item){

	switch(Item.Tipo){
		case FiguraLinha:			return Item.Indice<Desenho[0].Lines_Atr->GetContador_Line();
		case FiguraQuadrilatero:	return Item.Indice<Desenho[0].Quadrilatero->GetContadorQuadrilatero();
		case FiguraElipse:			return Item.Indice<Desenho[0].Elipse_Atr->GetContadorCircle_Atr();
	}
	return false;
}

//---------------------------------------------------------------------------
// Edicao da figura escolhida (PnlEdicao): cada ponto aparece com Xo, Yo, Zo, Vetor
// e Alpha. No Aplicar, se o Vetor ou o Alpha de um ponto foi mudado ele e refeito
// pelo polar; se nao, pelo Xo e Yo (e o EixoXo acha o Alpha e o Vetor). Os angulos
// aparecem e sao digitados em graus.
//---------------------------------------------------------------------------

int TDrawOpenGL::LarguraEdicao(){
	return PnlEdicao->Visible? PnlEdicao->Width : 0;
}

void TDrawOpenGL::PoeCampo(const UnicodeString &Campo,long double Valor){
	const UnicodeString Texto=FormatFloat("0.######",(double)Valor);
	VLEdicao->InsertRow(Campo,Texto,true);
	ValoresCarregados->Values[Campo]=Texto;
}

void TDrawOpenGL::PoePonto(const UnicodeString &Nome,const PontoPolar &P){
	PoeCampo(Nome+" Xo",P.Xo);
	PoeCampo(Nome+" Yo",P.Yo);
	PoeCampo(Nome+" Zo",P.Zo);
	PoeCampo(Nome+" Vetor",P.Vetor);
	PoeCampo(Nome+" Alpha (graus)",Graus(P.Alpha));
}

// aceita ponto ou virgula como separador decimal
bool TDrawOpenGL::LeCampo(const UnicodeString &Campo,long double &Valor){
	UnicodeString Texto=Trim(VLEdicao->Values[Campo]);
	const UnicodeString Separador=FormatSettings.DecimalSeparator;
	Texto=StringReplace(Texto,".",Separador,TReplaceFlags()<<rfReplaceAll);
	Texto=StringReplace(Texto,",",Separador,TReplaceFlags()<<rfReplaceAll);
	double Lido;
	if(!TryStrToFloat(Texto,Lido)){
		Status("Valor invalido em \""+Campo+"\"");
		return false;
	}
	Valor=Lido;
	return true;
}

bool TDrawOpenGL::Mudou(const UnicodeString &Campo){
	return Trim(VLEdicao->Values[Campo])!=ValoresCarregados->Values[Campo];
}

// P entra com o ponto guardado e sai com o ponto editado (igual, se nada mudou)
bool TDrawOpenGL::LePonto(const UnicodeString &Nome,PontoPolar &P){

	long double Xo,Yo,Zo,Vetor,Alpha;
	if(!LeCampo(Nome+" Xo",Xo)||!LeCampo(Nome+" Yo",Yo)||!LeCampo(Nome+" Zo",Zo)||
	   !LeCampo(Nome+" Vetor",Vetor)||!LeCampo(Nome+" Alpha (graus)",Alpha))
		return false;

	if(Mudou(Nome+" Zo"))
		P.Zo=Zo;

	if(Mudou(Nome+" Vetor")||Mudou(Nome+" Alpha (graus)"))
		P=Polar(Alpha*M_PI/180,Vetor,P.Zo);				// mudou pelo polar
	else if(Mudou(Nome+" Xo")||Mudou(Nome+" Yo")){
		P.Xo=Xo;										// mudou pelo cartesiano
		P.Yo=Yo;
		EixoXo(Xo,Yo,0,&P.Alpha,&P.Vetor);
	}
	return true;
}

// mostra no VLEdicao os valores da figura escolhida no CBFiguras
void TDrawOpenGL::CarregaEdicao(){

	if(ValoresCarregados==NULL)
		return;

	VLEdicao->Strings->Clear();
	ValoresCarregados->Clear();

	ItemFigura Item;
	const bool Tem=FiguraSelecionada(Item);
	BtnAplicar->Enabled=Tem;
	BtnRecarregar->Enabled=Tem;
	if(!Tem){
		LblEdicao->Caption="Escolha uma figura na lista";
		return;
	}

	const int i=Item.Indice;
	switch(Item.Tipo){
		case FiguraLinha:{
			Line *Lines=Desenho[0].Lines_Atr;
			const std::array<PontoPolar,2> P=LerPontas(Lines,i);
			LblEdicao->Caption=Format("Linha %d",ARRAYOFCONST((i+1)));
			PoePonto("M",P[0]);
			PoePonto("L",P[1]);
			PoeCampo("Giro Teta (graus)",Graus(Lines->GetAnguloTeta(i)));
			PoeCampo("Giro Alpha (graus)",Graus(Lines->GetAnguloAlpha(i)));
			break;
		}
		case FiguraQuadrilatero:{
			static const wchar_t *Canto[4]={L"M",L"L",L"M2",L"L2"};
			const std::array<PontoPolar,4> C=LerCantos(Desenho[0].Quadrilatero,i);
			LblEdicao->Caption=Format("Quadrilatero %d",ARRAYOFCONST((i+1)));
			for(int k=0;k<4;k++)
				PoePonto(Canto[k],C[k]);
			break;
		}
		case FiguraElipse:{
			Elipse *Elipses=Desenho[0].Elipse_Atr;
			LblEdicao->Caption=Format("Elipse %d",ARRAYOFCONST((i+1)));
			PoeCampo("Centro X",Elipses->GetCX(i));
			PoeCampo("Centro Y",Elipses->GetCY(i));
			PoeCampo("Centro Z",Elipses->GetCZ(i));
			PoeCampo("A",Elipses->GetA_Atr(i));
			PoeCampo("B",Elipses->GetB_Atr(i));
			PoeCampo("Giro X (graus)",Graus(Elipses->GetAnguloX(i)));
			PoeCampo("Giro Y (graus)",Graus(Elipses->GetAnguloY(i)));
			PoeCampo("Giro Z (graus)",Graus(Elipses->GetAnguloZ(i)));
			break;
		}
	}
}

// grava nas classes o que foi digitado no VLEdicao; nada muda se um valor for invalido
bool TDrawOpenGL::AplicaEdicao(){

	ItemFigura Item;
	if(!FiguraSelecionada(Item)){
		Status("Nenhuma figura escolhida");
		return false;
	}

	const int i=Item.Indice;
	switch(Item.Tipo){
		case FiguraLinha:{
			Line *Lines=Desenho[0].Lines_Atr;
			std::array<PontoPolar,2> P=LerPontas(Lines,i);
			long double Teta,Alpha;
			if(!LePonto("M",P[0])||!LePonto("L",P[1])||
			   !LeCampo("Giro Teta (graus)",Teta)||!LeCampo("Giro Alpha (graus)",Alpha))
				return false;
			GravarPontas(Lines,i,P);
			if(Mudou("Giro Teta (graus)"))
				Lines->Set_AngluloTeta(Teta*M_PI/180,i);
			if(Mudou("Giro Alpha (graus)"))
				Lines->Set_AngluloAlpha(Alpha*M_PI/180,i);
			break;
		}
		case FiguraQuadrilatero:{
			static const wchar_t *Canto[4]={L"M",L"L",L"M2",L"L2"};
			std::array<PontoPolar,4> C=LerCantos(Desenho[0].Quadrilatero,i);
			for(int k=0;k<4;k++)
				if(!LePonto(Canto[k],C[k]))
					return false;
			GravarCantos(Desenho[0].Quadrilatero,i,C);
			break;
		}
		case FiguraElipse:{
			Elipse *Elipses=Desenho[0].Elipse_Atr;
			long double CX,CY,CZ,A,B,GX,GY,GZ;
			if(!LeCampo("Centro X",CX)||!LeCampo("Centro Y",CY)||!LeCampo("Centro Z",CZ)||
			   !LeCampo("A",A)||!LeCampo("B",B)||
			   !LeCampo("Giro X (graus)",GX)||!LeCampo("Giro Y (graus)",GY)||!LeCampo("Giro Z (graus)",GZ))
				return false;
			if((A<=0)||(B<=0)){
				Status("A e B da elipse precisam ser maiores que zero");
				return false;
			}
			// o que nao foi mudado fica com o valor guardado (sem o arredondamento do texto)
			if(!Mudou("Centro X"))	CX=Elipses->GetCX(i);
			if(!Mudou("Centro Y"))	CY=Elipses->GetCY(i);
			if(!Mudou("Centro Z"))	CZ=Elipses->GetCZ(i);
			Elipses->SetCX(CX,i);
			Elipses->SetCY(CY,i);
			Elipses->SetCZ(CZ,i);
			Elipses->SetCenter_Atr(CX,CY,CZ,i);
			if(Mudou("A"))	Elipses->SetA_Atr(A,i);
			if(Mudou("B"))	Elipses->SetB_Atr(B,i);
			if(Mudou("Giro X (graus)"))	Elipses->Set_AnguloX(GX*M_PI/180,i);
			if(Mudou("Giro Y (graus)"))	Elipses->Set_AnguloY(GY*M_PI/180,i);
			if(Mudou("Giro Z (graus)"))	Elipses->Set_AnguloZ(GZ*M_PI/180,i);
			break;
		}
	}
	Status("Figura alterada");
	return true;
}

//---------------------------------------------------------------------------
// Edicao dos vertices com o mouse (ChkEditarVertices): clica num vertice para
// escolher e arrasta para mudar a posicao. O vertice anda no plano da tela e
// fica com a profundidade que tinha. So na visao 2D.
//---------------------------------------------------------------------------

bool TDrawOpenGL::EditandoVertices(){
	return ChkEditarVertices->Checked&&(!ChkPerspectiva->Checked);
}

// todos os vertices do desenho que podem ser arrastados
void TDrawOpenGL::ListaVertices(std::vector<VerticeFigura> &Vertices){
	Vertices.clear();
	for(int i=0;i<Desenho[0].Lines_Atr->GetContador_Line();i++)
		for(int k=0;k<2;k++)
			Vertices.push_back(VerticeFigura{FiguraLinha,i,k});
	for(int i=0;i<Desenho[0].Quadrilatero->GetContadorQuadrilatero();i++)
		for(int k=0;k<4;k++)
			Vertices.push_back(VerticeFigura{FiguraQuadrilatero,i,k});
	for(int i=0;i<Desenho[0].Elipse_Atr->GetContadorCircle_Atr();i++)
		for(int k=0;k<3;k++)
			Vertices.push_back(VerticeFigura{FiguraElipse,i,k});
}

// so os vertices das figuras selecionadas: a colecao e a escolhida no CBFiguras
void TDrawOpenGL::ListaVerticesSelecionados(std::vector<VerticeFigura> &Vertices){
	ItemFigura Escolhida;
	const bool TemEscolhida=FiguraSelecionada(Escolhida);
	std::vector<VerticeFigura> Todos;
	ListaVertices(Todos);
	Vertices.clear();
	for(size_t k=0;k<Todos.size();k++){
		const ItemFigura Item={Todos[k].Tipo,Todos[k].Indice};
		if((PosicaoNaColecao(Item)>=0)||(TemEscolhida&&(Item.Tipo==Escolhida.Tipo)&&(Item.Indice==Escolhida.Indice)))
			Vertices.push_back(Todos[k]);
	}
}

// onde o vertice aparece no mundo (ja com o giro proprio da figura)
void TDrawOpenGL::PosicaoVertice(const VerticeFigura &V,long double xyz[3]){

	const int i=V.Indice;
	switch(V.Tipo){
		case FiguraLinha:{
			Line *Lines=Desenho[0].Lines_Atr;
			if(V.Ponto==0)
				Polar3D(Lines->GetMAlpha_Atr(i),Lines->GetMVetor_Atr(i),Lines->GetMZo_Atr(i),xyz);
			else
				Polar3D(Lines->GetLAlpha_Atr(i),Lines->GetLVetor_Atr(i),Lines->GetLZo_Atr(i),xyz);
			GiraFigura(xyz,Lines->GetAnguloTeta(i),Lines->GetAnguloAlpha(i));
			break;
		}
		case FiguraQuadrilatero:{
			const std::array<PontoPolar,4> C=LerCantos(Desenho[0].Quadrilatero,i);
			xyz[0]=C[V.Ponto].Xo;
			xyz[1]=C[V.Ponto].Yo;
			xyz[2]=C[V.Ponto].Zo;
			break;
		}
		case FiguraElipse:{
			Elipse *Elipses=Desenho[0].Elipse_Atr;
			int U,Ve,N;
			EixosDaElipse(Elipses->GetTipoElipse_Atr(i),U,Ve,N);
			long double p[3]={Elipses->GetCX(i),Elipses->GetCY(i),Elipses->GetCZ(i)};
			if(V.Ponto==1){				// ponta do eixo A (o ponto 0 do contorno)
				p[U]+=Elipses->GetA_Atr(i);
				p[N]+=Elipses->GetAZo_Atr(i);
			}else if(V.Ponto==2){		// ponta do eixo B
				p[Ve]+=Elipses->GetB_Atr(i);
				p[N]+=Elipses->GetBZo_Atr(i);
			}
			long double G[3][3];
			GiroDaElipse(Elipses->GetAnguloX(i),Elipses->GetAnguloY(i),Elipses->GetAnguloZ(i),G);
			for(int l=0;l<3;l++)
				xyz[l]=G[l][0]*p[0]+G[l][1]*p[1]+G[l][2]*p[2];
			break;
		}
	}
}

// poe o vertice no ponto xyz do mundo e grava nas classes
void TDrawOpenGL::MoveVertice(const VerticeFigura &V,const long double xyz[3]){

	const int i=V.Indice;
	switch(V.Tipo){
		case FiguraLinha:{
			Line *Lines=Desenho[0].Lines_Atr;
			long double p[3]={xyz[0],xyz[1],xyz[2]};
			DesfazGiroFigura(p,Lines->GetAnguloTeta(i),Lines->GetAnguloAlpha(i));
			std::array<PontoPolar,2> P=LerPontas(Lines,i);
			PontoPolar &Q=P[V.Ponto];
			Q.Xo=p[0];
			Q.Yo=p[1];
			Q.Zo=p[2];
			EixoXo(Q.Xo,Q.Yo,0,&Q.Alpha,&Q.Vetor);
			GravarPontas(Lines,i,P);
			break;
		}
		case FiguraQuadrilatero:{
			std::array<PontoPolar,4> C=LerCantos(Desenho[0].Quadrilatero,i);
			PontoPolar &Q=C[V.Ponto];
			Q.Xo=xyz[0];
			Q.Yo=xyz[1];
			Q.Zo=xyz[2];
			EixoXo(Q.Xo,Q.Yo,0,&Q.Alpha,&Q.Vetor);
			GravarCantos(Desenho[0].Quadrilatero,i,C);
			break;
		}
		case FiguraElipse:{
			Elipse *Elipses=Desenho[0].Elipse_Atr;
			// volta para antes do giro proprio: q = Gt*xyz (G e ortogonal)
			long double G[3][3],q[3];
			GiroDaElipse(Elipses->GetAnguloX(i),Elipses->GetAnguloY(i),Elipses->GetAnguloZ(i),G);
			for(int c=0;c<3;c++)
				q[c]=G[0][c]*xyz[0]+G[1][c]*xyz[1]+G[2][c]*xyz[2];

			if(V.Ponto==0){				// centro: a elipse inteira anda
				Elipses->SetCX(q[0],i);
				Elipses->SetCY(q[1],i);
				Elipses->SetCZ(q[2],i);
				Elipses->SetCenter_Atr(q[0],q[1],q[2],i);
			}else{						// ponta de um eixo: muda o A ou o B
				int U,Ve,N;
				EixosDaElipse(Elipses->GetTipoElipse_Atr(i),U,Ve,N);
				const long double C[3]={Elipses->GetCX(i),Elipses->GetCY(i),Elipses->GetCZ(i)};
				const long double Raio=std::max(std::hypot(q[U]-C[U],q[Ve]-C[Ve]),0.001L);
				if(V.Ponto==1)
					Elipses->SetA_Atr(Raio,i);
				else
					Elipses->SetB_Atr(Raio,i);
			}
			break;
		}
	}
}

// mundo -> tela: s = M*xyz (h, v e profundidade, em unidades do desenho)
void TDrawOpenGL::ParaTela(const long double xyz[3],long double s[3]){
	long double M[3][3];
	MatrizDaVisao(M);
	for(int l=0;l<3;l++)
		s[l]=M[l][0]*xyz[0]+M[l][1]*xyz[1]+M[l][2]*xyz[2];
}

// tela -> mundo com uma profundidade dada: xyz = Mt*s
void TDrawOpenGL::DaTela(long double h,long double v,long double Prof,long double xyz[3]){
	long double M[3][3];
	MatrizDaVisao(M);
	for(int k=0;k<3;k++)
		xyz[k]=M[0][k]*h+M[1][k]*v+M[2][k]*Prof;
}

// o vertice mais perto do mouse, ate 8 pixels
bool TDrawOpenGL::AchaVertice(int X,int Y,VerticeFigura &V,const VerticeFigura *Ignora,bool SoSelecionadas){

	const int Topo=AlturaFerramentas();
	const long double Cx=(ClientWidth-LarguraEdicao())/2.0L;
	const long double Cy=Topo+(ClientHeight-Topo)/2.0L;
	const long double Raio=8;

	std::vector<VerticeFigura> Vertices;
	if(SoSelecionadas)
		ListaVerticesSelecionados(Vertices);
	else
		ListaVertices(Vertices);			// o encaixe gruda em qualquer figura

	long double Melhor=Raio*Raio;
	bool Achou=false;
	for(size_t k=0;k<Vertices.size();k++){
		if((Ignora!=NULL)&&(Vertices[k].Tipo==Ignora->Tipo)&&(Vertices[k].Indice==Ignora->Indice))
			continue;					// nao encaixa na propria figura arrastada
		long double p[3],s[3];
		PosicaoVertice(Vertices[k],p);
		ParaTela(p,s);
		const long double dx=Cx+s[0]*Escala-X;
		const long double dy=Cy-s[1]*Escala-Y;
		const long double d2=dx*dx+dy*dy;
		if(d2<=Melhor){
			Melhor=d2;
			V=Vertices[k];
			Achou=true;
		}
	}
	return Achou;
}

// ponto do mouse; com o Encaixar marcado, gruda no vertice mais perto (ate 8 pixels)
TDrawOpenGL::PontoTela TDrawOpenGL::PontoComEncaixe(int X,int Y,const VerticeFigura *Ignora){

	PontoTela P=TelaDoMouse(X,Y);
	VerticeFigura V;
	if(ChkEncaixe->Checked&&(!ChkPerspectiva->Checked)&&AchaVertice(X,Y,V,Ignora)){
		long double s[3];
		PosicaoVertice(V,P.Mundo);
		ParaTela(P.Mundo,s);
		P.h=s[0];
		P.v=s[1];
		P.TemMundo=true;
	}
	return P;
}

// marca o vertice em que o mouse encaixou. Chamada dentro da matriz da visao.
void TDrawOpenGL::DesenhaEncaixe(){

	if((!TemCursor)||(!Cursor.TemMundo)||ChkPerspectiva->Checked)
		return;

	glDisable(GL_DEPTH_TEST);
	glPointSize(11);
	glColor3f(1.0f,0.2f,0.9f);
	glBegin(GL_POINTS);
	Vertice(Cursor.Mundo);
	glEnd();
	glPointSize(1);
	glEnable(GL_DEPTH_TEST);
}

//---------------------------------------------------------------------------
// Desfazer e Refazer: antes de cada mudanca o desenho inteiro e copiado
//---------------------------------------------------------------------------

TDrawOpenGL::EstadoDesenho TDrawOpenGL::CapturaEstado(){

	EstadoDesenho E;

	Line *Lines=Desenho[0].Lines_Atr;
	E.Linhas.resize(Lines->GetContador_Line());
	for(int i=0;i<(int)E.Linhas.size();i++){
		LinhaGuardada &L=E.Linhas[i];
		L.P=LerPontas(Lines,i);
		L.Cor=Lines->GetCor_Line(i);
		L.Alpha=Lines->GetAnguloAlpha(i);
		L.Beta=Lines->GetAnguloBeta(i);
		L.Teta=Lines->GetAnguloTeta(i);
	}

	TQuadrilatero *Quad=Desenho[0].Quadrilatero;
	E.Quads.resize(Quad->GetContadorQuadrilatero());
	for(int i=0;i<(int)E.Quads.size();i++){
		E.Quads[i].C=LerCantos(Quad,i);
		E.Quads[i].Cor=Quad->GetCor(i);
	}

	Elipse *Elipses=Desenho[0].Elipse_Atr;
	E.Elipses.resize(Elipses->GetContadorCircle_Atr());
	for(int i=0;i<(int)E.Elipses.size();i++){
		ElipseGuardada &G=E.Elipses[i];
		G.A=Elipses->GetA_Atr(i);
		G.B=Elipses->GetB_Atr(i);
		G.AZo=Elipses->GetAZo_Atr(i);
		G.BZo=Elipses->GetBZo_Atr(i);
		G.CX=Elipses->GetCX(i);
		G.CY=Elipses->GetCY(i);
		G.CZ=Elipses->GetCZ(i);
		G.AnguloX=Elipses->GetAnguloX(i);
		G.AnguloY=Elipses->GetAnguloY(i);
		G.AnguloZ=Elipses->GetAnguloZ(i);
		G.Tipo=Elipses->GetTipoElipse_Atr(i);
		G.Cor=Elipses->GetColor_Circle_Atr(i);
	}

	E.Duplicacao=Duplicacao;
	E.ComecoDuplicacao=ComecoDuplicacao;
	return E;
}

// monta um desenho novo com as figuras guardadas; a visao continua como esta
void TDrawOpenGL::RestauraEstado(const EstadoDesenho &E){

	Objetos *Novo=new Objetos[1]();
	long double ABT[3];
	Desenho[0].GetAlpha_B_T(ABT);
	Novo[0].SetAlpha_B_T(ABT);

	const int NL=(int)E.Linhas.size();
	Line *Lines=Novo[0].Lines_Atr;
	Lines->Alocando_Line(NL);
	for(int i=0;i<NL;i++){
		GravarPontas(Lines,i,E.Linhas[i].P);
		Lines->SetCor_Line(E.Linhas[i].Cor,i);
		Lines->Set_AngluloAlpha(E.Linhas[i].Alpha,i);
		Lines->Set_AngluloBeta(E.Linhas[i].Beta,i);
		Lines->Set_AngluloTeta(E.Linhas[i].Teta,i);
	}
	Lines->SetContador_Line(NL);

	const int NQ=(int)E.Quads.size();
	TQuadrilatero *Quad=Novo[0].Quadrilatero;
	if(NQ>0)
		Quad->AlocarQuadrilatero(NQ);
	for(int i=0;i<NQ;i++){
		GravarCantos(Quad,i,E.Quads[i].C);
		Quad->SetCor(E.Quads[i].Cor,i);
	}
	Quad->SetContadorQuadrilatero(NQ);

	const int NE=(int)E.Elipses.size();
	Elipse *Elipses=Novo[0].Elipse_Atr;
	Elipses->AlocaElipse_Atr(NE);
	for(int i=0;i<NE;i++){
		const ElipseGuardada &G=E.Elipses[i];
		Elipses->SetA_Atr(G.A,i);
		Elipses->SetB_Atr(G.B,i);
		Elipses->SetAZo_Atr(G.AZo,i);
		Elipses->SetBZo_Atr(G.BZo,i);
		Elipses->SetCX(G.CX,i);
		Elipses->SetCY(G.CY,i);
		Elipses->SetCZ(G.CZ,i);
		Elipses->SetCenter_Atr(G.CX,G.CY,G.CZ,i);
		Elipses->Set_AnguloX(G.AnguloX,i);
		Elipses->Set_AnguloY(G.AnguloY,i);
		Elipses->Set_AnguloZ(G.AnguloZ,i);
		Elipses->SetTipoElipse_Atr(G.Tipo,i);
		Elipses->SetColor_Circle_Atr(G.Cor,i);
	}
	Elipses->SetContadorCircle_Atr(NE);

	delete[] Desenho;
	Desenho=Novo;
	Duplicacao=E.Duplicacao;
	ComecoDuplicacao=E.ComecoDuplicacao;
	Cliques.clear();
	TemVerticeSobMouse=false;
	AtualizaListaFiguras();
	Desenha();
}

// Antes e o desenho de antes da mudanca que acabou de ser feita (ou vai ser)
void TDrawOpenGL::EmpilhaDesfazer(const EstadoDesenho &Antes){
	static const size_t MaximoDesfazer=100;
	PilhaDesfazer.push_back(Antes);
	if(PilhaDesfazer.size()>MaximoDesfazer)
		PilhaDesfazer.erase(PilhaDesfazer.begin());
	PilhaRefazer.clear();
	AtualizaBotoesDesfazer();
}

void TDrawOpenGL::Desfazer(){
	if(Arrastando||PilhaDesfazer.empty())
		return;
	PilhaRefazer.push_back(CapturaEstado());
	const EstadoDesenho E=PilhaDesfazer.back();
	PilhaDesfazer.pop_back();
	RestauraEstado(E);
	AtualizaBotoesDesfazer();
	Status("Desfeito");
}

void TDrawOpenGL::Refazer(){
	if(Arrastando||PilhaRefazer.empty())
		return;
	PilhaDesfazer.push_back(CapturaEstado());
	const EstadoDesenho E=PilhaRefazer.back();
	PilhaRefazer.pop_back();
	RestauraEstado(E);
	AtualizaBotoesDesfazer();
	Status("Refeito");
}

void TDrawOpenGL::AtualizaBotoesDesfazer(){
	BtnDesfazer->Enabled=!PilhaDesfazer.empty();
	BtnRefazer->Enabled=!PilhaRefazer.empty();
}

//---------------------------------------------------------------------------
// Extrusao da figura escolhida no CBFiguras: as figuras novas sao quadrilateros
// comuns, que depois podem ser editados como qualquer outro
//---------------------------------------------------------------------------

void TDrawOpenGL::AdicionaQuad(const long double P[4][3],COLORREF Cor){

	TQuadrilatero *Quad=Desenho[0].Quadrilatero;
	const int Novo=Quad->GetContadorQuadrilatero();
	std::array<PontoPolar,4> C;

	for(int k=0;k<4;k++){
		C[k].Xo=P[k][0];
		C[k].Yo=P[k][1];
		C[k].Zo=P[k][2];
		EixoXo(C[k].Xo,C[k].Yo,0,&C[k].Alpha,&C[k].Vetor);
	}

	Quad->AlocarQuadrilatero(Novo+1);
	GravarCantos(Quad,Novo,C);
	Quad->SetCor(Cor,Novo);
	Quad->SetContadorQuadrilatero(Novo+1);
	Duplicacao.GuardaPlanoQuad(Novo,PlanoAtual());
}

// a parede que sai da aresta A-B na direcao d, escurecida conforme a direcao dela
void TDrawOpenGL::AdicionaLateral(const long double A[3],const long double B[3],const long double d[3],COLORREF Cor){

	const long double P[4][3]={
		{A[0],A[1],A[2]},
		{B[0],B[1],B[2]},
		{B[0]+d[0],B[1]+d[1],B[2]+d[2]},
		{A[0]+d[0],A[1]+d[1],A[2]+d[2]}
	};
	long double n[3];
	Produto(P[1],P[0],d,n);
	AdicionaQuad(P,CorSombreada(Cor,n));
}

// a linha vira uma parede, saindo da tela (false: ela esta na direcao da extrusao)
bool TDrawOpenGL::ExtrudaLinha(int i,long double H,const long double Tela[3]){

	long double A[3],B[3];
	PosicaoVertice(VerticeFigura{FiguraLinha,i,0},A);
	PosicaoVertice(VerticeFigura{FiguraLinha,i,1},B);
	const long double d[3]={Tela[0]*H,Tela[1]*H,Tela[2]*H};
	long double n[3];
	Produto(B,A,d,n);
	if(Modulo(n)<1e-9L)
		return false;
	AdicionaLateral(A,B,d,Desenho[0].Lines_Atr->GetCor_Line(i));
	return true;
}

// cada quadrilatero ganha o topo e as paredes, na normal dele virada para quem olha.
// A aresta dividida com outro quadrilatero da colecao que sobe na mesma direcao
// ficaria dentro do solido: ela nao ganha parede. Devolve quantos foram extrudados.
int TDrawOpenGL::ExtrudaQuads(const std::vector<int> &Indices,long double H,const long double Tela[3]){

	struct QuadExtrudado{
		long double C[4][3];
		long double d[3];
		COLORREF Cor;
	};
	std::vector<QuadExtrudado> Quads;

	for(size_t q=0;q<Indices.size();q++){
		QuadExtrudado Q;
		for(int k=0;k<4;k++)
			PosicaoVertice(VerticeFigura{FiguraQuadrilatero,Indices[q],k},Q.C[k]);

		// normal pelo metodo de Newell: funciona mesmo com dois cantos iguais
		long double n[3]={0,0,0};
		for(int k=0;k<4;k++){
			const long double *a=Q.C[k],*b=Q.C[(k+1)%4];
			n[0]+=(a[1]-b[1])*(a[2]+b[2]);
			n[1]+=(a[2]-b[2])*(a[0]+b[0]);
			n[2]+=(a[0]-b[0])*(a[1]+b[1]);
		}
		const long double Tam=Modulo(n);
		if(Tam<1e-9L)
			continue;					// sem area: nao da para extrudar
		const long double Sinal=(n[0]*Tela[0]+n[1]*Tela[1]+n[2]*Tela[2]<0)? -1.0L : 1.0L;
		for(int c=0;c<3;c++)
			Q.d[c]=n[c]/Tam*H*Sinal;
		Q.Cor=Desenho[0].Quadrilatero->GetCor(Indices[q]);
		Quads.push_back(Q);
	}

	const long double Tol=1e-6L;
	const long double TolDirecao=Tol*(1+std::fabs(H));
	const auto Iguais=[](const long double *a,const long double *b,long double Limite){
		return (std::fabs(a[0]-b[0])<=Limite)&&(std::fabs(a[1]-b[1])<=Limite)&&(std::fabs(a[2]-b[2])<=Limite);
	};

	for(size_t q=0;q<Quads.size();q++){
		const QuadExtrudado &Q=Quads[q];

		long double Topo[4][3];
		for(int k=0;k<4;k++)
			for(int c=0;c<3;c++)
				Topo[k][c]=Q.C[k][c]+Q.d[c];
		AdicionaQuad(Topo,Q.Cor);

		for(int k=0;k<4;k++){
			const long double *a=Q.C[k],*b=Q.C[(k+1)%4];
			if(Iguais(a,b,1e-9L))
				continue;				// dois cantos iguais: essa aresta nao existe

			bool Interna=false;
			for(size_t o=0;(o<Quads.size())&&(!Interna);o++){
				if((o==q)||(!Iguais(Q.d,Quads[o].d,TolDirecao)))
					continue;
				for(int e=0;(e<4)&&(!Interna);e++){
					const long double *c=Quads[o].C[e],*f=Quads[o].C[(e+1)%4];
					Interna=(Iguais(a,c,Tol)&&Iguais(b,f,Tol))||(Iguais(a,f,Tol)&&Iguais(b,c,Tol));
				}
			}
			if(!Interna)
				AdicionaLateral(a,b,Q.d,Q.Cor);
		}
	}
	return (int)Quads.size();
}

// a elipse vira um cilindro fechado
void TDrawOpenGL::ExtrudaElipse(int i,long double H,const long double Tela[3]){

	Elipse *Elipses=Desenho[0].Elipse_Atr;
	const int Tipo=Elipses->GetTipoElipse_Atr(i);
	int U,V,N;
	EixosDaElipse(Tipo,U,V,N);

	long double G[3][3];
	GiroDaElipse(Elipses->GetAnguloX(i),Elipses->GetAnguloY(i),Elipses->GetAnguloZ(i),G);
	const long double Sinal=(G[0][N]*Tela[0]+G[1][N]*Tela[1]+G[2][N]*Tela[2]<0)? -1.0L : 1.0L;
	const long double d[3]={G[0][N]*H*Sinal,G[1][N]*H*Sinal,G[2][N]*H*Sinal};
	const COLORREF Cor=Elipses->GetColor_Circle_Atr(i);

	long double Local[3]={Elipses->GetCX(i),Elipses->GetCY(i),Elipses->GetCZ(i)};
	long double Centro[3];
	for(int l=0;l<3;l++)
		Centro[l]=G[l][0]*Local[0]+G[l][1]*Local[1]+G[l][2]*Local[2];

	long double Pontos[PontosElipse][3];
	for(int k=0;k<PontosElipse;k++)
		PontoElipse3D(Elipses,i,k,Pontos[k]);

	for(int k=0;k<PontosElipse-1;k++)
		AdicionaLateral(Pontos[k],Pontos[k+1],d,Cor);

	// tampas de baixo e de cima, em leque a partir do centro
	for(int k=0;k+2<PontosElipse;k+=2)
		for(int Tampa=0;Tampa<2;Tampa++){
			const long double f=Tampa? 1.0L : 0.0L;
			long double Q[4][3];
			for(int c=0;c<3;c++){
				Q[0][c]=Centro[c]+f*d[c];
				Q[1][c]=Pontos[k][c]+f*d[c];
				Q[2][c]=Pontos[k+1][c]+f*d[c];
				Q[3][c]=Pontos[k+2][c]+f*d[c];
			}
			AdicionaQuad(Q,Cor);
		}

	// o contorno de cima e uma copia da elipse, deslocada na normal dela
	const int Novo=Elipses->GetContadorCircle_Atr();
	Local[N]+=H*Sinal;
	const long double A=Elipses->GetA_Atr(i),B=Elipses->GetB_Atr(i);
	const long double AZo=Elipses->GetAZo_Atr(i),BZo=Elipses->GetBZo_Atr(i);
	const long double AX=Elipses->GetAnguloX(i),AY=Elipses->GetAnguloY(i),AZ=Elipses->GetAnguloZ(i);
	Elipses->AlocaElipse_Atr(Novo+1);
	Elipses->SetA_Atr(A,Novo);
	Elipses->SetB_Atr(B,Novo);
	Elipses->SetAZo_Atr(AZo,Novo);
	Elipses->SetBZo_Atr(BZo,Novo);
	Elipses->SetCX(Local[0],Novo);
	Elipses->SetCY(Local[1],Novo);
	Elipses->SetCZ(Local[2],Novo);
	Elipses->SetCenter_Atr(Local[0],Local[1],Local[2],Novo);
	Elipses->Set_AnguloX(AX,Novo);
	Elipses->Set_AnguloY(AY,Novo);
	Elipses->Set_AnguloZ(AZ,Novo);
	Elipses->SetTipoElipse_Atr(Tipo,Novo);
	Elipses->SetColor_Circle_Atr(Cor,Novo);
	Elipses->SetContadorCircle_Atr(Novo+1);
	Duplicacao.GuardaPlanoCircle(Novo,PlanoAtual());
}

// extruda a colecao selecionada; sem colecao, a figura escolhida na lista.
// Tudo entra num passo so do Desfazer.
void TDrawOpenGL::Extruda(){

	std::vector<ItemFigura> Alvos(Colecao);
	if(Alvos.empty()){
		ItemFigura Item;
		if(FiguraSelecionada(Item))
			Alvos.push_back(Item);
	}
	if(Alvos.empty()){
		Status("Selecione as figuras (ou escolha uma na lista) para dar altura");
		return;
	}
	double Altura;
	if((!LeNumero(EdtExtrusao->Text,Altura))||(Altura==0)){
		Status("Digite uma altura diferente de zero");
		return;
	}

	// a terceira linha da matriz da visao e o eixo que sai da tela
	long double M[3][3];
	MatrizDaVisao(M);
	const long double *Tela=M[2];
	const long double H=Altura;
	const EstadoDesenho Antes=CapturaEstado();
	const int QuadsAntes=Desenho[0].Quadrilatero->GetContadorQuadrilatero();

	int Prontas=0;
	std::vector<int> Quads;				// os quadrilateros vao juntos, por causa das arestas comuns
	for(size_t k=0;k<Alvos.size();k++)
		switch(Alvos[k].Tipo){
			case FiguraLinha:
				if(ExtrudaLinha(Alvos[k].Indice,H,Tela))
					Prontas++;
				break;
			case FiguraQuadrilatero:
				Quads.push_back(Alvos[k].Indice);
				break;
			case FiguraElipse:
				ExtrudaElipse(Alvos[k].Indice,H,Tela);
				Prontas++;
				break;
		}
	Prontas+=ExtrudaQuads(Quads,H,Tela);

	if(Prontas==0){
		Status("Nenhuma figura pode ser extrudada: sem area ou na direcao da extrusao");
		return;
	}

	EmpilhaDesfazer(Antes);
	Colecao.clear();					// evita extrudar de novo a mesma colecao sem querer
	CancelaFigura();
	AtualizaListaFiguras();
	Desenha();
	const int Ignoradas=(int)Alvos.size()-Prontas;
	UnicodeString Texto=Format("Extrusao pronta: %d figuras, %d quadrilateros novos. Gire a visao para ver a altura.",
		ARRAYOFCONST((Prontas,Desenho[0].Quadrilatero->GetContadorQuadrilatero()-QuadsAntes)));
	if(Ignoradas>0)
		Texto+=Format(" %d ignoradas.",ARRAYOFCONST((Ignoradas)));
	Status(Texto);
}

//---------------------------------------------------------------------------
// Colecao de figuras (ChkSelecionar): clique marca ou desmarca uma figura e
// arrastar um retangulo marca as que estiverem inteiras dentro dele
//---------------------------------------------------------------------------

int TDrawOpenGL::PosicaoNaColecao(const ItemFigura &Item){
	for(size_t k=0;k<Colecao.size();k++)
		if((Colecao[k].Tipo==Item.Tipo)&&(Colecao[k].Indice==Item.Indice))
			return (int)k;
	return -1;
}

void TDrawOpenGL::AlternaNaColecao(const ItemFigura &Item){
	const int k=PosicaoNaColecao(Item);
	if(k>=0)
		Colecao.erase(Colecao.begin()+k);
	else
		Colecao.push_back(Item);
	AtualizaColecao();
}

// o numero de figuras aparece no botao de extrudar
void TDrawOpenGL::AtualizaColecao(){
	BtnExtrudar->Caption=Colecao.empty()? UnicodeString("Extrudar")
		: Format("Extrudar (%d)",ARRAYOFCONST(((int)Colecao.size())));
}

// a figura debaixo do mouse: a linha mais perto (ate 8 pixels) ou, se nao houver,
// o quadrilatero mais a frente que contem o ponto
bool TDrawOpenGL::AchaFigura(int X,int Y,ItemFigura &Achada){

	const PontoTela P=TelaDoMouse(X,Y);
	const long double Raio=8/Escala;
	long double Melhor=Raio*Raio;
	bool Achou=false;
	bool TemQuad=false;
	ItemFigura Quad={0,0};
	long double QuadProf=0;

	for(size_t f=0;f<Figuras.size();f++){
		std::vector<std::array<long double,3> > Pontos;
		bool Fechado;
		ContornoFigura(Figuras[f],Pontos,Fechado);
		const size_t n=Pontos.size();
		if(n<2)
			continue;

		long double Prof=0;
		bool Dentro=false;
		long double a[3],b[3];
		const size_t Lados=Fechado? n : n-1;
		for(size_t k=0;k<Lados;k++){
			ParaTela(Pontos[k].data(),a);
			ParaTela(Pontos[(k+1)%n].data(),b);
			Prof+=a[2];

			// distancia do mouse ao segmento a-b, no plano da tela
			const long double dh=b[0]-a[0],dv=b[1]-a[1];
			const long double L2=dh*dh+dv*dv;
			long double t=(L2>0)? ((P.h-a[0])*dh+(P.v-a[1])*dv)/L2 : 0;
			t=std::min(std::max(t,0.0L),1.0L);
			const long double eh=a[0]+t*dh-P.h,ev=a[1]+t*dv-P.v;
			const long double d2=eh*eh+ev*ev;
			if(d2<=Melhor){
				Melhor=d2;
				Achada=Figuras[f];
				Achou=true;
			}

			// o raio horizontal a partir do mouse cruza o contorno um numero impar de vezes
			if(((a[1]>P.v)!=(b[1]>P.v))&&(P.h<a[0]+(P.v-a[1])/(b[1]-a[1])*dh))
				Dentro=!Dentro;
		}

		if(Dentro&&(Figuras[f].Tipo==FiguraQuadrilatero)){
			Prof/=Lados;
			if((!TemQuad)||(Prof>QuadProf)){		// a profundidade cresce para fora da tela
				TemQuad=true;
				Quad=Figuras[f];
				QuadProf=Prof;
			}
		}
	}

	if((!Achou)&&TemQuad){
		Achada=Quad;
		return true;
	}
	return Achou;
}

// clique: alterna a figura; arraste: junta as que estao inteiras no retangulo
void TDrawOpenGL::TerminaSelecao(int X,int Y){

	if((std::fabs(RetFim.h-RetInicio.h)*Escala<4)&&(std::fabs(RetFim.v-RetInicio.v)*Escala<4)){
		ItemFigura Item;
		if(AchaFigura(X,Y,Item))
			AlternaNaColecao(Item);
	}else{
		const long double hMin=std::min(RetInicio.h,RetFim.h),hMax=std::max(RetInicio.h,RetFim.h);
		const long double vMin=std::min(RetInicio.v,RetFim.v),vMax=std::max(RetInicio.v,RetFim.v);
		for(size_t f=0;f<Figuras.size();f++){
			if(PosicaoNaColecao(Figuras[f])>=0)
				continue;
			std::vector<std::array<long double,3> > Pontos;
			bool Fechado;
			ContornoFigura(Figuras[f],Pontos,Fechado);
			bool Dentro=!Pontos.empty();
			for(size_t k=0;(k<Pontos.size())&&Dentro;k++){
				long double s[3];
				ParaTela(Pontos[k].data(),s);
				Dentro=(s[0]>=hMin)&&(s[0]<=hMax)&&(s[1]>=vMin)&&(s[1]<=vMax);
			}
			if(Dentro)
				Colecao.push_back(Figuras[f]);
		}
		AtualizaColecao();
	}
	Status(Format("%d figuras selecionadas",ARRAYOFCONST(((int)Colecao.size()))));
}

bool TDrawOpenGL::MesmoVertice(const VerticeFigura &A,const VerticeFigura &B){
	return (A.Tipo==B.Tipo)&&(A.Indice==B.Indice)&&(A.Ponto==B.Ponto);
}

UnicodeString TDrawOpenGL::NomeVertice(const VerticeFigura &V){
	static const wchar_t *Linha[2]={L"M",L"L"};
	static const wchar_t *Canto[4]={L"M",L"L",L"M2",L"L2"};
	static const wchar_t *DaElipse[3]={L"centro",L"eixo A",L"eixo B"};
	switch(V.Tipo){
		case FiguraLinha:			return Format("Linha %d, ponto %s",ARRAYOFCONST((V.Indice+1,UnicodeString(Linha[V.Ponto]))));
		case FiguraQuadrilatero:	return Format("Quadrilatero %d, canto %s",ARRAYOFCONST((V.Indice+1,UnicodeString(Canto[V.Ponto]))));
		case FiguraElipse:			return Format("Elipse %d, %s",ARRAYOFCONST((V.Indice+1,UnicodeString(DaElipse[V.Ponto]))));
	}
	return "";
}

// escolhe a figura no CBFiguras e mostra os valores dela no painel de edicao
void TDrawOpenGL::SelecionaFigura(int Tipo,int Indice){
	for(size_t k=0;k<Figuras.size();k++)
		if((Figuras[k].Tipo==Tipo)&&(Figuras[k].Indice==Indice)){
			CBFiguras->ItemIndex=(int)k;
			break;
		}
	CarregaEdicao();
}

// o membro Cursor (PontoTela) esconde a propriedade Cursor do form
void TDrawOpenGL::CursorDoMouse(TCursor Novo){
	if(TControl::Cursor!=Novo)
		TControl::Cursor=Novo;
}

// os vertices como pontos: o debaixo do mouse e o arrastado aparecem maiores.
// Chamada dentro da matriz da visao.
void TDrawOpenGL::DesenhaVertices(){

	if(!EditandoVertices())
		return;

	std::vector<VerticeFigura> Vertices;
	ListaVerticesSelecionados(Vertices);
	long double p[3];

	glDisable(GL_DEPTH_TEST);
	// cores: MoveTo (M, M2) em lime, LineTo (L, L2) em purple e os pontos da
	// elipse (centro, eixos A e B) em branco
	glPointSize(6);
	glBegin(GL_POINTS);
	for(size_t k=0;k<Vertices.size();k++){
		const VerticeFigura &V=Vertices[k];
		if(V.Tipo==FiguraElipse)
			glColor3f(0.9f,0.9f,0.9f);
		else if((V.Ponto%2)==0)						// M (0) e M2 (2)
			glColor3f(0.0f,1.0f,0.0f);				// lime
		else										// L (1) e L2 (3)
			glColor3f(0.5f,0.0f,0.5f);				// purple
		PosicaoVertice(V,p);
		Vertice(p);
	}
	glEnd();

	if(Arrastando||TemVerticeSobMouse){
		const VerticeFigura &V=Arrastando? VerticeAtivo : VerticeSobMouse;
		PosicaoVertice(V,p);
		glPointSize(11);
		if(Arrastando)
			glColor3f(1.0f,0.5f,0.1f);
		else
			glColor3f(0.3f,0.9f,1.0f);
		glBegin(GL_POINTS);
		Vertice(p);
		glEnd();
	}
	glPointSize(1);
	glEnable(GL_DEPTH_TEST);
}

//---------------------------------------------------------------------------
// Desenho em OpenGL (a partir das classes)
//---------------------------------------------------------------------------

void TDrawOpenGL::DesenhaEixos(){

	const double T=150;
	glBegin(GL_LINES);
		glColor3f(0.6f,0.15f,0.15f);	glVertex3d(-T,0,0);	glVertex3d(T,0,0);		// Xo
		glColor3f(0.15f,0.6f,0.15f);	glVertex3d(0,-T,0);	glVertex3d(0,T,0);		// Yo
		glColor3f(0.2f,0.3f,0.8f);		glVertex3d(0,0,-T);	glVertex3d(0,0,T);		// Zo
	glEnd();
}

void TDrawOpenGL::DesenhaLinhas(){

	Line *Lines=Desenho[0].Lines_Atr;
	long double M[3],L[3];

	glBegin(GL_LINES);
	for(int i=0;i<Lines->GetContador_Line();i++){
		Polar3D(Lines->GetMAlpha_Atr(i),Lines->GetMVetor_Atr(i),Lines->GetMZo_Atr(i),M);
		Polar3D(Lines->GetLAlpha_Atr(i),Lines->GetLVetor_Atr(i),Lines->GetLZo_Atr(i),L);
		GiraFigura(M,Lines->GetAnguloTeta(i),Lines->GetAnguloAlpha(i));
		GiraFigura(L,Lines->GetAnguloTeta(i),Lines->GetAnguloAlpha(i));

		CorGL(Lines->GetCor_Line(i));
		Vertice(M);
		Vertice(L);
	}
	glEnd();
}

// quadrilateros solidos; o contorno sai na mesma cor para aparecer de lado
void TDrawOpenGL::DesenhaQuadrilateros(){

	TQuadrilatero *Quad=Desenho[0].Quadrilatero;
	const int Quantidade=Quad->GetContadorQuadrilatero();
	std::vector<std::array<long double,3> > Cantos(4*(size_t)Quantidade);

	for(int i=0;i<Quantidade;i++){
		Polar3D(Quad->GetMAlpha(i), Quad->GetMVetor(i), Quad->GetMZo(i), Cantos[4*i+0].data());
		Polar3D(Quad->GetLAlpha(i), Quad->GetLVetor(i), Quad->GetLZo(i), Cantos[4*i+1].data());
		Polar3D(Quad->GetMAlpha2(i),Quad->GetMVetor2(i),Quad->GetMZo2(i),Cantos[4*i+2].data());
		Polar3D(Quad->GetLAlpha2(i),Quad->GetLVetor2(i),Quad->GetLZo2(i),Cantos[4*i+3].data());
	}

	// as faces ficam um pouco atras, para as linhas por cima nao piscarem
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1,1);
	glBegin(GL_QUADS);
	for(int i=0;i<Quantidade;i++){
		CorGL(Quad->GetCor(i));
		for(int k=0;k<4;k++)
			Vertice(Cantos[4*i+k].data());
	}
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void TDrawOpenGL::DesenhaElipses(){

	Elipse *Elipses=Desenho[0].Elipse_Atr;
	long double Ponto[3];

	for(int j=0;j<Elipses->GetContadorCircle_Atr();j++){

		CorGL(Elipses->GetColor_Circle_Atr(j));
		glBegin(GL_LINE_STRIP);
		for(int i=0;i<PontosElipse;i++){
			PontoElipse3D(Elipses,j,i,Ponto);		// a mesma conta do EixoXY, com o giro proprio
			Vertice(Ponto);
		}
		glEnd();
	}
}

// figura em andamento, direto na tela (sem a matriz da visao)
void TDrawOpenGL::DesenhaPrevia(){

	if(Cliques.empty())
		return;

	std::vector<PontoTela> Pontos(Cliques);
	if(TemCursor)
		Pontos.push_back(Cursor);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f,0.85f,0.2f);

	if(RGFigura->ItemIndex==FiguraElipse){
		if(Pontos.size()>=2){
			const long double R=std::hypot(Pontos[1].h-Pontos[0].h,Pontos[1].v-Pontos[0].v);
			glBegin(GL_LINE_LOOP);
			for(int i=0;i<PontosElipse-1;i++){
				const long double t=i*(2*M_PI/(PontosElipse-1));
				glVertex2d((double)(Pontos[0].h+R*std::cos(t)),(double)(Pontos[0].v+R*std::sin(t)));
			}
			glEnd();
		}
	}else{
		glBegin(GL_LINE_STRIP);
		for(size_t k=0;k<Pontos.size();k++)
			glVertex2d((double)Pontos[k].h,(double)Pontos[k].v);
		if((RGFigura->ItemIndex==FiguraQuadrilatero)&&(Pontos.size()==4))
			glVertex2d((double)Pontos[0].h,(double)Pontos[0].v);		// fecha o contorno
		glEnd();
	}

	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}

// o retangulo de selecao sendo arrastado
void TDrawOpenGL::DesenhaRetanguloSelecao(){

	if(!SelecionandoRet)
		return;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glColor3f(0.0f,1.0f,1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2d((double)RetInicio.h,(double)RetInicio.v);
	glVertex2d((double)RetFim.h,(double)RetInicio.v);
	glVertex2d((double)RetFim.h,(double)RetFim.v);
	glVertex2d((double)RetInicio.h,(double)RetFim.v);
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}

// Tres linhas de referencia ate o ponto do mouse (ja no mundo, com a profundidade):
// de (x,0,0) no Xo, de (0,y,0) no Yo e, na direcao Zo, de (x,y,0), a projecao no XoY.
// Tracejadas, na cor do eixo, e por cima de tudo. Chamada dentro da matriz da visao.
void TDrawOpenGL::DesenhaReferencia(){

	if((!TemCursor)||ChkPerspectiva->Checked)
		return;

	long double P[3];
	TelaParaMundo(Cursor,P);
	const double x=(double)P[0],y=(double)P[1],z=(double)P[2];

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0x0F0F);

	glBegin(GL_LINES);
		glColor3f(0.95f,0.4f,0.4f);		glVertex3d(x,0,0);	glVertex3d(x,y,z);		// do Xo
		glColor3f(0.4f,0.95f,0.4f);		glVertex3d(0,y,0);	glVertex3d(x,y,z);		// do Yo
		glColor3f(0.45f,0.6f,1.0f);		glVertex3d(x,y,0);	glVertex3d(x,y,z);		// da projecao no plano XoY
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	// os pes das linhas em cada eixo
	glPointSize(5);
	glBegin(GL_POINTS);
		glColor3f(0.95f,0.4f,0.4f);		glVertex3d(x,0,0);
		glColor3f(0.4f,0.95f,0.4f);		glVertex3d(0,y,0);
		glColor3f(0.45f,0.6f,1.0f);		glVertex3d(x,y,0);
	glEnd();
	glPointSize(1);

	glEnable(GL_DEPTH_TEST);
}

// os pontos do contorno de uma figura, no mundo
void TDrawOpenGL::ContornoFigura(const ItemFigura &Item,std::vector<std::array<long double,3> > &Pontos,bool &Fechado){

	Pontos.clear();
	Fechado=false;
	const int i=Item.Indice;

	switch(Item.Tipo){
		case FiguraLinha:{
			Line *Lines=Desenho[0].Lines_Atr;
			Pontos.resize(2);
			Polar3D(Lines->GetMAlpha_Atr(i),Lines->GetMVetor_Atr(i),Lines->GetMZo_Atr(i),Pontos[0].data());
			Polar3D(Lines->GetLAlpha_Atr(i),Lines->GetLVetor_Atr(i),Lines->GetLZo_Atr(i),Pontos[1].data());
			for(size_t k=0;k<Pontos.size();k++)
				GiraFigura(Pontos[k].data(),Lines->GetAnguloTeta(i),Lines->GetAnguloAlpha(i));
			break;
		}
		case FiguraQuadrilatero:{
			TQuadrilatero *Quad=Desenho[0].Quadrilatero;
			Pontos.resize(4);
			Polar3D(Quad->GetMAlpha(i), Quad->GetMVetor(i), Quad->GetMZo(i), Pontos[0].data());
			Polar3D(Quad->GetLAlpha(i), Quad->GetLVetor(i), Quad->GetLZo(i), Pontos[1].data());
			Polar3D(Quad->GetMAlpha2(i),Quad->GetMVetor2(i),Quad->GetMZo2(i),Pontos[2].data());
			Polar3D(Quad->GetLAlpha2(i),Quad->GetLVetor2(i),Quad->GetLZo2(i),Pontos[3].data());
			Fechado=true;
			break;
		}
		case FiguraElipse:{
			Pontos.resize(PontosElipse);
			for(int k=0;k<PontosElipse;k++)
				PontoElipse3D(Desenho[0].Elipse_Atr,i,k,Pontos[k].data());
			break;
		}
	}
}

// a colecao em ciano e a figura escolhida no CBFiguras em amarelo, grossas e por
// cima de tudo; a escolhida tem os pontos marcados. Chamada dentro da matriz da visao.
void TDrawOpenGL::DesenhaSelecionada(){

	std::vector<std::array<long double,3> > Pontos;
	bool Fechado;

	glDisable(GL_DEPTH_TEST);
	glColor3f(0.0f,1.0f,1.0f);
	glLineWidth(3);
	for(size_t c=0;c<Colecao.size();c++){
		ContornoFigura(Colecao[c],Pontos,Fechado);
		glBegin(Fechado? GL_LINE_LOOP : GL_LINE_STRIP);
		for(size_t k=0;k<Pontos.size();k++)
			Vertice(Pontos[k].data());
		glEnd();
	}
	glLineWidth(1);
	glEnable(GL_DEPTH_TEST);

	ItemFigura Item;
	if(!FiguraSelecionada(Item))
		return;
	ContornoFigura(Item,Pontos,Fechado);

	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f,1.0f,0.4f);
	glLineWidth(3);
	glBegin(Fechado? GL_LINE_LOOP : GL_LINE_STRIP);
	for(size_t k=0;k<Pontos.size();k++)
		Vertice(Pontos[k].data());
	glEnd();
	glLineWidth(1);

	// pontos que podem ser remodelados (na elipse, so o comeco do contorno)
	glPointSize(7);
	glBegin(GL_POINTS);
	const size_t Marcados=(Item.Tipo==FiguraElipse)? 1 : Pontos.size();
	for(size_t k=0;k<Marcados;k++)
		Vertice(Pontos[k].data());
	glEnd();
	glPointSize(1);
	glEnable(GL_DEPTH_TEST);
}

void TDrawOpenGL::Desenha(){

	if((Desenho==NULL)||(!HandleAllocated())||(!IniciaGL()))
		return;

	// o Render usa outro contexto: ele e devolvido no fim
	const HDC DCAnterior=wglGetCurrentDC();
	const HGLRC ContextoAnterior=wglGetCurrentContext();
	if(!wglMakeCurrent(hdc,hrc))
		return;

	const int Largura=std::max(ClientWidth-LarguraEdicao(),1);		// sem o painel de edicao, a direita
	const int Altura=std::max(ClientHeight-AlturaFerramentas(),1);
	glViewport(0,0,Largura,Altura);			// a parte de baixo do form, abaixo do painel

	glClearColor(0.0f,0.01f,0.1f,1.0f);		// o mesmo fundo do Render
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// meia largura e meia altura da area, em unidades do desenho
	const double MeioL=Largura/(2.0*(double)Escala);
	const double MeioA=Altura/(2.0*(double)Escala);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(ChkPerspectiva->Checked){
		// distancia em que a perspectiva mostra a mesma escala da visao 2D
		const double Distancia=MeioA/std::tan(22.5*M_PI/180);
		glMatrixMode(GL_PROJECTION);
		gluPerspective(45,(double)Largura/Altura,Distancia/100,Distancia*100);
		glMatrixMode(GL_MODELVIEW);
		glTranslated(0,0,-Distancia);
	}else{
		glMatrixMode(GL_PROJECTION);
		glOrtho(-MeioL,MeioL,-MeioA,MeioA,-1e6,1e6);
		glMatrixMode(GL_MODELVIEW);
	}

	// matriz da visao (M = V*R) na ordem de colunas do OpenGL
	long double M[3][3];
	MatrizDaVisao(M);
	GLdouble G[16]={0};
	for(int l=0;l<3;l++)
		for(int c=0;c<3;c++)
			G[c*4+l]=(GLdouble)M[l][c];
	G[15]=1;

	glPushMatrix();
	glMultMatrixd(G);
	DesenhaEixos();
	DesenhaQuadrilateros();
	DesenhaLinhas();
	DesenhaElipses();
	DesenhaSelecionada();
	DesenhaVertices();
	DesenhaReferencia();
	DesenhaEncaixe();
	glPopMatrix();

	if(!ChkPerspectiva->Checked){
		DesenhaPrevia();
		DesenhaRetanguloSelecao();
	}

	SwapBuffers(hdc);
	wglMakeCurrent(DCAnterior,ContextoAnterior);
}

//---------------------------------------------------------------------------
// Eventos
//---------------------------------------------------------------------------

void __fastcall TDrawOpenGL::FormPaint(TObject *Sender)
{
	Desenha();
}

void __fastcall TDrawOpenGL::FormResize(TObject *Sender)
{
	Desenha();
}

void __fastcall TDrawOpenGL::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(!NaAreaGL(Y))
		return;

	ActiveControl=NULL;					// sai dos campos de texto: o Ctrl+Z volta a ser do desenho

	if(Button==mbRight){				// gira a visao arrastando
		Girando=true;
		UltimoX=X;
		UltimoY=Y;
		return;
	}
	if(Button!=mbLeft)
		return;

	if(ChkPerspectiva->Checked){
		Status("Na visao 3D o mouse so gira: desmarque 3D para desenhar");
		return;
	}

	// editando vertices o clique escolhe um vertice e comeca o arraste: nao desenha
	if(EditandoVertices()){
		VerticeFigura V;
		if(AchaVertice(X,Y,V,NULL,true)){
			Arrastando=true;
			VerticeAtivo=V;
			EstadoAntesArraste=CapturaEstado();
			ArrasteMoveu=false;
			PosicaoVertice(V,InicioArraste);
			long double s[3];
			ParaTela(InicioArraste,s);
			ProfArraste=s[2];				// o vertice anda no plano da tela
			MouseCapture=true;				// o MouseUp chega mesmo fora da janela
			SelecionaFigura(V.Tipo,V.Indice);
			CursorDoMouse(crSizeAll);
			Status(NomeVertice(V)+": arraste para mudar a posicao (Esc desfaz)");
		}else
			Status("Nenhum vertice aqui: clique num ponto de uma figura selecionada");
		Desenha();
		return;
	}

	// selecionando figuras o clique marca e o arraste abre um retangulo: nao desenha
	if(ChkSelecionar->Checked){
		SelecionandoRet=true;
		RetInicio=RetFim=TelaDoMouse(X,Y);
		MouseCapture=true;
		return;
	}

	Cliques.push_back(PontoComEncaixe(X,Y));

	const EstadoDesenho Antes=CapturaEstado();
	bool Gravou=false;
	switch(RGFigura->ItemIndex){
		case FiguraLinha:
			if(Cliques.size()==2){
				GravaLinha();
				Gravou=true;
			}
			break;
		case FiguraQuadrilatero:
			if(Cliques.size()==4){
				GravaQuadrilatero();
				Gravou=true;
			}
			break;
		case FiguraElipse:
			if(Cliques.size()==2){
				GravaElipse();
				Gravou=true;
			}
			break;
		default:
			Cliques.clear();
	}
	if(Gravou){
		EmpilhaDesfazer(Antes);
		Cliques.clear();
		AtualizaListaFiguras();			// a figura nova entra no CBFiguras
	}
	Desenha();
}

void __fastcall TDrawOpenGL::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	if(Girando){
		// horizontal gira em volta de Yo (Beta) e vertical em volta de Xo (Alpha)
		long double ABT[3];
		Desenho[0].GetAlpha_B_T(ABT);
		ABT[1]+=(X-UltimoX)*M_PI/360;
		ABT[0]+=(Y-UltimoY)*M_PI/360;
		Desenho[0].SetAlpha_B_T(ABT);
		UltimoX=X;
		UltimoY=Y;
		Desenha();
		return;
	}

	if(Arrastando){
		// o vertice vai para o mouse, com a profundidade de quando foi clicado
		// (ou para o vertice de outra figura em que encaixou)
		Cursor=PontoComEncaixe(X,Y,&VerticeAtivo);
		long double P[3];
		if(Cursor.TemMundo){
			P[0]=Cursor.Mundo[0];
			P[1]=Cursor.Mundo[1];
			P[2]=Cursor.Mundo[2];
		}else
			DaTela(Cursor.h,Cursor.v,ProfArraste,P);
		MoveVertice(VerticeAtivo,P);
		ArrasteMoveu=true;
		Status(NomeVertice(VerticeAtivo)+Format(":  Xo=%.1f  Yo=%.1f  Zo=%.1f",
			ARRAYOFCONST(((double)P[0],(double)P[1],(double)P[2]))));
		Desenha();
		return;
	}

	if(SelecionandoRet){
		RetFim=TelaDoMouse(X,Y);
		Desenha();
		return;
	}

	const bool TinhaCursor=TemCursor;
	TemCursor=NaAreaGL(Y);
	if(!TemCursor){
		TemVerticeSobMouse=false;
		if(TinhaCursor)
			Desenha();			// mouse no painel: apaga as linhas de referencia
		return;
	}

	// editando vertices: mao em cima de um vertice
	if(EditandoVertices()){
		TemVerticeSobMouse=AchaVertice(X,Y,VerticeSobMouse,NULL,true);
		CursorDoMouse(TemVerticeSobMouse? crHandPoint : crDefault);
	}

	Cursor=(EditandoVertices()||ChkSelecionar->Checked)? TelaDoMouse(X,Y) : PontoComEncaixe(X,Y);
	if(!ChkPerspectiva->Checked){
		long double P[3];
		TelaParaMundo(Cursor,P);
		Status(Format("Xo=%.1f  Yo=%.1f  Zo=%.1f",ARRAYOFCONST(((double)P[0],(double)P[1],(double)P[2]))));
		Desenha();				// as linhas de referencia seguem o mouse
	}
}

void __fastcall TDrawOpenGL::FormMouseLeave(TObject *Sender)
{
	if(TemCursor){
		TemCursor=false;
		Desenha();
	}
}

void __fastcall TDrawOpenGL::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Button==mbRight)
		Girando=false;

	if((Button==mbLeft)&&SelecionandoRet){
		SelecionandoRet=false;
		MouseCapture=false;
		RetFim=TelaDoMouse(X,Y);
		TerminaSelecao(X,Y);
		Desenha();
	}

	if((Button==mbLeft)&&Arrastando){
		Arrastando=false;
		MouseCapture=false;
		CursorDoMouse(crHandPoint);
		Cursor.TemMundo=false;
		if(ArrasteMoveu)
			EmpilhaDesfazer(EstadoAntesArraste);
		AtualizaListaFiguras();			// a lista e o painel de edicao com a posicao nova
		Status(NomeVertice(VerticeAtivo)+" movido");
		Desenha();
	}
}

void __fastcall TDrawOpenGL::FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{
	Escala*=(WheelDelta>0)? 1.1L : 1/1.1L;
	Escala=std::min(std::max(Escala,0.01L),100.0L);
	Handled=true;
	Desenha();
}

void __fastcall TDrawOpenGL::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	// Ctrl+Z e Ctrl+Y sao do desenho, menos enquanto se digita num campo de texto
	if(Shift.Contains(ssCtrl)&&((Key=='Z')||(Key=='Y'))&&(dynamic_cast<TCustomEdit*>(ActiveControl)==NULL)){
		if(Key=='Z')
			Desfazer();
		else
			Refazer();
		Key=0;
		return;
	}

	if(Key!=VK_ESCAPE)
		return;

	if(Arrastando){				// Esc no arraste: o vertice volta para onde estava
		MoveVertice(VerticeAtivo,InicioArraste);
		Arrastando=false;
		MouseCapture=false;
		CursorDoMouse(crDefault);
		AtualizaListaFiguras();
		Cursor.TemMundo=false;
		Status("Arraste desfeito");
		Desenha();
		return;
	}
	if(SelecionandoRet){		// Esc no retangulo: desiste dele
		SelecionandoRet=false;
		MouseCapture=false;
		Desenha();
		return;
	}
	if(ChkSelecionar->Checked&&(!Colecao.empty())){		// Esc selecionando: limpa a colecao
		BtnLimparSelecaoClick(this);
		return;
	}
	CancelaFigura();			// Esc desiste da figura em andamento
}

void __fastcall TDrawOpenGL::RGFiguraClick(TObject *Sender)
{
	static const wchar_t *Dica[]={
		L"Linha: clique no inicio e no fim",
		L"Quadrilatero: clique nos 4 cantos, em volta do contorno",
		L"Elipse: clique no centro e depois na borda"
	};
	if((RGFigura->ItemIndex>=0)&&(RGFigura->ItemIndex<3))
		Status(Dica[RGFigura->ItemIndex]);
	CancelaFigura();
}

void __fastcall TDrawOpenGL::RGVisaoClick(TObject *Sender)
{
	static const wchar_t *Nome[]={L"Zo",L"Yo",L"Xo"};
	if((RGVisao->ItemIndex>=0)&&(RGVisao->ItemIndex<3))
		LblProfundidade->Caption=UnicodeString(Nome[RGVisao->ItemIndex])+" fixo";
	ReiniciaVisao();
}

// clicar de novo na visao que ja esta marcada nao dispara o OnClick do RadioGroup:
// o MouseUp de cada botao garante que os eixos voltem ao inicio mesmo assim
void __fastcall TDrawOpenGL::RGVisaoBotaoMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(Button==mbLeft)
		ReiniciaVisao();
}

// a visao volta aos angulos do inicio do programa (Alpha, Beta e Teta zerados):
// os eixos e o desenho ficam retos de novo, desfazendo o giro do botao direito
void TDrawOpenGL::ReiniciaVisao(){
	long double ABT[3]={0,0,0};
	Desenho[0].SetAlpha_B_T(ABT);
	CancelaFigura();				// limpa a figura em andamento e redesenha
}

void __fastcall TDrawOpenGL::BtnGiraXoClick(TObject *Sender)
{
	Gira(0,PassoGiro);
}

void __fastcall TDrawOpenGL::BtnGiraYoClick(TObject *Sender)
{
	Gira(1,PassoGiro);
}

void __fastcall TDrawOpenGL::BtnGiraZoClick(TObject *Sender)
{
	Gira(2,PassoGiro);
}

void __fastcall TDrawOpenGL::ChkPerspectivaClick(TObject *Sender)
{
	CancelaFigura();
}

void __fastcall TDrawOpenGL::BtnLimparClick(TObject *Sender)
{
	NovoDesenho(new Objetos[1]());
	Status("Desenho limpo");
}

void __fastcall TDrawOpenGL::BtnSalvarDENClick(TObject *Sender)
{
	SaveDialog1->Filter="Desenho EixoXY (*.DEN)|*.DEN";
	SaveDialog1->DefaultExt="DEN";
	if(!SaveDialog1->Execute())
		return;

	AnsiString Nome=SaveDialog1->FileName;
	UnicodeString Resposta;
	Name_Arquivos::Arquivo Arq;
	Arq.Salvar_Arquivo(Nome.c_str(),Desenho,&Resposta);
	Status(Resposta);
}

void __fastcall TDrawOpenGL::BtnAbrirGLBClick(TObject *Sender)
{
	OpenDialog1->Filter="glTF Binario (*.glb)|*.glb";
	OpenDialog1->DefaultExt="glb";
	if(!OpenDialog1->Execute())
		return;

	UnicodeString Resposta;
	Name_Arquivos::ExportGLB Importador;
	Objetos *Novo=Importador.Importar(OpenDialog1->FileName,&Resposta);
	if(Novo!=NULL)
		NovoDesenho(Novo);
	Status(Resposta);
}

void __fastcall TDrawOpenGL::BtnExportarGLBClick(TObject *Sender)
{
	SaveDialog1->Filter="glTF Binario (*.glb)|*.glb";
	SaveDialog1->DefaultExt="glb";
	if(!SaveDialog1->Execute())
		return;

	UnicodeString Resposta;
	Name_Arquivos::ExportGLB Exportador;
	Exportador.Exportar(SaveDialog1->FileName,Desenho,&Resposta);
	Status(Resposta);
}

void __fastcall TDrawOpenGL::BtnAbrirDENClick(TObject *Sender)
{
	OpenDialog1->Filter="Desenho EixoXY (*.DEN)|*.DEN";
	OpenDialog1->DefaultExt="DEN";
	if(!OpenDialog1->Execute())
		return;

	// o Abrir_Arquivo so le o desenho: nao mexe nos buffers do EixoXY
	AnsiString Nome=OpenDialog1->FileName;
	UnicodeString Resposta;
	Name_Arquivos::Arquivo Arq;
	Objetos *Novo=Arq.Abrir_Arquivo(Nome.c_str(),&Resposta);
	if(Novo!=NULL)
		NovoDesenho(Novo);
	Status(Resposta);
}

void __fastcall TDrawOpenGL::BtnDupTetaClick(TObject *Sender)
{
	Duplica(GiroNoPlanoDaTela);
}

void __fastcall TDrawOpenGL::BtnDupAlphaClick(TObject *Sender)
{
	Duplica(GiroNoEixoHorizontal);
}

// escolher uma figura na lista: ela fica destacada no desenho
void __fastcall TDrawOpenGL::CBFigurasChange(TObject *Sender)
{
	ItemFigura Item;
	if(FiguraSelecionada(Item)){
		Status(CBFiguras->Items->Strings[CBFiguras->ItemIndex]);
		if(ChkSelecionar->Checked)		// selecionando: a lista tambem marca e desmarca
			AlternaNaColecao(Item);
	}
	CarregaEdicao();
	Desenha();
}

void __fastcall TDrawOpenGL::ChkSelecionarClick(TObject *Sender)
{
	if(ChkSelecionar->Checked&&ChkEditarVertices->Checked)
		ChkEditarVertices->Checked=false;		// o mouse faz uma coisa de cada vez
	SelecionandoRet=false;
	CursorDoMouse(crDefault);
	CancelaFigura();
	if(!ChkSelecionar->Checked)
		Status("Desenho: o mouse volta a desenhar figuras");
	else if(ChkPerspectiva->Checked)
		Status("Desmarque Visualizar 3D para selecionar figuras");
	else
		Status("Clique nas figuras ou arraste um retangulo em volta delas");
}

void __fastcall TDrawOpenGL::BtnLimparSelecaoClick(TObject *Sender)
{
	Colecao.clear();
	AtualizaColecao();
	Status("Selecao limpa");
	Desenha();
}

void __fastcall TDrawOpenGL::ChkEditarVerticesClick(TObject *Sender)
{
	if(ChkEditarVertices->Checked&&ChkSelecionar->Checked)
		ChkSelecionar->Checked=false;			// o mouse faz uma coisa de cada vez
	Arrastando=false;
	TemVerticeSobMouse=false;
	CursorDoMouse(crDefault);
	if(!ChkEditarVertices->Checked)
		Status("Desenho: o mouse volta a desenhar figuras");
	else if(ChkPerspectiva->Checked)
		Status("Desmarque Visualizar 3D para editar os vertices");
	else
		Status("Editar vertices: aparecem so os das figuras selecionadas. Arraste um ponto para mover.");
	CancelaFigura();
}

void __fastcall TDrawOpenGL::BtnAplicarClick(TObject *Sender)
{
	const EstadoDesenho Antes=CapturaEstado();
	if(!AplicaEdicao())
		return;
	EmpilhaDesfazer(Antes);
	AtualizaListaFiguras();			// o texto da lista e a edicao com os valores novos
	Desenha();
}

void __fastcall TDrawOpenGL::BtnDesfazerClick(TObject *Sender)
{
	Desfazer();
}

void __fastcall TDrawOpenGL::BtnRefazerClick(TObject *Sender)
{
	Refazer();
}

void __fastcall TDrawOpenGL::BtnExtrudarClick(TObject *Sender)
{
	Extruda();
}

void __fastcall TDrawOpenGL::BtnRecarregarClick(TObject *Sender)
{
	CarregaEdicao();				// desiste do que foi digitado
	Status("Valores da figura recarregados");
}
//---------------------------------------------------------------------------
