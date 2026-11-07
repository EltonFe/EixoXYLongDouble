//---------------------------------------------------------------------------
// GLB.cpp - glTF 2.0 binario (.glb)
//
// Estrutura do arquivo gerado:
//   Cabecalho (12 bytes): "glTF", versao 2, tamanho total
//   Chunk JSON: cena, nos, malhas, accessors, bufferViews, material
//   Chunk BIN : posicoes e cores (float32)
//
// Malhas geradas:
//   "Linhas"         -> mode 1 (LINES)
//   "Quadrilateros"  -> mode 4 (TRIANGLES), 2 triangulos por quadrilatero
//   "Elipses"        -> mode 1 (LINES), 50 segmentos por elipse
//
// Os dados originais do EixoXY sao guardados em scenes[0].extras.EixoXY,
// o que permite reabrir o .glb sem perda. No three.js eles aparecem em
// gltf.scene.userData.EixoXY.
//---------------------------------------------------------------------------

#include "GLB.h"

#include <cfloat>
#include <clocale>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <set>
#include <utility>

namespace GLB{

namespace{

const double PI=3.14159265358979323846;

//--- Utilitarios -----------------------------------------------------------

double Finito(double v){

	return std::isfinite(v)?v:0.0;
}

//--- Numeros decimais exatos -----------------------------------------------
// O snprintf("%.17g") e o strtod do runtime da Embarcadero erram o ultimo
// digito/bit. Para que o .glb guarde os double do desenho sem perda, a
// conversao decimal e feita aqui com aritmetica inteira.

// v = m * 2^e exatamente (v >= 0)
void Decompoe(double v,uint64_t &m,int &e){

	double f=std::frexp(v,&e);
	m=(uint64_t)std::ldexp(f,53);
	e-=53;
}

// m * 2^e em decimal exato: valor = 0.D * 10^Exp10 (D sem zeros nas pontas)
void DecimalExato(uint64_t m,int e,std::string &D,int &Exp10){

	static const uint32_t Pot5[14]={1,5,25,125,625,3125,15625,78125,390625,1953125,
									9765625,48828125,244140625,1220703125};
	char Buffer[16];

	D.clear();
	Exp10=0;
	if(m==0) return;

	// inteiro grande em base 1e9 (menos significativo primeiro) = valor * 10^Casas
	std::vector<uint32_t> N;
	N.push_back((uint32_t)(m%1000000000ULL));
	N.push_back((uint32_t)((m/1000000000ULL)%1000000000ULL));
	N.push_back((uint32_t)(m/1000000000000000000ULL));

	int Casas=e<0?-e:0;					// m*2^-k = m*5^k / 10^k
	int Vezes=e<0?-e:e;

	while(Vezes>0){
		int k=e>0?(Vezes<30?Vezes:30):(Vezes<13?Vezes:13);
		uint64_t Fator=e>0?(1ULL<<k):Pot5[k];
		uint64_t Vai=0;
		for(size_t i=0;i<N.size();i++){
			uint64_t t=N[i]*Fator+Vai;
			N[i]=(uint32_t)(t%1000000000ULL);
			Vai=t/1000000000ULL;
		}
		while(Vai){ N.push_back((uint32_t)(Vai%1000000000ULL)); Vai/=1000000000ULL; }
		Vezes-=k;
	}

	while(N.size()>1&&N.back()==0) N.pop_back();

	std::snprintf(Buffer,sizeof(Buffer),"%u",N.back());
	D=Buffer;
	for(size_t i=N.size()-1;i-->0;){
		std::snprintf(Buffer,sizeof(Buffer),"%09u",N[i]);
		D+=Buffer;
	}

	Exp10=(int)D.size()-Casas;
	while(D.size()>1&&D[D.size()-1]=='0') D.resize(D.size()-1);
}

// compara 0.A*10^EA com 0.B*10^EB (ambos positivos)
int ComparaDecimal(const std::string &A,int EA,const std::string &B,int EB){

	if(A.empty()||B.empty()) return A.empty()?(B.empty()?0:-1):1;
	if(EA!=EB) return EA<EB?-1:1;

	size_t n=A.size()>B.size()?A.size():B.size();
	for(size_t i=0;i<n;i++){
		char a=i<A.size()?A[i]:'0';
		char b=i<B.size()?B[i]:'0';
		if(a!=b) return a<b?-1:1;
	}
	return 0;
}

// ponto medio exato entre dois double vizinhos (a < b, ambos >= 0)
void PontoMedio(double a,double b,std::string &D,int &Exp10){

	uint64_t ma,mb;
	int ea,eb;
	Decompoe(a,ma,ea);
	Decompoe(b,mb,eb);
	if(ma==0) ea=eb;
	if(mb==0) eb=ea;

	int em=ea<eb?ea:eb;
	DecimalExato((ma<<(ea-em))+(mb<<(eb-em)),em-1,D,Exp10);
}

bool MantissaImpar(double v){

	uint64_t Bits;
	std::memcpy(&Bits,&v,8);
	return (Bits&1)!=0;
}

// Converte double em texto com 17 digitos significativos (releitura sem perda)
std::string Num(double v){

char Buffer[40];

	v=Finito(v);
	if(v==std::floor(v)&&std::fabs(v)<1e15){
		std::snprintf(Buffer,sizeof(Buffer),"%.0f",v);
		return Buffer;
	}

	bool Negativo=v<0;
	if(Negativo) v=-v;

	uint64_t m;
	int e,Exp10;
	std::string D;
	Decompoe(v,m,e);
	DecimalExato(m,e,D,Exp10);

	if(D.size()>17){
		bool Sobe=D[17]>='5';
		D.resize(17);
		if(Sobe){
			int i=16;
			while(i>=0&&D[i]=='9') D[i--]='0';
			if(i<0){ D.insert(D.begin(),'1'); D.resize(17); Exp10++; }
			else D[i]++;
		}
		while(D.size()>1&&D[D.size()-1]=='0') D.resize(D.size()-1);
	}

	std::string S=Negativo?"-":"";
	S+=D[0];
	if(D.size()>1){ S+='.'; S+=D.substr(1); }
	if(Exp10-1!=0){
		std::snprintf(Buffer,sizeof(Buffer),"e%d",Exp10-1);
		S+=Buffer;
	}

	return S;
}

// Le um numero JSON e devolve o double mais proximo (arredondamento correto)
bool TextoParaDouble(const std::string &T,double &N){

	size_t i=0,n=T.size();
	bool Negativo=false;
	if(i<n&&(T[i]=='-'||T[i]=='+')) Negativo=T[i++]=='-';

	std::string D;
	int AntesDoPonto=0;
	bool Ponto=false;
	for(;i<n&&((T[i]>='0'&&T[i]<='9')||T[i]=='.');i++){
		if(T[i]=='.'){ if(Ponto) return false; Ponto=true; continue; }
		D+=T[i];
		if(!Ponto) AntesDoPonto++;
	}
	if(D.empty()) return false;

	long Expoente=0;
	if(i<n&&(T[i]=='e'||T[i]=='E')){
		i++;
		bool ExpNeg=false;
		if(i<n&&(T[i]=='-'||T[i]=='+')) ExpNeg=T[i++]=='-';
		if(i>=n) return false;
		for(;i<n&&T[i]>='0'&&T[i]<='9';i++)
			if(Expoente<100000) Expoente=Expoente*10+(T[i]-'0');
		if(ExpNeg) Expoente=-Expoente;
	}
	if(i!=n) return false;

	// valor = 0.D * 10^X
	long X=AntesDoPonto+Expoente;
	size_t Zeros=0;
	while(Zeros<D.size()&&D[Zeros]=='0') Zeros++;
	D.erase(0,Zeros);
	X-=(long)Zeros;
	while(!D.empty()&&D[D.size()-1]=='0') D.resize(D.size()-1);

	if(D.empty()||X<-400){ N=Negativo?-0.0:0.0; return true; }
	if(X>400){ N=Negativo?-HUGE_VAL:HUGE_VAL; return true; }
	if(D.size()>800) D.resize(800);

	// aproximacao inicial: 0.D (ate 19 digitos) * 10^X, com a potencia
	// dividida em duas para nao zerar antes da hora perto do underflow
	double v=0;
	for(size_t k=0;k<D.size()&&k<19;k++) v=v*10+(D[k]-'0');
	long Digitos=(long)(D.size()<19?D.size():19);
	long P1=(X-Digitos)/2,P2=(X-Digitos)-P1;
	v=v*std::pow(10.0,(double)P1)*std::pow(10.0,(double)P2);
	if(!std::isfinite(v)) v=DBL_MAX;

	// correcao: escolhe o double cujo intervalo de arredondamento contem o valor
	for(int k=0;k<256;k++){
		std::string DM;
		int EM;
		if(v>0){
			double Anterior=std::nextafter(v,0.0);
			PontoMedio(Anterior,v,DM,EM);
			int c=ComparaDecimal(D,(int)X,DM,EM);
			if(c<0||(c==0&&MantissaImpar(v))){ v=Anterior; continue; }
		}
		double Proximo=std::nextafter(v,HUGE_VAL);
		if(std::isfinite(Proximo)){
			PontoMedio(v,Proximo,DM,EM);
			int c=ComparaDecimal(D,(int)X,DM,EM);
			if(c>0||(c==0&&MantissaImpar(v))){ v=Proximo; continue; }
		}
		break;
	}

	N=Negativo?-v:v;
	return true;
}

std::string NumF(float v){

char Buffer[40];

	std::snprintf(Buffer,sizeof(Buffer),"%.9g",(double)v);
	for(char *p=Buffer;*p;p++)
		if(*p==',') *p='.';

	return Buffer;
}

void Polar(double Alpha,double Vetor,double Zo,double P[3]){

	P[0]=std::cos(Alpha)*Vetor;
	P[1]=std::sin(Alpha)*Vetor;
	P[2]=Zo;
}

// rotacao propria da linha: Rx(Alpha) * Rz(Teta)
void RotacionaLinha(double P[3],double Alpha,double Teta){

	double ct=std::cos(Finito(Teta)),st=std::sin(Finito(Teta));
	double ca=std::cos(Finito(Alpha)),sa=std::sin(Finito(Alpha));

	double x=P[0]*ct-P[1]*st;
	double y=P[0]*st+P[1]*ct;
	double z=P[2];

	P[0]=x;
	P[1]=y*ca-z*sa;
	P[2]=y*sa+z*ca;
}

//--- Cores: TColor (sRGB) <-> COLOR_0 do glTF (linear) ---------------------

double SrgbParaLinear(double c){

	return c<=0.04045 ? c/12.92 : std::pow((c+0.055)/1.055,2.4);
}

double LinearParaSrgb(double c){

	if(!(c>0)) return 0;
	if(c>=1) return 1;
	return c<=0.0031308 ? c*12.92 : 1.055*std::pow(c,1/2.4)-0.055;
}

void CorParaRGB(unsigned int Cor,float Rgb[3]){

	Rgb[0]=(float)SrgbParaLinear(( Cor     &0xFF)/255.0);
	Rgb[1]=(float)SrgbParaLinear(((Cor>>8 )&0xFF)/255.0);
	Rgb[2]=(float)SrgbParaLinear(((Cor>>16)&0xFF)/255.0);
}

unsigned int RGBParaCor(double R,double G,double B){

unsigned int r=(unsigned int)std::floor(LinearParaSrgb(R)*255+0.5);
unsigned int g=(unsigned int)std::floor(LinearParaSrgb(G)*255+0.5);
unsigned int b=(unsigned int)std::floor(LinearParaSrgb(B)*255+0.5);

	return r|(g<<8)|(b<<16);
}

//--- Leitura/Gravacao binaria little-endian --------------------------------

void GravaU32(std::vector<unsigned char> &V,uint32_t x){

	V.push_back((unsigned char)( x     &0xFF));
	V.push_back((unsigned char)((x>>8 )&0xFF));
	V.push_back((unsigned char)((x>>16)&0xFF));
	V.push_back((unsigned char)((x>>24)&0xFF));
}

uint32_t LeU32(const unsigned char *p){

	return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}

//--- Geometria acumulada para uma malha ------------------------------------

struct Primitiva{

