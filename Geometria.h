//---------------------------------------------------------------------------
// Geometria.h - contas do desenho que nao dependem de nenhum form
//
// Coordenadas polares (EixoXo), pontos das elipses, a transformacao "na tela"
// (visao girada por Alpha, Beta e Teta) e a duplicacao girando (Duplica Teta e
// Duplica Alpha). Usado pelo EixoXY, pelo Render e pelo DrawOpenGL.
// Aqui nao entra Canvas, buffer de tela nem componente da VCL: so as classes de
// desenho (Objetos, Line, Elipse e TQuadrilatero).
//---------------------------------------------------------------------------

#ifndef GeometriaH
#define GeometriaH

#include <vector>
#include <array>
#include "Arquivo.h"

//---------------------------------------------------------------------------
// Polar e elipses
//---------------------------------------------------------------------------

// (xo,yo) -> angulo polar (Alphat, de 0 a 2*PI) e modulo (MVetorXY); zo nao entra
bool EixoXo(long double xo,long double yo,long double zo,long double *Alphat,long double *MVetorXY);

// ponto da elipse de semi-eixos A e B no angulo polar Teta (medido no centro)
void PontoElipsePolar(long double A,long double B,long double Teta,
					  long double *u,long double *v,long double *Cos_t,long double *Sin_t);

// Ponto i (0 a 50) da elipse j no espaco 3D, ja com o giro proprio da elipse e antes
// da rotacao da vista
void PontoElipse3D(Name_Arquivos::Elipse *Elipse_Atr,int j,int i,long double xyz[3]);

// Giro proprio da elipse, em volta da origem e o mesmo em qualquer visao:
//     M = Rz(AnguloX) * Ry(AnguloZ) * Rx(AnguloY)
// Com um angulo so (FElipse) e o giro no plano da elipse de antes. Uma elipse
// desenhada com a visao girada guarda AnguloY=-Alpha, AnguloZ=-Beta e AnguloX=-Teta:
// assim M = Rt (R da visao) e ela fica no plano da tela em que foi desenhada.
void GiroDaElipse(long double AX,long double AY,long double AZ,long double M[3][3]);

// o contrario: os angulos de um giro M (para guardar o giro de uma copia duplicada)
void AngulosDoGiro(const long double M[3][3],long double *AX,long double *AY,long double *AZ);

//---------------------------------------------------------------------------
// Transformacao "na tela"
//
// A vista 2D mostra cada ponto p multiplicado pela matriz R = Rx(a)*Ry(b)*Rz(t), as
// mesmas formulas do Draw_Lines: na visao XoY aparecem as linhas 0 e 1 de R, na XoZ
// as linhas 0 e 2 e na YoZ as linhas 1 e 2; a linha que sobra e a profundidade.
// Para girar o desenho como ele aparece na tela, o ponto vai para as coordenadas da
// tela (v = R*p), gira ali e volta para o mundo (p' = Rt*v, porque R e ortogonal).
//---------------------------------------------------------------------------

enum TipoVisao { VisaoXoY, VisaoXoZ, VisaoYoZ };

// Teta: gira no plano da tela (em volta do eixo que sai da tela).
// Alpha: gira em volta do eixo horizontal da tela.
enum TipoGiro { GiroNoPlanoDaTela, GiroNoEixoHorizontal };

struct VisaoTela{
	long double R[3][3];
	int H;			// linha de R que vira o eixo horizontal da tela
	int V;			// linha de R que vira o eixo vertical da tela
	int N;			// linha de R que sai da tela (profundidade)
};

// Monta R para os angulos a (Alpha), b (Beta) e t (Teta) e escolhe os eixos da visao
VisaoTela MontaVisaoTela(long double a,long double b,long double t,TipoVisao Visao);

// Gira o ponto p (Xo,Yo,Zo do mundo) de Giro radianos na tela e devolve o resultado em p
void GiraNaTela(const VisaoTela &T,TipoGiro Tipo,long double Giro,long double p[3]);

// p entra com as coordenadas da tela (v) e sai com as do mundo: p = Rt*v
void TelaParaMundo(const VisaoTela &T,long double p[3]);

// o mesmo para um ponto polar (Alpha,Vetor,Zo); tambem devolve Xo e Yo
void PolarParaMundo(const VisaoTela &T,long double &Alpha,long double &Vetor,long double &Zo,long double &Xo,long double &Yo);

