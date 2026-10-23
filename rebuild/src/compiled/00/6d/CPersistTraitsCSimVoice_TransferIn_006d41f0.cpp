// 006d41f0 Traits::TransferIn
// Flags: /O2 /Oy /W3
// Leaf: TransferIn
// Layout:40e510 ret12 with ECX pointing at local String default proves StringTraits member; CStringParser shared header; parenthesis/comma token literals and output +0/+4 assignments are retail evidence.
#include "engine/CStringParser.h"
namespace N006d41f0 {

extern "C" int __cdecl strcmp(const char*,const char*);
#pragma intrinsic(strcmp)
int __fastcall Compare(const char*,const char*);
struct BasicString {const char* Data;bool Equal(const char*) const;};
struct String {BasicString* Data;String();String(const String&);~String(); __forceinline bool Equal(const char* s) const {if(!Data)return strcmp(s,"")==0;return Data->Equal(s);} __forceinline bool CompareEqual(const char* s)const{if(!Data)return strcmp(s,"")==0;return Compare(Data->Data,s)==0;}};
struct Parser:CStringParser {::N006d41f0::String ReadAsString();};
struct Context {unsigned int State;};
void __fastcall ReadInteger(Context*,Parser*,unsigned int*);
struct StringTraits {String Default;void TransferIn(Context*,Parser*,String*);};
struct SubValue {unsigned int Value;};
struct SubTraits {unsigned int Default;void TransferIn(Context*,Parser*,SubValue*);};
struct Element {String First;unsigned int Second;};
template<class T> struct Traits { void TransferIn(Context*,Parser*,T*);};
template<class T> void Traits<T>::TransferIn(Context* ctx,Parser* parser,T* out) {
 StringTraits tag;
 if(!parser->ReadAsString().CompareEqual("(")) return;
 tag.TransferIn(ctx,parser,&out->First);
 if(parser->ReadAsString().Equal(",")) {
  { unsigned int second;ReadInteger(ctx,parser,&second);out->Second=second; }
  if(parser->ReadAsString().Equal(")")) return;
 }
}
template void Traits<Element>::TransferIn(Context*,Parser*,Element*);

}