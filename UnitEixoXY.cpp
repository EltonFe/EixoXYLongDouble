//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitEixoXY.h"
#include "Arquivo.h"
#include "Geometria.h"
#include <fstream>
#include <string.h>
#include "DrawCursor.h"
#include "Render.h"
#include "draw/DrawUnit.h"
#include <vector>
#include <array>
#include <stdio.h>
#include <conio.h>

#include <gl\gl.h>
#include <gl\glu.h>

using Name_Arquivos::Objetos;
using Name_Arquivos::Line;
using std::ifstream;
using std::ofstream;


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//Varia veis Globais --------------------------------------------------------

TEixoXY *EixoXY;
bool Lancar=false;
int Kx_Partida;				// posicao e angulos do desenho antes do lancamento
int Ky_Partida;
long double ABT_Partida[3];
float t;
int IndexX_Circle;
long double Modulo_Elipse;
long double Raio_Elipse;
int Cx;
int Cy;
int Cx2;
int Cy2;
bool Move1=false;
bool Move=false;
bool Primeiro=false;
int ContarElipse=0;
int Yes;
TPoint P2[2];
std::vector<TPoint> DrawP;
TPoint MP[2];
TPoint MPQ[4];
long double FullLKx[100];
long double FullLKy[100];
bool Libera_Draw=true;
int *DrawX=0;
int *DrawY=0;
int Contador;
int KOx=960;
int KOy=540;
int Kx=960;
int Ky=540;
int Eixo_Kx=Screen->DesktopWidth/2;
int Eixo_Ky=Screen->DesktopHeight/2;
int Contar=0;



//Prototipacao Funcoes Globais-----------------------------------------------

void Eixos(TCanvas *Plano,int Kx,int Ky);
void Limpa_Lines(TCanvas *Plano,int **FullMLK,int Countador_Line);
void Limpa_VoltaLinha(TCanvas *Plano,TPoint **P1);
long double OX(long double x);
long double OY(long double y);
long double X(long double x);
long double Y(long double y);
int Configura(long double Desk, int x);
void LimpaMemoria();
long double Converte_Angulo(long double Angulo);
DWORD WINAPI  PegaTeclas(LPVOID lpParameters);
void GiraZo();

//---------------------------------------------------------------------------
__fastcall TEixoXY::TEixoXY(TComponent* Owner)
	: TForm(Owner)
{
    Eixo_Lancar=true;
	Objetos_Atr=new Objetos[1]();
	ObjEixos=new Objetos();
	FullMLK=0;
	FullMLK2=0;
	FullMLK3=0;
	MP2=0;
    MP3=0;
	Objetos_Atr[0].Lines_Atr->GetAlpha_B_T(ABT);
	Contador_Line=0;
	Contador_Circle=0;
	P_Circle=0;
	Kx=Configura((int)Kx,1);
	Ky=Configura(Ky,-1);
	KOx=Configura(KOx,1);
	KOy=Configura(KOy,-1);
	MP1=0;




}
//---------------------------------------------------------------------------
__fastcall TEixoXY::~TEixoXY()
{
	Tempo_Draw->Enabled=false;

	delete[] Objetos_Atr;			// criado com new Objetos[1]()
	delete ObjEixos;				// criado com new Objetos()
	Objetos_Atr=NULL;
	ObjEixos=NULL;
}
DWORD WINAPI  PegaTeclas(LPVOID lpParameters){
char Teclas;
	for(;;){

		Teclas=getch();
		if((int)(Teclas)==0){
			Teclas=getch();
			if(0){
			if((int)(Teclas)==72)
				GiraZo();
			if((int)(Teclas)==80)
				GiraZo();
			if((int)(Teclas)==75)
				GiraZo();
			if((int)(Teclas)==77)
				GiraZo();
			}


		}



	}

 return 0;
}

void ObjetosEixo(){
long double x1,x2,y1,y2;
x1=-Screen->DesktopWidth/2;
y1=0;
x2=Screen->DesktopWidth/2;
y2=0;





}




void Elipse(long double Raio,int Kx,int Ky){
	TPoint P1[51];
	for(int i=0;i<51;i++){
		P1[i].x=Raio*Cos(i*((2*M_PI)/50))+Kx;
		P1[i].y=Raio*Sin(i*((2*M_PI)/50))+Ky;

	}

   EixoXY->Canvas->Polyline(P1,50);
}
void LimpaElipse(long double Raio,int Kx,int Ky){
TPoint P1[51];
	for(int i=0;i<51;i++){
		P1[i].x=Raio*Cos(i*((2*M_PI)/50))+Kx;
		P1[i].y=Raio*Sin(i*((2*M_PI)/50))+Ky;

	}
   EixoXY->Canvas->Pen->Color=clWhite;
   EixoXY->Canvas->Polyline(P1,50);

}
int Configura(long double Desk, int x){
float k;
float k2;

	if (x<1){
		k=1080;
		k2=Screen->DesktopHeight;
	}else {
 		 k=1920;
 		 k2=Screen->DesktopWidth;
 	 }

return (k2/k)*Desk;
}


// Desenha varias figuras (polilinhas) com PolyPolyline: uma unica chamada ao GDI
// para cada sequencia de figuras seguidas com a mesma cor. A ordem do desenho e
// mantida. Figuras[i] tem PontosPorFigura pontos; Cor(i) devolve a cor da figura i.
template<class FuncCor>
void PolyPolylinePorCor(TCanvas *Plano,TPoint **Figuras,int Quantidade,int PontosPorFigura,FuncCor Cor){

static std::vector<TPoint> Pontos;		// static: reaproveita a memoria entre os quadros
static std::vector<DWORD> Tamanhos;

	if((Figuras==NULL)||(Quantidade<=0))
		return;

	Pontos.clear();
	Tamanhos.clear();

	for(int i=0;i<Quantidade;i++){
		COLORREF Cor_i=Cor(i);

		Pontos.insert(Pontos.end(),Figuras[i],Figuras[i]+PontosPorFigura);
		Tamanhos.push_back(PontosPorFigura);

		// desenha o grupo quando a proxima figura muda de cor (ou acabou)
		if((i==Quantidade-1)||(Cor(i+1)!=Cor_i)){
			Plano->Pen->Color=(TColor)Cor_i;
			PolyPolyline(Plano->Handle,(POINT*)Pontos.data(),Tamanhos.data(),(DWORD)Tamanhos.size());
			Pontos.clear();
			Tamanhos.clear();
		}
	}

}

void TEixoXY::Draw_ElipseDirect(TCanvas *Plano,Objetos *Objetos_Atr,TPoint **P_Circle){

Name_Arquivos::Elipse *Elipse_Atr=Objetos_Atr[0].Elipse_Atr;

	// cada elipse tem 51 pontos (o ultimo fecha no primeiro)
	PolyPolylinePorCor(Plano,P_Circle,Elipse_Atr->GetContadorCircle_Atr(),51,
		[Elipse_Atr](int i){ return Elipse_Atr->GetColor_Circle_Atr(i); });

}
static TipoVisao VisaoAtual();

// Pontos de tela das elipses: o PontoElipse3D ja traz o giro proprio da elipse e a
// visao e a mesma das linhas (MontaVisaoTela), em qualquer visao
TPoint  **TEixoXY::Draw_Elipse(TCanvas *Plano,Objetos *Objetos_Atr){


TPoint **TmpP_Circle;
long double Ponto[3];
const int Quantidade=Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr();
const long double *ABT=EixoXY->Objetos_Atr[0].GetAlpha_B_T();
const VisaoTela T=MontaVisaoTela(ABT[0],ABT[1],ABT[2],VisaoAtual());

		TmpP_Circle=BufP_Circle.Redimensiona(std::max(Contador_Circle,Quantidade)+15,51);

	for(int j=0;j<Quantidade;j++){
		IndexX_Circle=j;

		for (int i=0;i<51;i++){

			PontoElipse3D(Objetos_Atr[0].Elipse_Atr,j,i,Ponto);

			const long double h=T.R[T.H][0]*Ponto[0]+T.R[T.H][1]*Ponto[1]+T.R[T.H][2]*Ponto[2];
			const long double v=T.R[T.V][0]*Ponto[0]+T.R[T.V][1]*Ponto[1]+T.R[T.V][2]*Ponto[2];

			TmpP_Circle[j][i].x=(int) X(RoundTo(h,0));
			TmpP_Circle[j][i].y= (int)Y(RoundTo(v,0));

		}

	}

	FElipse->BtnElipse->Enabled=true;

	return TmpP_Circle;
}



