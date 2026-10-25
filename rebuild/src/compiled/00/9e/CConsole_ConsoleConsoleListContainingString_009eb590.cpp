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