//---------------------------------------------------------------------------

#ifndef UnitQuadrilateroH
#define UnitQuadrilateroH
//---------------------------------------------------------------------------
#include <vector>
#include "Matriz.h"
#include "Cores.h"

class TQuadrilatero{
		private:
		std::vector<long double> MAlpha;
		std::vector<long double> MVetor;
		std::vector<long double> MAlpha2;
		std::vector<long double> MVetor2;
		std::vector<long double> LAlpha;
		std::vector<long double> LVetor;
		std::vector<long double> LAlpha2;
		std::vector<long double> LVetor2;
		std::vector<long double> MZo;
		std::vector<long double> LZo;
		std::vector<long double> MZo2;
		std::vector<long double> LZo2;
		std::vector<long double> MYo;
		std::vector<long double> LYo;
		std::vector<long double> MYo2;
		std::vector<long double> LYo2;
		std::vector<long double> MXo;
		std::vector<long double> LXo;
		std::vector<long double> MXo2;
		std::vector<long double> LXo2;

		std::vector<COLORREF> Cor_Quadrilatero;
		int ContadorQuadrilatero;

		public:

		TQuadrilatero();
		void AlocarQuadrilatero(int Tam);

		void SetMAlpha(long double,int);
		void SetMAlpha(long double *);

		long double GetMAlpha(int)const;
		long double *GetMAlpha()const;

		void SetMVetor(long double,int);
		void SetMVetor(long double *);

		long double GetMVetor(int) const;
		long double *GetMVetor()const;

		void SetMAlpha2(long double,int);
		void SetMAlpha2(long double *);

		long double GetMAlpha2(int)const;
		long double *GetMAlpha2()const;

		void SetMVetor2(long double,int);
		void SetMVetor2(long double *);

		long double GetMVetor2(int)const;
		long double *GetMVetor2()const;

		void SetLAlpha(long double,int);
		void SetLAlpha(long double *);

		long double GetLAlpha(int)const;
		long double *GetLAlpha()const;

		void SetLVetor(long double,int);
		void SetLVetor(long double *);

		long double GetLVetor(int)const;
		long double *GetLVetor()const;

		void SetLAlpha2(long double,int);
		void SetLAlpha2(long double *);

		long double GetLAlpha2(int)const;
		long double *GetLAlpha2()const;

		void SetLVetor2(long double,int);
		void SetLVetor2(long double*);

		long double GetLVetor2(int)const;
		long double *GetLVetor2()const;

		void SetContadorQuadrilatero(int);
		int GetContadorQuadrilatero()const;

		void SetMZo(long double ,int);
		void SetMZo(long double *);

		long double GetMZo(int)const;
		long double *GetMZo()const;

		void SetLZo(long double, int );
		void SetLZo(long double* );

		long double GetLZo(int)const;
		long double *GetLZo()const;

		void SetMZo2(long double ,int);
		void SetMZo2(long double *);

		long double GetMZo2(int)const;
		long double *GetMZo2()const;

		void SetLZo2(long double, int );
		void SetLZo2(long double *);

		long double GetLZo2(int)const;
		long double *GetLZo2()const;

		void SetMYo(long double,int);
		long double GetMYo(int )const;

		void SetLYo(long double,int);
		long double GetLYo(int)const;

		void SetMYo2(long double,int);
		long double GetMYo2(int)const;

		void SetLYo2(long double,int);
		long double GetLYo2(int)const;

		void SetMXo(long double,int);
		long double GetMXo(int )const;

		void SetLXo(long double,int);
		long double GetLXo(int)const;

		void SetMXo2(long double,int);
		long double GetMXo2(int)const;

		void SetLXo2(long double,int);
		long double GetLXo2(int)const;

		void SetCor(COLORREF ,int);
		void SetCor(COLORREF *);


		COLORREF GetCor(int )const;
		COLORREF *GetCor()const;

		// vetor inteiro: Get devolve uma referencia (sem copia), Set copia todos os valores
		const std::vector<long double> &GetVetor_MAlpha()const;
		void SetVetor_MAlpha(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MVetor()const;
		void SetVetor_MVetor(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LAlpha()const;
		void SetVetor_LAlpha(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LVetor()const;
		void SetVetor_LVetor(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MAlpha2()const;
		void SetVetor_MAlpha2(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MVetor2()const;
		void SetVetor_MVetor2(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LAlpha2()const;
		void SetVetor_LAlpha2(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LVetor2()const;
		void SetVetor_LVetor2(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MXo()const;
		void SetVetor_MXo(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MYo()const;
		void SetVetor_MYo(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MZo()const;
		void SetVetor_MZo(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LXo()const;
		void SetVetor_LXo(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LYo()const;
		void SetVetor_LYo(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LZo()const;
		void SetVetor_LZo(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MXo2()const;
		void SetVetor_MXo2(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MYo2()const;
		void SetVetor_MYo2(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_MZo2()const;
		void SetVetor_MZo2(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LXo2()const;
		void SetVetor_LXo2(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LYo2()const;
		void SetVetor_LYo2(const std::vector<long double> &Valores);
		const std::vector<long double> &GetVetor_LZo2()const;
		void SetVetor_LZo2(const std::vector<long double> &Valores);
		const std::vector<COLORREF> &GetVetor_Cor_Quadrilatero()const;
		void SetVetor_Cor_Quadrilatero(const std::vector<COLORREF> &Valores);

};


#endif