void TEixoXY::Grava_Arquivo(Objetos *Objetos_Atr,char *Nome_Arquivo){

int Contador_Line=Objetos_Atr[0].Lines_Atr->GetContador_Line();
int Contador_Circle=Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr();

	ShowMessage("Contador_Line: "+IntToStr(Contador_Line));

	if((Contador_Line>0)||(Contador_Circle>0)){

		ofstream Arquivo_WR(Nome_Arquivo,ios::binary);

		if(!Arquivo_WR){

			ShowMessage("Erro ao Tentar Gravar o Arquivo!!");

		}else{


				Arquivo_WR.write(reinterpret_cast<const char*>(&Contador_Line),sizeof(int));
				Arquivo_WR.write(reinterpret_cast<const char*>(&Contador_Circle),sizeof(int));
				Arquivo_WR.write(reinterpret_cast<const char*>(Objetos_Atr[0].GetAlpha_B_T()),3*sizeof(long double));


				if(Contador_Line>0){


					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Lines_Atr->GetMAlpha_Atr()),Contador_Line*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Lines_Atr->GetLAlpha_Atr()),Contador_Line*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Lines_Atr->GetMVetor_Atr()),Contador_Line*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Lines_Atr->GetLVetor_Atr()),Contador_Line*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Lines_Atr->GetMZo_Atr()),Contador_Line*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Lines_Atr->GetLZo_Atr()),Contador_Line*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Lines_Atr->GetCor_Line()),Contador_Line*sizeof(COLORREF));

				}



				if (Contador_Circle>0) {

					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetA_Atr()),Contador_Circle*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetB_Atr()),Contador_Circle*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetAZo_Atr()),Contador_Circle*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetBZo_Atr()),Contador_Circle*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetCX()),Contador_Circle*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetCY()),Contador_Circle*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetCZ()),Contador_Circle*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetTipoElipse_Atr()),Contador_Circle*sizeof(int));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetColor_Circle_Atr()),Contador_Circle*sizeof(COLORREF));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetAnguloX()),Contador_Circle*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetAnguloY()),Contador_Circle*sizeof(long double));
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetAnguloZ()),Contador_Circle*sizeof(long double));

				}



				ShowMessage("Gravacao Completa "+AnsiString(Nome_Arquivo));
				Arquivo_WR.close();
			}




	}else
		ShowMessage("Não Há Linhas ou Elipses");





}
void TEixoXY::Ler_Arquivo(Objetos *Objetos_Atr,char * Nome_Arquivo){


std::vector<long double> Tmp_MAlpha;
std::vector<long double> Tmp_LAlpha;
std::vector<long double> Tmp_MVetor;
std::vector<long double> Tmp_LVetor;
std::vector<long double> Tmp_MZo;
std::vector<long double> Tmp_LZo;
long double Tmp_ABT[3];
std::vector<COLORREF> TmpCor_Line;

std::vector<long double> Tmp_A_Atr;
std::vector<long double> Tmp_B_Atr;
std::vector<long double> Tmp_AZo_Atr;
std::vector<long double> Tmp_BZo_Atr;
std::vector<long double> Tmp_CX;
std::vector<long double> Tmp_CY;
std::vector<long double> Tmp_CZ;
std::vector<long double> Tmp_AnguloX;
std::vector<long double> Tmp_AnguloY;
std::vector<long double> Tmp_AnguloZ;

int Tmp_IndexX=0;
std::vector<COLORREF> Tmp_Color_Circle_Atr;
std::vector<int> Tmp_TipoElipse_Atr;
long double Tmp_Alpha_B_T[3];

		Limpa_Lines(EixoXY->Canvas,FullMLK,EixoXY->Contador_Line);
		Limpa_Elipses(EixoXY->Canvas,P_Circle,EixoXY->Contador_Circle);
        LimpaMemoria();

		



		ifstream Arquivo_RR(Nome_Arquivo,ios::binary|ios::in);

		if(Arquivo_RR){


					Arquivo_RR.read(reinterpret_cast<char*>(&Contador_Line),sizeof(int));

					Arquivo_RR.read(reinterpret_cast<char*>(&Contador_Circle),sizeof(int));

					Arquivo_RR.close();

				if((Contador_Line>0)||(Contador_Circle>0)){

					ifstream Arquivo_RR(Nome_Arquivo,ios::binary|ios::in);

					if(Arquivo_RR){


					Arquivo_RR.read(reinterpret_cast<char*>(&Contador_Line),sizeof(int));

					Arquivo_RR.read(reinterpret_cast<char*>(&Contador_Circle),sizeof(int));
					Arquivo_RR.read(reinterpret_cast<char*>(Tmp_Alpha_B_T),3*sizeof(long double));

					Objetos_Atr[0].Lines_Atr->Alocando_Line(Contador_Line+1);

					EixoXY->Objetos_Atr[0].SetAlpha_B_T(Tmp_Alpha_B_T);
                    for(int i=0;i<3;i++)
							EixoXY->ABT[i]=Tmp_Alpha_B_T[i];


						if(Contador_Line>0){

							Tmp_MAlpha.resize(Contador_Line);
							Tmp_LAlpha.resize(Contador_Line);
							Tmp_MVetor.resize(Contador_Line);
							Tmp_LVetor.resize(Contador_Line);
							Tmp_MZo.resize(Contador_Line);
							Tmp_LZo.resize(Contador_Line);

							TmpCor_Line.resize(Contador_Line);





							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_MAlpha.data()),Contador_Line*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_LAlpha.data()),Contador_Line*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_MVetor.data()),Contador_Line*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_LVetor.data()),Contador_Line*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_MZo.data()),Contador_Line*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_LZo.data()),Contador_Line*sizeof(long double));

							Arquivo_RR.read(reinterpret_cast<char *>(TmpCor_Line.data()),Contador_Line*sizeof(COLORREF));



							EixoXY->Objetos_Atr[0].Lines_Atr->Alocando_Line(Contador_Line);
							FullMLK=BufFullMLK.Redimensiona(Contador_Line,4);

							for(int i=0;i<Contador_Line;i++){

								EixoXY->Objetos_Atr[0].Lines_Atr->SetContador_Line(Contador_Line);
								EixoXY->Objetos_Atr[0].Lines_Atr->SetMAlpha_Atr(Tmp_MAlpha[i],i);
								EixoXY->Objetos_Atr[0].Lines_Atr->SetLAlpha_Atr(Tmp_LAlpha[i],i);
								EixoXY->Objetos_Atr[0].Lines_Atr->SetMVetor_Atr(Tmp_MVetor[i],i);
								EixoXY->Objetos_Atr[0].Lines_Atr->SetLVetor_Atr(Tmp_LVetor[i],i);
								EixoXY->Objetos_Atr[0].Lines_Atr->SetMZo_Atr(Tmp_MZo[i],i);
								EixoXY->Objetos_Atr[0].Lines_Atr->SetLZo_Atr(Tmp_LZo[i],i);
								EixoXY->Objetos_Atr[0].Lines_Atr->SetCor_Line(TmpCor_Line[i],i);

							}




							EixoXY->Contador_Line=Contador_Line;
							EixoXY->MP1=Draw_Lines(EixoXY->Canvas,EixoXY->Objetos_Atr,FullMLK);
							Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);

						}



						if(Contador_Circle>0){

							Tmp_A_Atr.resize(Contador_Circle);
							Tmp_B_Atr.resize(Contador_Circle);
							Tmp_AZo_Atr.resize(Contador_Circle);
							Tmp_BZo_Atr.resize(Contador_Circle);
							Tmp_CX.resize(Contador_Circle);
							Tmp_CY.resize(Contador_Circle);
							Tmp_CZ.resize(Contador_Circle);
							Tmp_Color_Circle_Atr.resize(Contador_Circle);
							Tmp_TipoElipse_Atr.resize(Contador_Circle);
							Tmp_AnguloX.resize(Contador_Circle);
							Tmp_AnguloY.resize(Contador_Circle);
							Tmp_AnguloZ.resize(Contador_Circle);



							EixoXY->P_Circle=EixoXY->BufP_Circle.Redimensiona(Contador_Circle,51);


							EixoXY->Objetos_Atr[0].Elipse_Atr->AlocaElipse_Atr(Contador_Circle);



							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_A_Atr.data()),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_B_Atr.data()),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_AZo_Atr.data()),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_BZo_Atr.data()),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_CX.data()),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_CY.data()),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_CZ.data()),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_TipoElipse_Atr.data()),Contador_Circle*sizeof(int));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_Color_Circle_Atr.data()),Contador_Circle*sizeof(COLORREF));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_AnguloX.data()),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_AnguloY.data()),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_AnguloZ.data()),Contador_Circle*sizeof(long double));



							EixoXY->Objetos_Atr[0].Elipse_Atr->SetA_Atr(Tmp_A_Atr.data());
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetB_Atr(Tmp_B_Atr.data());
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetAZo_Atr(Tmp_AZo_Atr.data());
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetBZo_Atr(Tmp_BZo_Atr.data());
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetCX(Tmp_CX.data());
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetCY(Tmp_CY.data());
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetCZ(Tmp_CZ.data());
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetTipoElipse_Atr(Tmp_TipoElipse_Atr.data());
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetColor_Circle_Atr(Tmp_Color_Circle_Atr.data());
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetContadorCircle_Atr(Contador_Circle);
							EixoXY->Objetos_Atr[0].Elipse_Atr->Set_AnguloX(Tmp_AnguloX.data());
							EixoXY->Objetos_Atr[0].Elipse_Atr->Set_AnguloY(Tmp_AnguloY.data());
							EixoXY->Objetos_Atr[0].Elipse_Atr->Set_AnguloZ(Tmp_AnguloZ.data());

							EixoXY->Contador_Circle=Contador_Circle;

							EixoXY->P_Circle=EixoXY->Draw_Elipse(EixoXY->Canvas,EixoXY->Objetos_Atr);
							Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);

						}

						Arquivo_RR.close();

					}else{
							ShowMessage("Erro de Leitura ..."+AnsiString(Nome_Arquivo));

							Arquivo_RR.close();

						}







					ShowMessage("Leitura Completa...");



				}





		}else{

				ShowMessage("Erro de Leitura ...");

				 Arquivo_RR.close();

			}










}
void TEixoXY::Limpa_Elipses(TCanvas *Plano,TPoint **P_Circle,int Contador_Circle){
	Plano->Pen->Color=clWhite;
	for(int i=0;i<Contador_Circle;i++){

		Plano->Polyline(P_Circle[i],50);

	}



}

void Limpa_Lines(TCanvas *Plano,int **FullMLK,int Countador_Line){

TPoint MP[2];

	for (int i = 0; i < Countador_Line; i++) {
		MP[0].x=FullMLK[i][0];
		MP[0].y=FullMLK[i][1];
		MP[1].x=FullMLK[i][2];
		MP[1].y=FullMLK[i][3];

		Plano->Pen->Color=clWhite;
		Plano->Polyline(MP,1);

	}
}




//---------------------------------------------------------------------------
// Duplica Teta e Duplica Alpha
//
// As contas (giro na tela, plano de cada figura, quantas copias cada uma ja tem)
// ficam no Duplicador (Geometria.cpp). Aqui fica so o que e do EixoXY: a visao
// escolhida nos RadioButtons e os contadores e buffers de tela deste form.
//---------------------------------------------------------------------------

static TipoVisao VisaoAtual(){
	if(EixoXY->RBVisaoXoZ->Checked)
		return VisaoXoZ;
	if(EixoXY->RBVisaoYoZ->Checked)
		return VisaoYoZ;
	return VisaoXoY;
}

// originais, plano de cada figura e copias do desenho do EixoXY
static Duplicador Duplicacao;

// plano da visao de agora, para uma figura que acabou de ser desenhada
static PlanoDoDesenho PlanoAtual(){
	const long double *ABT=EixoXY->Objetos_Atr[0].GetAlpha_B_T();
	PlanoDoDesenho P;
	P.Guardado=true;
	P.ABT[0]=ABT[0];
	P.ABT[1]=ABT[1];
	P.ABT[2]=ABT[2];
	P.Visao=VisaoAtual();
	return P;
}

// Duplica as figuras e acerta os contadores e os buffers de tela do EixoXY, que o
// Duplicador nao conhece
static void DuplicaNoEixoXY(Objetos *Objetos_Atr,const long double *ABT,bool Comeco,TipoGiro Tipo){

	Duplicacao.Duplica(Objetos_Atr,ABT,VisaoAtual(),Comeco,Tipo,M_PI/80);

	const int Linhas=Objetos_Atr[0].Lines_Atr->GetContador_Line();
	if(Linhas>EixoXY->Contador_Line){
		EixoXY->FullMLK=EixoXY->BufFullMLK.Redimensiona(Linhas,4);
		EixoXY->Contador_Line=Linhas;
	}
	if(Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero()>0)
		Contador=Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero();	// o proximo quadrilatero do mouse vai depois das copias
	if(Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr()>0)
		EixoXY->Contador_Circle=Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr();
}

// Cada chamada acrescenta uma copia dos objetos originais (linhas, quadrilateros e
// elipses) girada em volta do eixo que sai da tela, no plano em que cada figura
// foi desenhada: cada figura gira PI/80 a mais que a sua copia anterior.
void TEixoXY::DuplicaValoresTeta(Objetos *Objetos_Atr,long double *ABT){

	DuplicaNoEixoXY(Objetos_Atr,ABT,Faz,GiroNoPlanoDaTela);
	Faz=false;
}

// Mesma logica do DuplicaValoresTeta, mas cada copia gira mais PI/80 em volta do
// eixo horizontal da tela.
void TEixoXY::DuplicaValoresAlpha(Objetos *Objetos_Atr){

	DuplicaNoEixoXY(Objetos_Atr,ABT,Faz,GiroNoEixoHorizontal);
	Faz=false;
}

//---------------------------------------------------------------------------
// Figuras novas no plano da visao girada
//
// O FormMouseDown grava cada clique como se a visao nao estivesse girada: OX(x) no
// eixo horizontal da visao, OY(y) no vertical e o valor fixo (Zo, Yo ou Xo) na
// profundidade. Isso e justamente o ponto nas coordenadas da tela (v). Quando a
// figura termina, ela passa para o mundo pelo plano girado (p = Rt*v), e por isso
// aparece onde foi desenhada mesmo com a visao girada. As figuras antigas nao mudam.
// Com a visao sem giro (ABT = 0) R e a identidade e nada e alterado.
//---------------------------------------------------------------------------

static bool VisaoSemGiro(){
	const long double *ABT=EixoXY->Objetos_Atr[0].GetAlpha_B_T();
	return ABT[0]==0 && ABT[1]==0 && ABT[2]==0;
}

// R da visao atual (os angulos do ABT mais os angulos proprios da figura)
static VisaoTela VisaoDoDesenho(long double MaisAlpha,long double MaisBeta,long double MaisTeta){
	const long double *ABT=EixoXY->Objetos_Atr[0].GetAlpha_B_T();
	return MontaVisaoTela(ABT[0]+MaisAlpha,ABT[1]+MaisBeta,ABT[2]+MaisTeta,VisaoAtual());
}

// linha i acabou de ser desenhada: as duas pontas vao para o plano da visao
static void LevaLinhaParaVisao(int i){

	Name_Arquivos::Line *Lines=EixoXY->Objetos_Atr[0].Lines_Atr;

	// linha nova comeca sem rotacao propria (o indice pode ter sobrado de um Desfazer)
	Lines->Set_AngluloTeta(0,i);
	Lines->Set_AngluloAlpha(0,i);
	Duplicacao.GuardaPlanoLine(i,PlanoAtual());		// lembra o plano em que a linha foi desenhada
	if(VisaoSemGiro())
		return;

	const VisaoTela T=VisaoDoDesenho(0,0,0);
	long double Alpha,Vetor,Zo,Xo,Yo;

	Alpha=Lines->GetMAlpha_Atr(i); Vetor=Lines->GetMVetor_Atr(i); Zo=Lines->GetMZo_Atr(i);
	PolarParaMundo(T,Alpha,Vetor,Zo,Xo,Yo);
	Lines->SetMAlpha_Atr(Alpha,i);
	Lines->SetMVetor_Atr(Vetor,i);
	Lines->SetMZo_Atr(Zo,i);
	Lines->SetMXo_Atr(Xo,i);
	Lines->SetMYo_Atr(Yo,i);

	Alpha=Lines->GetLAlpha_Atr(i); Vetor=Lines->GetLVetor_Atr(i); Zo=Lines->GetLZo_Atr(i);
	PolarParaMundo(T,Alpha,Vetor,Zo,Xo,Yo);
	Lines->SetLAlpha_Atr(Alpha,i);
	Lines->SetLVetor_Atr(Vetor,i);
	Lines->SetLZo_Atr(Zo,i);
	Lines->SetLXo_Atr(Xo,i);
	Lines->SetLYo_Atr(Yo,i);
}

