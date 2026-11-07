//---------------------------------------------------------------------------
// Geometria.cpp - contas do desenho que nao dependem de nenhum form (veja Geometria.h)
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Geometria.h"
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// EixoXo: converte o ponto (xo,yo) do plano XoY para coordenadas polares.
//   *MVetorXY = distancia ate a origem: sqrt(xo*xo + yo*yo)
//   *Alphat   = angulo a partir do eixo Xo, de 0 a 2*PI no sentido anti-horario
//               (como no codigo original, sobre o eixo -Yo devolve -PI/2)
// zo nao entra na conta; o parametro fica para nao mudar as chamadas.
// Devolve false quando o ponto esta na origem (Alpha e Vetor ficam 0).
bool EixoXo(long double xo,long double yo,long double zo,long double *Alphat,long double *MVetorXY){

	if(xo==0 && yo==0){			// origem: nao tem angulo
		*Alphat=0;
		*MVetorXY=0;
		return false;
	}

	const long double Vetor=Sqrt(xo*xo+yo*yo);
	*MVetorXY=Vetor;

	// sobre os eixos
	if(xo==0){
		*Alphat=(yo>0)? M_PI/2 : -M_PI/2;
		return true;
	}
	if(yo==0){
		*Alphat=(xo>0)? 0 : M_PI;
		return true;
	}

	// fora dos eixos: Base e o angulo agudo com o eixo Xo (0 a PI/2),
	// que depois e ajustado conforme o quadrante
	const long double Base=acos(fabsl(xo)/Vetor);

	if(xo>0 && yo>0)		*Alphat=Base;				// 1o quadrante
	else if(xo<0 && yo>0)	*Alphat=M_PI-Base;			// 2o quadrante
	else if(xo<0 && yo<0)	*Alphat=M_PI+Base;			// 3o quadrante
	else					*Alphat=2*M_PI-Base;		// 4o quadrante

	return true;
}


// Ponto da elipse de semi-eixos A e B no angulo polar Teta (medido no centro):
//     r(Teta) = A*B / sqrt( (B*cos(Teta))^2 + (A*sin(Teta))^2 )
//     u = r*cos(Teta)   v = r*sin(Teta)
// Cos_t e Sin_t sao o cosseno e o seno do mesmo ponto escrito como (A*cos t, B*sin t);
// com eles a inclinacao AZ*cos(t)+BZ*sin(t) continua no mesmo plano de antes.
void PontoElipsePolar(long double A,long double B,long double Teta,
					  long double *u,long double *v,long double *Cos_t,long double *Sin_t){

	const long double Cos_Teta=Cos(Teta);
	const long double Sin_Teta=Sin(Teta);
	const long double Raiz=Sqrt((B*Cos_Teta)*(B*Cos_Teta)+(A*Sin_Teta)*(A*Sin_Teta));

	if(Raiz==0){				// A e B zero: a elipse e so o centro
		*u=*v=*Cos_t=*Sin_t=0;
		return;
	}

	const long double r=A*B/Raiz;

	*u=r*Cos_Teta;
	*v=r*Sin_Teta;
	*Cos_t=B*Cos_Teta/Raiz;
	*Sin_t=A*Sin_Teta/Raiz;
}