	std::vector<float> Pos;
	std::vector<float> Cor;

	void Add(const double P[3],const float C[3],double Escala){

		for(int i=0;i<3;i++){
			double v=Finito(P[i])*Escala;
			if(v> FLT_MAX) v= FLT_MAX;		// float32 do glTF
			if(v<-FLT_MAX) v=-FLT_MAX;
			Pos.push_back((float)v);
			Cor.push_back(C[i]);
		}
	}
};

//--- JSON minimo (leitura) -------------------------------------------------

struct Json{

	enum TipoJson{Nulo,Booleano,Numero,Texto,Lista,Objeto};

	TipoJson Tipo;
	double N;
	bool B;
	std::string S;
	std::vector<Json> A;
	std::vector<std::pair<std::string,Json> > O;

	Json():Tipo(Nulo),N(0),B(false){}

	const Json *Campo(const char *Chave) const{

		if(Tipo!=Objeto) return 0;
		for(size_t i=0;i<O.size();i++)
			if(O[i].first==Chave) return &O[i].second;
		return 0;
	}

	const Json *Item(size_t i) const{

		if(Tipo!=Lista||i>=A.size()) return 0;
		return &A[i];
	}
};

class LeitorJson{

	const char *p;
	const char *Fim;
	int Profundidade;