// quadrilatero i acabou de ser desenhado: os 4 cantos vao para o plano da visao
static void LevaQuadParaVisao(int i){

	Duplicacao.GuardaPlanoQuad(i,PlanoAtual());		// lembra o plano em que o quadrilatero foi desenhado
	if(VisaoSemGiro())
		return;

	TQuadrilatero *Quad=EixoXY->Objetos_Atr[0].Quadrilatero;
	const VisaoTela T=VisaoDoDesenho(0,0,0);
	long double Alpha,Vetor,Zo,Xo,Yo;

	Alpha=Quad->GetMAlpha(i); Vetor=Quad->GetMVetor(i); Zo=Quad->GetMZo(i);
	PolarParaMundo(T,Alpha,Vetor,Zo,Xo,Yo);
	Quad->SetMAlpha(Alpha,i); Quad->SetMVetor(Vetor,i); Quad->SetMZo(Zo,i); Quad->SetMXo(Xo,i); Quad->SetMYo(Yo,i);

	Alpha=Quad->GetLAlpha(i); Vetor=Quad->GetLVetor(i); Zo=Quad->GetLZo(i);
	PolarParaMundo(T,Alpha,Vetor,Zo,Xo,Yo);
	Quad->SetLAlpha(Alpha,i); Quad->SetLVetor(Vetor,i); Quad->SetLZo(Zo,i); Quad->SetLXo(Xo,i); Quad->SetLYo(Yo,i);

	Alpha=Quad->GetMAlpha2(i); Vetor=Quad->GetMVetor2(i); Zo=Quad->GetMZo2(i);
	PolarParaMundo(T,Alpha,Vetor,Zo,Xo,Yo);
	Quad->SetMAlpha2(Alpha,i); Quad->SetMVetor2(Vetor,i); Quad->SetMZo2(Zo,i); Quad->SetMXo2(Xo,i); Quad->SetMYo2(Yo,i);

	Alpha=Quad->GetLAlpha2(i); Vetor=Quad->GetLVetor2(i); Zo=Quad->GetLZo2(i);
	PolarParaMundo(T,Alpha,Vetor,Zo,Xo,Yo);
	Quad->SetLAlpha2(Alpha,i); Quad->SetLVetor2(Vetor,i); Quad->SetLZo2(Zo,i); Quad->SetLXo2(Xo,i); Quad->SetLYo2(Yo,i);
}

// elipse i acabou de ser desenhada: o centro fica como foi clicado (no plano do
// TipoElipse) e o giro proprio da elipse leva o circulo inteiro para o plano da tela
// girada: AnguloY=-Alpha, AnguloZ=-Beta e AnguloX=-Teta fazem M = Rt (GiroDaElipse).
// Com a visao sem giro os angulos ficam 0 e nada muda.
static void LevaElipseParaVisao(int i){

	Duplicacao.GuardaPlanoCircle(i,PlanoAtual());		// lembra o plano em que a elipse foi desenhada

	Name_Arquivos::Elipse *Elipses=EixoXY->Objetos_Atr[0].Elipse_Atr;
	const long double *ABT=EixoXY->Objetos_Atr[0].GetAlpha_B_T();
	Elipses->Set_AnguloY(-ABT[0],i);
	Elipses->Set_AnguloZ(-ABT[1],i);
	Elipses->Set_AnguloX(-ABT[2],i);
}

// Desenha os quadrilateros com os pontos de tela devolvidos pelo Draw_Quadrilatero:
// MLP_Quadrilane tem 5 pontos por quadrilatero (os 4 cantos e o 5o fecha no 1o).
void TEixoXY::DrawQuadrilineDirect(TCanvas *Plano,Objetos *Objetos_Atr,TPoint **MLP_Quadrilane){

TQuadrilatero *Quadrilatero=Objetos_Atr[0].Quadrilatero;
const int Quantidade=Quadrilatero->GetContadorQuadrilatero();

	// sem pontos calculados, ou calculados para menos quadrilateros do que existem
	if((MLP_Quadrilane==NULL)||(Quantidade>BufMP2.GetLinhas()))
		return;

	// uma chamada PolyPolyline para cada sequencia de quadrilateros da mesma cor
	PolyPolylinePorCor(Plano,MLP_Quadrilane,Quantidade,5,
		[Quadrilatero](int i){ return Quadrilatero->GetCor(i); });

}

void TEixoXY::Draw_Lines_Direct(TCanvas *Plano,Objetos *Objetos_Atr, int **FullMLK){

Name_Arquivos::Line *Lines_Atr=Objetos_Atr[0].Lines_Atr;

	// cada linha tem 2 pontos, ja calculados em MP1 pelo Draw_Lines
	PolyPolylinePorCor(Plano,EixoXY->MP1,Lines_Atr->GetContador_Line(),2,
	[Lines_Atr](int i){ return  Lines_Atr->GetCor_Line(i);});


}


TPoint **TEixoXY::Draw_Lines(TCanvas *Plano,Objetos *Objetos_Atr,int **FullMLK){

TPoint P1[2];
long double Mxyz[2];
long double Lxyz[2];
long double ZMo,ZLo,XMo,XLo,YMo,YLo;
long double xo;
long double yo;
long double zo;
long double ABT[3];
TPoint **MP1=0;


	for(int i=0;i<3;i++)
	   ABT[i]=Objetos_Atr[0].GetAlpha_B_T(i);
	   Contador_Line=Objetos_Atr[0].Lines_Atr->GetContador_Line();
	   MP1=BufMP1.Redimensiona(Contador_Line+15,2);



	try{
		for(int i=0;i<Objetos_Atr[0].Lines_Atr->GetContador_Line();i++){

			long double Teta=Objetos_Atr[0].Lines_Atr->GetAnguloTeta(i);
			long double Alpha=Objetos_Atr[0].Lines_Atr->GetAnguloAlpha(i);
			if(RBVisaoXoY->Checked){

				xo=Cos(Objetos_Atr[0].Lines_Atr->GetMAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetMVetor_Atr(i);
				yo=Sin(Objetos_Atr[0].Lines_Atr->GetMAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetMVetor_Atr(i);
				zo=Objetos_Atr[0].Lines_Atr->GetMZo_Atr(i);



				Mxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2]+Teta)) -( yo*Cos( ABT[1])*Sin(ABT[2]+Teta))+zo*Sin(ABT[1]);

				Mxyz[1]=(xo)*(Sin(ABT[0]+Alpha)*Sin(ABT[1])*Cos(ABT[2]+Teta)+
				Cos(ABT[0]+Alpha)*Sin(ABT[2]+Teta))+(yo)*(-Sin(ABT[0]+Alpha)*Sin(ABT[1])*Sin(ABT[2]+Teta)+
				Cos(ABT[0]+Alpha)*Cos(ABT[2]+Teta))+zo*(-Sin(ABT[0]+Alpha)*Cos(ABT[1]));

				xo=Cos(Objetos_Atr[0].Lines_Atr->GetLAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetLVetor_Atr(i);
				yo=Sin(Objetos_Atr[0].Lines_Atr->GetLAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetLVetor_Atr(i);
				zo=Objetos_Atr[0].Lines_Atr->GetLZo_Atr(i);


				Lxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2]+Teta)) - yo*Cos( ABT[1])* Sin(ABT[2]+Teta)+zo*Sin(ABT[1]);

				Lxyz[1]=(xo)*(Sin(ABT[0]+Alpha)*Sin(ABT[1])*Cos(ABT[2]+Teta)+
				Cos(ABT[0]+Alpha)*Sin(ABT[2]+Teta))+( yo )*(-Sin(ABT[0]+Alpha)*Sin(ABT[1])*Sin(ABT[2]+Teta)+
				Cos(ABT[0]+Alpha)*Cos(ABT[2]+Teta))+zo*(-Sin(ABT[0]+Alpha)*Cos(ABT[1]));


				FullMLK[i][0]=P1[0].x=MP1[i][0].x=X(RoundTo(Mxyz[0],0));
				FullMLK[i][1]=P1[0].y=MP1[i][0].y=Y(RoundTo(Mxyz[1],0));
				FullMLK[i][2]=P1[1].x=MP1[i][1].x=X(RoundTo(Lxyz[0],0));
				FullMLK[i][3]=P1[1].y=MP1[i][1].y=Y(RoundTo(Lxyz[1],0));





			}

			if(RBVisaoXoZ->Checked){

				xo=Cos(Objetos_Atr[0].Lines_Atr->GetMAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetMVetor_Atr(i);
				yo=Sin(Objetos_Atr[0].Lines_Atr->GetMAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetMVetor_Atr(i);
				zo=Objetos_Atr[0].Lines_Atr->GetMZo_Atr(i);

				Mxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2]+Teta)) -( yo*Cos( ABT[1])*Sin(ABT[2]+Teta))+zo*Sin(ABT[1]);

				Mxyz[1]=(xo)*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2]+Teta)+
					Sin(ABT[0])*Sin(ABT[2]+Teta))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2]+Teta)+
					Sin(ABT[0])*Cos(ABT[2]+Teta))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				xo=Cos(Objetos_Atr[0].Lines_Atr->GetLAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetLVetor_Atr(i);
				yo=Sin(Objetos_Atr[0].Lines_Atr->GetLAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetLVetor_Atr(i);
				zo=Objetos_Atr[0].Lines_Atr->GetLZo_Atr(i);

				Lxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2]+Teta)) - yo*Cos( ABT[1])* Sin(ABT[2]+Teta)+zo*Sin(ABT[1]);

				Lxyz[1]=(xo  )*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2]+Teta)+
					Sin(ABT[0])*Sin(ABT[2]+Teta))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2]+Teta)+
					Sin(ABT[0])*Cos(ABT[2]+Teta))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				FullMLK[i][0]=P1[0].x=MP1[i][0].x=X(RoundTo(Mxyz[0],0));
				FullMLK[i][1]=P1[0].y=MP1[i][0].y=Y(RoundTo(Mxyz[1],0));
				FullMLK[i][2]=P1[1].x=MP1[i][1].x=X(RoundTo(Lxyz[0],0));
				FullMLK[i][3]=P1[1].y=MP1[i][1].y=Y(RoundTo(Lxyz[1],0));





			}


			if(RBVisaoYoZ->Checked){

				xo=Cos(Objetos_Atr[0].Lines_Atr->GetMAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetMVetor_Atr(i);
				yo=Sin(Objetos_Atr[0].Lines_Atr->GetMAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetMVetor_Atr(i);
				zo=Objetos_Atr[0].Lines_Atr->GetMZo_Atr(i);

				Mxyz[0]=(xo)*(Sin(ABT[0])*Sin(ABT[1])*Cos(ABT[2]+Teta)+
				Cos(ABT[0])*Sin(ABT[2]+Teta))+(yo)*(-Sin(ABT[0])*Sin(ABT[1])*Sin(ABT[2]+Teta)+
				Cos(ABT[0])*Cos(ABT[2]+Teta))+zo*(-Sin(ABT[0])*Cos(ABT[1]));

				Mxyz[1]=(xo)*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2]+Teta)+
				Sin(ABT[0])*Sin(ABT[2]+Teta))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2]+Teta)+
				Sin(ABT[0])*Cos(ABT[2]+Teta))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				xo=Cos(Objetos_Atr[0].Lines_Atr->GetLAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetLVetor_Atr(i);
				yo=Sin(Objetos_Atr[0].Lines_Atr->GetLAlpha_Atr(i))*Objetos_Atr[0].Lines_Atr->GetLVetor_Atr(i);
				zo=Objetos_Atr[0].Lines_Atr->GetLZo_Atr(i);

				Lxyz[0]=(xo)*(Sin(ABT[0])*Sin(ABT[1])*Cos(ABT[2]+Teta)+
				Cos(ABT[0])*Sin(ABT[2]+Teta))+( yo )*(-Sin(ABT[0])*Sin(ABT[1])*Sin(ABT[2]+Teta)+
				Cos(ABT[0])*Cos(ABT[2]+Teta))+zo*(-Sin(ABT[0])*Cos(ABT[1]));

				Lxyz[1]=(xo  )*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2]+Teta)+
				Sin(ABT[0])*Sin(ABT[2]+Teta))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2]+Teta)+
				Sin(ABT[0])*Cos(ABT[2]+Teta))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				FullMLK[i][0]=P1[0].x=MP1[i][0].x=X(RoundTo(Mxyz[0],0));
				FullMLK[i][1]=P1[0].y=MP1[i][0].y=Y(RoundTo(Mxyz[1],0));
				FullMLK[i][2]=P1[1].x=MP1[i][1].x=X(RoundTo(Lxyz[0],0));
				FullMLK[i][3]=P1[1].y=MP1[i][1].y=Y(RoundTo(Lxyz[1],0));




			}





		}



	}

	catch(Exception &ex){

	ShowMessage("Erro ao Imprimir o Grafico");

	}



