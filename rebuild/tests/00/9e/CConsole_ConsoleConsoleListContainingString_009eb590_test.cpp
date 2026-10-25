// 009eb590 CConsole::ConsoleConsoleListContainingString
// Flags: /O2 /Oy /W3
// Leaf: ConsoleConsoleListContainingString
#include <string.h>
namespace X_009eb590{
#pragma pack(push,1)
// Layout: CCharStringData retail17 bytes: text0,length4,capacity8,owned12,refs13.
struct CCharStringData{char*Text;unsigned long Length,Capacity;bool Owned;long References;};
#pragma pack(pop)
extern const char EmptyString[];
struct CCharString{CCharStringData*PStringData;CCharString();CCharString(const CCharString&);~CCharString();CCharString GetAsLowerCase()const;__forceinline operator const char*()const{if(!PStringData)return EmptyString;return PStringData->Text;}__forceinline bool Contains(const CCharString&r)const{return strstr((const char*)*this,(const char*)r)!=0;}};
// Layout: PDB CGenericVar28, ValString+8; CConsoleInputBase8 Name+4.
struct CGenericVar{unsigned Prefix,Type;CCharString ValString;long ValSLONG;unsigned long ValULONG;bool ValBool;float ValFloat;};
namespace std{template<class T>struct vector{T*Start,*Finish,*Storage;};}
struct CConsoleInputBase{unsigned Prefix;CCharString Name;};
struct Node{unsigned Colour;Node*Parent,*Left,*Right;CCharString Key;CConsoleInputBase*Input;void*Info;};
struct Iterator{Node*P;__forceinline Iterator&operator++(){Node*x=P;if(x->Right){x=x->Right;while(x->Left)x=x->Left;}else{Node*y=x->Parent;while(x==y->Right){x=y;y=y->Parent;}if(x->Right!=y)x=y;}P=x;return *this;}};
// Layout: CConsole retail RegisteredConsoleInputs header at+40; PDB donor+48 differs by two preceding map sizes. No active CConsole header exists.
struct CConsole{unsigned char Prefix[0x40];Node*Header;void SendMessageToUser(const CCharString&);static void __fastcall ConsoleConsoleListContainingString(CConsole&,const std::vector<CGenericVar>&);};
void __fastcall CConsole::ConsoleConsoleListContainingString(CConsole&console,const std::vector<CGenericVar>&args){
 Iterator it;it.P=console.Header->Left;for(;it.P!=console.Header;++it){bool found;{CCharString text(args.Start[0].ValString);CConsoleInputBase*input=it.P->Input;found=input->Name.GetAsLowerCase().Contains(text.GetAsLowerCase());}if(found)console.SendMessageToUser(it.P->Input->Name);}
}
}

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
namespace X_009eb590{
const char EmptyString[]="";static unsigned live,foundCount;static char foundNames[8][32];
CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString&r):PStringData(r.PStringData){if(PStringData)++PStringData->References;}CCharString::~CCharString(){if(PStringData&&!--PStringData->References){if(PStringData->Owned)free(PStringData->Text);delete PStringData;--live;}}
static void Set(CCharString&s,const char*text){if(s.PStringData&&!--s.PStringData->References){free(s.PStringData->Text);delete s.PStringData;--live;}s.PStringData=new CCharStringData;unsigned n=strlen(text);s.PStringData->Text=(char*)malloc(n+1);memcpy(s.PStringData->Text,text,n+1);s.PStringData->Length=n;s.PStringData->Capacity=n+1;s.PStringData->Owned=true;s.PStringData->References=1;++live;}
CCharString CCharString::GetAsLowerCase()const{CCharString out;Set(out,(const char*)*this);for(char*p=out.PStringData->Text;*p;++p)*p=(char)tolower((unsigned char)*p);return out;}
void CConsole::SendMessageToUser(const CCharString&s){assert(foundCount<8);strcpy(foundNames[foundCount++],(const char*)s);}
}
int main(){using namespace X_009eb590;{CConsole c;memset(&c,0,sizeof(c));Node header,nodes[3];CConsoleInputBase input[3];memset(&header,0,sizeof(header));for(int i=0;i<3;++i){nodes[i].Parent=nodes[i].Left=nodes[i].Right=0;nodes[i].Input=&input[i];}Set(input[0].Name,"AlphaBeta");Set(input[1].Name,"BETA");Set(input[2].Name,"gamma");header.Parent=&nodes[1];header.Left=&nodes[0];header.Right=&nodes[2];nodes[1].Parent=&header;nodes[1].Left=&nodes[0];nodes[1].Right=&nodes[2];nodes[0].Parent=nodes[2].Parent=&nodes[1];c.Header=&header;CGenericVar arg;std::vector<CGenericVar>args;args.Start=&arg;args.Finish=args.Storage=&arg+1;Set(arg.ValString,"bEtA");unsigned before=live;foundCount=0;CConsole::ConsoleConsoleListContainingString(c,args);assert(foundCount==2&&strcmp(foundNames[0],"AlphaBeta")==0&&strcmp(foundNames[1],"BETA")==0&&live==before);Set(arg.ValString,"nomatch");foundCount=0;CConsole::ConsoleConsoleListContainingString(c,args);assert(foundCount==0);Set(arg.ValString,"");foundCount=0;CConsole::ConsoleConsoleListContainingString(c,args);assert(foundCount==3);header.Left=&header;foundCount=0;CConsole::ConsoleConsoleListContainingString(c,args);assert(foundCount==0);}assert(live==0);puts("009eb590_TEST PASS");return 0;}