// Ponto i (0 a 50) da elipse j no espaco 3D, antes da rotacao da vista; o ponto
// 50 fecha no 0. Usado pelo Draw_Elipse, pela visao 3D do Render (Form1) e pelo
// DrawOpenGL. O ponto e montado no plano do TipoElipse e depois recebe o giro
// proprio da elipse (GiroDaElipse).
void PontoElipse3D(Name_Arquivos::Elipse *Elipse_Atr,int j,int i,long double xyz[3]){

long double u,v,Cos_t,Sin_t;
long double p[3];
long double M[3][3];

	PontoElipsePolar(Elipse_Atr->GetA_Atr(j),Elipse_Atr->GetB_Atr(j),i*((2*M_PI)/50),&u,&v,&Cos_t,&Sin_t);

	// deslocamento para fora do plano da elipse (elipse inclinada)
	const long double Inclinacao=Elipse_Atr->GetAZo_Atr(j)*Cos_t+Elipse_Atr->GetBZo_Atr(j)*Sin_t;

	switch(Elipse_Atr->GetTipoElipse_Atr(j)){

		case 0:			// plano XY
			p[0]=u+Elipse_Atr->GetCX(j);
			p[1]=v+Elipse_Atr->GetCY(j);
			p[2]=Inclinacao+Elipse_Atr->GetCZ(j);
		break;

		case 1:			// plano XZ
			p[0]=u+Elipse_Atr->GetCX(j);
			p[2]=v+Elipse_Atr->GetCZ(j);
			p[1]=Inclinacao+Elipse_Atr->GetCY(j);
		break;

		case 2:			// plano YZ
			p[1]=u+Elipse_Atr->GetCY(j);
			p[2]=v+Elipse_Atr->GetCZ(j);
			p[0]=Inclinacao+Elipse_Atr->GetCX(j);
		break;

		default:
			p[0]=p[1]=p[2]=0;
		break;
	}

	GiroDaElipse(Elipse_Atr->GetAnguloX(j),Elipse_Atr->GetAnguloY(j),Elipse_Atr->GetAnguloZ(j),M);
	for(int l=0;l<3;l++)
		xyz[l]=M[l][0]*p[0]+M[l][1]*p[1]+M[l][2]*p[2];
}

// M = Rz(AX)*Ry(AZ)*Rx(AY) (veja Geometria.h)
void GiroDaElipse(long double AX,long double AY,long double AZ,long double M[3][3]){

	const long double ct=Cos(AX),st=Sin(AX),cb=Cos(AZ),sb=Sin(AZ),ca=Cos(AY),sa=Sin(AY);

	M[0][0]=ct*cb;	M[0][1]=ct*sb*sa-st*ca;	M[0][2]=ct*sb*ca+st*sa;
	M[1][0]=st*cb;	M[1][1]=st*sb*sa+ct*ca;	M[1][2]=st*sb*ca-ct*sa;
	M[2][0]=-sb;	M[2][1]=cb*sa;			M[2][2]=cb*ca;
}

void AngulosDoGiro(const long double M[3][3],long double *AX,long double *AY,long double *AZ){

	long double s=-M[2][0];				// sin(AZ)
	if(s>1) s=1;
	if(s<-1) s=-1;
	*AZ=asinl(s);

	if(fabsl(s)<1-1e-12L){
		*AY=atan2l(M[2][1],M[2][2]);
		*AX=atan2l(M[1][0],M[0][0]);
	}else{
		// AZ = +-90 graus: AnguloY e AnguloX giram no mesmo eixo, fica tudo no AnguloX
		*AY=0;
		*AX=atan2l(-M[0][1],M[1][1]);
	}
}

//---------------------------------------------------------------------------
// Transformacao "na tela" (veja Geometria.h)
//---------------------------------------------------------------------------

VisaoTela MontaVisaoTela(long double a,long double b,long double t,TipoVisao Visao){

	const long double ca=Cos(a),sa=Sin(a),cb=Cos(b),sb=Sin(b),ct=Cos(t),st=Sin(t);
	VisaoTela T;

	T.R[0][0]=cb*ct;			T.R[0][1]=-cb*st;			T.R[0][2]=sb;
	T.R[1][0]=sa*sb*ct+ca*st;	T.R[1][1]=-sa*sb*st+ca*ct;	T.R[1][2]=-sa*cb;
	T.R[2][0]=-ca*sb*ct+sa*st;	T.R[2][1]=ca*sb*st+sa*ct;	T.R[2][2]=ca*cb;

	switch(Visao){
		case VisaoXoY:	T.H=0; T.V=1; break;
		case VisaoXoZ:	T.H=0; T.V=2; break;
		default:		T.H=1; T.V=2; break;		// YoZ
	}
	T.N=3-T.H-T.V;
	return T;
}

void GiraNaTela(const VisaoTela &T,TipoGiro Tipo,long double Giro,long double p[3]){

	// os dois eixos de tela que giram juntos
	const int E1=(Tipo==GiroNoPlanoDaTela)? T.H : T.V;
	const int E2=(Tipo==GiroNoPlanoDaTela)? T.V : T.N;

	long double v[3];
	for(int l=0;l<3;l++)
		v[l]=T.R[l][0]*p[0]+T.R[l][1]*p[1]+T.R[l][2]*p[2];		// mundo -> tela

	const long double c=Cos(Giro),s=Sin(Giro);
	const long double v1=v[E1],v2=v[E2];
	v[E1]=v1*c-v2*s;
	v[E2]=v1*s+v2*c;

	for(int k=0;k<3;k++)
		p[k]=T.R[0][k]*v[0]+T.R[1][k]*v[1]+T.R[2][k]*v[2];		// tela -> mundo (R transposta)
}