return  MP1;
}

TPoint **TEixoXY::Draw_Quadrilatero(TCanvas *Plano,Objetos *Objetos_Atr,int **TmpFullMLK2, int **TmpFullMLK3){

TPoint P1[2];
long double Mxyz[2];
long double Lxyz[2];
long double xo;
long double yo;
long double zo;
long double ABT[3];
TPoint **MP2=0;				// 5 pontos de tela por quadrilatero (o ultimo fecha no primeiro)
	int ContadorQuadrilatero=Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero();
    FullMLK2=BufFullMLK2.Redimensiona(std::max(Contador,ContadorQuadrilatero)+1,4);

   	FullMLK3=BufFullMLK3.Redimensiona(std::max(Contador,ContadorQuadrilatero)+1,4);

	for(int i=0;i<3;i++)
	   ABT[i]=Objetos_Atr[0].GetAlpha_B_T(i);

	   MP2=BufMP2.Redimensiona(ContadorQuadrilatero+15,5);




	try{
		for(int i=0;i<ContadorQuadrilatero;i++){


			if(RBVisaoXoY->Checked){

				xo=Cos(Objetos_Atr[0].Quadrilatero->GetMAlpha(i))*Objetos_Atr[0].Quadrilatero->GetMVetor(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetMAlpha(i))*Objetos_Atr[0].Quadrilatero->GetMVetor(i);
				zo=Objetos_Atr[0].Quadrilatero->GetMZo(i);



				Mxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2])) -( yo*Cos( ABT[1])*Sin(ABT[2]))+zo*Sin(ABT[1]);

				Mxyz[1]=(xo)*(Sin(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Cos(ABT[0])*Sin(ABT[2]))+(yo)*(-Sin(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Cos(ABT[0])*Cos(ABT[2]))+zo*(-Sin(ABT[0])*Cos(ABT[1]));


				xo=Cos(Objetos_Atr[0].Quadrilatero->GetLAlpha(i))*Objetos_Atr[0].Quadrilatero->GetLVetor(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetLAlpha(i))*Objetos_Atr[0].Quadrilatero->GetLVetor(i);
				zo=Objetos_Atr[0].Quadrilatero->GetLZo(i);


				Lxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2])) - yo*Cos( ABT[1])* Sin(ABT[2])+zo*Sin(ABT[1]);

				Lxyz[1]=(xo)*(Sin(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Cos(ABT[0])*Sin(ABT[2]))+( yo )*(-Sin(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Cos(ABT[0])*Cos(ABT[2]))+zo*(-Sin(ABT[0])*Cos(ABT[1]));



				FullMLK2[i][0]=X(RoundTo(Mxyz[0],0));
				FullMLK2[i][1]=Y(RoundTo(Mxyz[1],0));
				FullMLK2[i][2]=X(RoundTo(Lxyz[0],0));
				FullMLK2[i][3]=Y(RoundTo(Lxyz[1],0));





			}

			if(RBVisaoXoZ->Checked){

				xo=Cos(Objetos_Atr[0].Quadrilatero->GetMAlpha(i))*Objetos_Atr[0].Quadrilatero->GetMVetor(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetMAlpha(i))*Objetos_Atr[0].Quadrilatero->GetMVetor(i);
				zo=Objetos_Atr[0].Quadrilatero->GetMZo(i);

				Mxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2])) -( yo*Cos( ABT[1])*Sin(ABT[2]))+zo*Sin(ABT[1]);

				Mxyz[1]=(xo)*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
					Sin(ABT[0])*Sin(ABT[2]))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
					Sin(ABT[0])*Cos(ABT[2]))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				xo=Cos(Objetos_Atr[0].Quadrilatero->GetLAlpha(i))*Objetos_Atr[0].Quadrilatero->GetLVetor(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetLAlpha(i))*Objetos_Atr[0].Quadrilatero->GetLVetor(i);
				zo=Objetos_Atr[0].Quadrilatero->GetLZo(i);

				Lxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2])) - yo*Cos( ABT[1])* Sin(ABT[2])+zo*Sin(ABT[1]);

				Lxyz[1]=(xo  )*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
					Sin(ABT[0])*Sin(ABT[2]))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
					Sin(ABT[0])*Cos(ABT[2]))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				FullMLK2[i][0]=X(RoundTo(Mxyz[0],0));
				FullMLK2[i][1]=Y(RoundTo(Mxyz[1],0));
				FullMLK2[i][2]=X(RoundTo(Lxyz[0],0));
				FullMLK2[i][3]=Y(RoundTo(Lxyz[1],0));





			}


			if(RBVisaoYoZ->Checked){

				xo=Cos(Objetos_Atr[0].Quadrilatero->GetMAlpha(i))*Objetos_Atr[0].Quadrilatero->GetMVetor(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetMAlpha(i))*Objetos_Atr[0].Quadrilatero->GetMVetor(i);
				zo=Objetos_Atr[0].Quadrilatero->GetMZo(i);

				Mxyz[0]=(xo)*(Sin(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Cos(ABT[0])*Sin(ABT[2]))+(yo)*(-Sin(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Cos(ABT[0])*Cos(ABT[2]))+zo*(-Sin(ABT[0])*Cos(ABT[1]));

				Mxyz[1]=(xo)*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Sin(ABT[0])*Sin(ABT[2]))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Sin(ABT[0])*Cos(ABT[2]))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				xo=Cos(Objetos_Atr[0].Quadrilatero->GetLAlpha(i))*Objetos_Atr[0].Quadrilatero->GetLVetor(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetLAlpha(i))*Objetos_Atr[0].Quadrilatero->GetLVetor(i);
				zo=Objetos_Atr[0].Quadrilatero->GetLZo(i);

				Lxyz[0]=(xo)*(Sin(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Cos(ABT[0])*Sin(ABT[2]))+( yo )*(-Sin(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Cos(ABT[0])*Cos(ABT[2]))+zo*(-Sin(ABT[0])*Cos(ABT[1]));

				Lxyz[1]=(xo  )*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Sin(ABT[0])*Sin(ABT[2]))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Sin(ABT[0])*Cos(ABT[2]))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				FullMLK2[i][0]=X(RoundTo(Mxyz[0],0));
				FullMLK2[i][1]=Y(RoundTo(Mxyz[1],0));
				FullMLK2[i][2]=X(RoundTo(Lxyz[0],0));
				FullMLK2[i][3]=Y(RoundTo(Lxyz[1],0));




			}




			if(RBVisaoXoY->Checked){

				xo=Cos(Objetos_Atr[0].Quadrilatero->GetMAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetMVetor2(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetMAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetMVetor2(i);
				zo=Objetos_Atr[0].Quadrilatero->GetMZo2(i);



				Mxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2])) -( yo*Cos( ABT[1])*Sin(ABT[2]))+zo*Sin(ABT[1]);

				Mxyz[1]=(xo)*(Sin(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Cos(ABT[0])*Sin(ABT[2]))+(yo)*(-Sin(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Cos(ABT[0])*Cos(ABT[2]))+zo*(-Sin(ABT[0])*Cos(ABT[1]));


				xo=Cos(Objetos_Atr[0].Quadrilatero->GetLAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetLVetor2(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetLAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetLVetor2(i);
				zo=Objetos_Atr[0].Quadrilatero->GetLZo2(i);


				Lxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2])) - yo*Cos( ABT[1])* Sin(ABT[2])+zo*Sin(ABT[1]);

				Lxyz[1]=(xo)*(Sin(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Cos(ABT[0])*Sin(ABT[2]))+( yo )*(-Sin(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Cos(ABT[0])*Cos(ABT[2]))+zo*(-Sin(ABT[0])*Cos(ABT[1]));



				FullMLK3[i][0]=X(RoundTo(Mxyz[0],0));
				FullMLK3[i][1]=Y(RoundTo(Mxyz[1],0));
				FullMLK3[i][2]=X(RoundTo(Lxyz[0],0));
				FullMLK3[i][3]=Y(RoundTo(Lxyz[1],0));





			}

			if(RBVisaoXoZ->Checked){

				xo=Cos(Objetos_Atr[0].Quadrilatero->GetMAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetMVetor2(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetMAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetMVetor2(i);
				zo=Objetos_Atr[0].Quadrilatero->GetMZo2(i);

				Mxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2])) -( yo*Cos( ABT[1])*Sin(ABT[2]))+zo*Sin(ABT[1]);

				Mxyz[1]=(xo)*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
					Sin(ABT[0])*Sin(ABT[2]))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
					Sin(ABT[0])*Cos(ABT[2]))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				xo=Cos(Objetos_Atr[0].Quadrilatero->GetLAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetLVetor2(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetLAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetLVetor2(i);
				zo=Objetos_Atr[0].Quadrilatero->GetLZo2(i);

				Lxyz[0]=(xo)*(Cos(ABT[1])*Cos(ABT[2])) - yo*Cos( ABT[1])* Sin(ABT[2])+zo*Sin(ABT[1]);

				Lxyz[1]=(xo  )*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
					Sin(ABT[0])*Sin(ABT[2]))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
					Sin(ABT[0])*Cos(ABT[2]))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				FullMLK3[i][0]=X(RoundTo(Mxyz[0],0));
				FullMLK3[i][1]=Y(RoundTo(Mxyz[1],0));
				FullMLK3[i][2]=X(RoundTo(Lxyz[0],0));
				FullMLK3[i][3]=Y(RoundTo(Lxyz[1],0));





			}


			if(RBVisaoYoZ->Checked){

				xo=Cos(Objetos_Atr[0].Quadrilatero->GetMAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetMVetor2(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetMAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetMVetor2(i);
				zo=Objetos_Atr[0].Quadrilatero->GetMZo2(i);

				Mxyz[0]=(xo)*(Sin(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Cos(ABT[0])*Sin(ABT[2]))+(yo)*(-Sin(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Cos(ABT[0])*Cos(ABT[2]))+zo*(-Sin(ABT[0])*Cos(ABT[1]));

				Mxyz[1]=(xo)*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Sin(ABT[0])*Sin(ABT[2]))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Sin(ABT[0])*Cos(ABT[2]))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				xo=Cos(Objetos_Atr[0].Quadrilatero->GetLAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetLVetor2(i);
				yo=Sin(Objetos_Atr[0].Quadrilatero->GetLAlpha2(i))*Objetos_Atr[0].Quadrilatero->GetLVetor2(i);
				zo=Objetos_Atr[0].Quadrilatero->GetLZo2(i);

				Lxyz[0]=(xo)*(Sin(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Cos(ABT[0])*Sin(ABT[2]))+( yo )*(-Sin(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Cos(ABT[0])*Cos(ABT[2]))+zo*(-Sin(ABT[0])*Cos(ABT[1]));

				Lxyz[1]=(xo  )*(-Cos(ABT[0])*Sin(ABT[1])*Cos(ABT[2])+
				Sin(ABT[0])*Sin(ABT[2]))+(yo)*(Cos(ABT[0])*Sin(ABT[1])*Sin(ABT[2])+
				Sin(ABT[0])*Cos(ABT[2]))+zo*(Cos(ABT[0])*Cos(ABT[1]));

				FullMLK3[i][0]=X(RoundTo(Mxyz[0],0));
				FullMLK3[i][1]=Y(RoundTo(Mxyz[1],0));
				FullMLK3[i][2]=X(RoundTo(Lxyz[0],0));
				FullMLK3[i][3]=Y(RoundTo(Lxyz[1],0));




			}

			// os 5 pontos do contorno: M e L do primeiro lado, M e L do segundo, e fecha no primeiro
			MP2[i][0].x=FullMLK2[i][0];
			MP2[i][0].y=FullMLK2[i][1];
			MP2[i][1].x=FullMLK2[i][2];
			MP2[i][1].y=FullMLK2[i][3];

			MP2[i][2].x=FullMLK3[i][0];
			MP2[i][2].y=FullMLK3[i][1];
			MP2[i][3].x=FullMLK3[i][2];
			MP2[i][3].y=FullMLK3[i][3];

			MP2[i][4]=MP2[i][0];





		}



	}

	catch(Exception &ex){

	ShowMessage("Erro ao Imprimir o Grafico");

	}



return  MP2;
}



long double X(long double x){
return x+Kx;
}


long double Y(long double y){
return Ky-y;
}




long double Raio(int x1,int y1,int x2,int y2){
	long double Angulo;
	long double x3;
	long double y3;
	long double Modulo_Elipse;
	x3=::OX(x2)-::OX(x1);
	y3=::OY(y2)-::OY(y1);
	EixoXo(x3,y3,0,&Angulo,&Modulo_Elipse);




	return Modulo_Elipse;

}

long double OX(long double x){

return (x-KOx);

}

long double OY(long double y){
  return (KOy-y);
}

//--------------------------------------------------------------------------
//Imprime Os Eixos Cartesianos
void Eixos(TCanvas *Plano,int Kx,int Ky){
TPoint P1[2];


	P1[0].x=0;
	P1[0].y=Ky;
	P1[1].x=Screen->DesktopWidth;
	P1[1].y=Ky;
	Plano->Pen->Color=clBlack;
	Plano->Polyline(P1,1);

	if(EixoXY->RBVisaoXoY->Checked){

		P1[0].x=Kx;
		P1[0].y=0;
		P1[1].x=Kx;
		P1[1].y=Screen->DesktopHeight;
		Plano->Pen->Color=clPurple;
		Plano->Polyline(P1,1);
	}

	if(EixoXY->RBVisaoXoZ->Checked){

		P1[0].x=Kx;
		P1[0].y=0;
		P1[1].x=Kx;
		P1[1].y=Screen->DesktopHeight;
		Plano->Pen->Color=clLime;
		Plano->Polyline(P1,1);
	}

		if(EixoXY->RBVisaoYoZ->Checked){

		P1[0].x=Kx;
		P1[0].y=0;
		P1[1].x=Kx;
		P1[1].y=Screen->DesktopHeight;
		Plano->Pen->Color=clAqua;
		Plano->Polyline(P1,1);
	}




}
//---------------------------------------------------------------------------
void __fastcall TEixoXY::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)

{
TPoint P1;

float x;
float y;
  float Ang,Mtar;

  long double Angulo;
  TPoint P3[2];





	if ((Line->Checked)&&(::Move)) {

			EixoXY->Canvas->Pen->Color=clWhite;
			if((MP[1].x>0)||(MP[1].y>0)){

				EixoXY->Canvas->Polyline(MP,1);

			}

			P1=EixoXY->ScreenToClient(Mouse->CursorPos);
			FullLKx[Contador]=MP[1].x=P1.x;
			FullLKy[Contador]=MP[1].y=P1.y;

			EixoXY->Canvas->Pen->Color=clRed;

			EixoXY->Canvas->Polyline(MP,1);


	}

	if((RBElipse->Checked)&&(::Move)){
		LimpaElipse(Raio(Cx2,Cy2,Cx,Cy),Cx,Cy);
		P1=EixoXY->ScreenToClient(Mouse->CursorPos);


		Cx2=P1.x;
		Cy2=P1.Y;
		Raio_Elipse=Raio(P1.x,P1.y,Cx,Cy);
		EixoXY->Canvas->Pen->Color=clPurple;
		Elipse(Raio_Elipse,Cx,Cy);
	}

	if((RBElipse->Checked)&&(Move1)){

		LimpaElipse(Raio_Elipse,Cx,Cy);
		P1=EixoXY->ScreenToClient(Mouse->CursorPos);
		Cx=P1.x;
		Cy=P1.y;

		EixoXY->Canvas->Pen->Color=clRed;
		Elipse(Raio_Elipse,Cx,Cy);


	}

	if((RBQuadrilatero->Checked)&&(::Move)){

		EixoXY->Canvas->Pen->Color=clWhite;
		EixoXY->Canvas->Polyline(MPQ,Contar);


			P1=EixoXY->ScreenToClient(Mouse->CursorPos);
			
			MPQ[Contar].x=P1.x;
			MPQ[Contar].Y=P1.y;
			EixoXY->Canvas->Pen->Color=clBlue;
			EixoXY->Canvas->Polyline(MPQ,Contar);
			if(Contar==3){

			   EixoXY->Canvas->Pen->Color=clWhite;
			   EixoXY->Canvas->Polyline(P2,1);
			   P2[1].x=P1.x;
			   P2[1].y=P1.y;

			   EixoXY->Canvas->Pen->Color=clBlue;
			   EixoXY->Canvas->Polyline(P2,1);



			}
            if(Contar==2){

			   EixoXY->Canvas->Pen->Color=clWhite;
			   EixoXY->Canvas->Polyline(P2,1);
			   P2[1].x=P1.x;
			   P2[1].y=P1.y;

			   EixoXY->Canvas->Pen->Color=clBlue;
			   EixoXY->Canvas->Polyline(P2,1);



			}



	}




	FDrawCursor->Top=Y+50;
	FDrawCursor->Left=X+20;

	FDrawCursor->DrawCursor->Caption="{ ("+FloatToStr(OX(X))+");("+FloatToStr(OY(Y))+") }";
}
//---------------------------------------------------------------------------
void __fastcall TEixoXY::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{

TPoint P1;

long double Angulo;
long double Modulo_Line;
int **TmpFullMLK2;
int **TmpFullMLK3;
long double Xo;




	P1=EixoXY->ScreenToClient(Mouse->CursorPos);

	if ((Button== mbLeft)&&(Line->Checked)){

		Objetos_Atr[0].Lines_Atr->Alocando_Line(Contador_Line+1);



		Objetos_Atr[0].Lines_Atr->SetCor_Line(Cores->Selected,Contador_Line);
		Contador_Line=Objetos_Atr[0].Lines_Atr->GetContador_Line();
		FullMLK=BufFullMLK.Redimensiona(Contador_Line+1,4);



		if(Contar<1){










		}




		if(RBVisaoXoY->Checked){

			Contar++;

			if ((Line->Checked)&&(Contar==1)) {



				MP[0].x=P1.x;
				MP[0].y=P1.y;
				Libera_Draw=true;
				Tempo_Draw->Enabled=true;


				EixoXo(::OX(MP[0].x),::OY(MP[0].y),0,&Angulo,&Modulo_Line);

				Objetos_Atr[0].Lines_Atr->SetMAlpha_Atr(Angulo,Contador_Line);
				Objetos_Atr[0].Lines_Atr->SetMVetor_Atr(Modulo_Line,Contador_Line);
				Objetos_Atr[0].Lines_Atr->SetMZo_Atr(EdtZo->Text.ToDouble(),Contador_Line);

				::Move=true;
			}


			if ((Line->Checked)&&(Contar==2)){

				EixoXo(::OX(MP[1].x),::OY(MP[1].y),0,&Angulo,&Modulo_Line);
				Objetos_Atr[0].Lines_Atr->SetLAlpha_Atr(Angulo,Contador_Line);
				Objetos_Atr[0].Lines_Atr->SetLVetor_Atr(Modulo_Line,Contador_Line);
				Objetos_Atr[0].Lines_Atr->SetLZo_Atr(EdtZo->Text.ToDouble(),Contador_Line);





				::Move=false;
				Contar=0;



				Contador_Line++;
				Objetos_Atr[0].Lines_Atr->SetContador_Line(Contador_Line);
				LevaLinhaParaVisao(Contador_Line-1);		// a linha nova vai para o plano da visao girada

				MP1=EixoXY->Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);

				Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
				Libera_Draw=false;
				Tempo_Draw->Enabled=false;
			}

		}

		if(RBVisaoXoZ->Checked){

			Contar++;
			float Yo=EdtYo->Text.ToDouble();

				// linha nova comeca sem rotacao (antes lia ABT[Contador_Line], fora do vetor ABT[3])
				Objetos_Atr[0].Lines_Atr->Set_AngluloTeta(0,Contador_Line);
				Objetos_Atr[0].Lines_Atr->Set_AngluloAlpha(0,Contador_Line);
				if ((Line->Checked)&&(Contar==1)) {


					MP[0].x=P1.x;
					MP[0].y=P1.y;
					Libera_Draw=true;
					Tempo_Draw->Enabled=true;


					EixoXo(::OX(MP[0].x),Yo,0,&Angulo,&Modulo_Line);

					Objetos_Atr[0].Lines_Atr->SetMAlpha_Atr(Angulo,Contador_Line);
					Objetos_Atr[0].Lines_Atr->SetMVetor_Atr(Modulo_Line,Contador_Line);
					Objetos_Atr[0].Lines_Atr->SetMZo_Atr(::OY(MP[0].y),Contador_Line);
					Objetos_Atr[0].Lines_Atr->SetMYo_Atr(Yo,Contador_Line);
					::Move=true;
				}

				if ((Line->Checked)&&(Contar==2)){

					EixoXo((float)::OX(MP[1].x),Yo,0,&Angulo,&Modulo_Line);
					Objetos_Atr[0].Lines_Atr->SetLAlpha_Atr(Angulo,Contador_Line);
					Objetos_Atr[0].Lines_Atr->SetLVetor_Atr(Modulo_Line,Contador_Line);
					Objetos_Atr[0].Lines_Atr->SetLZo_Atr(::OY(MP[1].y),Contador_Line);
					Objetos_Atr[0].Lines_Atr->SetLYo_Atr(Yo,Contador_Line);




					::Move=false;
					Contar=0;



					Contador_Line++;
					Objetos_Atr[0].Lines_Atr->SetContador_Line(Contador_Line);
					LevaLinhaParaVisao(Contador_Line-1);		// a linha nova vai para o plano da visao girada
					MP1=EixoXY->Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
					Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
                    Libera_Draw=false;
                    Tempo_Draw->Enabled=false;
				}







		}

		if(RBVisaoYoZ->Checked){

			Contar++;
			Xo=EdtXo->Text.ToDouble();

				if ((Line->Checked)&&(Contar==1)) {

					MP[0].x=P1.x;
					MP[0].y=P1.y;

					Libera_Draw=true;
					Tempo_Draw->Enabled=true;

					EixoXo(Xo,::OX(MP[0].x),0,&Angulo,&Modulo_Line);

					Objetos_Atr[0].Lines_Atr->SetMAlpha_Atr(Angulo,Contador_Line);
					Objetos_Atr[0].Lines_Atr->SetMVetor_Atr(Modulo_Line,Contador_Line);
					Objetos_Atr[0].Lines_Atr->SetMZo_Atr(::OY(MP[0].y),Contador_Line);
					Objetos_Atr[0].Lines_Atr->SetMXo_Atr(Xo,Contador_Line);
					::Move=true;
				}

				if ((Line->Checked)&&(Contar==2)){

					EixoXo(Xo,::OX(MP[1].x),0,&Angulo,&Modulo_Line);
					Objetos_Atr[0].Lines_Atr->SetLAlpha_Atr(Angulo,Contador_Line);
					Objetos_Atr[0].Lines_Atr->SetLVetor_Atr(Modulo_Line,Contador_Line);
					Objetos_Atr[0].Lines_Atr->SetLZo_Atr(::OY(MP[1].y),Contador_Line);
                    Objetos_Atr[0].Lines_Atr->SetLXo_Atr(Xo,Contador_Line);




					::Move=false;
					Contar=0;



					Contador_Line++;
					Objetos_Atr[0].Lines_Atr->SetContador_Line(Contador_Line);
					LevaLinhaParaVisao(Contador_Line-1);		// a linha nova vai para o plano da visao girada
					MP1=EixoXY->Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
                    Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
                    Libera_Draw=false;
                    Tempo_Draw->Enabled=false;
				}







		}


		if (Contar>6) {
			 Contar=0;
		}


	}


	if ((Button== mbLeft)&&(RBElipse->Checked)){
			int Contador_Circle=EixoXY->Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr();


			Primeiro=true;
			EixoXY->Objetos_Atr[0].Elipse_Atr->AlocaElipse_Atr(Contador_Circle+5);

            if ((RBElipse->Checked)&&(Contar==3)){

				::Move1=false;
				Contar=0;
				Primeiro=false;
				MP[0].x=P1.x;
				MP[0].y=P1.y;

				Cx2=::OX(P1.x);
				Cy2=::OY(P1.y);
				Libera_Draw=false;
				Tempo_Draw->Enabled=false;

				long double A=Raio_Elipse;
                Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
				EixoXY->Objetos_Atr[0].Elipse_Atr->SetColor_Circle_Atr(Cores->Selected,Contador_Circle-1);

				EixoXY->Objetos_Atr[0].Elipse_Atr->SetA_Atr(A,Contador_Circle-1);
				EixoXY->Objetos_Atr[0].Elipse_Atr->SetB_Atr(A,Contador_Circle-1);
				EixoXY->Objetos_Atr[0].Elipse_Atr->SetAZo_Atr(0,Contador_Circle-1);
				EixoXY->Objetos_Atr[0].Elipse_Atr->SetBZo_Atr(0,Contador_Circle-1);

				

				if(RBVisaoXoY->Checked){
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetTipoElipse_Atr(0,Contador_Circle-1);
                    EixoXY->Objetos_Atr[0].Elipse_Atr->SetCX(Cx2,Contador_Circle-1);
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetCY(Cy2,Contador_Circle-1);


				}

				if(RBVisaoXoZ->Checked){
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetTipoElipse_Atr(1,Contador_Circle-1);
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetCX(Cx2,Contador_Circle-1);
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetCZ(Cy2,Contador_Circle-1);


				}

				if(RBVisaoYoZ->Checked){
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetTipoElipse_Atr(2,Contador_Circle-1);
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetCY(Cx2,Contador_Circle-1);
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetCZ(Cy2,Contador_Circle-1);


				}

				::Move1=false;




				LevaElipseParaVisao(Contador_Circle-1);		// centro movido: plano da visao girada
				LimpaElipse(Raio_Elipse,Cx,Cy);

				EixoXY->P_Circle=EixoXY->Draw_Elipse(EixoXY->Canvas,EixoXY->Objetos_Atr);
				EixoXY->Draw_ElipseDirect(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->P_Circle);


				Contar=0;
			}

			if ((RBElipse->Checked)&&(Contar==2)){


				Contar++;
                Move1=true;


				Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);


			}



			if ((RBElipse->Checked)&&(Contar==1)) {


			   Contar++;
				MP[0].x=P1.x;
				MP[0].y=P1.y;
				Libera_Draw=true;
				Tempo_Draw->Enabled=true;
				Cx2=P1.x;
				Cy2=P1.y;

				Yes=MessageDlg("Quer Movimentar",mtConfirmation,TMsgDlgButtons() <<mbYes<<mbNo,0);
				if(Yes==mrYes){

				}else{
					Primeiro=false;
                    Contar=0;
                    Libera_Draw=false;
                    Tempo_Draw->Enabled=false;
					}

				long double A=Raio(Cx,Cy,Cx2,Cy2);

				EixoXY->Objetos_Atr[0].Elipse_Atr->SetColor_Circle_Atr(Cores->Selected,Contador_Circle);

				EixoXY->Objetos_Atr[0].Elipse_Atr->SetA_Atr(A,Contador_Circle);
				EixoXY->Objetos_Atr[0].Elipse_Atr->SetB_Atr(A,Contador_Circle);
				EixoXY->Objetos_Atr[0].Elipse_Atr->SetAZo_Atr(0,Contador_Circle);
				EixoXY->Objetos_Atr[0].Elipse_Atr->SetBZo_Atr(0,Contador_Circle);



				if(RBVisaoXoY->Checked){
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetTipoElipse_Atr(0,Contador_Circle);
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetCX(::OX(Cx),Contador_Circle);
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetCY(::OY(Cy),Contador_Circle);


				}

				if(RBVisaoXoZ->Checked){
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetTipoElipse_Atr(1,Contador_Circle);
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetCX(::OX(Cx),Contador_Circle);
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetCZ(::OY(Cy),Contador_Circle);


				}

				if(RBVisaoYoZ->Checked){
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetTipoElipse_Atr(2,Contador_Circle);
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetCY(::OX(Cx),Contador_Circle);
					EixoXY->Objetos_Atr[0].Elipse_Atr->SetCZ(::OY(Cy),Contador_Circle);


				}
				::Move=false;

				Contador_Circle++;
				EixoXY->Contador_Circle=Contador_Circle;
				EixoXY->Objetos_Atr[0].Elipse_Atr->SetContadorCircle_Atr(Contador_Circle);
				LevaElipseParaVisao(Contador_Circle-1);		// a elipse nova vai para o plano da visao girada
				LimpaElipse(Raio(int(P1.x),int(P1.y),Cx,Cy),Cx,Cy);
				Modulo_Elipse=0;
				EixoXY->P_Circle=EixoXY->Draw_Elipse(EixoXY->Canvas,EixoXY->Objetos_Atr);
				EixoXY->Draw_ElipseDirect(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->P_Circle);


			}

			if ((RBElipse->Checked)&&(Contar==0)&&(Primeiro)) {
				Libera_Draw=true;
				Tempo_Draw->Enabled=true;
				Contar++;
				::Move=true;
				Cx=P1.x;
				Cy=P1.y;
			}










	}

	if ((Button== mbLeft)&&(RBQuadrilatero->Checked)){

		Objetos_Atr[0].Quadrilatero->AlocarQuadrilatero(Contador+10);



		Objetos_Atr[0].Quadrilatero->SetCor(Cores->Selected,Contador);

		P1=EixoXY->ScreenToClient(Mouse->CursorPos);
		long double Xo=EdtXo->Text.ToDouble();
		long double Zo=EdtZo->Text.ToDouble();
		long double Yo=EdtYo->Text.ToDouble();

		if(RBVisaoXoY->Checked){


			switch(Contar){

				case 0 :
						Libera_Draw=true;
						Tempo_Draw->Enabled=true;

						MPQ[0].x=P1.x;
						MPQ[0].y=P1.y;
						P2[0].x=MPQ[0].x;
						P2[0].y=MPQ[0].y;

						EixoXo(::OX(MPQ[0].x),::OY(MPQ[0].y),0,&Angulo,&Modulo_Line);

						Objetos_Atr[0].Quadrilatero->SetMAlpha(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetMVetor(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetMZo(Zo,Contador);
						Contar++;
						::Move=true;

					break;

				case 1  :

						EixoXo(::OX(MPQ[1].x),::OY(MPQ[1].y),0,&Angulo,&Modulo_Line);
						Objetos_Atr[0].Quadrilatero->SetLAlpha(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetLVetor(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetLZo(Zo,Contador);






						Contar++;







				break;

				case 2  :

						MP[0].x=P1.x;
						MP[0].y=P1.y;

						Libera_Draw=true;
						Tempo_Draw->Enabled=true;


						EixoXo(::OX(MPQ[2].x),::OY(MPQ[2].y),0,&Angulo,&Modulo_Line);

						Objetos_Atr[0].Quadrilatero->SetMAlpha2(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetMVetor2(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetMZo2(Zo,Contador);
						Contar++;



					break;

				case 3  :

						EixoXo(::OX(MPQ[3].x),::OY(MPQ[3].y),0,&Angulo,&Modulo_Line);
						Objetos_Atr[0].Quadrilatero->SetLAlpha2(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetLVetor2(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetLZo2(Zo,Contador);
						Contador++;
						Objetos_Atr[0].Quadrilatero->SetContadorQuadrilatero(Contador);
						LevaQuadParaVisao(Contador-1);		// o quadrilatero novo vai para o plano da visao girada

						EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

						EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);

						::Move=false;

						Contar=0;
                        Libera_Draw=false;
                        Tempo_Draw->Enabled=false;

					break;

				default :

						Contar=0;


			}


		}

		if(RBVisaoXoZ->Checked){

			switch(Contar){

				case 0 :
						Libera_Draw=true;
						Tempo_Draw->Enabled=true;
						MPQ[0].x=P1.x;
						MPQ[0].y=P1.y;
						P2[0].x=MPQ[0].x;
						P2[0].y=MPQ[0].y;
						Libera_Draw=true;
						Tempo_Draw->Enabled=true;
						EixoXo(::OX(MPQ[0].x),Yo,0,&Angulo,&Modulo_Line);

						Objetos_Atr[0].Quadrilatero->SetMAlpha(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetMVetor(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetMZo(::OY(MPQ[0].y),Contador);
						Objetos_Atr[0].Quadrilatero->SetMYo(Yo,Contador);

						Contar++;
						::Move=true;

					break;

				case 1  :
						EixoXo(::OX(MPQ[1].x),Yo,0,&Angulo,&Modulo_Line);
						Objetos_Atr[0].Quadrilatero->SetLAlpha(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetLVetor(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetLZo(::OY(MPQ[1].y),Contador);
						Objetos_Atr[0].Quadrilatero->SetLYo(Yo,Contador);






						Contar++;







				break;

				case 2  :
						MP[0].x=P1.x;
						MP[0].y=P1.y;
						Libera_Draw=true;
						Tempo_Draw->Enabled=true;


						EixoXo(::OX(MPQ[2].x),Yo,0,&Angulo,&Modulo_Line);

						Objetos_Atr[0].Quadrilatero->SetMAlpha2(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetMVetor2(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetMZo2(::OY(MPQ[2].y),Contador);
						Objetos_Atr[0].Quadrilatero->SetMYo2(Yo,Contador);
						Contar++;



					break;

				case 3  :
						EixoXo(::OX(MPQ[3].x),Yo,0,&Angulo,&Modulo_Line);

						Objetos_Atr[0].Quadrilatero->SetLAlpha2(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetLVetor2(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetLZo2(::OY(MPQ[3].y),Contador);
						Objetos_Atr[0].Quadrilatero->SetLYo2(Yo,Contador);
						Contador++;
						Objetos_Atr[0].Quadrilatero->SetContadorQuadrilatero(Contador);
						LevaQuadParaVisao(Contador-1);		// o quadrilatero novo vai para o plano da visao girada

						EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

						EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);
						::Move=false;
						Libera_Draw=false;
						Tempo_Draw->Enabled=false;
						Contar=0;


					break;

				default :

						Contar=0;


			}
		}

		if(RBVisaoYoZ->Checked){

			switch(Contar){

				case 0 :
						Libera_Draw=true;
						Tempo_Draw->Enabled=true;
						MPQ[0].x=P1.x;
						MPQ[0].y=P1.y;
						P2[0].x=MPQ[0].x;
						P2[0].y=MPQ[0].y;
						Libera_Draw=true;
						Tempo_Draw->Enabled=true;
						EixoXo(Xo,::OX(MPQ[0].x),0,&Angulo,&Modulo_Line);

						Objetos_Atr[0].Quadrilatero->SetMAlpha(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetMVetor(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetMZo(::OY(MPQ[0].y),Contador);
						Objetos_Atr[0].Quadrilatero->SetMXo(Xo,Contador);

						Contar++;
						::Move=true;

					break;

				case 1  :
						EixoXo(Xo,::OX(MPQ[1].x),0,&Angulo,&Modulo_Line);
						Objetos_Atr[0].Quadrilatero->SetLAlpha(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetLVetor(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetLZo(::OY(MPQ[1].y),Contador);
						Objetos_Atr[0].Quadrilatero->SetLXo(Xo,Contador);





						Contar++;







				break;

				case 2  :
						MP[0].x=P1.x;
						MP[0].y=P1.y;
						Libera_Draw=true;
						Tempo_Draw->Enabled=true;


						EixoXo(Xo,::OX(MPQ[2].x),0,&Angulo,&Modulo_Line);

						Objetos_Atr[0].Quadrilatero->SetMAlpha2(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetMVetor2(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetMZo2(::OY(MPQ[2].y),Contador);
						Objetos_Atr[0].Quadrilatero->SetMXo2(Xo,Contador);
						Contar++;



					break;

				case 3  :
						EixoXo(Xo,::OX(MPQ[3].x),0,&Angulo,&Modulo_Line);

						Objetos_Atr[0].Quadrilatero->SetLAlpha2(Angulo,Contador);
						Objetos_Atr[0].Quadrilatero->SetLVetor2(Modulo_Line,Contador);
						Objetos_Atr[0].Quadrilatero->SetLZo2(::OY(MPQ[3].y),Contador);
                        Objetos_Atr[0].Quadrilatero->SetLXo2(Xo,Contador);
						Contador++;
						Objetos_Atr[0].Quadrilatero->SetContadorQuadrilatero(Contador);
						LevaQuadParaVisao(Contador-1);		// o quadrilatero novo vai para o plano da visao girada

						EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

						EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);

						::Move=false;
                        Libera_Draw=false;
                        Tempo_Draw->Enabled=false;
						Contar=0;


					break;

				default :

						Contar=0;


			}
		}




	}



}
//---------------------------------------------------------------------------



void __fastcall TEixoXY::Tempo_DrawTimer(TObject *Sender)
{

	if(Libera_Draw){

		Objetos_Atr->Draw(Objetos_Atr);

	}else
	 Lancamento();




}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::ButtonEixo_ZoClick(TObject *Sender)
{

	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
	LimpaQuadrilatero(EixoXY->Canvas,MP2,Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero());

	// ABT[2] e o angulo em volta do eixo Zo (Teta nas formulas do Draw_Lines)
	ABT[2]+=M_PI/80;
	Objetos_Atr[0].SetAlpha_B_T(ABT);

	// recalcula os pontos de tela com o angulo novo (essas funcoes so calculam)...
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

	// ...e pinta linhas, elipses e quadrilateros de uma vez
	Objetos_Atr->Draw(Objetos_Atr);


}
//---------------------------------------------------------------------------
void GiraZo(){

	Limpa_Lines(EixoXY->Canvas,EixoXY->FullMLK,EixoXY->Contador_Line);
	EixoXY->Limpa_Elipses(EixoXY->Canvas,EixoXY->P_Circle,EixoXY->Contador_Circle);

	EixoXY->ABT[2]+=M_PI/80;
	EixoXY->Objetos_Atr[0].SetAlpha_B_T(EixoXY->ABT);

	EixoXY->MP1=EixoXY->Draw_Lines(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->FullMLK);
	EixoXY->Draw_Lines_Direct(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->FullMLK);
	EixoXY->P_Circle=EixoXY->Draw_Elipse(EixoXY->Canvas,EixoXY->Objetos_Atr);
	EixoXY->Draw_ElipseDirect(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->P_Circle);

	EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->FullMLK2,EixoXY->FullMLK3);
	EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->MP2);
}

void __fastcall TEixoXY::Tempo_EixoTimer(TObject *Sender)
{


		Eixos(EixoXY->Canvas,Eixo_Kx,Eixo_Ky);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::LineClick(TObject *Sender)
{
    Line->Checked=true;
	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	memset(ABT,0,3*sizeof(float));
	Objetos_Atr[0].Lines_Atr->SetAlpha_B_T_Atr(ABT);
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::RBVisaoXoZClick(TObject *Sender)
{

	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
	LimpaQuadrilatero(EixoXY->Canvas,EixoXY->MP2,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );


	EdtXo->Enabled=false;
	EdtZo->Enabled=false;
	EdtYo->Enabled=true;
	EdtXo->Text="0,0";
	EdtYo->Text="0,0";
	EdtZo->Text="0,0";
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
	EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
	EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);
}
//---------------------------------------------------------------------------
void __fastcall TEixoXY::RBVisaoXoYClick(TObject *Sender)
{

	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
	LimpaQuadrilatero(EixoXY->Canvas,EixoXY->MP2,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );


	EdtXo->Enabled=false;
	EdtZo->Enabled=false;
	EdtYo->Enabled=true;
	EdtXo->Text="0,0";
	EdtYo->Text="0,0";
	EdtZo->Text="0,0";
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
	EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
	EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::BEixoYoClick(TObject *Sender)
{


	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
	LimpaQuadrilatero(EixoXY->Canvas,MP2,Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero());
	ABT[1]+=M_PI/80;
	Objetos_Atr[0].SetAlpha_B_T(ABT);
    	// recalcula os pontos de tela com o angulo novo (essas funcoes so calculam)...
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

	// ...e pinta linhas, elipses e quadrilateros de uma vez
	Objetos_Atr->Draw(Objetos_Atr);

}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::BEixoXoClick(TObject *Sender)
{

	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
    LimpaQuadrilatero(EixoXY->Canvas,MP2,Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero());

	ABT[0]+=M_PI/80;
	Objetos_Atr[0].SetAlpha_B_T(ABT);
		// recalcula os pontos de tela com o angulo novo (essas funcoes so calculam)...
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

	// ...e pinta linhas, elipses e quadrilateros de uma vez
	Objetos_Atr->Draw(Objetos_Atr);

}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::RBVisaoYoZClick(TObject *Sender)
{
	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
    LimpaQuadrilatero(EixoXY->Canvas,EixoXY->MP2,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );


	EdtYo->Enabled=false;
	EdtZo->Enabled=false;
	EdtXo->Enabled=true;
    EdtXo->Text="0,0";
	EdtYo->Text="0,0";
	EdtZo->Text="0,0";
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
	EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
	EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::BAumentarClick(TObject *Sender)
{
	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
	LimpaQuadrilatero(EixoXY->Canvas,EixoXY->MP2,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );

	int ContadorQuadrilatero=EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero();

	for (int i = 0; i < Contador_Line; i++) {
		Objetos_Atr[0].Lines_Atr->SetMVetor_Atr(Objetos_Atr[0].Lines_Atr->GetMVetor_Atr(i)*1.1,i);
		Objetos_Atr[0].Lines_Atr->SetLVetor_Atr(Objetos_Atr[0].Lines_Atr->GetLVetor_Atr(i)*1.1,i);
		Objetos_Atr[0].Lines_Atr->SetMZo_Atr(Objetos_Atr[0].Lines_Atr->GetMZo_Atr(i)*1.1,i);
		Objetos_Atr[0].Lines_Atr->SetLZo_Atr(Objetos_Atr[0].Lines_Atr->GetLZo_Atr(i)*1.1,i);

	}

	for(int i=0;i<Contador_Circle;i++){
		Objetos_Atr[0].Elipse_Atr->SetA_Atr(Objetos_Atr[0].Elipse_Atr->GetA_Atr(i)*1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetB_Atr(Objetos_Atr[0].Elipse_Atr->GetB_Atr(i)*1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetAZo_Atr(Objetos_Atr[0].Elipse_Atr->GetAZo_Atr(i)*1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetBZo_Atr(Objetos_Atr[0].Elipse_Atr->GetBZo_Atr(i)*1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetCX(Objetos_Atr[0].Elipse_Atr->GetCX(i)*1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetCY(Objetos_Atr[0].Elipse_Atr->GetCY(i)*1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetCZ(Objetos_Atr[0].Elipse_Atr->GetCZ(i)*1.1,i);




	}

    for(int i=0;i<ContadorQuadrilatero;i++){

		Objetos_Atr[0].Quadrilatero->SetMVetor(Objetos_Atr[0].Quadrilatero->GetMVetor(i)*1.1,i);
		Objetos_Atr[0].Quadrilatero->SetMVetor2(Objetos_Atr[0].Quadrilatero->GetMVetor2(i)*1.1,i);
		Objetos_Atr[0].Quadrilatero->SetLVetor(Objetos_Atr[0].Quadrilatero->GetLVetor(i)*1.1,i);
		Objetos_Atr[0].Quadrilatero->SetLVetor2(Objetos_Atr[0].Quadrilatero->GetLVetor2(i)*1.1,i);
		Objetos_Atr[0].Quadrilatero->SetMZo(Objetos_Atr[0].Quadrilatero->GetMZo(i)*1.1,i);
		Objetos_Atr[0].Quadrilatero->SetLZo(Objetos_Atr[0].Quadrilatero->GetLZo(i)*1.1,i);
		Objetos_Atr[0].Quadrilatero->SetMZo2(Objetos_Atr[0].Quadrilatero->GetMZo2(i)*1.1,i);
		Objetos_Atr[0].Quadrilatero->SetLZo2(Objetos_Atr[0].Quadrilatero->GetLZo2(i)*1.1,i);

	}


	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
    EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
	EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);


}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::BDiminuirClick(TObject *Sender)
{


	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
	LimpaQuadrilatero(EixoXY->Canvas,EixoXY->MP2,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );

	int ContadorQuadrilatero=EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero();
	for (int i = 0; i < Contador_Line; i++) {

		Objetos_Atr[0].Lines_Atr->SetMVetor_Atr(Objetos_Atr[0].Lines_Atr->GetMVetor_Atr(i)/1.1,i);
		Objetos_Atr[0].Lines_Atr->SetLVetor_Atr(Objetos_Atr[0].Lines_Atr->GetLVetor_Atr(i)/1.1,i);
		Objetos_Atr[0].Lines_Atr->SetMZo_Atr(Objetos_Atr[0].Lines_Atr->GetMZo_Atr(i)/1.1,i);
		Objetos_Atr[0].Lines_Atr->SetLZo_Atr(Objetos_Atr[0].Lines_Atr->GetLZo_Atr(i)/1.1,i);

	}

    	for(int i=0;i<Contador_Circle;i++){
		Objetos_Atr[0].Elipse_Atr->SetA_Atr(Objetos_Atr[0].Elipse_Atr->GetA_Atr(i)/1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetB_Atr(Objetos_Atr[0].Elipse_Atr->GetB_Atr(i)/1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetAZo_Atr(Objetos_Atr[0].Elipse_Atr->GetAZo_Atr(i)/1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetBZo_Atr(Objetos_Atr[0].Elipse_Atr->GetBZo_Atr(i)/1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetCX(Objetos_Atr[0].Elipse_Atr->GetCX(i)/1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetCY(Objetos_Atr[0].Elipse_Atr->GetCY(i)/1.1,i);
		Objetos_Atr[0].Elipse_Atr->SetCZ(Objetos_Atr[0].Elipse_Atr->GetCZ(i)/1.1,i);




	}
	for(int i=0;i<ContadorQuadrilatero;i++){

		Objetos_Atr[0].Quadrilatero->SetMVetor(Objetos_Atr[0].Quadrilatero->GetMVetor(i)/1.1,i);
		Objetos_Atr[0].Quadrilatero->SetMVetor2(Objetos_Atr[0].Quadrilatero->GetMVetor2(i)/1.1,i);
		Objetos_Atr[0].Quadrilatero->SetLVetor(Objetos_Atr[0].Quadrilatero->GetLVetor(i)/1.1,i);
		Objetos_Atr[0].Quadrilatero->SetLVetor2(Objetos_Atr[0].Quadrilatero->GetLVetor2(i)/1.1,i);
		Objetos_Atr[0].Quadrilatero->SetMZo(Objetos_Atr[0].Quadrilatero->GetMZo(i)/1.1,i);
		Objetos_Atr[0].Quadrilatero->SetLZo(Objetos_Atr[0].Quadrilatero->GetLZo(i)/1.1,i);
		Objetos_Atr[0].Quadrilatero->SetMZo2(Objetos_Atr[0].Quadrilatero->GetMZo2(i)/1.1,i);
		Objetos_Atr[0].Quadrilatero->SetLZo2(Objetos_Atr[0].Quadrilatero->GetLZo2(i)/1.1,i);

	}


	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
	EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
	EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::ConstruirLinha1Click(TObject *Sender)
{
	ConstrucaoLines->Visible=true;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TEixoXY::PElipseClick(TObject *Sender)
{

	FElipse->Visible=true;

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------





void __fastcall TEixoXY::Limpa_DesenhoClick(TObject *Sender)
{
	Libera_Draw=false;
	Tempo_Draw->Enabled=false;
	Lancar=false;						// interrompe um lancamento em andamento
	Eixo_Lancar=true;

	// apaga os dados: linhas, elipses, quadrilateros, rotacao e cliques pela metade
	LimpaMemoria();

	// apaga a tela inteira: o formulario repinta o fundo (Color) e os paineis
	Repaint();
	Eixos(Canvas,Eixo_Kx,Eixo_Ky);
}
//---------------------------------------------------------------------------
void LimpaMemoria(){


	EixoXY->Objetos_Atr[0].Lines_Atr->SetContador_Line(0);
	EixoXY->Objetos_Atr[0].Elipse_Atr->SetContadorCircle_Atr(0);
    EixoXY->Objetos_Atr[0].Quadrilatero->SetContadorQuadrilatero(0);
	EixoXY->Contador_Line=0;
	EixoXY->Contador_Circle=0;
	Contador=0;							// contador global dos quadrilateros
	Contar=0;							// cliques de uma figura pela metade
	ContarElipse=0;
	::Move=false;


	for(int i=0;i<3;i++) {
		EixoXY->Objetos_Atr[0].SetAlpha_B_T(0,i);
		EixoXY->ABT[i]=0;

	}

	Kx=960;
	Ky=540;
    Libera_Draw=false;
    EixoXY->Tempo_Draw->Enabled=false;
	Duplicacao.EsquecePlanos();					// desenho novo: nenhuma figura tem plano guardado
	EixoXY->Faz=true;


}
void __fastcall TEixoXY::Volta_LinhaClick(TObject *Sender)
{
 int i=EixoXY->Objetos_Atr[0].Lines_Atr->GetContador_Line();
 if(i>0){
	EixoXY->Objetos_Atr[0].Lines_Atr->SetContador_Line(i-1);
    Contador_Line--;
	Duplicacao.EsqueceLinhasApagadas(i-1);	// a linha apagada nao e mais copiada
	Limpa_VoltaLinha(EixoXY->Canvas,EixoXY->MP1);
 }
}
//---------------------------------------------------------------------------
void Limpa_VoltaLinha(TCanvas *Plano,TPoint **P1){

int i=EixoXY->Objetos_Atr[0].Lines_Atr->GetContador_Line();
	Plano->Pen->Color=clWhite;
	Plano->Polyline(P1[i],1);

}
void __fastcall TEixoXY::BtnLancarClick(TObject *Sender)
{
// so guarda a partida se nao houver um lancamento em andamento
if(!Lancar){
	Kx_Partida=Kx;
	Ky_Partida=Ky;
	Objetos_Atr[0].GetAlpha_B_T(ABT_Partida);
}

Lancar=true;
t=0;
Eixo_Lancar=false;

// o Lancamento() roda pelo else do Tempo_DrawTimer
Libera_Draw=false;
Tempo_Draw->Enabled=true;



}
//---------------------------------------------------------------------------

long double Converte_Angulo(long double Angulo){


return (Angulo/180)*M_PI;

}

void __fastcall TEixoXY::FormClose(TObject *Sender, TCloseAction &Action)
{
Libera_Draw=false;
Tempo_Draw->Enabled=false;
}
//---------------------------------------------------------------------------

void TEixoXY::Lancamento(){
Libera_Draw=false;

long double Vo;

int i;
long double Alpha_Beta_Teta[3];

	for(int i=0;i<3;i++)
		Alpha_Beta_Teta[i]=EixoXY->Objetos_Atr[0].GetAlpha_B_T(i);
long double Angulo;
	if(Lancar==true){
		if((!EdtVelocidadeInicial->Text.IsEmpty())&&(!EdtAngulo->Text.IsEmpty())){
				  Angulo=Converte_Angulo(EdtAngulo->Text.ToDouble());
				  Vo=EdtVelocidadeInicial->Text.ToDouble();
				if(Lancar==true){
				   EixoXY->Lancar=true;
					if(t==0){
						Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
						Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
						Kx=400;
						Ky=700;



					}

					if(t<30) {


					   Kx=400+Vo *Cos(Angulo )*t;
					   Ky=700-(Vo*Sin(Angulo)*t -(10*t*t)/2.0);
						Alpha_Beta_Teta[0]+=M_PI/150;
						Alpha_Beta_Teta[1]+=M_PI/150;
						Alpha_Beta_Teta[2]+=M_PI/150;
						for(int i=0;i<3;i++){
							EixoXY->Objetos_Atr[0].SetAlpha_B_T(Alpha_Beta_Teta[i],i);

						}

						EixoXY->Kx1=Kx;
						EixoXY->Ky1=Ky;




                        Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
						Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
						LimpaQuadrilatero(EixoXY->Canvas,EixoXY->MP2,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );
						MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);


						Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
						P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);

						Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
						EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
						EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);


					   t+=0.030;

					}else{

						Lancar=false;
						Eixo_Lancar=true;
						Tempo_Draw->Enabled=false;	// para o timer antes da mensagem

						// volta o desenho para o local e os angulos de antes do lancamento
						Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
						Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
						LimpaQuadrilatero(EixoXY->Canvas,EixoXY->MP2,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );

						Kx=Kx_Partida;
						Ky=Ky_Partida;
						EixoXY->Kx1=Kx;
						EixoXY->Ky1=Ky;
						EixoXY->Objetos_Atr[0].SetAlpha_B_T(ABT_Partida);

						MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
						P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
						EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
						Objetos_Atr->Draw(Objetos_Atr);

						ShowMessage("Fim do Lançamento...!");



						}
				   Eixo_Lancar=false;
				}
		} else{
				 Lancar=false;
				 Tempo_Draw->Enabled=false;
				 ShowMessage("Preencha os Campos velocidade e Angulo" );
				}
	}





}


void __fastcall TEixoXY::FormCreate(TObject *Sender)
{

Libera_Draw=false;
Tempo_Draw->Enabled=false;
Faz=true;


}
//---------------------------------------------------------------------------

// Apaga os quadrilateros redesenhando o contorno na cor do fundo do formulario.
// MLP_Quadrilatero e o TPoint** do Draw_Quadrilatero (5 pontos por quadrilatero);
// todos vao numa unica chamada PolyPolyline, ja que a cor e a mesma.
void TEixoXY::LimpaQuadrilatero(TCanvas *Plano,TPoint **MLP_Quadrilatero,int ContadorQuadrilatero){

static std::vector<TPoint> Pontos;		// static: reaproveita a memoria entre as chamadas
static std::vector<DWORD> Tamanhos;

	if((MLP_Quadrilatero==NULL)||(ContadorQuadrilatero<=0)||(ContadorQuadrilatero>BufMP2.GetLinhas()))
		return;

	Pontos.clear();
	Tamanhos.clear();

	for(int i=0;i<ContadorQuadrilatero;i++){
		Pontos.insert(Pontos.end(),MLP_Quadrilatero[i],MLP_Quadrilatero[i]+5);
		Tamanhos.push_back(5);
	}

	Plano->Pen->Color=Color;			// cor do fundo do EixoXY (clWhite)
	PolyPolyline(Plano->Handle,(POINT*)Pontos.data(),Tamanhos.data(),(DWORD)Tamanhos.size());

}
void __fastcall TEixoXY::Button1Click(TObject *Sender)
{
    Form1->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::Button2Click(TObject *Sender)
{
	if(Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero()>0){
			EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
			EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);

		}
}
//---------------------------------------------------------------------------


void __fastcall TEixoXY::BtnDupTetaClick(TObject *Sender)
{
	DuplicaValoresTeta(Objetos_Atr,ABT);
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	MP2=Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

    Objetos_Atr->Draw(Objetos_Atr);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::BtnDupAlphaClick(TObject *Sender)
{
	DuplicaValoresAlpha(Objetos_Atr);
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	MP2=Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

	Objetos_Atr->Draw(Objetos_Atr);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::Salvar1Click(TObject *Sender)
{
 UnicodeString Nome_Arquivo,Resposta;
   Arquivo Arqu;
   char  N_Arquivo[500];

	if(DlgSalvar->Execute()){

		Nome_Arquivo=DlgSalvar->FileName;
		if(Nome_Arquivo.Length()>=(int)sizeof(N_Arquivo))
			Nome_Arquivo.SetLength(sizeof(N_Arquivo)-1);

		for(int i=0;i<Nome_Arquivo.Length();i++){
			N_Arquivo[i]=(char)Nome_Arquivo[i+1];

		}

		N_Arquivo[ Nome_Arquivo.Length()]='\0';

	}else
		return;						// cancelado: nao grava

   if(0)
   Grava_Arquivo(Objetos_Atr,N_Arquivo);

   Arqu.Salvar_Arquivo(N_Arquivo,Objetos_Atr,&Resposta);
   ShowMessage(Resposta);

}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::Abrir1Click(TObject *Sender)
{

AnsiString Nome_Arquivo;
Arquivo Arqui;
Objetos *Tmp_Objetos;
int Tmp_Contador_Line;
UnicodeString Resposta;


	Libera_Draw=false;
	Tempo_Draw->Enabled=false;

	Cancela_Line->Checked=true;



	if(!DlgAbrir->Execute())
		return;

	Nome_Arquivo=DlgAbrir->FileName;

		Limpa_Lines(EixoXY->Canvas,FullMLK,EixoXY->Contador_Line);
		Limpa_Elipses(EixoXY->Canvas,P_Circle,EixoXY->Contador_Circle);
		LimpaMemoria();


		Tmp_Objetos=Arqui.Abrir_Arquivo(Nome_Arquivo.c_str(),&Resposta);
		if(Tmp_Objetos!=NULL){
			delete[] Objetos_Atr;		// libera o desenho anterior
			Objetos_Atr=Tmp_Objetos;

			// os buffers de tela do EixoXY acompanham o desenho aberto
			const int Linhas=Objetos_Atr[0].Lines_Atr->GetContador_Line();
			const int Quads=Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero();
			const int Elipses=Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr();
			if(Linhas>0)
				FullMLK=BufFullMLK.Redimensiona(Linhas,4);
			if(Quads>0){
				FullMLK2=BufFullMLK2.Redimensiona(Quads,4);
				FullMLK3=BufFullMLK3.Redimensiona(Quads,4);
			}
			if(Elipses>0)
				P_Circle=BufP_Circle.Redimensiona(Elipses,51);
		}
		ShowMessage(Resposta);
		if(0)
		if(Objetos_Atr!=NULL){
			for(int i=0;i<3;i++)
				ABT[i]=Objetos_Atr[0].GetAlpha_B_T(i);
		}
		if(Objetos_Atr!=NULL){
			Objetos_Atr[0].GetAlpha_B_T(ABT);
			EixoXY->Contador_Circle=Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr();
			Contador=Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero();	// contador global dos quadrilateros
			MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
			Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
			P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
			Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
			EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
			EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);

        }





}
//---------------------------------------------------------------------------



void __fastcall TEixoXY::Cancela_LineClick(TObject *Sender)
{
	Cancela_Line->Checked=true;
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::RBQuadrilateroClick(TObject *Sender)
{
    RBQuadrilatero->Checked=true;
}
//---------------------------------------------------------------------------





void __fastcall TEixoXY::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(Key==VK_UP)
		ShowMessage("UP");
	if(Key==VK_LEFT)
		ShowMessage("LEFT");
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Menu Export: glTF 2.0 binario (.glb) para three.js, Blender etc.
//---------------------------------------------------------------------------

void __fastcall TEixoXY::ExportarGLB1Click(TObject *Sender)
{
UnicodeString Resposta;
ExportGLB Exportador;
TSaveDialog *DlgGLB=new TSaveDialog(this);

	DlgGLB->Title="Exportar para glTF (.glb)";
	DlgGLB->Filter="glTF Binario (*.glb)|*.glb";
	DlgGLB->DefaultExt="glb";
	DlgGLB->Options=DlgGLB->Options<<ofOverwritePrompt;

	if(DlgGLB->Execute()){
		Exportador.Exportar(DlgGLB->FileName,Objetos_Atr,&Resposta);
		ShowMessage(Resposta);
	}

	delete DlgGLB;
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::ImportarGLB1Click(TObject *Sender)
{
UnicodeString Resposta;
ExportGLB Importador;
Objetos *Tmp_Objetos=NULL;
TOpenDialog *DlgGLB=new TOpenDialog(this);

	DlgGLB->Title="Importar glTF (.glb)";
	DlgGLB->Filter="glTF Binario (*.glb)|*.glb";
	DlgGLB->DefaultExt="glb";
	DlgGLB->Options=DlgGLB->Options<<ofFileMustExist;

	if(DlgGLB->Execute())
		Tmp_Objetos=Importador.Importar(DlgGLB->FileName,&Resposta);

	delete DlgGLB;

	if(Resposta.IsEmpty())
		return;

	if(Tmp_Objetos!=NULL){

		Libera_Draw=false;
		Tempo_Draw->Enabled=false;
		Cancela_Line->Checked=true;

		Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
		Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
		LimpaQuadrilatero(EixoXY->Canvas,EixoXY->MP2,Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero());
		LimpaMemoria();

		delete[] Objetos_Atr;			// libera o desenho anterior
		Objetos_Atr=Tmp_Objetos;
		Objetos_Atr[0].GetAlpha_B_T(ABT);

		Contador_Line=Objetos_Atr[0].Lines_Atr->GetContador_Line();
		Contador_Circle=Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr();
		Contador=Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero();	// contador global dos quadrilateros

		FullMLK=BufFullMLK.Redimensiona(Contador_Line+1,4);

		MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
		Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
		P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
		Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
		EixoXY->MP2=EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
		EixoXY->DrawQuadrilineDirect(EixoXY->Canvas,Objetos_Atr,EixoXY->MP2);
	}

	ShowMessage(Resposta);
}
//---------------------------------------------------------------------------

// Abre o DrawOpenGL: desenho proprio em OpenGL com as mesmas classes de desenho
void __fastcall TEixoXY::DrawOpenGL1Click(TObject *Sender)
{
	DrawOpenGL->Show();
	DrawOpenGL->BringToFront();
}
//---------------------------------------------------------------------------
