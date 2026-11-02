//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitEixoXY.h"
#include "Arquivo.h"
#include <fstream>
#include <string.h>
#include "DrawCursor.h"
#include "Render.h"
#include <vector>
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
int Contador_LineTeta;
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



long double **Mxyz1=0;
long double **Lxyz1=0;
//Prototipacao Funcoes Globais-----------------------------------------------

long double **Realocar_Ponteiro_Duplo(long double **,int );
TPoint **Realocar_P1(TPoint **,int);
int **Realocar_Ponteiro_Duplo_Inteiro(int **,int );
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
	Mxyz1=0;
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
TPoint **Realocar_P1(TPoint **P1,int Count){


	P1=(TPoint **)realloc(P1,Count*sizeof(TPoint*));
	for(int i=0;i<2;i++){
		P1[i]=0;
		P1[i]=(TPoint *)realloc(P1[i],2*sizeof(TPoint));
	}



	return  P1;
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


long double *TEixoXY::Realocar(long double *Ponteiro,int Tamanho){
long double *Tmp_Ponteiro=0;

	Tmp_Ponteiro=Ponteiro;
	Tmp_Ponteiro=(long double *)realloc(Tmp_Ponteiro,Tamanho*sizeof(long double));

	Ponteiro=Tmp_Ponteiro;
	return Ponteiro;

}

int *TEixoXY::Realocar(int *Ponteiro,int Tamanho){
int *Tmp_Ponteiro=0;

	Tmp_Ponteiro=Ponteiro;
	Tmp_Ponteiro=(int *)realloc(Tmp_Ponteiro,Tamanho*sizeof(int));

	Ponteiro=Tmp_Ponteiro;
	return Ponteiro;

}

unsigned int *TEixoXY::Realocar(unsigned int *Ponteiro,int Tamanho){
unsigned int *Tmp_Ponteiro=0;

	Tmp_Ponteiro=Ponteiro;
	Tmp_Ponteiro=(unsigned int *)realloc(Tmp_Ponteiro,Tamanho*sizeof(unsigned int));

	Ponteiro=Tmp_Ponteiro;
	return Ponteiro;

}

unsigned long  *TEixoXY::Realocar(unsigned long *Ponteiro,int Tamanho){
unsigned long *Tmp_Ponteiro=0;

	Tmp_Ponteiro=Ponteiro;
	Tmp_Ponteiro=(unsigned long *)realloc(Tmp_Ponteiro,Tamanho*sizeof(unsigned long));

	Ponteiro=Tmp_Ponteiro;

	return Ponteiro;

}
TPoint **TEixoXY::Realocar_Ponteiro_DuploC(TPoint **Ponteiro,int Tamanho){



	   Ponteiro=0;
	   Ponteiro=(TPoint **)realloc(Ponteiro,Tamanho*sizeof(TPoint*));
	   for(int i=0;i<Tamanho;i++){
			Ponteiro[i]=0;
			Ponteiro[i]=(TPoint*)realloc(Ponteiro[i],51*sizeof(TPoint));

	   }

	return Ponteiro;


}

TPoint **TEixoXY::Realocar_Ponteiro_DuploL(TPoint **Ponteiro,int Tamanho){



	   Ponteiro=0;
	   Ponteiro=(TPoint **)realloc(Ponteiro,Tamanho*sizeof(TPoint*));
	   for(int i=0;i<Tamanho;i++){
			Ponteiro[i]=0;
			Ponteiro[i]=(TPoint*)realloc(Ponteiro[i],2*sizeof(TPoint));

	   }

	return Ponteiro;


}

TPoint **TEixoXY::Realocar_Ponteiro_DuploL2(TPoint **Ponteiro,int Tamanho){



	   Ponteiro=0;
	   Ponteiro=(TPoint **)realloc(Ponteiro,Tamanho*sizeof(TPoint*));
	   for(int i=0;i<Tamanho;i++){
			Ponteiro[i]=0;
			Ponteiro[i]=(TPoint*)realloc(Ponteiro[i],5*sizeof(TPoint));

	   }

	return Ponteiro;


}

void Rotacao_Vetores_3D_Circle_MP2(long double xyz[3],long double xe,long double ye, long double  ze, long double Alpha_Beta_Teta[3],int IndexX_Circle)
{

	long double *AnguloX=EixoXY->Objetos_Atr[0].Elipse_Atr->GetAnguloX();
	long double *AnguloY=EixoXY->Objetos_Atr[0].Elipse_Atr->GetAnguloY();
	long double *AnguloZ=EixoXY->Objetos_Atr[0].Elipse_Atr->GetAnguloZ();

	int IndexX=IndexX_Circle;

	if(EixoXY->RBVisaoYoZ->Checked){
		//Yo
		xyz[0]=xe*(Sin(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Sin(Alpha_Beta_Teta[1])*Cos(Alpha_Beta_Teta[2]+AnguloX[IndexX])+
			Cos(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Sin(Alpha_Beta_Teta[2]+AnguloX[IndexX]))+ye*(-Sin(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Sin(Alpha_Beta_Teta[1])*Sin(Alpha_Beta_Teta[2]+AnguloX[IndexX])+
			Cos(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Cos(Alpha_Beta_Teta[2]+AnguloX[IndexX]))+ze*(-Sin(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Cos(Alpha_Beta_Teta[1]));


		//Zo
		xyz[1]=xe*(-Cos(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Sin(Alpha_Beta_Teta[1])*Cos(Alpha_Beta_Teta[2]+AnguloX[IndexX])+
			Sin(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Sin(Alpha_Beta_Teta[2]+AnguloX[IndexX]))+ye*(Cos(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Sin(Alpha_Beta_Teta[1])*Sin(Alpha_Beta_Teta[2]+AnguloX[IndexX])+
			Sin(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Cos(Alpha_Beta_Teta[2]+AnguloX[IndexX]))+ze*(Cos(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Cos(Alpha_Beta_Teta[1]));

	}





	if(EixoXY->RBVisaoXoY->Checked){
		//Xo
		xyz[0]=xe*(Cos(Alpha_Beta_Teta[1])*Cos(Alpha_Beta_Teta[2]+AnguloX[IndexX])) -
			( ye*Cos( Alpha_Beta_Teta[1])*Sin(Alpha_Beta_Teta[2]+AnguloX[IndexX]))+ze*Sin(Alpha_Beta_Teta[1]);

		//Yo
		xyz[1]=xe*(Sin(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Sin(Alpha_Beta_Teta[1])*Cos(Alpha_Beta_Teta[2]+AnguloX[IndexX])+
			Cos(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Sin(Alpha_Beta_Teta[2]+AnguloY[IndexX]+AnguloX[IndexX]))+ye*(-Sin(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[1])*Sin(Alpha_Beta_Teta[2]+AnguloX[IndexX])+
			Cos(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Cos(Alpha_Beta_Teta[2]+AnguloX[IndexX]))+ze*(-Sin(Alpha_Beta_Teta[0]+AnguloY[IndexX])*Cos(Alpha_Beta_Teta[1]));


	}




	if(EixoXY->RBVisaoXoZ->Checked){
		//Xo
		xyz[0]=xe*(Cos(Alpha_Beta_Teta[1]+AnguloZ[IndexX])*Cos(Alpha_Beta_Teta[2])) -
		( ye*Cos( Alpha_Beta_Teta[1]+AnguloZ[IndexX])*Sin(Alpha_Beta_Teta[2]))+ze*Sin(Alpha_Beta_Teta[1]+AnguloZ[IndexX]);
		//Zo
		xyz[1]=xe*(-Cos(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[1]+AnguloZ[IndexX])*Cos(Alpha_Beta_Teta[2])+
		Sin(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[2]))+ye*(Cos(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[1]+AnguloZ[IndexX])*Sin(Alpha_Beta_Teta[2])+
		Sin(Alpha_Beta_Teta[0])*Cos(Alpha_Beta_Teta[2]))+ze*(Cos(Alpha_Beta_Teta[0])*Cos(Alpha_Beta_Teta[1]+AnguloZ[IndexX]));


	}




}

void TEixoXY::Draw_ElipseDirect(TCanvas *Plano,Objetos *Objetos_Atr,TPoint **P_Circle){

	for(int i=0;i<Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr();i++){

		Plano->Pen->Color=(TColor)Objetos_Atr[0].Elipse_Atr->GetColor_Circle_Atr(i);
		Plano->Polyline(P_Circle[i],50);

	}

}
TPoint  **TEixoXY::Draw_Elipse(TCanvas *Plano,Objetos *Objetos_Atr){


TPoint **TmpP_Circle;
long double sx1;
long double sy1;
long double sz1;
long double Circle_XYZ[3];

		TmpP_Circle=EixoXY->Realocar_Ponteiro_DuploC(P_Circle,Contador_Circle+15);

	int *TipoElipse=Objetos_Atr[0].Elipse_Atr->GetTipoElipse_Atr();



	for(int j=0;j<Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr();j++){
		float A=Objetos_Atr[0].Elipse_Atr->GetA_Atr(j);
		float B=Objetos_Atr[0].Elipse_Atr->GetB_Atr(j);
		float AZ=Objetos_Atr[0].Elipse_Atr->GetAZo_Atr(j);
		float BZ=Objetos_Atr[0].Elipse_Atr->GetBZo_Atr(j);
		IndexX_Circle=j;


		for (int i=0;i<51;i++){

			switch(TipoElipse[j]){

				case 0:

					sx1=A*Cos(i*((2*M_PI)/50)) + Objetos_Atr[0].Elipse_Atr->GetCX(j);
					sy1=B*Sin(i*((2*M_PI)/50)) + Objetos_Atr[0].Elipse_Atr->GetCY(j);
					sz1=AZ*Cos(i*((2*M_PI)/50))+BZ*Sin((i*((2*M_PI)/50)))+ Objetos_Atr[0].Elipse_Atr->GetCZ(j);

				break;

				case 1:


					sx1=A*Cos((i*((2*M_PI)/50)))+ Objetos_Atr[0].Elipse_Atr->GetCX(j);
					sz1=B*Sin(i*((2*M_PI)/50))+ Objetos_Atr[0].Elipse_Atr->GetCZ(j);
					sy1=AZ*Cos((i*((2*M_PI)/50)))+BZ*Sin((i*((2*M_PI)/50)))+ Objetos_Atr[0].Elipse_Atr->GetCY(j);


				break;

				case 2:

					sy1=A*Cos((i*((2*M_PI)/50)))+ Objetos_Atr[0].Elipse_Atr->GetCY(j);
					sz1=B*Sin(i*((2*M_PI)/50))+ Objetos_Atr[0].Elipse_Atr->GetCZ(j);
					sx1=AZ*Cos((i*((2*M_PI)/50)))+BZ*Sin((i*((2*M_PI)/50)))+ Objetos_Atr[0].Elipse_Atr->GetCX(j);

				break;
				default:

					sx1=0;
					sy1=0;
					sz1=0;
				break;

			}





			 Rotacao_Vetores_3D_Circle_MP2( Circle_XYZ,sx1,sy1,
			 sz1,EixoXY->Objetos_Atr[0].GetAlpha_B_T(),IndexX_Circle);




			TmpP_Circle[j][i].x=(int) X(RoundTo(Circle_XYZ[0],0));
			TmpP_Circle[j][i].y= (int)Y(RoundTo(Circle_XYZ[1],0));


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
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Lines_Atr->GetCor_Line()),Contador_Line*sizeof(unsigned int));

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
					Arquivo_WR.write(reinterpret_cast<const char *>(Objetos_Atr[0].Elipse_Atr->GetColor_Circle_Atr()),Contador_Circle*sizeof(unsigned int));
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


long double *Tmp_MAlpha=0;
long double *Tmp_LAlpha=0;
long double *Tmp_MVetor=0;
long double *Tmp_LVetor=0;
long double *Tmp_MZo=0;
long double *Tmp_LZo=0;
long double Tmp_ABT[3];
unsigned int *TmpCor_Line=0;

long double *Tmp_A_Atr=0;
long double *Tmp_B_Atr=0;
long double *Tmp_AZo_Atr=0;
long double *Tmp_BZo_Atr=0;
long double *Tmp_CX=0;
long double *Tmp_CY=0;
long double *Tmp_CZ=0;
long double *Tmp_AnguloX=0;
long double *Tmp_AnguloY=0;
long double *Tmp_AnguloZ=0;

int Tmp_IndexX=0;
unsigned int *Tmp_Color_Circle_Atr=0;
int *Tmp_TipoElipse_Atr=0;
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

							Tmp_MAlpha=EixoXY->Realocar(Tmp_MAlpha,Contador_Line);
							Tmp_LAlpha=EixoXY->Realocar(Tmp_LAlpha,Contador_Line);
							Tmp_MVetor=EixoXY->Realocar(Tmp_MVetor,Contador_Line);
							Tmp_LVetor=EixoXY->Realocar(Tmp_LVetor,Contador_Line);
							Tmp_MZo=EixoXY->Realocar(Tmp_MZo,Contador_Line);
							Tmp_LZo=EixoXY->Realocar(Tmp_LZo,Contador_Line);

							TmpCor_Line=EixoXY->Realocar(TmpCor_Line,Contador_Line);





							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_MAlpha),Contador_Line*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_LAlpha),Contador_Line*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_MVetor),Contador_Line*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_LVetor),Contador_Line*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_MZo),Contador_Line*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_LZo),Contador_Line*sizeof(long double));

							Arquivo_RR.read(reinterpret_cast<char *>(TmpCor_Line),Contador_Line*sizeof(unsigned int));



							EixoXY->Objetos_Atr[0].Lines_Atr->Alocando_Line(Contador_Line);
							FullMLK=Realocar_Ponteiro_Duplo_Inteiro(FullMLK,Contador_Line);

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

							Tmp_A_Atr=EixoXY->Realocar(Tmp_A_Atr,Contador_Circle);
							Tmp_B_Atr=EixoXY->Realocar(Tmp_B_Atr,Contador_Circle);
							Tmp_AZo_Atr=EixoXY->Realocar(Tmp_AZo_Atr,Contador_Circle);
							Tmp_BZo_Atr=EixoXY->Realocar(Tmp_BZo_Atr,Contador_Circle);
							Tmp_CX=EixoXY->Realocar(Tmp_CX,Contador_Circle);
							Tmp_CY=EixoXY->Realocar(Tmp_CY,Contador_Circle);
							Tmp_CZ=EixoXY->Realocar(Tmp_CZ,Contador_Circle);
							Tmp_Color_Circle_Atr=EixoXY->Realocar(Tmp_Color_Circle_Atr,Contador_Circle);
							Tmp_TipoElipse_Atr=EixoXY->Realocar(Tmp_TipoElipse_Atr,Contador_Circle);
							Tmp_AnguloX=EixoXY->Realocar(Tmp_AnguloX,Contador_Circle);
							Tmp_AnguloY=EixoXY->Realocar(Tmp_AnguloY,Contador_Circle);
							Tmp_AnguloZ=EixoXY->Realocar(Tmp_AnguloZ,Contador_Circle);



							EixoXY->P_Circle=EixoXY->Realocar_Ponteiro_DuploC(EixoXY->P_Circle,Contador_Circle);


							EixoXY->Objetos_Atr[0].Elipse_Atr->AlocaElipse_Atr(Contador_Circle);



							Arquivo_RR.read(reinterpret_cast<char *>(Tmp_A_Atr),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast< char *>(Tmp_B_Atr),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast< char *>(Tmp_AZo_Atr),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast< char *>(Tmp_BZo_Atr),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast< char *>(Tmp_CX),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast< char *>(Tmp_CY),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast< char *>(Tmp_CZ),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast< char *>(Tmp_TipoElipse_Atr),Contador_Circle*sizeof(int));
							Arquivo_RR.read(reinterpret_cast< char *>(Tmp_Color_Circle_Atr),Contador_Circle*sizeof(unsigned int));
							Arquivo_RR.read(reinterpret_cast< char *>(Tmp_AnguloX),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast< char *>(Tmp_AnguloY),Contador_Circle*sizeof(long double));
							Arquivo_RR.read(reinterpret_cast< char *>(Tmp_AnguloZ),Contador_Circle*sizeof(long double));



							EixoXY->Objetos_Atr[0].Elipse_Atr->SetA_Atr(Tmp_A_Atr);
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetB_Atr(Tmp_B_Atr);
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetAZo_Atr(Tmp_AZo_Atr);
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetBZo_Atr(Tmp_BZo_Atr);
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetCX(Tmp_CX);
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetCY(Tmp_CY);
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetCZ(Tmp_CZ);
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetTipoElipse_Atr(Tmp_TipoElipse_Atr);
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetColor_Circle_Atr(Tmp_Color_Circle_Atr);
							EixoXY->Objetos_Atr[0].Elipse_Atr->SetContadorCircle_Atr(Contador_Circle);
							EixoXY->Objetos_Atr[0].Elipse_Atr->Set_AnguloX(Tmp_AnguloX);
							EixoXY->Objetos_Atr[0].Elipse_Atr->Set_AnguloY(Tmp_AnguloY);
							EixoXY->Objetos_Atr[0].Elipse_Atr->Set_AnguloZ(Tmp_AnguloZ);

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




long double **Realocar_Ponteiro_Duplo(long double **Ponteiro,int Count){


	Ponteiro=(long double **)realloc(Ponteiro,Count*sizeof(long double*));
	for(int i=0;i<Count;i++){
		Ponteiro[i]=0;
		Ponteiro[i]=(long double*)realloc(Ponteiro[i],2*sizeof(long double));
	}

   return Ponteiro;
}

int **Realocar_Ponteiro_Duplo_Inteiro(int **Ponteiro,int Count){

int **Tmp_Ponteiro=0;
int *Ptr=0;

	Tmp_Ponteiro=Ponteiro;
	Tmp_Ponteiro=(int**)realloc(Tmp_Ponteiro,Count*sizeof(int *));


	for(int i=0;i<Count;i++){

		Tmp_Ponteiro[i]=0;
		Tmp_Ponteiro[i]=(int*)realloc(Tmp_Ponteiro[i],4*sizeof(int));

	}

	return Tmp_Ponteiro;

}

void TEixoXY::DuplicaValoresTeta(Objetos *Objetos_Atr){

if(Faz)
Contador_LineTeta=Objetos_Atr[0].Lines_Atr->GetContador_Line();

	ABT[2]+=M_PI/80;

	if(Objetos_Atr[0].Lines_Atr->GetContador_Line()>0){
		for(int i=0;i<Contador_LineTeta;i++){
			Objetos_Atr[0].Lines_Atr->Alocando_Line(EixoXY->Contador_Line+10);
			FullMLK=Realocar_Ponteiro_Duplo_Inteiro(FullMLK,EixoXY->Contador_Line+1);
			Objetos_Atr[0].Lines_Atr->SetMAlpha_Atr(Objetos_Atr[0].Lines_Atr->GetMAlpha_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetMVetor_Atr(Objetos_Atr[0].Lines_Atr->GetMVetor_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetLAlpha_Atr(Objetos_Atr[0].Lines_Atr->GetLAlpha_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetLVetor_Atr(Objetos_Atr[0].Lines_Atr->GetLVetor_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetMZo_Atr(Objetos_Atr[0].Lines_Atr->GetMZo_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetLZo_Atr(Objetos_Atr[0].Lines_Atr->GetLZo_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetCor_Line(Objetos_Atr[0].Lines_Atr->GetCor_Line(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->Set_AngluloTeta(ABT[2],EixoXY->Contador_Line);

			EixoXY->Contador_Line++;
			Objetos_Atr[0].Lines_Atr->SetContador_Line(EixoXY->Contador_Line);

		}


	}


Faz=false;

}
void TEixoXY::DuplicaValoresAlpha(Objetos *Objetos_Atr){

if(EixoXY->Faz)
Contador_LineTeta=Objetos_Atr[0].Lines_Atr->GetContador_Line();

	EixoXY->ABT[0]+=M_PI/80;

	if(Objetos_Atr[0].Lines_Atr->GetContador_Line()>0){
		for(int i=0;i<Contador_LineTeta;i++){
			Objetos_Atr[0].Lines_Atr->Alocando_Line(EixoXY->Contador_Line+10);
			EixoXY->FullMLK=Realocar_Ponteiro_Duplo_Inteiro(EixoXY->FullMLK,EixoXY->Contador_Line+1);
			Objetos_Atr[0].Lines_Atr->SetMAlpha_Atr(Objetos_Atr[0].Lines_Atr->GetMAlpha_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetMVetor_Atr(Objetos_Atr[0].Lines_Atr->GetMVetor_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetLAlpha_Atr(Objetos_Atr[0].Lines_Atr->GetLAlpha_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetLVetor_Atr(Objetos_Atr[0].Lines_Atr->GetLVetor_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetMZo_Atr(Objetos_Atr[0].Lines_Atr->GetMZo_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetLZo_Atr(Objetos_Atr[0].Lines_Atr->GetLZo_Atr(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->SetCor_Line(Objetos_Atr[0].Lines_Atr->GetCor_Line(i),EixoXY->Contador_Line);
			Objetos_Atr[0].Lines_Atr->Set_AngluloAlpha(EixoXY->ABT[0],EixoXY->Contador_Line);

			EixoXY->Contador_Line++;
			Objetos_Atr[0].Lines_Atr->SetContador_Line(EixoXY->Contador_Line);

		}


	}


EixoXY->Faz=false;




}


void TEixoXY::Draw_Lines_DirectQuadrilatero(TCanvas *Plano,Objetos *Objetos_Atr, int **FullMLK2,int **FullMLK3){

TPoint MP[5];
int Tmp_Contador_Line=Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero();





	for (int i = 0; i < Tmp_Contador_Line; i++) {
		MP2[i][0].x=FullMLK2[i][0];
		MP2[i][0].y=FullMLK2[i][1];
		MP2[i][1].x=FullMLK2[i][2];
		MP2[i][1].y=FullMLK2[i][3];

		MP2[i][2].x=FullMLK3[i][0];
		MP2[i][2].y=FullMLK3[i][1];
		MP2[i][3].x=FullMLK3[i][2];
		MP2[i][3].y=FullMLK3[i][3];

		MP2[i][4].x=FullMLK2[i][0];
		MP2[i][4].y=FullMLK2[i][1];



        Plano->Pen->Color=(TColor)Objetos_Atr[0].Quadrilatero->GetCor(i);
		Plano->Polyline(MP2[i],4);

	}



}

void TEixoXY::Draw_Lines_Direct(TCanvas *Plano,Objetos *Objetos_Atr, int **FullMLK){

TPoint MP[5];
int Tmp_Contador_Line=Objetos_Atr[0].Lines_Atr->GetContador_Line();


	for (int i = 0; i < Tmp_Contador_Line; i++) {
		MP[0].x=FullMLK[i][0];
		MP[0].y=FullMLK[i][1];
		MP[1].x=FullMLK[i][2];
		MP[1].y=FullMLK[i][3];




		Plano->Pen->Color=(TColor)Objetos_Atr[0].Lines_Atr->GetCor_Line(i);

		Plano->Polyline(EixoXY->MP1[i],1);

	}


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
	   MP1=EixoXY->Realocar_Ponteiro_DuploL(MP1,Contador_Line+15);



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
	int ContadorQuadrilatero=Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero();
    FullMLK2=Realocar_Ponteiro_Duplo_Inteiro(FullMLK2,Contador+1);

   	FullMLK3=Realocar_Ponteiro_Duplo_Inteiro(FullMLK3,Contador+1);

	for(int i=0;i<3;i++)
	   ABT[i]=Objetos_Atr[0].GetAlpha_B_T(i);

	   MP2=EixoXY->Realocar_Ponteiro_DuploL2(MP2,ContadorQuadrilatero);
	   MP3=EixoXY->Realocar_Ponteiro_DuploL2(MP3,ContadorQuadrilatero);




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

				FullMLK2[i][0]=P1[0].x=MP2[i][0].x=X(RoundTo(Mxyz[0],0));
				FullMLK2[i][1]=P1[0].y=MP2[i][0].y=Y(RoundTo(Mxyz[1],0));
				FullMLK2[i][2]=P1[1].x=MP2[i][1].x=X(RoundTo(Lxyz[0],0));
				FullMLK2[i][3]=P1[1].y=MP2[i][1].y=Y(RoundTo(Lxyz[1],0));





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

				FullMLK3[i][0]=P1[0].x=MP3[i][0].x=X(RoundTo(Mxyz[0],0));
				FullMLK3[i][1]=P1[0].y=MP3[i][0].y=Y(RoundTo(Mxyz[1],0));
				FullMLK3[i][2]=P1[1].x=MP3[i][1].x=X(RoundTo(Lxyz[0],0));
				FullMLK3[i][3]=P1[1].y=MP3[i][1].y=Y(RoundTo(Lxyz[1],0));





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





		}



	}

	catch(Exception &ex){

	ShowMessage("Erro ao Imprimir o Grafico");

	}



return  MP1;
}



long double X(long double x){
return x+Kx;
}


long double Y(long double y){
return Ky-y;
}




//---------------------------------------------------------------------------
bool EixoXo(long double xo,long double yo,long double zo,  long double *Alphat,long double *MVetorXY){

long double MXYVetor;

long double MVetorXY1;
long double Alphat1=0;






	MVetorXY1=Sqrt((xo*xo) +(yo*yo));
	if (MVetorXY1<0) {
		*Alphat=0;
		*MVetorXY=0;
		return 0;
	}

	if((xo==0)&&(yo==0)){
		*Alphat=0;
		*MVetorXY=0;
		return 0;

	}

	MXYVetor=MVetorXY1;


	*MVetorXY=MVetorXY1;

	if (xo==0) {

		if (yo>0){
			*Alphat=M_PI/2;

		 return 0;

		}

		if (yo<0) {
			*Alphat=-M_PI/2;

			return 0;
		}


	}else{

		if (yo==0) {
			if(xo>0){
				*Alphat=0;
			return 0;
			}

			if(xo<0){
				*Alphat=M_PI;

				 return 0;
			}


		}

		if ((xo<0)&&(yo<0)) {

		 try{
			xo=-xo;

			Alphat1=acos(xo/MVetorXY1)+M_PI;
			xo=-xo;
			}catch(Exception &ex){

				ShowMessage("Divisão por Zero");


				}


		}


		if ((xo<0)&&(yo>0)) {

			try{
				xo=-xo;

				Alphat1=M_PI-acos(xo/MVetorXY1);

				xo=-xo;
			}catch(Exception &ex){
				ShowMessage("Divisão por Zero");
				}
		  }

		if ((xo>0)&&(yo<0)) {

			try{
				Alphat1=2*M_PI-acos(xo/MVetorXY1);
			}catch(Exception &ex){
				ShowMessage("Divisão por Zero");
				}
		}

		if ((xo>0)&&(yo>0)) {

			try{

				Alphat1=acos(xo/MVetorXY1);

			}catch(Exception &ex){

				ShowMessage("Divisão por Zero");

				}
		}

	  }



	*Alphat=Alphat1;





  return 0;

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
		FullMLK=Realocar_Ponteiro_Duplo_Inteiro(FullMLK,Contador_Line+1);



		if(Contar<1){










		}




		if(RBVisaoXoY->Checked){

			Contar++;

			if ((Line->Checked)&&(Contar==1)) {



				MP[0].x=P1.x;
				MP[0].y=P1.y;
				Libera_Draw=true;


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



				Mxyz1=Realocar_Ponteiro_Duplo(Mxyz1,Contador_Line+1);
				Lxyz1=Realocar_Ponteiro_Duplo(Mxyz1,Contador_Line+1);
				Contador_Line++;
				Objetos_Atr[0].Lines_Atr->SetContador_Line(Contador_Line);

				MP1=EixoXY->Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);

				Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
				Libera_Draw=false;
			}

		}

		if(RBVisaoXoZ->Checked){

			Contar++;
			float Yo=EdtYo->Text.ToDouble();

				Objetos_Atr[0].Lines_Atr->Set_AngluloTeta(ABT[Contador_Line],Contador_Line);
				if ((Line->Checked)&&(Contar==1)) {


					MP[0].x=P1.x;
					MP[0].y=P1.y;
					Libera_Draw=true;


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



					Mxyz1=Realocar_Ponteiro_Duplo(Mxyz1,Contador_Line+1);
					Lxyz1=Realocar_Ponteiro_Duplo(Mxyz1,Contador_Line+1);
					Contador_Line++;
					Objetos_Atr[0].Lines_Atr->SetContador_Line(Contador_Line);
					MP1=EixoXY->Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
					Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
                    Libera_Draw=false;
				}







		}

		if(RBVisaoYoZ->Checked){

			Contar++;
			Xo=EdtXo->Text.ToDouble();

				if ((Line->Checked)&&(Contar==1)) {

					MP[0].x=P1.x;
					MP[0].y=P1.y;

					Libera_Draw=true;

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



					Mxyz1=Realocar_Ponteiro_Duplo(Mxyz1,Contador_Line+1);
					Lxyz1=Realocar_Ponteiro_Duplo(Mxyz1,Contador_Line+1);
					Contador_Line++;
					Objetos_Atr[0].Lines_Atr->SetContador_Line(Contador_Line);
					MP1=EixoXY->Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
                    Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
                    Libera_Draw=false;
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
				Cx2=P1.x;
				Cy2=P1.y;

				Yes=MessageDlg("Quer Movimentar",mtConfirmation,TMsgDlgButtons() <<mbYes<<mbNo,0);
				if(Yes==mrYes){

				}else{
					Primeiro=false;
                    Contar=0;
                    Libera_Draw=false;
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
				LimpaElipse(Raio(int(P1.x),int(P1.y),Cx,Cy),Cx,Cy);
				Modulo_Elipse=0;
				EixoXY->P_Circle=EixoXY->Draw_Elipse(EixoXY->Canvas,EixoXY->Objetos_Atr);
				EixoXY->Draw_ElipseDirect(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->P_Circle);


			}

			if ((RBElipse->Checked)&&(Contar==0)&&(Primeiro)) {
				Libera_Draw=true;
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

						EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

						Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

						::Move=false;

						Contar=0;
                        Libera_Draw=false;

					break;

				default :

						Contar=0;


			}


		}

		if(RBVisaoXoZ->Checked){

			switch(Contar){

				case 0 :
						Libera_Draw=true;
						MPQ[0].x=P1.x;
						MPQ[0].y=P1.y;
						P2[0].x=MPQ[0].x;
						P2[0].y=MPQ[0].y;
						Libera_Draw=true;
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

						EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

						Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
						::Move=false;
						Libera_Draw=false;
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
						MPQ[0].x=P1.x;
						MPQ[0].y=P1.y;
						P2[0].x=MPQ[0].x;
						P2[0].y=MPQ[0].y;
						Libera_Draw=true;
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

						EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

						Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

						::Move=false;
                        Libera_Draw=false;
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
		if(Contador_Line>0){

			try{
			Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
			}catch(Exception &ex){

			}


		}

		if(Objetos_Atr->Elipse_Atr->GetContadorCircle_Atr()>0){

			Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
		}

		if(Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero()>0){
			Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

		}

	}else
	 Lancamento();




}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::ButtonEixo_ZoClick(TObject *Sender)
{
	Cancela_Line->Checked=true;
	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);

	ABT[2]+=M_PI/80;
	Objetos_Atr[0].SetAlpha_B_T(ABT);

	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);

	EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
	Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

}
//---------------------------------------------------------------------------
void GiraZo(){
	EixoXY->Cancela_Line->Checked=true;
	Limpa_Lines(EixoXY->Canvas,EixoXY->FullMLK,EixoXY->Contador_Line);
	EixoXY->Limpa_Elipses(EixoXY->Canvas,EixoXY->P_Circle,EixoXY->Contador_Circle);

	EixoXY->ABT[2]+=M_PI/80;
	EixoXY->Objetos_Atr[0].SetAlpha_B_T(EixoXY->ABT);

	EixoXY->MP1=EixoXY->Draw_Lines(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->FullMLK);
	EixoXY->Draw_Lines_Direct(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->FullMLK);
	EixoXY->P_Circle=EixoXY->Draw_Elipse(EixoXY->Canvas,EixoXY->Objetos_Atr);
	EixoXY->Draw_ElipseDirect(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->P_Circle);

	EixoXY->Draw_Quadrilatero(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->FullMLK2,EixoXY->FullMLK3);
	EixoXY->Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->FullMLK2,EixoXY->FullMLK3);
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
	LimpaQuadrilatero(EixoXY->Canvas,FullMLK2,FullMLK3,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );


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
	EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
	Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
}
//---------------------------------------------------------------------------
void __fastcall TEixoXY::RBVisaoXoYClick(TObject *Sender)
{

	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
	LimpaQuadrilatero(EixoXY->Canvas,FullMLK2,FullMLK3,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );


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
	EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
	Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::BEixoYoClick(TObject *Sender)
{
	Cancela_Line->Checked=true;

	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
	ABT[1]+=M_PI/80;
	Objetos_Atr[0].SetAlpha_B_T(ABT);
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::BEixoXoClick(TObject *Sender)
{
	Cancela_Line->Checked=true;
	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);

	ABT[0]+=M_PI/80;
	Objetos_Atr[0].SetAlpha_B_T(ABT);
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
	P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
	Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::RBVisaoYoZClick(TObject *Sender)
{
	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
    LimpaQuadrilatero(EixoXY->Canvas,FullMLK2,FullMLK3,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );


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
	EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
	Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::BAumentarClick(TObject *Sender)
{
	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
	LimpaQuadrilatero(EixoXY->Canvas,FullMLK2,FullMLK3,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );

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
    EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
	Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);


}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::BDiminuirClick(TObject *Sender)
{


	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
	LimpaQuadrilatero(EixoXY->Canvas,FullMLK2,FullMLK3,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );

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
	EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
	Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
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
	Limpa_Lines(EixoXY->Canvas,FullMLK,Contador_Line);
	Limpa_Elipses(EixoXY->Canvas,P_Circle,Contador_Circle);
	LimpaQuadrilatero(EixoXY->Canvas,FullMLK2,FullMLK3,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );
	LimpaMemoria();
}
//---------------------------------------------------------------------------
void LimpaMemoria(){


	EixoXY->Objetos_Atr[0].Lines_Atr->SetContador_Line(0);
	EixoXY->Objetos_Atr[0].Elipse_Atr->SetContadorCircle_Atr(0);
    EixoXY->Objetos_Atr[0].Quadrilatero->SetContadorQuadrilatero(0);
	EixoXY->Contador_Line=0;
	EixoXY->Contador_Circle=0;


	for(int i=0;i<3;i++) {
		EixoXY->Objetos_Atr[0].SetAlpha_B_T(0,i);
		EixoXY->ABT[i]=0;

	}

	Kx=960;
	Ky=540;
    Libera_Draw=false;
	EixoXY->Faz=true;


}
void __fastcall TEixoXY::Volta_LinhaClick(TObject *Sender)
{
 int i=EixoXY->Objetos_Atr[0].Lines_Atr->GetContador_Line();
 if(i>0){
	EixoXY->Objetos_Atr[0].Lines_Atr->SetContador_Line(i-1);
    Contador_Line--;
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
Lancar=true;
t=0;
Eixo_Lancar=false;



}
//---------------------------------------------------------------------------

long double Converte_Angulo(long double Angulo){


return (Angulo/180)*M_PI;

}

void __fastcall TEixoXY::FormClose(TObject *Sender, TCloseAction &Action)
{
Libera_Draw=false;
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
						LimpaQuadrilatero(EixoXY->Canvas,FullMLK2,FullMLK3,EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero() );
						MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);


						Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
						P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);

						Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
						EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
						Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);


					   t+=0.030;

					}else{
						Lancar=false;
						Eixo_Lancar=true;
						ShowMessage("Fim do Lançamento...!!!");
						}
				   Eixo_Lancar=false;
				}
		} else{
				 Lancar=false;
				 ShowMessage("Preencha os Campos velocidade e Angulo" );
				}
	}





}


void __fastcall TEixoXY::FormCreate(TObject *Sender)
{

Libera_Draw=false;
Faz=true;


}
//---------------------------------------------------------------------------

void TEixoXY::LimpaQuadrilatero(TCanvas *Plano,int **FullMLK2,int **FullMLK3, int Countador_Line){

int Tmp_Contador_Line=EixoXY->Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero();

	Plano->Pen->Color=clWhite;

	for (int i = 0; i < Tmp_Contador_Line; i++) {
		MP2[i][0].x=FullMLK2[i][0];
		MP2[i][0].y=FullMLK2[i][1];
		MP2[i][1].x=FullMLK2[i][2];
		MP2[i][1].y=FullMLK2[i][3];

		MP2[i][2].x=FullMLK3[i][0];
		MP2[i][2].y=FullMLK3[i][1];
		MP2[i][3].x=FullMLK3[i][2];
		MP2[i][3].y=FullMLK3[i][3];

		MP2[i][4].x=FullMLK2[i][0];
		MP2[i][4].y=FullMLK2[i][1];




		Plano->Polyline(MP2[i],4);

	}


}
void __fastcall TEixoXY::Button1Click(TObject *Sender)
{
    Form1->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::Button2Click(TObject *Sender)
{
	if(Objetos_Atr[0].Quadrilatero->GetContadorQuadrilatero()>0){
			EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
			Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

		}
}
//---------------------------------------------------------------------------


void __fastcall TEixoXY::BtnDupTetaClick(TObject *Sender)
{
	DuplicaValoresTeta(Objetos_Atr);
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::BtnDupAlphaClick(TObject *Sender)
{
	DuplicaValoresAlpha(Objetos_Atr);
	MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
	Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);

}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::Salvar1Click(TObject *Sender)
{
 UnicodeString Nome_Arquivo,Resposta;
   Arquivo *Arqu=new Arquivo;
   char  N_Arquivo[500];

	if(DlgSalvar->Execute()){

		Nome_Arquivo=DlgSalvar->FileName;

		for(int i=0;i<Nome_Arquivo.Length();i++){
			N_Arquivo[i]=(char)Nome_Arquivo[i+1];

		}

		N_Arquivo[ Nome_Arquivo.Length()]='\0';

	}

   if(0)
   Grava_Arquivo(Objetos_Atr,N_Arquivo);

   Arqu->Salvar_Arquivo(N_Arquivo,Objetos_Atr,&Resposta);
   ShowMessage(Resposta);

}
//---------------------------------------------------------------------------

void __fastcall TEixoXY::Abrir1Click(TObject *Sender)
{

AnsiString Nome_Arquivo;
Arquivo *Arqui=new Arquivo();
Objetos *Tmp_Objetos;
int Tmp_Contador_Line;
UnicodeString Resposta;


	Libera_Draw=false;

	Cancela_Line->Checked=true;



	if(DlgAbrir->Execute()){

		Nome_Arquivo=DlgAbrir->FileName;



	}

		Limpa_Lines(EixoXY->Canvas,FullMLK,EixoXY->Contador_Line);
		Limpa_Elipses(EixoXY->Canvas,P_Circle,EixoXY->Contador_Circle);
		LimpaMemoria();


		Objetos_Atr=Arqui->Abrir_Arquivo(Nome_Arquivo.c_str(), &P_Circle,&FullMLK,&Resposta);
		ShowMessage(Resposta);
		if(0)
		if(Objetos_Atr!=NULL){
			for(int i=0;i<3;i++)
				ABT[i]=Objetos_Atr[0].GetAlpha_B_T(i);
		}
		if(Objetos_Atr!=NULL){
			Objetos_Atr[0].GetAlpha_B_T(ABT);
			EixoXY->Contador_Circle=Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr();
			MP1=Draw_Lines(EixoXY->Canvas,Objetos_Atr,FullMLK);
			Draw_Lines_Direct(EixoXY->Canvas,Objetos_Atr,FullMLK);
			P_Circle=Draw_Elipse(EixoXY->Canvas,Objetos_Atr);
			Draw_ElipseDirect(EixoXY->Canvas,Objetos_Atr,P_Circle);
			EixoXY->Draw_Quadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);
			Draw_Lines_DirectQuadrilatero(EixoXY->Canvas,Objetos_Atr,FullMLK2,FullMLK3);

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

