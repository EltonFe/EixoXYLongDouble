//---------------------------------------------------------------------------

#pragma hdrstop

#include "UnitQuadrilatero.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)




TQuadrilatero::TQuadrilatero():ContadorQuadrilatero(0){

}
void TQuadrilatero::AlocarQuadrilatero(int Tam){



	MAlpha.resize(Tam);
	MVetor.resize(Tam);
	MAlpha2.resize(Tam);
	MVetor2.resize(Tam);
	LAlpha.resize(Tam);
	LVetor.resize(Tam);
	LAlpha2.resize(Tam);
	LVetor2.resize(Tam);

	MZo.resize(Tam);
	LZo.resize(Tam);
	MZo2.resize(Tam);
	LZo2.resize(Tam);
	MYo.resize(Tam);
	LYo.resize(Tam);
	MYo2.resize(Tam);
	LYo2.resize(Tam);
	MXo.resize(Tam);
	LXo.resize(Tam);
	MXo2.resize(Tam);
	LXo2.resize(Tam);
	Cor_Quadrilatero.resize(Tam);

}


void TQuadrilatero::SetCor(COLORREF Cor,int index){

	GravarVetor(Cor_Quadrilatero,index,TColorParaCOLORREF(Cor));

}
void TQuadrilatero::SetCor(COLORREF *Cor){

	CopiaVetor(Cor_Quadrilatero,Cor);
	NormalizaCores(Cor_Quadrilatero);

}

COLORREF TQuadrilatero::GetCor(int index)const{

	return LerVetor(Cor_Quadrilatero,index);

}

COLORREF *TQuadrilatero::GetCor()const{

	return const_cast<COLORREF*>(Cor_Quadrilatero.data());

}


void TQuadrilatero::SetMAlpha(long double MA,int Index){

GravarVetor(MAlpha,Index,MA);
}

void TQuadrilatero::SetMAlpha(long double *MA){
	CopiaVetor(MAlpha,MA);
}


long double TQuadrilatero::GetMAlpha(int Index)const{

return LerVetor(MAlpha,Index);

}

long double * TQuadrilatero::GetMAlpha()const{

	return const_cast<long double*>(MAlpha.data());
}


void TQuadrilatero::SetMVetor(long double MV,int Index){

 GravarVetor(MVetor,Index,MV);
}

void TQuadrilatero::SetMVetor(long double *MV){

	CopiaVetor(MVetor,MV);

}


long double TQuadrilatero::GetMVetor(int Index) const{

	return LerVetor(MVetor,Index);

}

long double * TQuadrilatero::GetMVetor()const{

	return const_cast<long double*>(MVetor.data());

}


void TQuadrilatero::SetMAlpha2(long double MA2,int Index){

GravarVetor(MAlpha2,Index,MA2);

}

void TQuadrilatero::SetMAlpha2(long double *MA2){

	CopiaVetor(MAlpha2,MA2);

}


long double  TQuadrilatero::GetMAlpha2(int Index)const{

	return LerVetor(MAlpha2,Index);


}

long double  *TQuadrilatero::GetMAlpha2()const{

	return const_cast<long double*>(MAlpha2.data());


}

void TQuadrilatero::SetMVetor2(long double MV2,int Index){

GravarVetor(MVetor2,Index,MV2);

}

void TQuadrilatero::SetMVetor2(long double *MV2){

CopiaVetor(MVetor2,MV2);

}

long double TQuadrilatero::GetMVetor2(int Index)const{

	return LerVetor(MVetor2,Index);

}
long double *TQuadrilatero::GetMVetor2()const{

	return const_cast<long double*>(MVetor2.data());

}


void TQuadrilatero::SetLAlpha(long double LA,int Index){

GravarVetor(LAlpha,Index,LA);
}

void TQuadrilatero::SetLAlpha(long double *LA){

CopiaVetor(LAlpha,LA);
}


long double TQuadrilatero::GetLAlpha(int Index)const{

	return LerVetor(LAlpha,Index);

}

long double *TQuadrilatero::GetLAlpha()const{

	return const_cast<long double*>(LAlpha.data());

}

void TQuadrilatero::SetLVetor(long double LV,int Index){

GravarVetor(LVetor,Index,LV);

}

void TQuadrilatero::SetLVetor(long double *LV){

CopiaVetor(LVetor,LV);

}

long double TQuadrilatero::GetLVetor(int Index)const{

	return LerVetor(LVetor,Index);

}