// Um ponto das figuras: guardado em polar no plano XY (Alpha,Vetor) com a altura Zo.
// Xo e Yo sao o mesmo ponto em cartesiano (x=cos(Alpha)*Vetor, y=sin(Alpha)*Vetor).
struct PontoPolar{
	long double Alpha;
	long double Vetor;
	long double Zo;
	long double Xo;
	long double Yo;
};

PontoPolar Polar(long double Alpha,long double Vetor,long double Zo);

// O mesmo giro para um ponto polar: devolve o ponto girado (polar, Zo, Xo e Yo)
PontoPolar GiraPolarNaTela(const VisaoTela &T,TipoGiro Tipo,long double Giro,const PontoPolar &P);

// Linha: as duas pontas, M (MoveTo) e L (LineTo)
std::array<PontoPolar,2> LerPontas(Name_Arquivos::Line *Lines,int i);
void GravarPontas(Name_Arquivos::Line *Lines,int Index,const std::array<PontoPolar,2> &P);

// Quadrilatero: os quatro cantos na ordem do contorno, M, L, M2 e L2
std::array<PontoPolar,4> LerCantos(const TQuadrilatero *Q,int i);
void GravarCantos(TQuadrilatero *Q,int Index,const std::array<PontoPolar,4> &C);

//---------------------------------------------------------------------------
// Duplicacao girando na tela (Duplica Teta e Duplica Alpha)
//---------------------------------------------------------------------------

// Plano em que uma figura foi desenhada: as copias herdam o plano do original, e
// assim cada figura e duplicada no plano em que foi desenhada, mesmo que a visao
// seja girada depois. Figuras sem plano guardado (abertas de arquivo) usam o
// plano da visao atual.
struct PlanoDoDesenho{
	bool Guardado;			// false: figura sem plano guardado (usa a visao atual)
	long double ABT[3];		// angulos da visao quando a figura foi desenhada
	TipoVisao Visao;		// visao (XoY, XoZ ou YoZ) quando a figura foi desenhada
};

// Quantas vezes uma figura original ja foi duplicada: a primeira copia sai Passo
// a partir de onde ela foi desenhada, a segunda 2*Passo, e assim por diante.
struct VezesDuplicada{
	int Teta;		// copias feitas girando no plano da tela
	int Alpha;		// copias feitas girando no eixo horizontal
};

// Lembra, para um desenho (Objetos), quais figuras sao originais, o plano em que
// cada uma foi desenhada e quantas copias ja tem. Cada form que duplica tem o seu.
class Duplicador{

	public:

	Duplicador();

	// Acrescenta uma copia de cada figura original (linhas, quadrilateros e elipses)
	// girada na tela: cada figura gira Passo radianos a mais que a sua copia anterior,
	// contando a partir de onde ela foi desenhada. ABT e Visao sao a visao atual (o
	// desenho como esta na tela); ABT nao e alterado. Comeco: todas as figuras de
	// agora passam a ser as originais.
	// Os contadores das classes (GetContador_Line...) ja saem atualizados; quem chama
	// acerta os seus proprios contadores e buffers de tela.
	void Duplica(Name_Arquivos::Objetos *Desenho,const long double *ABT,TipoVisao Visao,
				 bool Comeco,TipoGiro Tipo,long double Passo);

	// figura i acabou de ser desenhada no plano P
	void GuardaPlanoLine(int i,const PlanoDoDesenho &P);
	void GuardaPlanoQuad(int i,const PlanoDoDesenho &P);
	void GuardaPlanoCircle(int i,const PlanoDoDesenho &P);

	// desenho limpo ou arquivo aberto: nenhuma figura tem plano guardado
	void EsquecePlanos();

	// o Desfazer deixou Total linhas: as apagadas nao sao mais copiadas
	void EsqueceLinhasApagadas(int Total);

	private:

	// o que e lembrado de um tipo de figura (os vetores andam junto com os indices)
	struct Figuras{
		std::vector<int> Originais;				// indices das figuras que sao copiadas
		int Vistos;								// quantas existiam no fim da ultima duplicacao
		std::vector<PlanoDoDesenho> Planos;
		std::vector<VezesDuplicada> Vezes;
	};

	Figuras Linhas;
	Figuras Quads;
	Figuras Circulos;
};

#endif
