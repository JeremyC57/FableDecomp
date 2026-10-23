// 006d41f0 N006d41f0::CPersistTraits<Element>::TransferIn (Element is a pair<CCharString,long>-shaped 8B record; catalog CPersistTraits<CSimVoice> is wrong, PDB CSimVoice is 28B)
// Flags: /O2 /Oy /W3
// Leaf: TransferIn
// Layout:40e510 ret12 with ECX pointing at local CCharString default proves StringTraits member; CStringParser shared header; parenthesis/comma token literals and output +0/+4 assignments are retail evidence.
#include "engine/CStringParser.h"
namespace N006d41f0 {

extern "C" int __cdecl strcmp(const char*,const char*);
#pragma intrinsic(strcmp)
int __fastcall Compare(const char*,const char*);
struct CBasicString {const char* Data;bool Equal(const char*) const;};
struct CCharString {CBasicString* Data;CCharString();CCharString(const CCharString&);~CCharString(); __forceinline bool Equal(const char* s) const {if(!Data)return strcmp(s,"")==0;return Data->Equal(s);} __forceinline bool CompareEqual(const char* s)const{if(!Data)return strcmp(s,"")==0;return Compare(Data->Data,s)==0;}};
struct Parser:CStringParser {::N006d41f0::CCharString ReadAsString();};
struct CPersistContext {unsigned int State;};
void __fastcall ReadInteger(CPersistContext*,Parser*,unsigned int*);
struct StringTraits {CCharString Default;void TransferIn(CPersistContext*,Parser*,CCharString*);};
struct SubValue {unsigned int Value;};
struct SubTraits {unsigned int Default;void TransferIn(CPersistContext*,Parser*,SubValue*);};
struct Element {CCharString first;unsigned int second;};
template<class T> struct CPersistTraits { void TransferIn(CPersistContext*,Parser*,T*);};
template<class T> void CPersistTraits<T>::TransferIn(CPersistContext* ctx,Parser* parser,T* out) {
 StringTraits tag;
 if(!parser->ReadAsString().CompareEqual("(")) return;
 tag.TransferIn(ctx,parser,&out->first);
 if(parser->ReadAsString().Equal(",")) {
  { unsigned int second;ReadInteger(ctx,parser,&second);out->second=second; }
  if(parser->ReadAsString().Equal(")")) return;
 }
}
template void CPersistTraits<Element>::TransferIn(CPersistContext*,Parser*,Element*);

}