long double *TQuadrilatero::GetLVetor()const{

	return const_cast<long double*>(LVetor.data());

}

void TQuadrilatero::SetLAlpha2(long double LA2,int Index){

GravarVetor(LAlpha2,Index,LA2);
}

void TQuadrilatero::SetLAlpha2(long double *LA2){

CopiaVetor(LAlpha2,LA2);
}

long double TQuadrilatero::GetLAlpha2(int Index)const{

	return LerVetor(LAlpha2,Index);

}

long double *TQuadrilatero::GetLAlpha2()const{

	return const_cast<long double*>(LAlpha2.data());

}

void TQuadrilatero::SetLVetor2(long double LV2,int Index){

	GravarVetor(LVetor2,Index,LV2);

}

void TQuadrilatero::SetLVetor2(long double *LV2){

	CopiaVetor(LVetor2,LV2);

}


long double TQuadrilatero::GetLVetor2(int Index)const{

	return LerVetor(LVetor2,Index);

}

long double *TQuadrilatero::GetLVetor2()const{

	return const_cast<long double*>(LVetor2.data());

}

void TQuadrilatero::SetContadorQuadrilatero(int Contador){

	ContadorQuadrilatero=Contador;
}
int TQuadrilatero::GetContadorQuadrilatero()const{

	return ContadorQuadrilatero;

}

void TQuadrilatero::SetMZo(long double MZ,int Index){

	GravarVetor(MZo,Index,MZ);
}

void TQuadrilatero::SetMZo(long double *MZ){

	CopiaVetor(MZo,MZ);
}

long double TQuadrilatero::GetMZo(int Index)const{
	return LerVetor(MZo,Index);
}



long double *TQuadrilatero::GetMZo()const{
	return const_cast<long double*>(MZo.data());
}


void TQuadrilatero::SetLZo(long double LZ, int Index){

	GravarVetor(LZo,Index,LZ);
}

void TQuadrilatero::SetLZo(long double *LZ){

	CopiaVetor(LZo,LZ);
}



long double TQuadrilatero::GetLZo(int Index)const{
	return LerVetor(LZo,Index);

}
long double *TQuadrilatero::GetLZo()const{
	return const_cast<long double*>(LZo.data());

}



void TQuadrilatero::SetMZo2(long double MZ2,int Index){

	GravarVetor(MZo2,Index,MZ2);
}

void TQuadrilatero::SetMZo2(long double *MZ2){

	CopiaVetor(MZo2,MZ2);
}


long double TQuadrilatero::GetMZo2(int Index)const{
	return LerVetor(MZo2,Index);
}

long double *TQuadrilatero::GetMZo2()const{
	return const_cast<long double*>(MZo2.data());
}

void TQuadrilatero::SetLZo2(long double LZ2, int Index){

	GravarVetor(LZo2,Index,LZ2);
}

void TQuadrilatero::SetLZo2(long double *LZ2){

	CopiaVetor(LZo2,LZ2);
}

long double TQuadrilatero::GetLZo2(int Index)const{
	return LerVetor(LZo2,Index);

}

long double *TQuadrilatero::GetLZo2()const{
	return const_cast<long double*>(LZo2.data());

}

void TQuadrilatero::SetMYo(long double Valor,int Index){

	GravarVetor(MYo,Index,Valor);

}
long double TQuadrilatero::GetMYo(int Index)const{

	return LerVetor(MYo,Index);

}


void TQuadrilatero::SetLYo(long double Valor,int Index){

	GravarVetor(LYo,Index,Valor);

}
long double TQuadrilatero::GetLYo(int Index)const{

	return LerVetor(LYo,Index);

}

void TQuadrilatero::SetMYo2(long double Valor,int Index){

	GravarVetor(MYo2,Index,Valor);

}
long double TQuadrilatero::GetMYo2(int Index)const{

	return LerVetor(MYo2,Index);

}

void TQuadrilatero::SetLYo2(long double Valor,int Index){

	GravarVetor(LYo2,Index,Valor);
}
long double TQuadrilatero::GetLYo2(int Index)const{

	return LerVetor(LYo2,Index);

}

void TQuadrilatero::SetMXo(long double Valor,int Index){

	GravarVetor(MXo,Index,Valor);

}
long double TQuadrilatero::GetMXo(int Index)const{

	return LerVetor(MXo,Index);

}


