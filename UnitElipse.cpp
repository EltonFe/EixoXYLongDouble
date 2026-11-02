//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitElipse.h"
#include "UnitEixoXY.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFElipse *FElipse;
//---------------------------------------------------------------------------
__fastcall TFElipse::TFElipse(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

long double ConverteAngulo(long double Angulo){

return (Angulo*2*M_PI)/360;
}

void Desrotacao_Vetores_3D_Circle_MP2(long double xyz[3],long double xe,long double ye, long double  ze,long double Angulo)
{


   long double Alpha_Beta_Teta[3];


	if(FElipse->RBEixoYZ->Checked){
		//Yo
		Alpha_Beta_Teta[0]=Angulo;
		xyz[1]=xe*(Sin(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[1])*Cos(Alpha_Beta_Teta[2])+
			Cos(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[2]))+ye*(-Sin(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[1])*Sin(Alpha_Beta_Teta[2])+
			Cos(Alpha_Beta_Teta[0])*Cos(Alpha_Beta_Teta[2]))+ze*(-Sin(Alpha_Beta_Teta[0])*Cos(Alpha_Beta_Teta[1]));


		//Zo
		xyz[2]=xe*(-Cos(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[1])*Cos(Alpha_Beta_Teta[2])+
			Sin(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[2]))+ye*(Cos(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[1])*Sin(Alpha_Beta_Teta[2])+
			Sin(Alpha_Beta_Teta[0])*Cos(Alpha_Beta_Teta[2]))+ze*(Cos(Alpha_Beta_Teta[0])*Cos(Alpha_Beta_Teta[1]));
		xyz[0]=xe;
	}





	if(FElipse->RBEixoXY->Checked){
		//Xo

		Alpha_Beta_Teta[2]=Angulo;
		xyz[0]=xe*(Cos(Alpha_Beta_Teta[1])*Cos(Alpha_Beta_Teta[2])) -
			( ye*Cos( Alpha_Beta_Teta[1])*Sin(Alpha_Beta_Teta[2]))+ze*Sin(Alpha_Beta_Teta[1]);

		//Yo
		xyz[1]=xe*(Sin(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[1])*Cos(Alpha_Beta_Teta[2])+
			Cos(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[2]))+ye*(-Sin(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[1])*Sin(Alpha_Beta_Teta[2])+
			Cos(Alpha_Beta_Teta[0])*Cos(Alpha_Beta_Teta[2]))+ze*(-Sin(Alpha_Beta_Teta[0])*Cos(Alpha_Beta_Teta[1]));

		xyz[2]=ze;
	}




	if(FElipse->RBEixoXZ->Checked){
		//Xo
		Alpha_Beta_Teta[1]=Angulo;
		xyz[0]=xe*(Cos(Alpha_Beta_Teta[1])*Cos(Alpha_Beta_Teta[2])) -
		( ye*Cos( Alpha_Beta_Teta[1])*Sin(Alpha_Beta_Teta[2]))+ze*Sin(Alpha_Beta_Teta[1]);
		//Zo
		xyz[2]=xe*(-Cos(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[1])*Cos(Alpha_Beta_Teta[2])+
		Sin(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[2]))+ye*(Cos(Alpha_Beta_Teta[0])*Sin(Alpha_Beta_Teta[1])*Sin(Alpha_Beta_Teta[2])+
		Sin(Alpha_Beta_Teta[0])*Cos(Alpha_Beta_Teta[2]))+ze*(Cos(Alpha_Beta_Teta[0])*Cos(Alpha_Beta_Teta[1]));

		xyz[1]=ye;
	}




}

void __fastcall TFElipse::BtnElipseClick(TObject *Sender)
{
long double cx=0;
long double cy=0;
long double cz=0;
long double DiferencaY=0;
long double Angulo=0;

long double CXYZ[3];




int Contador_Circle=EixoXY->Objetos_Atr[0].Elipse_Atr->GetContadorCircle_Atr();

	BtnElipse->Enabled=false;

	EixoXY->Objetos_Atr[0].Elipse_Atr->AlocaElipse_Atr(Contador_Circle+1);


	EixoXY->Objetos_Atr[0].Elipse_Atr->SetColor_Circle_Atr(Cores->Selected,Contador_Circle);

	EixoXY->Objetos_Atr[0].Elipse_Atr->SetA_Atr(EdtA->Text.ToDouble(),Contador_Circle);
	EixoXY->Objetos_Atr[0].Elipse_Atr->SetB_Atr(EdtB->Text.ToDouble(),Contador_Circle);
	EixoXY->Objetos_Atr[0].Elipse_Atr->SetAZo_Atr(EdtAZo->Text.ToDouble(),Contador_Circle);
	EixoXY->Objetos_Atr[0].Elipse_Atr->SetBZo_Atr(EdtBZo->Text.ToDouble(),Contador_Circle);
	Angulo=StrToFloat(EdtAngulo->Text);

	DiferencaY=-(EdtCentroX->Text.ToDouble()*Sin(ConverteAngulo(EdtAngulo->Text.ToDouble())));
	cx=EdtCentroX->Text.ToDouble();
	cy=EdtCentroY->Text.ToDouble();
	cz=EdtCentroZ->Text.ToDouble();


	Desrotacao_Vetores_3D_Circle_MP2(CXYZ,cx,cy,cz,-ConverteAngulo(EdtAngulo->Text.ToDouble()));
	cx=CXYZ[0];
	cy=CXYZ[1];
	cz=CXYZ[2];

	EixoXY->Objetos_Atr[0].Elipse_Atr->SetCX(cx,Contador_Circle);
	EixoXY->Objetos_Atr[0].Elipse_Atr->SetCY(cy,Contador_Circle);
    EixoXY->Objetos_Atr[0].Elipse_Atr->SetCZ(cz,Contador_Circle);
	EixoXY->Objetos_Atr[0].Elipse_Atr->SetCenter_Atr((float)cx,(float)cy,(float)cz,Contador_Circle);

	if(RBEixoXY->Checked){
		EixoXY->Objetos_Atr[0].Elipse_Atr->SetTipoElipse_Atr(0,Contador_Circle);

		EixoXY->Objetos_Atr[0].Elipse_Atr->Set_AnguloX(ConverteAngulo(StrToFloat(EdtAngulo->Text)),Contador_Circle);

		EixoXY->Objetos_Atr[0].Elipse_Atr->GetAnguloX(0);

	}
	if(RBEixoXZ->Checked){
		EixoXY->Objetos_Atr[0].Elipse_Atr->SetTipoElipse_Atr(1,Contador_Circle);
        EixoXY->Objetos_Atr[0].Elipse_Atr->Set_AnguloZ(-ConverteAngulo(StrToFloat(EdtAngulo->Text)),Contador_Circle);


	}
	if(RBEixoYZ->Checked){
		EixoXY->Objetos_Atr[0].Elipse_Atr->SetTipoElipse_Atr(2,Contador_Circle);
		EixoXY->Objetos_Atr[0].Elipse_Atr->Set_AnguloY(ConverteAngulo(StrToFloat(EdtAngulo->Text)),Contador_Circle);

	}


	Contador_Circle++;
	EixoXY->Contador_Circle=Contador_Circle;
	EixoXY->Objetos_Atr[0].Elipse_Atr->SetContadorCircle_Atr(Contador_Circle);

	EixoXY->P_Circle=EixoXY->Draw_Elipse(EixoXY->Canvas,EixoXY->Objetos_Atr);
	EixoXY->Draw_ElipseDirect(EixoXY->Canvas,EixoXY->Objetos_Atr,EixoXY->P_Circle);
}
//---------------------------------------------------------------------------


