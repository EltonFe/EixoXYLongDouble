//---------------------------------------------------------------------------

#ifndef RenderH
#define RenderH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "glm/glm/glm.hpp"
#include "glm/glm/ext.hpp"
#include <vector>
#include "Arquivo.h"
using Name_Arquivos::Objetos;
using Name_Arquivos::Line;
using Name_Arquivos::Arquivo;

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TTimer *Timer1;
	TButton *Button1;
	TButton *Button2;
	TButton *BtnLancar;
	TRadioGroup *RadioGroup1;
	TPanel *Panel1;
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);


	void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall BtnLancarClick(TObject *Sender);
	void __fastcall RadioGroup1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
    int     xs,ys;
    HDC     hdc;            // device context
    HGLRC   hrc;            // rendering context
    int  ogl_inicialized;
	int  ogl_init();
	void ogl_exit();
	void ogl_draw();
	void ogl_resize();
	// cantos dos quadrilateros (um por quadrilatero, crescem com o desenho)
	std::vector<glm::vec3> P1,P2,P3,P4;
    bool Lancar;
	void Draw_Lines_Direct_Render(TCanvas *Plano,Objetos *Objetos_Atr, int **FullMLK);
	void Draw_Lines_Direct_RenderQuadrilatero(TCanvas *Plano,Objetos *Objetos_Atr, int **FullMLK,int **FullMLK3);
	void DrawDirect_RenderQuadrilateroSolid(Objetos *Objetos_Atr,int **FullMLK,int **FullMLK3);
	void DrawDirect(Objetos *Objetos_Atr);
	static void MoveTo(int x,int y);
	static void LineTo(int x,int y);
	float Kx;
	float Ky;
	float t;
	std::vector<glm::vec4> Vetor1,Vetor2,Vetor3,Vetor4;

	// lancamento balistico na visao 3D (mesma fisica do TEixoXY::Lancamento)
	float Vo_Lancamento;				// velocidade inicial
	float Angulo_Lancamento;			// radianos
	unsigned long long Inicio_Lancamento;	// GetTickCount64 do clique em Lancar
	float Giro;						// graus do giro sobre os proprios eixos
	void Lancamento();
	glm::vec4 Rotaciona(float Angulo,float x,float y,float z);
	void RotacionaDesenho(Objetos *Objetos_Atr);
	glm::mat4 Rotate;
	float Angulo;
	void DrawQuadrilateros(const Objetos *);	// quadrilateros solidos (GL_QUADS) na visao 3D
	void DrawElipses(const Objetos *);	// elipses na visao 3D
	void Draw_Elipses_Render(Objetos *Objetos_Atr);	// elipses na visao 2D
	void DrawLines(const Objetos *);//Desenha uma linha em Form1
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