void TelaParaMundo(const VisaoTela &T,long double p[3]){
	const long double v[3]={p[0],p[1],p[2]};
	for(int k=0;k<3;k++)
		p[k]=T.R[0][k]*v[0]+T.R[1][k]*v[1]+T.R[2][k]*v[2];
}

void PolarParaMundo(const VisaoTela &T,long double &Alpha,long double &Vetor,long double &Zo,long double &Xo,long double &Yo){
	long double p[3]={Cos(Alpha)*Vetor,Sin(Alpha)*Vetor,Zo};
	TelaParaMundo(T,p);
	Xo=p[0];
	Yo=p[1];
	Zo=p[2];
	EixoXo(Xo,Yo,0,&Alpha,&Vetor);
}

PontoPolar Polar(long double Alpha,long double Vetor,long double Zo){
	return PontoPolar{Alpha,Vetor,Zo,Cos(Alpha)*Vetor,Sin(Alpha)*Vetor};
}

PontoPolar GiraPolarNaTela(const VisaoTela &T,TipoGiro Tipo,long double Giro,const PontoPolar &P){

	long double p[3]={P.Xo,P.Yo,P.Zo};
	GiraNaTela(T,Tipo,Giro,p);

	PontoPolar Girado;
	Girado.Xo=p[0];
	Girado.Yo=p[1];
	Girado.Zo=p[2];
	EixoXo(Girado.Xo,Girado.Yo,0,&Girado.Alpha,&Girado.Vetor);
	return Girado;
}

std::array<PontoPolar,2> LerPontas(Name_Arquivos::Line *Lines,int i){
	return {{
		Polar(Lines->GetMAlpha_Atr(i),Lines->GetMVetor_Atr(i),Lines->GetMZo_Atr(i)),
		Polar(Lines->GetLAlpha_Atr(i),Lines->GetLVetor_Atr(i),Lines->GetLZo_Atr(i))
	}};
}

void GravarPontas(Name_Arquivos::Line *Lines,int Index,const std::array<PontoPolar,2> &P){
	const PontoPolar &M=P[0],&L=P[1];

	Lines->SetMAlpha_Atr(M.Alpha,Index);
	Lines->SetMVetor_Atr(M.Vetor,Index);
	Lines->SetLAlpha_Atr(L.Alpha,Index);
	Lines->SetLVetor_Atr(L.Vetor,Index);

	Lines->SetMXo_Atr(M.Xo,Index);
	Lines->SetMYo_Atr(M.Yo,Index);
	Lines->SetMZo_Atr(M.Zo,Index);
	Lines->SetLXo_Atr(L.Xo,Index);
	Lines->SetLYo_Atr(L.Yo,Index);
	Lines->SetLZo_Atr(L.Zo,Index);
}

std::array<PontoPolar,4> LerCantos(const TQuadrilatero *Q,int i){
	return {{
		Polar(Q->GetMAlpha(i), Q->GetMVetor(i), Q->GetMZo(i)),
		Polar(Q->GetLAlpha(i), Q->GetLVetor(i), Q->GetLZo(i)),
		Polar(Q->GetMAlpha2(i),Q->GetMVetor2(i),Q->GetMZo2(i)),
		Polar(Q->GetLAlpha2(i),Q->GetLVetor2(i),Q->GetLZo2(i))
	}};
}

