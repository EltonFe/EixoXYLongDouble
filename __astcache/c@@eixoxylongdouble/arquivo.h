//---------------------------------------------------------------------------

#ifndef ArquivoH
#define ArquivoH
//---------------------------------------------------------------------------
#include <System.Types.hpp>
#include <System.Classes.hpp>
#include <vector>
#include <iterator>
#include <System.Math.hpp>
#include <stdlib.h>
#include <malloc.h>
#include <vector>
#include "UnitQuadrilatero.h"

using namespace std;


using namespace std;

namespace Name_Arquivos{

	void LimpaMemoria();






	class Elipse{

	private:
			int ContadorCircle_Atr;

			long double *AZo_Atr;
			long double *BZo_Atr;
			long double *A_Atr;
			long double *B_Atr;
			long double *CX;
			long double *CY;
			long double *CZ;
			unsigned int *Color_Circle_Atr;
			int *TipoElipse_Atr;
			long double Alpha_Beta_Teta_Circle_Atr[3];
			int *IndexX;
			int IndexY;
			int IndexZ;

			long double *AnguloX;
			long double *AnguloY;
            long double *AnguloZ;

			//Tmp_Variaveis-----------------------------------------------------

			int Tmp_ContadorCircle_Atr;
			long double **Tmp_Center_Atr;
			long double *Tmp_AZo_Atr;
			long double *Tmp_BZo_Atr;
			long double *Tmp_A_Atr;
			long double *Tmp_B_Atr;
			unsigned int *Tmp_Color_Circle_Atr;
			int *Tmp_TipoElipse_Atr;


	public:
			Elipse();

			long double **Center_Atr;
			void AlocaElipse_Atr(int Count);
			void SetContadorCircle_Atr(int CCircle);
			int GetContadorCircle_Atr();
			void SetCenter_Atr(long double **C_Atr);
			void SetCenter_Atr(long double **,int);
			void SetCenter_Atr(long double,long double,long double,int);
			long double **GetCenter_Atr();
			void GetCenter_Atr(long double **,int );
			void SetAZo_Atr(long double *);
			void SetAZo_Atr(long double,int);
			long double *GetAZo_Atr();
			void SetBZo_Atr(long double *);
			void SetBZo_Atr(long double,int);
			long double *GetBZo_Atr();
			long double GetBZo_Atr(int);
			void SetA_Atr(long double *);
			void SetA_Atr(long double ,int);
			long double *GetA_Atr();
			long double GetA_Atr(int Index);
			long double GetAZo_Atr(int Index);
			void SetB_Atr(long double *);
			void SetB_Atr(long double,int);
			long double *GetB_Atr();
			long double GetB_Atr(int Index);
			void SetCX(long double _CX,int Index);
			void SetCX(long double *_CX);
			long double GetCX(int Index);
			long double *GetCX();

			void SetCY(long double _CY,int Index);
			void SetCY(long double *_CY);
			long double GetCY(int Index);
			long double *GetCY();

			void SetCZ(long double _CZ,int Index);
			void SetCZ(long double *_CZ);

			void Set_AnguloX(long double *);
			void Set_AnguloX(long double,int);

			void Set_AnguloY(long double *);
			void Set_AnguloY(long double,int);

			void Set_AnguloZ(long double *);
			void Set_AnguloZ(long double,int);

			long double GetCZ(int Index);
			long double *GetCZ();

			void SetColor_Circle_Atr(unsigned int *);
			void SetColor_Circle_Atr(unsigned int,int);
			unsigned int *GetColor_Circle_Atr();
			unsigned int GetColor_Circle_Atr(int Index);
			void SetAlpha_Beta_Teta_Circle_Atr(long double *);
			long double *GetAlpha_Beta_Teta_Atr();
			void SetTipoElipse_Atr(int *);
			void SetTipoElipse_Atr(int,int);
			int *GetTipoElipse_Atr();
			int GetTipoElipse_Atr(int Index);
			long double GetAnguloX(int Index);
			long double *GetAnguloX();
			long double GetAnguloY(int Index);
			long double *GetAnguloY();
			long double GetAnguloZ(int Index);
			long double *GetAnguloZ();
			void Limpa_Elipse();

			void Set_IndexX(int*);
			void Set_IndexX(int,int);
			int *GetIndexX();
            int GetIndexX(int);
			void Set_IndexY(int);
			int GetIndexY();









	};

	class Line{
		private:
		long double *MAlpha_Atr;
		long double *MVetor_Atr;
		long double *LAlpha_Atr;
		long double *LVetor_Atr;
		long double *MXo_Atr;
		long double *LXo_Atr;
		long double *MYo_Atr;
		long double *LYo_Atr;
		long double *MZo_Atr;
		long double *LZo_Atr;
		unsigned int *Cor_Line;
		int **FullMLK=0;
		//Tmp_Var---------------------------------------------------------------

		long double *Tmp_MA;
		long double *Tmp_LA;
		long double *Tmp_MV;
		long double *Tmp_LV;
		long double *Tmp_MX;
		long double *Tmp_LX;
		long double *Tmp_MY;
		long double *Tmp_LY;
		long double *Tmp_MZ;
		long double *Tmp_LZ;
		unsigned int *Tmp_CL;



