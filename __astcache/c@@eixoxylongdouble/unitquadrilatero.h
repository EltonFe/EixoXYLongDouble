//---------------------------------------------------------------------------

#ifndef UnitQuadrilateroH
#define UnitQuadrilateroH
//---------------------------------------------------------------------------
#include <malloc.h>

class TQuadrilatero{
		private:
		long double *MAlpha;
		long double *MVetor;
		long double *MAlpha2;
		long double *MVetor2;
		long double *LAlpha;
		long double *LVetor;
		long double *LAlpha2;
		long double *LVetor2;
		long double *MZo;
		long double *LZo;
		long double *MZo2;
		long double *LZo2;
		long double *MYo;
		long double *LYo;
		long double *MYo2;
		long double *LYo2;
		long double *MXo;
		long double *LXo;
		long double *MXo2;
		long double *LXo2;

		unsigned int *Cor_Quadrilatero;
		int ContadorQuadrilatero;

		public:

		TQuadrilatero();
		void AlocarQuadrilatero(int Tam);
		unsigned int  *RealocarQuadrilatero(unsigned int *Ponteiro,int Tamanho);
		long double  *RealocarQuadrilatero(long double *Ponteiro,int Tamanho);

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

		void SetCor(unsigned int ,int);
		void SetCor(unsigned int *);


		unsigned int GetCor(int )const;
		unsigned int *GetCor()const;

};


#endif
