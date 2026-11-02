//---------------------------------------------------------------------------
// GLB.h - Exportacao/Importacao do desenho no formato glTF 2.0 binario (.glb),
// o formato lido pelo GLTFLoader do three.js, Blender, Windows 3D Viewer etc.
//
// Este modulo usa apenas C++ padrao (sem VCL) para poder ser testado sozinho.
// A ligacao com as classes do programa fica em Name_Arquivos::ExportGLB
// (Arquivo.h / Arquivo.cpp).
//---------------------------------------------------------------------------

#ifndef GLBH
#define GLBH

#include <string>
#include <vector>

namespace GLB{

	// Os campos seguem exatamente os dados gravados no .DEN:
	// cada ponto e guardado em coordenadas polares no plano XY
	// (x=cos(Alpha)*Vetor, y=sin(Alpha)*Vetor) mais a altura Zo.

	struct Linha{
		double MAlpha,MVetor,MZo;	// ponto inicial
		double LAlpha,LVetor,LZo;	// ponto final
		unsigned int Cor;			// TColor RGB (0x00BBGGRR)
		// rotacao propria da linha (Duplica Alpha/Teta): gira em Z por Teta e
		// depois em X por Alpha, como em TEixoXY::Draw_Lines; Beta nao e usado
		double AnguloAlpha=0,AnguloBeta=0,AnguloTeta=0;
	};

	struct Quadrilatero{
		double MAlpha,MVetor,MZo;
		double LAlpha,LVetor,LZo;
		double MAlpha2,MVetor2,MZo2;
		double LAlpha2,LVetor2,LZo2;
		unsigned int Cor;
	};

	struct Elipse{
		double A,B,AZo,BZo;
		double CX,CY,CZ;
		double AnguloX,AnguloY,AnguloZ;
		int Tipo;					// 0 = plano XY, 1 = plano XZ, 2 = plano YZ
		unsigned int Cor;
	};

	struct Desenho{
		double Alpha_B_T[3];
		std::vector<Linha> Linhas;
		std::vector<Quadrilatero> Quadrilateros;
		std::vector<Elipse> Elipses;
		Desenho(){ Alpha_B_T[0]=Alpha_B_T[1]=Alpha_B_T[2]=0; }
	};

	const int SegmentosElipse=50;

	// Escala: quantos metros do glTF vale 1 unidade (pixel) do EixoXY.
	// 0.01 => 100 pixels viram 1 metro no three.js.
	const double EscalaPadrao=0.01;

	// Grava o desenho em .glb. Retorna false e preenche Erro em caso de falha.
	bool Exportar(const wchar_t *Nome_Arquivo,const Desenho &D,double Escala,std::string &Erro);

	// Le um .glb. Se o arquivo foi gerado pelo EixoXY, recupera os dados
	// originais (linhas, quadrilateros e elipses). Se veio de outro programa
	// (Blender, three.js...), converte a geometria em linhas (arestas).
	bool Importar(const wchar_t *Nome_Arquivo,Desenho &D,double Escala,std::string &Erro);

	// Pontos 3D (unidades do EixoXY) de uma elipse, ja com a rotacao aplicada.
	void PontosElipse(const Elipse &E,double Pontos[SegmentosElipse][3]);

}

#endif
