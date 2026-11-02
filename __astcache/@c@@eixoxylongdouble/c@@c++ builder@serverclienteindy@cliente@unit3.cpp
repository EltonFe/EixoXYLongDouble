//---------------------------------------------------------------------------

#include <vcl.h>
#include <thread>
#pragma hdrstop

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
std::atomic< bool> run=true;
void mensagemRecebida();
std::thread t;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button1Click(TObject *Sender)
{
	IdTCPClient1->IOHandler->WriteLn("Hello Server!");
}
//---------------------------------------------------------------------------
void mensagemRecebida(){
   UnicodeString MSG;
	while(run){
		  try{
			MSG=Form3->IdTCPClient1->Socket->ReadLn();
			Form3->Memo1->Lines->Add(MSG);
		  }catch(Exception &e){
			ShowMessage("Socket Desconectado");
            break;
		  }
	}

}
void __fastcall TForm3::Button2Click(TObject *Sender)
{
	UnicodeString MSG;
	IdTCPClient1->Connect();
	if(IdTCPClient1->Connected())
		Memo1->Lines->Add("Connectado");
	t=std::thread(mensagemRecebida);
    t.detach();


}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormClose(TObject *Sender, TCloseAction &Action)
{
	run=false;
	if(t.joinable())
        t.join();
}
//---------------------------------------------------------------------------
