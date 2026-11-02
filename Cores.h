//---------------------------------------------------------------------------
// Cores.h - as cores do desenho (linhas, elipses e quadrilateros) sao guardadas
// como COLORREF do Windows: 0x00BBGGRR, vermelho no byte baixo.
//
// Um TColor do VCL tem o mesmo formato para cores RGB, mas tambem pode ser uma
// cor do sistema (0xFF0000xx, ex. clBtnFace) ou clNone. Os Set...Cor das classes
// passam tudo por TColorParaCOLORREF, entao o que fica guardado e sempre RGB.
//---------------------------------------------------------------------------

#ifndef CoresH
#define CoresH

#include <windows.h>
#include <vector>

// o .DEN grava cada cor com 4 bytes
static_assert(sizeof(COLORREF)==4,"COLORREF precisa ter 4 bytes");

// TColor (ou COLORREF) -> RGB. Faz o mesmo que o ColorToRGB do VCL, sem depender dele.
inline COLORREF TColorParaCOLORREF(unsigned long Cor){

	if((Cor&0xFF000000)==0xFF000000)			// cor do sistema: o byte baixo e o indice
		return GetSysColor((int)(Cor&0xFF));

	return Cor&0x00FFFFFF;
}

// Converte todas as cores do vetor (usado pelos Set...(ponteiro))
inline void NormalizaCores(std::vector<COLORREF> &Cores){

	for(size_t i=0;i<Cores.size();i++)
		Cores[i]=TColorParaCOLORREF(Cores[i]);
}

#endif
//---------------------------------------------------------------------------
