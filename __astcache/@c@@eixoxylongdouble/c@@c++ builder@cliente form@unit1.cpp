//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ustring.h>
 #include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <string>
using std::string;
#include <iomanip>


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------

SOCKET wClient;
char Buff[512];

DWORD WINAPI Recebe(LPVOID d);

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{


}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
 WSADATA wSockDATA;
 int iResult=0;
 int Result;
 iResult=WSAStartup(MAKEWORD(2,2),&wSockDATA);

 if (iResult!=0) {
		 Memo1->Lines->Add("ERRO WSAStartup");
 }

  ::wClient=socket(AF_INET,SOCK_STREAM  ,IPPROTO_TCP);
  if (::wClient==SOCKET_ERROR) {
	  Form1->Memo1->Lines->Add("ERROR SOCKET INVALIDO");
  }









}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
int Result;
int Porta;
sockaddr_in wService;


     UnicodeString b;
 b=Edit1->Text;
AnsiString ansiB(b);

// allocate enough memory for your char array (and the null terminator)
char* str = new char[ansiB.Length()+1];

// copy the contents of the AnsiString into your char array
StringCchCopyA(str,ansiB.Length()+1,ansiB.c_str());

// the rest of your code goes here





   Porta=EditPorta->Text.ToInt();
   wService.sin_port=htons(Porta);
   wService.sin_family=AF_INET;
   wService.sin_addr.s_addr=inet_addr(str);

 Result=connect(::wClient,(SOCKADDR *)&wService,sizeof(wService));
   if (Result==SOCKET_ERROR) {
		Form1->Memo1->Lines->Add("ERROR AO CONECTAR NO SOCKET");

   }else{Form1->Memo1->Lines->Add("CLIENTE CONECTADO COM SUCESSO");
		delete[] str;

		CreateThread(NULL,0,Recebe,NULL,0,NULL);
   }





}

//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
 UnicodeString b;
 b=Memo2->Text+'\n';
AnsiString ansiB(b);
char Char='\n';
// allocate enough memory for your char array (and the null terminator)
char* str = (char*)malloc(ansiB.Length()+1);
memset(str,0,ansiB.Length()+1);
int c=ansiB.Length();
// copy the contents of the AnsiString into your char array
StringCchCopyA(str,ansiB.Length()+1,ansiB.c_str());

// the rest of your code goes here
UnicodeString String=(UnicodeString)str;



  send(::wClient,str,(int)strlen(str),0);
// remember to delete your char array when done

delete[] str;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Memo2KeyPress(TObject *Sender, System::WideChar &Key)
{
if (Key==VK_RETURN ){


 UnicodeString b;
 b=Memo2->Text;
	Memo2->Clear();

AnsiString ansiB(b);

// allocate enough memory for your char array (and the null terminator)
char* str = new char[ansiB.Length()+1];

// copy the contents of the AnsiString into your char array
StringCchCopyA(str,ansiB.Length()+1,ansiB.c_str());

// the rest of your code goes here
  send(::wClient,str,(int)strlen(str),0);
// remember to delete your char array when done
delete[] str;

   Memo2->Clear();
   }
}
//---------------------------------------------------------------------------





void __fastcall TForm1::Memo2KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
if (Key==VK_RETURN){
   UnicodeString b;
  b=Memo2->Text;

	Memo2->Clear();

AnsiString ansiB(b);

// allocate enough memory for your char array (and the null terminator)
char* str = new char[ansiB.Length()-1];

// copy the contents of the AnsiString into your char array
StringCchCopyA(str,ansiB.Length()-1,ansiB.c_str());

// the rest of your code goes here
  send(::wClient,str,(int)strlen(str),0);
// remember to delete your char array when done
delete[] str;
  Memo2->Clear();}
}
//---------------------------------------------------------------------------


DWORD WINAPI Recebe(LPVOID d){
int Resultado, Tamanho;
char  Buff[512];
	for(;;){
		memset(Buff,0,sizeof(Buff));
		Resultado=recv(::wClient,Buff,512,0);
		if(Resultado>0){

			UnicodeString Buffer(Buff);
			Form1->Memo1->Lines->Add(Buffer);

		} else{
				if(Resultado==0){

				 ShowMessage("Conexao fechada");


				}else{
						Form1->Edit1->Text=WSAGetLastError();

					}

			}



	}


}
