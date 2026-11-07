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
#include "Matriz.h"
#include "Cores.h"
#include <vector>
#include "UnitQuadrilatero.h"

using namespace std;


using namespace std;

namespace Name_Arquivos{

	void LimpaMemoria();






	class Elipse{

	private:
			int ContadorCircle_Atr;

			std::vector<long double> AZo_Atr;
			std::vector<long double> BZo_Atr;
			std::vector<long double> A_Atr;
			std::vector<long double> B_Atr;
			std::vector<long double> CX;
			std::vector<long double> CY;
			std::vector<long double> CZ;
			std::vector<COLORREF> Color_Circle_Atr;
			std::vector<int> TipoElipse_Atr;
			long double Alpha_Beta_Teta_Circle_Atr[3];
			std::vector<int> IndexX;
			int IndexY;
			int IndexZ;

			std::vector<long double> AnguloX;
			std::vector<long double> AnguloY;
            std::vector<long double> AnguloZ;

			Matriz<long double> Center_Atr;	// centro (x,y,z) de cada elipse


	public:
			Elipse();

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

			void SetColor_Circle_Atr(COLORREF *);
			void SetColor_Circle_Atr(COLORREF,int);
			COLORREF *GetColor_Circle_Atr();
			COLORREF GetColor_Circle_Atr(int Index);
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

			// vetor inteiro: Get devolve uma referencia (sem copia), Set copia todos os valores
			const std::vector<long double> &GetVetor_AZo_Atr()const;
			void SetVetor_AZo_Atr(const std::vector<long double> &Valores);
			const std::vector<long double> &GetVetor_BZo_Atr()const;
			void SetVetor_BZo_Atr(const std::vector<long double> &Valores);
			const std::vector<long double> &GetVetor_A_Atr()const;
			void SetVetor_A_Atr(const std::vector<long double> &Valores);
			const std::vector<long double> &GetVetor_B_Atr()const;
			void SetVetor_B_Atr(const std::vector<long double> &Valores);
			const std::vector<long double> &GetVetor_CX()const;
			void SetVetor_CX(const std::vector<long double> &Valores);
			const std::vector<long double> &GetVetor_CY()const;
			void SetVetor_CY(const std::vector<long double> &Valores);
			const std::vector<long double> &GetVetor_CZ()const;
			void SetVetor_CZ(const std::vector<long double> &Valores);
			const std::vector<COLORREF> &GetVetor_Color_Circle_Atr()const;
			void SetVetor_Color_Circle_Atr(const std::vector<COLORREF> &Valores);
			const std::vector<int> &GetVetor_TipoElipse_Atr()const;
			void SetVetor_TipoElipse_Atr(const std::vector<int> &Valores);
			const std::vector<int> &GetVetor_IndexX()const;
			void SetVetor_IndexX(const std::vector<int> &Valores);
			const std::vector<long double> &GetVetor_AnguloX()const;
			void SetVetor_AnguloX(const std::vector<long double> &Valores);
			const std::vector<long double> &GetVetor_AnguloY()const;
			void SetVetor_AnguloY(const std::vector<long double> &Valores);
			const std::vector<long double> &GetVetor_AnguloZ()const;
			void SetVetor_AnguloZ(const std::vector<long double> &Valores);









	};

	class Line{
		private:
		std::vector<long double> MAlpha_Atr;
		std::vector<long double> MVetor_Atr;
		std::vector<long double> LAlpha_Atr;
		std::vector<long double> LVetor_Atr;
		std::vector<long double> MXo_Atr;
		std::vector<long double> LXo_Atr;
		std::vector<long double> MYo_Atr;
		std::vector<long double> LYo_Atr;
		std::vector<long double> MZo_Atr;
		std::vector<long double> LZo_Atr;
		std::vector<COLORREF> Cor_Line;
		int Contador_Line;
		long double Alpha_B_T[3];
		std::vector<long double> Angulo_Alpha;
		std::vector<long double> Angulo_Beta;
		std::vector<long double> Angulo_Teta;

		// refaz MXo/MYo/LXo/LYo a partir de MAlpha/MVetor e LAlpha/LVetor
		void CalculaXoYo();

		public:

		Line();
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


