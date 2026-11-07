//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("UnitConstrucaoLine.cpp", ConstrucaoLines);
USEFORM("Render.cpp", Form1);
USEFORM("UnitElipse.cpp", FElipse);
USEFORM("UnitEixoXY.cpp", EixoXY);
USEFORM("draw\DrawUnit.cpp", DrawOpenGL);
USEFORM("DrawCursor.cpp", FDrawCursor);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TEixoXY), &EixoXY);
		Application->CreateForm(__classid(TConstrucaoLines), &ConstrucaoLines);
		Application->CreateForm(__classid(TFElipse), &FElipse);
		Application->CreateForm(__classid(TFDrawCursor), &FDrawCursor);
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(TDrawOpenGL), &DrawOpenGL);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