	void Espacos(){

		while(p<Fim&&(*p==' '||*p=='\t'||*p=='\n'||*p=='\r')) p++;
	}

	bool Texto(std::string &S){

		if(p>=Fim||*p!='"') return false;
		p++;
		while(p<Fim&&*p!='"'){
			if(*p=='\\'){
				p++;
				if(p>=Fim) return false;
				char c=*p++;
				switch(c){
					case 'n': S+='\n'; break;
					case 't': S+='\t'; break;
					case 'r': S+='\r'; break;
					case 'b': S+='\b'; break;
					case 'f': S+='\f'; break;
					case 'u':{
						if(Fim-p<4) return false;
						unsigned int u=0;
						for(int i=0;i<4;i++){
							char h=*p++;
							u<<=4;
							if(h>='0'&&h<='9') u|=h-'0';
							else if(h>='a'&&h<='f') u|=h-'a'+10;
							else if(h>='A'&&h<='F') u|=h-'A'+10;
							else return false;
						}
						// grava em UTF-8 (pares substitutos viram '?')
						if(u<0x80) S+=(char)u;
						else if(u<0x800){ S+=(char)(0xC0|(u>>6)); S+=(char)(0x80|(u&0x3F)); }
						else if(u>=0xD800&&u<=0xDFFF) S+='?';
						else{ S+=(char)(0xE0|(u>>12)); S+=(char)(0x80|((u>>6)&0x3F)); S+=(char)(0x80|(u&0x3F)); }
						break;
					}
					default: S+=c; break;
				}
			}else
				S+=*p++;
		}
		if(p>=Fim) return false;
		p++;
		return true;
	}

	bool Numero(double &N){

		const char *Inicio=p;
		while(p<Fim&&*p&&(std::strchr("+-0123456789.eE",*p)!=0)) p++;
		if(p==Inicio) return false;

		return TextoParaDouble(std::string(Inicio,p),N);
	}

	bool Palavra(const char *W){

		size_t n=std::strlen(W);
		if((size_t)(Fim-p)<n||std::strncmp(p,W,n)!=0) return false;
		p+=n;
		return true;
	}

public:

	LeitorJson(const char *Texto,size_t Tamanho):p(Texto),Fim(Texto+Tamanho),Profundidade(0){}

	bool Valor(Json &J){

		Espacos();
		if(p>=Fim) return false;
		if(++Profundidade>200) return false;

		bool Ok=true;

		if(*p=='{'){
			J.Tipo=Json::Objeto;
			p++;
			Espacos();
			if(p<Fim&&*p=='}') p++;
			else for(;;){
				std::string Chave;
				Espacos();
				if(!Texto(Chave)){ Ok=false; break; }
				Espacos();
				if(p>=Fim||*p!=':'){ Ok=false; break; }
				p++;
				J.O.push_back(std::make_pair(Chave,Json()));
				if(!Valor(J.O.back().second)){ Ok=false; break; }
				Espacos();
				if(p<Fim&&*p==','){ p++; continue; }
				if(p<Fim&&*p=='}'){ p++; break; }
				Ok=false;
				break;
			}
		}else if(*p=='['){
			J.Tipo=Json::Lista;
			p++;
			Espacos();
			if(p<Fim&&*p==']') p++;
			else for(;;){
				J.A.push_back(Json());
				if(!Valor(J.A.back())){ Ok=false; break; }
				Espacos();
				if(p<Fim&&*p==','){ p++; continue; }
				if(p<Fim&&*p==']'){ p++; break; }
				Ok=false;
				break;
			}
		}else if(*p=='"'){
			J.Tipo=Json::Texto;
			Ok=Texto(J.S);
		}else if(Palavra("true")){
			J.Tipo=Json::Booleano; J.B=true;
		}else if(Palavra("false")){
			J.Tipo=Json::Booleano; J.B=false;
		}else if(Palavra("null")){
			J.Tipo=Json::Nulo;
		}else{
			J.Tipo=Json::Numero;
			Ok=Numero(J.N);
		}

		Profundidade--;
		return Ok;
	}
};

double NumeroDe(const Json *J,double Padrao){

	return (J&&J->Tipo==Json::Numero)?J->N:Padrao;
}

long long InteiroDe(const Json *J,long long Padrao){

	return (J&&J->Tipo==Json::Numero)?(long long)J->N:Padrao;
}

//--- Matriz 4x4 (coluna-maior, como no glTF) -------------------------------

struct Matriz{

	double m[16];

	Matriz(){ for(int i=0;i<16;i++) m[i]=(i%5==0)?1:0; }

	Matriz operator*(const Matriz &B) const{

		Matriz C;
		for(int c=0;c<4;c++)
			for(int r=0;r<4;r++){
				double s=0;
				for(int k=0;k<4;k++) s+=m[k*4+r]*B.m[c*4+k];
				C.m[c*4+r]=s;
			}
		return C;
	}