		void SetCor_Line(COLORREF Valor,int Index);
		void SetCor_Line(COLORREF *Ptr);

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
		long double GetMAlpha_Atr(int Index);
		long double *GetMAlpha_Atr();

		long double GetLAlpha_Atr(int Index);
		long double *GetLAlpha_Atr();

		long double GetMVetor_Atr(int Index);
		long double *GetMVetor_Atr();

		long double GetLVetor_Atr(int Index);
		long double *GetLVetor_Atr();

		// x e y de cada ponta, calculados: Xo=cos(Alpha)*Vetor, Yo=sin(Alpha)*Vetor
		long double GetMXo_Atr(int)const;
		long double *GetMXo_Atr();

		long double GetLXo_Atr(int Index)const;
		long double *GetLXo_Atr();




		long double GetMYo_Atr(int)const;
		long double *GetMYo_Atr();

		long double GetLYo_Atr(int)const;
		long double *GetLYo_Atr();

		long double GetMZo_Atr(int Index);
		long double *GetMZo_Atr();

		long double GetLZo_Atr(int Index);
		long double *GetLZo_Atr();

		long double GetAlpha_B_T(long double *ABT);
		long double *GetAlpha_B_T();
		long double GetAlpha_B_T(int Index);

		long double *GetMAlpha_AtrPtr();

		COLORREF GetCor_Line(int Index);
		COLORREF *GetCor_Line();
		int GetContador_Line();

		// vetor inteiro: Get devolve uma referencia (sem copia), Set copia todos os valores
		const std::vector<long double> &GetVetor_MAlpha_Atr()const;
		void SetVetor_MAlpha_Atr(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MVetor_Atr()const;
		void SetVetor_MVetor_Atr(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LAlpha_Atr()const;
		void SetVetor_LAlpha_Atr(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LVetor_Atr()const;
		void SetVetor_LVetor_Atr(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MXo_Atr();
		void SetVetor_MXo_Atr(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MYo_Atr();
		void SetVetor_MYo_Atr(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MZo_Atr()const;
		void SetVetor_MZo_Atr(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LXo_Atr();
		void SetVetor_LXo_Atr(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LYo_Atr();
		void SetVetor_LYo_Atr(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LZo_Atr()const;
		void SetVetor_LZo_Atr(const std::vector<long double> &Valores);
		const std::vector<COLORREF> &GetVetor_Cor_Line()const;
		void SetVetor_Cor_Line(const std::vector<COLORREF> &Valores);
		const std::vector<long double> &GetVetor_Angulo_Alpha()const;
		void SetVetor_Angulo_Alpha(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_Angulo_Beta()const;
		void SetVetor_Angulo_Beta(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_Angulo_Teta()const;
		void SetVetor_Angulo_Teta(const std::vector<long double> &Valores);









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
		~Objetos();

		// cada Objetos e dono de Elipse_Atr, Lines_Atr, Quadrilatero, List e P2:
		// uma copia apagaria a mesma memoria duas vezes
		Objetos(const Objetos &)=delete;
		Objetos &operator=(const Objetos &)=delete;



		void Rotacao_Vetores_3D_Circle_MP2(float xyz[],float xe,float ye, float  ze, float Alpha_Beta_Teta[3]);

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

		// Desenha linhas, elipses e quadrilateros de Objetos_Atr no Canvas do EixoXY
		void Draw(Objetos *Objetos_Atr);
	};





	class Arquivo{
	public:
		int Count_Line;
		char Nome[100];

		Arquivo();
		// so le o arquivo: quem chama acerta os seus buffers de tela
		Objetos *Abrir_Arquivo(char *Nome_Arquivo,UnicodeString *Resposta);
		void Salvar_Arquivo(char *nome_Arquivo,Objetos *Objetos_Atr,UnicodeString *Resposta);

	};

	// Exporta/importa o desenho no formato glTF 2.0 binario (.glb), lido pelo
	// GLTFLoader do three.js, Blender etc. O formato esta descrito em GLB.cpp.
	class ExportGLB{
	public:
		long double Escala;		// metros do glTF por unidade do EixoXY (padrao 0.01)

		ExportGLB();
		void Exportar(UnicodeString Nome_Arquivo,Objetos *Objetos_Atr,UnicodeString *Resposta);
		Objetos *Importar(UnicodeString Nome_Arquivo,UnicodeString *Resposta);

	};








}


#endif