void GravarCantos(TQuadrilatero *Q,int Index,const std::array<PontoPolar,4> &C){
	const PontoPolar &M=C[0],&L=C[1],&M2=C[2],&L2=C[3];

	Q->SetMAlpha(M.Alpha,Index);
	Q->SetMVetor(M.Vetor,Index);
	Q->SetLAlpha(L.Alpha,Index);
	Q->SetLVetor(L.Vetor,Index);
	Q->SetMAlpha2(M2.Alpha,Index);
	Q->SetMVetor2(M2.Vetor,Index);
	Q->SetLAlpha2(L2.Alpha,Index);
	Q->SetLVetor2(L2.Vetor,Index);

	Q->SetMXo(M.Xo,Index);
	Q->SetMYo(M.Yo,Index);
	Q->SetMZo(M.Zo,Index);
	Q->SetLXo(L.Xo,Index);
	Q->SetLYo(L.Yo,Index);
	Q->SetLZo(L.Zo,Index);

	Q->SetMXo2(M2.Xo,Index);
	Q->SetMYo2(M2.Yo,Index);
	Q->SetMZo2(M2.Zo,Index);
	Q->SetLXo2(L2.Xo,Index);
	Q->SetLYo2(L2.Yo,Index);
	Q->SetLZo2(L2.Zo,Index);
}

//---------------------------------------------------------------------------
// Duplicador
//---------------------------------------------------------------------------

// O Desfazer apagou figuras: tira da lista os originais que nao existem mais.
static void EsqueceApagadas(std::vector<int> &Originais,int &Vistos,int Total){
	while(!Originais.empty() && Originais.back()>=Total)
		Originais.pop_back();
	if(Vistos>Total)
		Vistos=Total;
}

// Atualiza a lista de originais de um tipo de figura antes de duplicar.
//   Comeco: todas as Total figuras sao originais.
//   Depois: entram as que apareceram desde a ultima duplicacao (desenhadas pelo
//   usuario); as copias ja estavam contadas em Vistos e ficam de fora.
// Devolve a posicao em Originais onde comecam as figuras que entraram agora.
static size_t AtualizaOriginais(std::vector<int> &Originais,int &Vistos,int Total,bool Comeco){
	if(Comeco){
		Originais.clear();
		Vistos=0;
	}
	EsqueceApagadas(Originais,Vistos,Total);
	const size_t Novas=Originais.size();
	for(int i=Vistos;i<Total;i++)
		Originais.push_back(i);
	return Novas;
}

// Plano da figura i; sem plano guardado, o plano da visao atual (ABT e Visao)
static PlanoDoDesenho PlanoDe(const std::vector<PlanoDoDesenho> &Planos,int i,const long double *ABT,TipoVisao Visao){
	if(i>=0 && i<(int)Planos.size() && Planos[i].Guardado)
		return Planos[i];
	PlanoDoDesenho P;
	P.Guardado=false;
	P.ABT[0]=ABT[0];
	P.ABT[1]=ABT[1];
	P.ABT[2]=ABT[2];
	P.Visao=Visao;
	return P;
}

static void GuardaPlano(std::vector<PlanoDoDesenho> &Planos,int i,const PlanoDoDesenho &P){
	if(i<0)
		return;
	if(i>=(int)Planos.size())
		Planos.resize(i+1,PlanoDoDesenho());		// indices novos comecam sem plano guardado
	Planos[i]=P;
}

// mais uma copia da figura i: devolve quantas vezes ela ja foi duplicada contando esta
static int ProximaVez(std::vector<VezesDuplicada> &Vezes,int i,TipoGiro Tipo){
	if(i>=(int)Vezes.size())
		Vezes.resize(i+1,VezesDuplicada());
	return (Tipo==GiroNoPlanoDaTela)? ++Vezes[i].Teta : ++Vezes[i].Alpha;
}

// as figuras que acabaram de entrar como originais (Originais[Novas..]) comecam do zero
static void ComecaContagem(std::vector<VezesDuplicada> &Vezes,const std::vector<int> &Originais,size_t Novas){
	for(size_t k=Novas;k<Originais.size();k++){
		const int i=Originais[k];
		if(i>=(int)Vezes.size())
			Vezes.resize(i+1,VezesDuplicada());
		Vezes[i]=VezesDuplicada();
	}
}

Duplicador::Duplicador(){
	Linhas.Vistos=0;
	Quads.Vistos=0;
	Circulos.Vistos=0;
}

void Duplicador::GuardaPlanoLine(int i,const PlanoDoDesenho &P){
	GuardaPlano(Linhas.Planos,i,P);
}

void Duplicador::GuardaPlanoQuad(int i,const PlanoDoDesenho &P){
	GuardaPlano(Quads.Planos,i,P);
}