void TQuadrilatero::SetLXo(long double Valor,int Index){

	GravarVetor(LXo,Index,Valor);

}
long double TQuadrilatero::GetLXo(int Index)const{

	return LerVetor(LXo,Index);

}

void TQuadrilatero::SetMXo2(long double Valor,int Index){

	GravarVetor(MXo2,Index,Valor);

}
long double TQuadrilatero::GetMXo2(int Index)const{

	return LerVetor(MXo2,Index);

}

void TQuadrilatero::SetLXo2(long double Valor,int Index){

	GravarVetor(LXo2,Index,Valor);
}
long double TQuadrilatero::GetLXo2(int Index)const{

	return LerVetor(LXo2,Index);

}

//------------------------------------------------------------------------------
// TQuadrilatero: Get/Set do vetor inteiro de cada membro
//------------------------------------------------------------------------------
const std::vector<long double> &TQuadrilatero::GetVetor_MAlpha()const{

	return MAlpha;

}
void TQuadrilatero::SetVetor_MAlpha(const std::vector<long double> &Valores){

	MAlpha=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_MVetor()const{

	return MVetor;

}
void TQuadrilatero::SetVetor_MVetor(const std::vector<long double> &Valores){

	MVetor=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_LAlpha()const{

	return LAlpha;

}
void TQuadrilatero::SetVetor_LAlpha(const std::vector<long double> &Valores){

	LAlpha=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_LVetor()const{

	return LVetor;

}
void TQuadrilatero::SetVetor_LVetor(const std::vector<long double> &Valores){

	LVetor=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_MAlpha2()const{

	return MAlpha2;

}
void TQuadrilatero::SetVetor_MAlpha2(const std::vector<long double> &Valores){

	MAlpha2=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_MVetor2()const{

	return MVetor2;

}
void TQuadrilatero::SetVetor_MVetor2(const std::vector<long double> &Valores){

	MVetor2=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_LAlpha2()const{

	return LAlpha2;

}
void TQuadrilatero::SetVetor_LAlpha2(const std::vector<long double> &Valores){

	LAlpha2=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_LVetor2()const{

	return LVetor2;

}
void TQuadrilatero::SetVetor_LVetor2(const std::vector<long double> &Valores){

	LVetor2=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_MXo()const{

	return MXo;

}
void TQuadrilatero::SetVetor_MXo(const std::vector<long double> &Valores){

	MXo=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_MYo()const{

	return MYo;

}
void TQuadrilatero::SetVetor_MYo(const std::vector<long double> &Valores){

	MYo=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_MZo()const{

	return MZo;

}
void TQuadrilatero::SetVetor_MZo(const std::vector<long double> &Valores){

	MZo=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_LXo()const{

	return LXo;

}
void TQuadrilatero::SetVetor_LXo(const std::vector<long double> &Valores){

	LXo=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_LYo()const{

	return LYo;

}
void TQuadrilatero::SetVetor_LYo(const std::vector<long double> &Valores){

	LYo=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_LZo()const{

	return LZo;

}
void TQuadrilatero::SetVetor_LZo(const std::vector<long double> &Valores){

	LZo=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_MXo2()const{

	return MXo2;

}
void TQuadrilatero::SetVetor_MXo2(const std::vector<long double> &Valores){

	MXo2=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_MYo2()const{

	return MYo2;

}
void TQuadrilatero::SetVetor_MYo2(const std::vector<long double> &Valores){

	MYo2=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_MZo2()const{

	return MZo2;

}
void TQuadrilatero::SetVetor_MZo2(const std::vector<long double> &Valores){

	MZo2=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_LXo2()const{

	return LXo2;

}
void TQuadrilatero::SetVetor_LXo2(const std::vector<long double> &Valores){

	LXo2=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_LYo2()const{

	return LYo2;

}
void TQuadrilatero::SetVetor_LYo2(const std::vector<long double> &Valores){

	LYo2=Valores;

}

const std::vector<long double> &TQuadrilatero::GetVetor_LZo2()const{

	return LZo2;

}
void TQuadrilatero::SetVetor_LZo2(const std::vector<long double> &Valores){

	LZo2=Valores;

}

const std::vector<COLORREF> &TQuadrilatero::GetVetor_Cor_Quadrilatero()const{

	return Cor_Quadrilatero;

}
void TQuadrilatero::SetVetor_Cor_Quadrilatero(const std::vector<COLORREF> &Valores){

	Cor_Quadrilatero=Valores;
	NormalizaCores(Cor_Quadrilatero);

}