		int Contador_Line;
		long double Alpha_B_T[3];
		long double *Angulo_Alpha;
		long double *Angulo_Beta;
		long double *Angulo_Teta;

		public:

		Line();
		long double *Realocar(long double *Ponteiro,int Count);
		unsigned long * Realocar(unsigned long *Ponteiro,int Count);
		unsigned int * Realocar(unsigned int *Ponteiro,int Count);
		int  *Realocar_Int(int *Ponteiro,int Tamanho);
		void Alocando_Line(int Count);
		//----------------------------------------------------------------------
		void SetMAlpha_Atr(long double Valor,int Index);
		void SetMAlpha_Atr(long double *Ptr);

		void SetLAlpha_Atr(long double Valor,int Index);
		void SetLAlpha_Atr(long double *Ptr);

		void SetMVetor_Atr(long double Valor,int Index);
		void SetMVetor_Atr(long double *Ptr);

		void SetLVetor_Atr(long double Valor,int Index);
		void SetLVetor_Atr(long double *Ptr);

		void SetMXo_Atr(long double Valor,int Index);
		void SetMXo_Atr(long double *Ptr);

		void SetLXo_Atr(long double Valor,int Index);
		void SetLXo_Atr(long double *Ptr);

		void SetMYo_Atr(long double Valor,int Index);
		void SetLYo_Atr(long double Valor,int Index);


		void SetMZo_Atr(long double Valor,int Index);
		void SetMZo_Atr(long double *Ptr);

		void SetLZo_Atr(long double Valor,int Index);
		void SetLZo_Atr(long double *Ptr);

		void SetAlpha_B_T_Atr(long double *);


		void SetCor_Line(unsigned int Valor,int Index);
		void SetCor_Line(unsigned int *Ptr);

		void Set_AngluloAlpha(long double,int);
		void Set_AngluloAlpha(long double *);

		long double GetAnguloAlpha(int);
		long double *GetAngloAlpha();

		void Set_AngluloBeta(long double,int);
		void Set_AngluloBeta(long double *);

		long double GetAnguloBeta(int);
		long double *GetAngloBeta();

		void Set_AngluloTeta(long double,int);
		void Set_AngluloTeta(long double *);

		long double GetAnguloTeta(int);
		long double *GetAngloTeta();

		void SetContador_Line(int Valor);
		//----------------------------------------------------------------------
		int **GetFullMLK();
		long double GetMAlpha_Atr(int Index);
		long double *GetMAlpha_Atr();

		long double GetLAlpha_Atr(int Index);
		long double *GetLAlpha_Atr();

		long double GetMVetor_Atr(int Index);
		long double *GetMVetor_Atr();

		long double GetLVetor_Atr(int Index);
		long double *GetLVetor_Atr();



		long double GetMZo_Atr(int Index);
		long double *GetMZo_Atr();

		long double GetLZo_Atr(int Index);
		long double *GetLZo_Atr();

		long double GetAlpha_B_T(long double *ABT);
		long double *GetAlpha_B_T();
		long double GetAlpha_B_T(int Index);

		long double *GetMAlpha_AtrPtr();

		unsigned int GetCor_Line(int Index);
		unsigned int *GetCor_Line();
		int GetContador_Line();









	};

	 class Objetos
	{
		private:
		bool Visao_XY;
		bool Visao_XZ;
		long double Alpha_B_T[3];


		public :

		//---------------------------------------------------------------------

		//---------------------------------------------------------------------
		Elipse *Elipse_Atr;
		Line *Lines_Atr;
		TQuadrilatero *Quadrilatero;
		bool Chave_Point;
		float **Mxy;
		float **Lxy;
		TList *List;
		TPoint **Point;
		TPoint *P2;





		int Count_Line;
		UnicodeString Teste();
		Objetos();



		void Rotacao_Vetores_3D_Circle_MP2(float xyz[],float xe,float ye, float  ze, float Alpha_Beta_Teta[3]);

		float ** Aloca_P1(int Count);
		TPoint **Aloca_TPoint(int Count);
//		void  Aloca_Point(int count,TPoint ***p1);
		float X(float x);
		float Y(float y);

		void SetVisao_XY(bool);
		bool GetVisao_XY();
		void SetVisao_XZ(bool);
		bool GetVisao_XZ();
		void SetAlpha_B_T(long double ABT,int Index);
		void SetAlpha_B_T(long double ABT[3]);
		long double * GetAlpha_B_T();
		long double GetAlpha_B_T(int Index);
        void GetAlpha_B_T(long double ABT[]);
	};





	class Arquivo{
	public:
		int Count_Line;
		char Nome[100];

		Arquivo();
		Objetos *Abrir_Arquivo(char *Nome_Arquivo, TPoint ***RP_Circle,int ***FullMLK1,UnicodeString *Resposta);
		void Salvar_Arquivo(char *nome_Arquivo,Objetos *Objetos_Atr,UnicodeString *Resposta);

	};








}


#endif