void Duplicador::GuardaPlanoCircle(int i,const PlanoDoDesenho &P){
	GuardaPlano(Circulos.Planos,i,P);
}

void Duplicador::EsquecePlanos(){
	Linhas.Planos.clear();
	Quads.Planos.clear();
	Circulos.Planos.clear();
}

void Duplicador::EsqueceLinhasApagadas(int Total){
	EsqueceApagadas(Linhas.Originais,Linhas.Vistos,Total);
}

void Duplicador::Duplica(Name_Arquivos::Objetos *Desenho,const long double *ABT,TipoVisao Visao,
						 bool Comeco,TipoGiro Tipo,long double Passo){

Name_Arquivos::Line *Lines=Desenho[0].Lines_Atr;
TQuadrilatero *Quad=Desenho[0].Quadrilatero;
Name_Arquivos::Elipse *Elipses=Desenho[0].Elipse_Atr;

// originais: no comeco todas as figuras; depois entram tambem as desenhadas
// desde a ultima duplicacao, para elas continuarem girando com as outras
const size_t NovasLine=AtualizaOriginais(Linhas.Originais,Linhas.Vistos,Lines->GetContador_Line(),Comeco);
const size_t NovasQuad=AtualizaOriginais(Quads.Originais,Quads.Vistos,Quad->GetContadorQuadrilatero(),Comeco);
const size_t NovasCircle=AtualizaOriginais(Circulos.Originais,Circulos.Vistos,Elipses->GetContadorCircle_Atr(),Comeco);

// as figuras que entraram agora comecam a contar do zero: a primeira copia delas
// sai Passo a partir de onde foram desenhadas
if(Comeco){
	Linhas.Vezes.clear();
	Quads.Vezes.clear();
	Circulos.Vezes.clear();
}
ComecaContagem(Linhas.Vezes,Linhas.Originais,NovasLine);
ComecaContagem(Quads.Vezes,Quads.Originais,NovasQuad);
ComecaContagem(Circulos.Vezes,Circulos.Originais,NovasCircle);

	// linhas: cada linha aparece na tela com os angulos da visao mais os dela
	// (na visao XoY o AnguloAlpha soma no Alpha, como no Draw_Lines)
	if(Lines->GetContador_Line()>0){
		for(size_t k=0;k<Linhas.Originais.size();k++){
			const int i=Linhas.Originais[k];
			const int Novo=Lines->GetContador_Line();
			Lines->Alocando_Line(Novo+10);

			const long double Teta=Lines->GetAnguloTeta(i),Alpha=Lines->GetAnguloAlpha(i);
			// a linha gira no plano em que foi desenhada (a copia herda esse plano)
			const PlanoDoDesenho P=PlanoDe(Linhas.Planos,i,ABT,Visao);
			const long double Giro=ProximaVez(Linhas.Vezes,i,Tipo)*Passo;	// 1a copia: Passo, 2a: 2*Passo...
			const VisaoTela T=MontaVisaoTela(P.ABT[0]+(P.Visao==VisaoXoY? Alpha : 0),P.ABT[1],P.ABT[2]+Teta,P.Visao);

			std::array<PontoPolar,2> Pontas=LerPontas(Lines,i);
			for(PontoPolar &Ponta : Pontas)
				Ponta=GiraPolarNaTela(T,Tipo,Giro,Ponta);
			GravarPontas(Lines,Novo,Pontas);

			Lines->SetCor_Line(Lines->GetCor_Line(i),Novo);
			// a copia usa os mesmos angulos de desenho do original: o giro ja esta nos pontos
			Lines->Set_AngluloTeta(Teta,Novo);
			Lines->Set_AngluloAlpha(Alpha,Novo);
			GuardaPlano(Linhas.Planos,Novo,P);

			Lines->SetContador_Line(Novo+1);
		}
	}

	// quadrilateros: aparecem na tela so com os angulos da visao (Draw_Quadrilatero)
	if(Quad->GetContadorQuadrilatero()>0){
		for(size_t k=0;k<Quads.Originais.size();k++){
			const int i=Quads.Originais[k];
			// o quadrilatero gira no plano em que foi desenhado (a copia herda esse plano)
			const PlanoDoDesenho P=PlanoDe(Quads.Planos,i,ABT,Visao);
			const long double Giro=ProximaVez(Quads.Vezes,i,Tipo)*Passo;	// 1a copia: Passo, 2a: 2*Passo...
			const VisaoTela T=MontaVisaoTela(P.ABT[0],P.ABT[1],P.ABT[2],P.Visao);
			const int Novo=Quad->GetContadorQuadrilatero();
			Quad->AlocarQuadrilatero(Novo+10);

			std::array<PontoPolar,4> Cantos=LerCantos(Quad,i);
			for(PontoPolar &Canto : Cantos)
				Canto=GiraPolarNaTela(T,Tipo,Giro,Canto);
			GravarCantos(Quad,Novo,Cantos);

			Quad->SetCor(Quad->GetCor(i),Novo);
			GuardaPlano(Quads.Planos,Novo,P);

			Quad->SetContadorQuadrilatero(Novo+1);
		}
	}

	// elipses: o giro na tela (G, em volta da origem) entra no giro proprio da elipse,
	// M' = G*M. Como o giro proprio tambem e em volta da origem, o centro e o formato
	// guardados ficam os do original e a copia inteira (centro e plano) sai girada.
	if(Elipses->GetContadorCircle_Atr()>0){
		for(size_t k=0;k<Circulos.Originais.size();k++){
			const int i=Circulos.Originais[k];
			const int Novo=Elipses->GetContadorCircle_Atr();
			Elipses->AlocaElipse_Atr(Novo+5);

			// a elipse gira no plano em que foi desenhada (a copia herda esse plano)
			const PlanoDoDesenho P=PlanoDe(Circulos.Planos,i,ABT,Visao);
			const long double Giro=ProximaVez(Circulos.Vezes,i,Tipo)*Passo;	// 1a copia: Passo, 2a: 2*Passo...
			const VisaoTela T=MontaVisaoTela(P.ABT[0],P.ABT[1],P.ABT[2],P.Visao);

			// cada coluna do giro proprio da original gira na tela
			long double M[3][3];
			GiroDaElipse(Elipses->GetAnguloX(i),Elipses->GetAnguloY(i),Elipses->GetAnguloZ(i),M);
			for(int c=0;c<3;c++){
				long double Coluna[3]={M[0][c],M[1][c],M[2][c]};
				GiraNaTela(T,Tipo,Giro,Coluna);
				for(int l=0;l<3;l++)
					M[l][c]=Coluna[l];
			}
			long double AX,AY,AZ;
			AngulosDoGiro(M,&AX,&AY,&AZ);

			const long double CX=Elipses->GetCX(i),CY=Elipses->GetCY(i),CZ=Elipses->GetCZ(i);
			Elipses->SetCX(CX,Novo);
			Elipses->SetCY(CY,Novo);
			Elipses->SetCZ(CZ,Novo);
			Elipses->SetCenter_Atr(CX,CY,CZ,Novo);

			Elipses->SetA_Atr(Elipses->GetA_Atr(i),Novo);
			Elipses->SetB_Atr(Elipses->GetB_Atr(i),Novo);
			Elipses->SetAZo_Atr(Elipses->GetAZo_Atr(i),Novo);
			Elipses->SetBZo_Atr(Elipses->GetBZo_Atr(i),Novo);
			Elipses->SetTipoElipse_Atr(Elipses->GetTipoElipse_Atr(i),Novo);
			Elipses->SetColor_Circle_Atr(Elipses->GetColor_Circle_Atr(i),Novo);
			Elipses->Set_IndexX(Elipses->GetIndexX(i),Novo);

			Elipses->Set_AnguloX(AX,Novo);
			Elipses->Set_AnguloY(AY,Novo);
			Elipses->Set_AnguloZ(AZ,Novo);
			GuardaPlano(Circulos.Planos,Novo,P);

			Elipses->SetContadorCircle_Atr(Novo+1);
		}
	}

// tudo o que existe agora (originais e copias) ja foi visto: so o que for
// desenhado daqui em diante entra como original na proxima duplicacao
Linhas.Vistos=Lines->GetContador_Line();
Quads.Vistos=Quad->GetContadorQuadrilatero();
Circulos.Vistos=Elipses->GetContadorCircle_Atr();
}