	void Aplica(const double P[3],double R[3]) const{

		for(int r=0;r<3;r++)
			R[r]=m[r]*P[0]+m[4+r]*P[1]+m[8+r]*P[2]+m[12+r];
	}
};

Matriz MatrizDoNo(const Json &No){

	Matriz M;
	const Json *Mat=No.Campo("matrix");

	if(Mat&&Mat->Tipo==Json::Lista&&Mat->A.size()==16){
		for(int i=0;i<16;i++) M.m[i]=NumeroDe(Mat->Item(i),0);
		return M;
	}

	double T[3]={0,0,0},Q[4]={0,0,0,1},S[3]={1,1,1};
	const Json *JT=No.Campo("translation");
	const Json *JR=No.Campo("rotation");
	const Json *JS=No.Campo("scale");
	for(int i=0;i<3;i++){
		if(JT) T[i]=NumeroDe(JT->Item(i),0);
		if(JS) S[i]=NumeroDe(JS->Item(i),1);
	}
	if(JR) for(int i=0;i<4;i++) Q[i]=NumeroDe(JR->Item(i),i==3?1:0);

	double x=Q[0],y=Q[1],z=Q[2],w=Q[3];
	M.m[0]=(1-2*(y*y+z*z))*S[0]; M.m[1]=(2*(x*y+z*w))*S[0];   M.m[2]=(2*(x*z-y*w))*S[0];
	M.m[4]=(2*(x*y-z*w))*S[1];   M.m[5]=(1-2*(x*x+z*z))*S[1]; M.m[6]=(2*(y*z+x*w))*S[1];
	M.m[8]=(2*(x*z+y*w))*S[2];   M.m[9]=(2*(y*z-x*w))*S[2];   M.m[10]=(1-2*(x*x+y*y))*S[2];
	M.m[12]=T[0]; M.m[13]=T[1]; M.m[14]=T[2];

	return M;
}

//--- Leitura de accessors --------------------------------------------------

bool LerAccessor(const Json &Raiz,const std::vector<unsigned char> &Bin,long long Indice,
				 std::vector<double> &Saida,int &Componentes,std::string &Erro){

	const Json *Acc=Raiz.Campo("accessors")?Raiz.Campo("accessors")->Item((size_t)Indice):0;
	if(!Acc||Indice<0){ Erro="accessor invalido"; return false; }

	const Json *Tipo=Acc->Campo("type");
	std::string T=Tipo?Tipo->S:"";
	Componentes= T=="SCALAR"?1: T=="VEC2"?2: T=="VEC3"?3: T=="VEC4"?4: 0;
	if(Componentes==0){ Erro="tipo de accessor nao suportado: "+T; return false; }

	long long Count=InteiroDe(Acc->Campo("count"),0);
	if(Count<0||Count>50000000){ Erro="quantidade invalida no accessor"; return false; }

	const Json *JBV=Acc->Campo("bufferView");
	if(!JBV){ Erro="accessor sem bufferView (sparse) nao suportado"; return false; }

	const Json *BV=Raiz.Campo("bufferViews")?Raiz.Campo("bufferViews")->Item((size_t)InteiroDe(JBV,-1)):0;
	if(!BV){ Erro="bufferView invalido"; return false; }

	long long Buffer=InteiroDe(BV->Campo("buffer"),0);
	const Json *Buf=Raiz.Campo("buffers")?Raiz.Campo("buffers")->Item((size_t)Buffer):0;
	if(!Buf||Buffer!=0||Buf->Campo("uri")){
		Erro="buffer externo (.bin separado) nao suportado; use um .glb";
		return false;
	}

	int TipoComp=(int)InteiroDe(Acc->Campo("componentType"),0);
	int TamComp= (TipoComp==5120||TipoComp==5121)?1: (TipoComp==5122||TipoComp==5123)?2:
				 (TipoComp==5125||TipoComp==5126)?4: 0;
	if(TamComp==0){ Erro="componentType nao suportado"; return false; }

	const Json *JNorm=Acc->Campo("normalized");
	bool Normalizado=JNorm&&JNorm->Tipo==Json::Booleano&&JNorm->B;

	long long OffBV =InteiroDe(BV->Campo("byteOffset"),0);
	long long TamBV =InteiroDe(BV->Campo("byteLength"),0);
	long long OffAcc=InteiroDe(Acc->Campo("byteOffset"),0);
	long long TamElem=(long long)Componentes*TamComp;
	long long Passo=InteiroDe(BV->Campo("byteStride"),0);
	if(Passo<=0) Passo=TamElem;

	long long Inicio=OffBV+OffAcc;
	if(OffBV<0||TamBV<0||OffAcc<0||OffBV+TamBV>(long long)Bin.size()||
	   (Count>0&&Inicio+Passo*(Count-1)+TamElem>OffBV+TamBV)){
		Erro="accessor fora dos limites do buffer";
		return false;
	}

	Saida.resize((size_t)(Count*Componentes));

	for(long long i=0;i<Count;i++){
		const unsigned char *p=&Bin[(size_t)(Inicio+Passo*i)];
		for(int c=0;c<Componentes;c++,p+=TamComp){
			double v=0;
			switch(TipoComp){
				case 5120: v=(signed char)p[0];                         if(Normalizado) v=v/127.0<-1?-1:v/127.0; break;
				case 5121: v=p[0];                                      if(Normalizado) v/=255.0;   break;
				case 5122: v=(int16_t)(p[0]|(p[1]<<8));                 if(Normalizado) v=v/32767.0<-1?-1:v/32767.0; break;
				case 5123: v=(uint16_t)(p[0]|(p[1]<<8));                if(Normalizado) v/=65535.0; break;
				case 5125: v=LeU32(p);                                  break;
				case 5126:{ uint32_t u=LeU32(p); float f; std::memcpy(&f,&u,4); v=f; break; }
			}
			Saida[(size_t)(i*Componentes+c)]=v;
		}
	}

	return true;
}

//--- Exportacao: montagem do JSON ------------------------------------------

std::string Juntar(const std::vector<std::string> &V){

	std::string S;
	for(size_t i=0;i<V.size();i++){
		if(i) S+=",";
		S+=V[i];
	}
	return S;
}

struct MontadorGLB{

	std::vector<unsigned char> Bin;
	std::vector<std::string> BufferViews,Accessors,Meshes,Nodes;

	int AddView(const std::vector<float> &Dados){

		size_t Offset=Bin.size();
		size_t Tam=Dados.size()*sizeof(float);
		Bin.resize(Offset+Tam);
		if(Tam) std::memcpy(&Bin[Offset],&Dados[0],Tam);	// Windows e little-endian

		BufferViews.push_back("{\"buffer\":0,\"byteOffset\":"+Num((double)Offset)+
							  ",\"byteLength\":"+Num((double)Tam)+",\"target\":34962}");
		return (int)BufferViews.size()-1;
	}

	void AddMalha(const char *Nome,const Primitiva &P,int Modo){

		if(P.Pos.empty()) return;

		float Min[3],Max[3];
		for(int c=0;c<3;c++){ Min[c]=P.Pos[c]; Max[c]=P.Pos[c]; }
		for(size_t i=0;i<P.Pos.size();i++){
			int c=(int)(i%3);
			if(P.Pos[i]<Min[c]) Min[c]=P.Pos[i];
			if(P.Pos[i]>Max[c]) Max[c]=P.Pos[i];
		}

		std::string Count=Num((double)(P.Pos.size()/3));

		int VPos=AddView(P.Pos);
		Accessors.push_back("{\"bufferView\":"+Num(VPos)+",\"componentType\":5126,\"count\":"+Count+
			",\"type\":\"VEC3\",\"min\":["+NumF(Min[0])+","+NumF(Min[1])+","+NumF(Min[2])+
			"],\"max\":["+NumF(Max[0])+","+NumF(Max[1])+","+NumF(Max[2])+"]}");
		int APos=(int)Accessors.size()-1;

		int VCor=AddView(P.Cor);
		Accessors.push_back("{\"bufferView\":"+Num(VCor)+",\"componentType\":5126,\"count\":"+Count+
			",\"type\":\"VEC3\"}");
		int ACor=(int)Accessors.size()-1;

		Meshes.push_back(std::string("{\"name\":\"")+Nome+"\",\"primitives\":[{\"attributes\":{\"POSITION\":"+
			Num(APos)+",\"COLOR_0\":"+Num(ACor)+"},\"mode\":"+Num(Modo)+",\"material\":0}]}");
		Nodes.push_back(std::string("{\"name\":\"")+Nome+"\",\"mesh\":"+Num((double)Meshes.size()-1)+"}");
	}
};

std::string ExtrasEixoXY(const Desenho &D,double Escala){

	std::string S="{\"versao\":1,\"escala\":"+Num(Escala)+",\"alpha_b_t\":["+
		Num(D.Alpha_B_T[0])+","+Num(D.Alpha_B_T[1])+","+Num(D.Alpha_B_T[2])+"]";

	std::vector<std::string> V;

	for(size_t i=0;i<D.Linhas.size();i++){
		const Linha &L=D.Linhas[i];
		V.push_back("["+Num(L.MAlpha)+","+Num(L.MVetor)+","+Num(L.MZo)+","+
					Num(L.LAlpha)+","+Num(L.LVetor)+","+Num(L.LZo)+","+Num(L.Cor)+","+
					Num(L.AnguloAlpha)+","+Num(L.AnguloBeta)+","+Num(L.AnguloTeta)+"]");
	}
	S+=",\"linhas\":["+Juntar(V)+"]";
	V.clear();

	for(size_t i=0;i<D.Quadrilateros.size();i++){
		const Quadrilatero &Q=D.Quadrilateros[i];
		V.push_back("["+Num(Q.MAlpha)+","+Num(Q.MVetor)+","+Num(Q.MZo)+","+
					Num(Q.LAlpha)+","+Num(Q.LVetor)+","+Num(Q.LZo)+","+
					Num(Q.MAlpha2)+","+Num(Q.MVetor2)+","+Num(Q.MZo2)+","+
					Num(Q.LAlpha2)+","+Num(Q.LVetor2)+","+Num(Q.LZo2)+","+Num(Q.Cor)+"]");
	}
	S+=",\"quadrilateros\":["+Juntar(V)+"]";
	V.clear();

	for(size_t i=0;i<D.Elipses.size();i++){
		const Elipse &E=D.Elipses[i];
		V.push_back("["+Num(E.A)+","+Num(E.B)+","+Num(E.AZo)+","+Num(E.BZo)+","+
					Num(E.CX)+","+Num(E.CY)+","+Num(E.CZ)+","+
					Num(E.AnguloX)+","+Num(E.AnguloY)+","+Num(E.AnguloZ)+","+
					Num(E.Tipo)+","+Num(E.Cor)+"]");
	}
	S+=",\"elipses\":["+Juntar(V)+"]}";

	return S;
}

//--- Importacao ------------------------------------------------------------

bool LerArquivo(const wchar_t *Nome_Arquivo,std::vector<unsigned char> &Dados){

	FILE *F=_wfopen(Nome_Arquivo,L"rb");
	if(!F) return false;

	unsigned char Buffer[65536];
	size_t n;
	while((n=std::fread(Buffer,1,sizeof(Buffer),F))>0)
		Dados.insert(Dados.end(),Buffer,Buffer+n);

	bool Ok=!std::ferror(F);
	std::fclose(F);
	return Ok;
}

// Recupera os dados exatos gravados pelo proprio EixoXY
bool ImportarExtras(const Json &Raiz,const Json &Extras,Desenho &D){

	const Json *ABT=Extras.Campo("alpha_b_t");
	for(int i=0;i<3;i++) D.Alpha_B_T[i]=NumeroDe(ABT?ABT->Item(i):0,0);

	const Json *JL=Extras.Campo("linhas");
	if(JL&&JL->Tipo==Json::Lista)
		for(size_t i=0;i<JL->A.size();i++){
			const Json &R=JL->A[i];
			if(R.Tipo!=Json::Lista||R.A.size()<7) continue;
			Linha L;
			L.MAlpha=R.A[0].N; L.MVetor=R.A[1].N; L.MZo=R.A[2].N;
			L.LAlpha=R.A[3].N; L.LVetor=R.A[4].N; L.LZo=R.A[5].N;
			L.Cor=(unsigned int)R.A[6].N;
			if(R.A.size()>=10){				// rotacao da linha (versao com angulos)
				L.AnguloAlpha=R.A[7].N;
				L.AnguloBeta=R.A[8].N;
				L.AnguloTeta=R.A[9].N;
			}
			D.Linhas.push_back(L);
		}

	const Json *JQ=Extras.Campo("quadrilateros");
	if(JQ&&JQ->Tipo==Json::Lista)
		for(size_t i=0;i<JQ->A.size();i++){
			const Json &R=JQ->A[i];
			if(R.Tipo!=Json::Lista||R.A.size()<13) continue;
			Quadrilatero Q;
			Q.MAlpha =R.A[0].N; Q.MVetor =R.A[1].N;  Q.MZo =R.A[2].N;
			Q.LAlpha =R.A[3].N; Q.LVetor =R.A[4].N;  Q.LZo =R.A[5].N;
			Q.MAlpha2=R.A[6].N; Q.MVetor2=R.A[7].N;  Q.MZo2=R.A[8].N;
			Q.LAlpha2=R.A[9].N; Q.LVetor2=R.A[10].N; Q.LZo2=R.A[11].N;
			Q.Cor=(unsigned int)R.A[12].N;
			D.Quadrilateros.push_back(Q);
		}

	const Json *JE=Extras.Campo("elipses");
	if(JE&&JE->Tipo==Json::Lista)
		for(size_t i=0;i<JE->A.size();i++){
			const Json &R=JE->A[i];
			if(R.Tipo!=Json::Lista||R.A.size()<12) continue;
			Elipse E;
			E.A=R.A[0].N;  E.B=R.A[1].N;  E.AZo=R.A[2].N; E.BZo=R.A[3].N;
			E.CX=R.A[4].N; E.CY=R.A[5].N; E.CZ=R.A[6].N;
			E.AnguloX=R.A[7].N; E.AnguloY=R.A[8].N; E.AnguloZ=R.A[9].N;
			E.Tipo=(int)R.A[10].N;
			E.Cor=(unsigned int)R.A[11].N;
			D.Elipses.push_back(E);
		}

	(void)Raiz;
	return true;
}

void AddLinhaCartesiana(Desenho &D,const double P1[3],const double P2[3],unsigned int Cor,double Escala){

	Linha L;
	double x1=P1[0]/Escala,y1=P1[1]/Escala,z1=P1[2]/Escala;
	double x2=P2[0]/Escala,y2=P2[1]/Escala,z2=P2[2]/Escala;

	L.MVetor=std::sqrt(x1*x1+y1*y1); L.MAlpha=std::atan2(y1,x1); L.MZo=z1;
	L.LVetor=std::sqrt(x2*x2+y2*y2); L.LAlpha=std::atan2(y2,x2); L.LZo=z2;
	L.Cor=Cor;

	D.Linhas.push_back(L);
}

// Converte a geometria de um .glb qualquer em linhas
bool ImportarPrimitiva(const Json &Raiz,const std::vector<unsigned char> &Bin,const Json &Prim,
					   const Matriz &M,Desenho &D,double Escala,std::string &Erro){

	const Json *Attr=Prim.Campo("attributes");
	const Json *JPos=Attr?Attr->Campo("POSITION"):0;
	if(!JPos) return true;

	int Modo=(int)InteiroDe(Prim.Campo("mode"),4);
	if(Modo<1||Modo>6) return true;			// pontos: ignorados

	std::vector<double> Pos,Cor,Idx;
	int CompPos=0,CompCor=0,CompIdx=0;

	if(!LerAccessor(Raiz,Bin,InteiroDe(JPos,-1),Pos,CompPos,Erro)) return false;
	if(CompPos!=3){ Erro="POSITION deve ser VEC3"; return false; }
	size_t NV=Pos.size()/3;

	const Json *JCor=Attr->Campo("COLOR_0");
	if(JCor&&!LerAccessor(Raiz,Bin,InteiroDe(JCor,-1),Cor,CompCor,Erro)) return false;
	if(CompCor<3) Cor.clear();

	// cor do material, se nao houver cor por vertice
	unsigned int CorMaterial=0;			// preto (clBlack)
	const Json *Mats=Raiz.Campo("materials");
	const Json *Mat=Mats?Mats->Item((size_t)InteiroDe(Prim.Campo("material"),-1)):0;
	const Json *Pbr=Mat?Mat->Campo("pbrMetallicRoughness"):0;
	const Json *Base=Pbr?Pbr->Campo("baseColorFactor"):0;
	if(Base) CorMaterial=RGBParaCor(NumeroDe(Base->Item(0),1),NumeroDe(Base->Item(1),1),NumeroDe(Base->Item(2),1));

	std::vector<uint32_t> I;
	const Json *JIdx=Prim.Campo("indices");
	if(JIdx){
		if(!LerAccessor(Raiz,Bin,InteiroDe(JIdx,-1),Idx,CompIdx,Erro)) return false;
		for(size_t i=0;i<Idx.size();i++){
			if(Idx[i]<0||Idx[i]>=NV){ Erro="indice fora dos limites"; return false; }
			I.push_back((uint32_t)Idx[i]);
		}
	}else
		for(size_t i=0;i<NV;i++) I.push_back((uint32_t)i);

	std::vector<std::pair<uint32_t,uint32_t> > Segmentos;
	std::set<std::pair<uint32_t,uint32_t> > Arestas;	// evita arestas repetidas em triangulos
	size_t n=I.size();

	switch(Modo){
		case 1: for(size_t i=0;i+1<n;i+=2) Segmentos.push_back(std::make_pair(I[i],I[i+1])); break;
		case 2:
		case 3: for(size_t i=0;i+1<n;i++)  Segmentos.push_back(std::make_pair(I[i],I[i+1]));
				if(Modo==2&&n>2) Segmentos.push_back(std::make_pair(I[n-1],I[0]));
				break;
		default:{
			size_t NT= Modo==4 ? n/3 : (n>=3?n-2:0);
			for(size_t t=0;t<NT;t++){
				uint32_t v[3];
				if(Modo==4){ v[0]=I[3*t]; v[1]=I[3*t+1]; v[2]=I[3*t+2]; }
				else if(Modo==5){ v[0]=I[t]; v[1]=I[t+1]; v[2]=I[t+2]; }
				else{ v[0]=I[0]; v[1]=I[t+1]; v[2]=I[t+2]; }
				for(int k=0;k<3;k++){
					uint32_t a=v[k],b=v[(k+1)%3];
					if(a==b) continue;
					std::pair<uint32_t,uint32_t> Aresta(a<b?a:b,a<b?b:a);
					if(Arestas.insert(Aresta).second) Segmentos.push_back(std::make_pair(a,b));
				}
			}
		}
	}

	for(size_t s=0;s<Segmentos.size();s++){
		uint32_t a=Segmentos[s].first,b=Segmentos[s].second;
		double Pa[3],Pb[3];
		M.Aplica(&Pos[a*3],Pa);
		M.Aplica(&Pos[b*3],Pb);

		unsigned int C=CorMaterial;
		if(!Cor.empty())
			C=RGBParaCor(Cor[a*CompCor],Cor[a*CompCor+1],Cor[a*CompCor+2]);

		AddLinhaCartesiana(D,Pa,Pb,C,Escala);
	}

	return true;
}

bool ImportarNo(const Json &Raiz,const std::vector<unsigned char> &Bin,long long IndiceNo,
				const Matriz &Pai,Desenho &D,double Escala,std::string &Erro,int Nivel){

	if(Nivel>64){ Erro="hierarquia de nos muito profunda (ciclo?)"; return false; }

	const Json *No=Raiz.Campo("nodes")?Raiz.Campo("nodes")->Item((size_t)IndiceNo):0;
	if(!No||IndiceNo<0) return true;

	Matriz M=Pai*MatrizDoNo(*No);

	const Json *JMesh=No->Campo("mesh");
	if(JMesh){
		const Json *Mesh=Raiz.Campo("meshes")?Raiz.Campo("meshes")->Item((size_t)InteiroDe(JMesh,-1)):0;
		const Json *Prims=Mesh?Mesh->Campo("primitives"):0;
		if(Prims)
			for(size_t i=0;i<Prims->A.size();i++)
				if(!ImportarPrimitiva(Raiz,Bin,Prims->A[i],M,D,Escala,Erro)) return false;
	}

	const Json *Filhos=No->Campo("children");
	if(Filhos)
		for(size_t i=0;i<Filhos->A.size();i++)
			if(!ImportarNo(Raiz,Bin,InteiroDe(&Filhos->A[i],-1),M,D,Escala,Erro,Nivel+1)) return false;

	return true;
}

} // namespace anonimo

//---------------------------------------------------------------------------

void PontosElipse(const Elipse &E,double Pontos[SegmentosElipse][3]){

	// Giro proprio da elipse em volta da origem, o mesmo do EixoXY (GiroDaElipse):
	// M = Rz(AnguloX)*Ry(AnguloZ)*Rx(AnguloY)
	const double ct=std::cos(Finito(E.AnguloX)),st=std::sin(Finito(E.AnguloX));
	const double cb=std::cos(Finito(E.AnguloZ)),sb=std::sin(Finito(E.AnguloZ));
	const double ca=std::cos(Finito(E.AnguloY)),sa=std::sin(Finito(E.AnguloY));
	const double M[3][3]={
		{ ct*cb,	ct*sb*sa-st*ca,	ct*sb*ca+st*sa	},
		{ st*cb,	st*sb*sa+ct*ca,	st*sb*ca-ct*sa	},
		{-sb,		cb*sa,			cb*ca			}
	};

	for(int i=0;i<SegmentosElipse;i++){

		double t=i*(2*PI/SegmentosElipse);
		double c=std::cos(t),s=std::sin(t);
		double x,y,z;

		// mesmas formulas de TEixoXY::Draw_Elipse
		switch(E.Tipo){
			case 1:
				x=E.A*c+E.CX;
				z=E.B*s+E.CZ;
				y=E.AZo*c+E.BZo*s+E.CY;
				break;
			case 2:
				y=E.A*c+E.CY;
				z=E.B*s+E.CZ;
				x=E.AZo*c+E.BZo*s+E.CX;
				break;
			default:
				x=E.A*c+E.CX;
				y=E.B*s+E.CY;
				z=E.AZo*c+E.BZo*s+E.CZ;
				break;
		}

		for(int l=0;l<3;l++)
			Pontos[i][l]=M[l][0]*x+M[l][1]*y+M[l][2]*z;
	}
}

//---------------------------------------------------------------------------

bool Exportar(const wchar_t *Nome_Arquivo,const Desenho &D,double Escala,std::string &Erro){

	if(!(Escala>0)) Escala=EscalaPadrao;

	Primitiva Linhas,Quads,Elipses;
	float C[3];
	double P[4][3];

	for(size_t i=0;i<D.Linhas.size();i++){
		const Linha &L=D.Linhas[i];
		CorParaRGB(L.Cor,C);
		Polar(L.MAlpha,L.MVetor,L.MZo,P[0]);
		Polar(L.LAlpha,L.LVetor,L.LZo,P[1]);
		RotacionaLinha(P[0],L.AnguloAlpha,L.AnguloTeta);
		RotacionaLinha(P[1],L.AnguloAlpha,L.AnguloTeta);
		Linhas.Add(P[0],C,Escala);
		Linhas.Add(P[1],C,Escala);
	}

	for(size_t i=0;i<D.Quadrilateros.size();i++){
		const Quadrilatero &Q=D.Quadrilateros[i];
		CorParaRGB(Q.Cor,C);
		// mesma ordem de vertices usada no GL_QUADS de Render.cpp
		Polar(Q.MAlpha ,Q.MVetor ,Q.MZo ,P[0]);
		Polar(Q.LAlpha ,Q.LVetor ,Q.LZo ,P[1]);
		Polar(Q.MAlpha2,Q.MVetor2,Q.MZo2,P[2]);
		Polar(Q.LAlpha2,Q.LVetor2,Q.LZo2,P[3]);
		static const int Tri[6]={0,1,2,0,2,3};
		for(int k=0;k<6;k++) Quads.Add(P[Tri[k]],C,Escala);
	}

	for(size_t i=0;i<D.Elipses.size();i++){
		double Pts[SegmentosElipse][3];
		CorParaRGB(D.Elipses[i].Cor,C);
		PontosElipse(D.Elipses[i],Pts);
		for(int k=0;k<SegmentosElipse;k++){
			Elipses.Add(Pts[k],C,Escala);
			Elipses.Add(Pts[(k+1)%SegmentosElipse],C,Escala);
		}
	}

	if(Linhas.Pos.empty()&&Quads.Pos.empty()&&Elipses.Pos.empty()){
		Erro="Nao ha linhas, quadrilateros ou elipses para exportar.";
		return false;
	}

	MontadorGLB G;
	G.AddMalha("Linhas",Linhas,1);
	G.AddMalha("Quadrilateros",Quads,4);
	G.AddMalha("Elipses",Elipses,1);

	std::vector<std::string> IndicesNos;
	for(size_t i=0;i<G.Nodes.size();i++) IndicesNos.push_back(Num((double)i));

	std::string Texto=
		"{\"asset\":{\"version\":\"2.0\",\"generator\":\"EixoXY\"},"
		"\"extensionsUsed\":[\"KHR_materials_unlit\"],"
		"\"scene\":0,"
		"\"scenes\":[{\"name\":\"EixoXY\",\"nodes\":["+Juntar(IndicesNos)+"],"
			"\"extras\":{\"EixoXY\":"+ExtrasEixoXY(D,Escala)+"}}],"
		"\"nodes\":["+Juntar(G.Nodes)+"],"
		"\"meshes\":["+Juntar(G.Meshes)+"],"
		"\"materials\":[{\"name\":\"EixoXY\",\"pbrMetallicRoughness\":{\"baseColorFactor\":[1,1,1,1],"
			"\"metallicFactor\":0,\"roughnessFactor\":1},\"doubleSided\":true,"
			"\"extensions\":{\"KHR_materials_unlit\":{}}}],"
		"\"accessors\":["+Juntar(G.Accessors)+"],"
		"\"bufferViews\":["+Juntar(G.BufferViews)+"],"
		"\"buffers\":[{\"byteLength\":"+Num((double)G.Bin.size())+"}]}";

	while(Texto.size()%4) Texto+=' ';			// chunk JSON alinhado com espacos
	while(G.Bin.size()%4) G.Bin.push_back(0);	// chunk BIN alinhado com zeros

	std::vector<unsigned char> Arquivo;
	uint32_t Total=12+8+(uint32_t)Texto.size()+8+(uint32_t)G.Bin.size();

	GravaU32(Arquivo,0x46546C67);				// "glTF"
	GravaU32(Arquivo,2);
	GravaU32(Arquivo,Total);
	GravaU32(Arquivo,(uint32_t)Texto.size());
	GravaU32(Arquivo,0x4E4F534A);				// "JSON"
	Arquivo.insert(Arquivo.end(),Texto.begin(),Texto.end());
	GravaU32(Arquivo,(uint32_t)G.Bin.size());
	GravaU32(Arquivo,0x004E4942);				// "BIN"
	Arquivo.insert(Arquivo.end(),G.Bin.begin(),G.Bin.end());

	FILE *F=_wfopen(Nome_Arquivo,L"wb");
	if(!F){
		Erro="Nao foi possivel criar o arquivo.";
		return false;
	}
	bool Ok=std::fwrite(&Arquivo[0],1,Arquivo.size(),F)==Arquivo.size();
	Ok=(std::fclose(F)==0)&&Ok;
	if(!Ok) Erro="Erro ao gravar o arquivo.";

	return Ok;
}

//---------------------------------------------------------------------------

bool Importar(const wchar_t *Nome_Arquivo,Desenho &D,double Escala,std::string &Erro){

	if(!(Escala>0)) Escala=EscalaPadrao;
	D=Desenho();

	std::vector<unsigned char> Dados;
	if(!LerArquivo(Nome_Arquivo,Dados)){ Erro="Nao foi possivel ler o arquivo."; return false; }

	if(Dados.size()<20||LeU32(&Dados[0])!=0x46546C67){
		Erro="O arquivo nao e um .glb (glTF binario).";
		return false;
	}
	if(LeU32(&Dados[4])!=2){ Erro="Versao de glTF nao suportada (esperado 2)."; return false; }

	uint32_t Total=LeU32(&Dados[8]);
	if(Total>Dados.size()){ Erro="Arquivo .glb incompleto."; return false; }

	uint32_t TamJson=LeU32(&Dados[12]);
	if(LeU32(&Dados[16])!=0x4E4F534A||20+(uint64_t)TamJson>Total){
		Erro="Chunk JSON invalido.";
		return false;
	}

	std::vector<unsigned char> Bin;
	uint64_t Pos=20+(uint64_t)TamJson;
	if(Pos+8<=Total&&LeU32(&Dados[(size_t)Pos+4])==0x004E4942){
		uint32_t TamBin=LeU32(&Dados[(size_t)Pos]);
		if(Pos+8+TamBin>Total){ Erro="Chunk BIN invalido."; return false; }
		Bin.assign(Dados.begin()+(size_t)Pos+8,Dados.begin()+(size_t)(Pos+8+TamBin));
	}

	Json Raiz;
	LeitorJson Leitor((const char*)&Dados[20],TamJson);
	if(!Leitor.Valor(Raiz)||Raiz.Tipo!=Json::Objeto){ Erro="JSON do glTF invalido."; return false; }

	const Json *Req=Raiz.Campo("extensionsRequired");
	if(Req&&!Req->A.empty()){
		Erro="O arquivo exige a extensao "+Req->A[0].S+" (nao suportada). Exporte sem compressao.";
		return false;
	}

	long long IndiceCena=InteiroDe(Raiz.Campo("scene"),0);
	const Json *Cenas=Raiz.Campo("scenes");
	const Json *Cena=Cenas?Cenas->Item((size_t)IndiceCena):0;

	// Arquivo do proprio EixoXY: recupera os dados originais sem perda.
	// (Se outro programa regravou o .glb, o "generator" muda e a geometria e lida.)
	const Json *Asset=Raiz.Campo("asset");
	const Json *Gerador=Asset?Asset->Campo("generator"):0;
	const Json *Extras=Cena?Cena->Campo("extras"):0;
	const Json *DadosEixoXY=Extras?Extras->Campo("EixoXY"):0;
	if(Gerador&&Gerador->S.compare(0,6,"EixoXY")==0&&DadosEixoXY&&DadosEixoXY->Tipo==Json::Objeto)
		return ImportarExtras(Raiz,*DadosEixoXY,D);

	// Arquivo de outro programa: converte malhas em linhas
	Matriz Identidade;
	if(Cena){
		const Json *Nos=Cena->Campo("nodes");
		if(Nos)
			for(size_t i=0;i<Nos->A.size();i++)
				if(!ImportarNo(Raiz,Bin,InteiroDe(&Nos->A[i],-1),Identidade,D,Escala,Erro,0)) return false;
	}else{
		const Json *Meshes=Raiz.Campo("meshes");
		if(Meshes)
			for(size_t m=0;m<Meshes->A.size();m++){
				const Json *Prims=Meshes->A[m].Campo("primitives");
				if(Prims)
					for(size_t i=0;i<Prims->A.size();i++)
						if(!ImportarPrimitiva(Raiz,Bin,Prims->A[i],Identidade,D,Escala,Erro)) return false;
			}
	}

	if(D.Linhas.empty()){
		Erro="Nenhuma linha ou malha encontrada no arquivo.";
		return false;
	}

	return true;
}

}
