// 006d41f0 N006d41f0::CPersistTraits<Element>::TransferIn (Element is a pair<CCharString,long>-shaped 8B record; catalog CPersistTraits<CSimVoice> is wrong, PDB CSimVoice is 28B)
// Flags: /O2 /Oy /W3
// Leaf: TransferIn
// Layout:40e510 ret12 with ECX pointing at local CCharString default proves StringTraits member; CStringParser shared header; parenthesis/comma token literals and output +0/+4 assignments are retail evidence.
// Name (2026-10-31, hand621 NAME_FIXES): the element-string reader 0x0040E510 is the global
//   ?TransferIn@?$CPersistTraits@VCCharString@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCCharString@@@Z (Ego_r.pdb
//   0x0040C1F0 / FableWin.pdb 0x0189E580), so StringTraits is that global specialization (was N006d41f0::StringTraits with
//   pointer parameters); the context and string are passed through reference casts. Bytes unchanged.
#include "fable_compiler.h"
#include "engine/CStringParser.h"
namespace N006d41f0 {

extern "C" int __cdecl strcmp(const char*,const char*);
#pragma intrinsic(strcmp)
int __fastcall Compare(const char*,const char*);
struct CBasicString {const char* Data;bool Equal(const char*) const;};
struct CCharString {CBasicString* Data;CCharString();CCharString(const CCharString&);~CCharString(); FABLE_FORCEINLINE bool Equal(const char* s) const {if(!Data)return strcmp(s,"")==0;return Data->Equal(s);} FABLE_FORCEINLINE bool CompareEqual(const char* s)const{if(!Data)return strcmp(s,"")==0;return Compare(Data->Data,s)==0;}};
struct Parser:CStringParser {::N006d41f0::CCharString ReadAsString();};
struct CPersistContext {unsigned int State;};
void __fastcall ReadInteger(CPersistContext*,Parser*,unsigned int*);
}
class CCharString;
class CPersistContext;
template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CCharString> {N006d41f0::CCharString Default;void TransferIn(CPersistContext& context,CStringParser& parser,CCharString& value);};   // 0x0040E510
namespace N006d41f0 {
typedef ::CPersistTraits< ::CCharString> StringTraits;
struct SubValue {unsigned int Value;};
struct SubTraits {unsigned int Default;void TransferIn(CPersistContext*,Parser*,SubValue*);};
struct Element {CCharString first;unsigned int second;};
template<class T> struct CPersistTraits { void TransferIn(CPersistContext*,Parser*,T*);};
template<class T> void CPersistTraits<T>::TransferIn(CPersistContext* ctx,Parser* parser,T* out) {
 StringTraits tag;
 if(!parser->ReadAsString().CompareEqual("(")) return;
 tag.TransferIn(reinterpret_cast< ::CPersistContext&>(*ctx),*parser,reinterpret_cast< ::CCharString&>(out->first));
 if(parser->ReadAsString().Equal(",")) {
  { unsigned int second;ReadInteger(ctx,parser,&second);out->second=second; }
  if(parser->ReadAsString().Equal(")")) return;
 }
}
template void CPersistTraits<Element>::TransferIn(CPersistContext*,Parser*,Element*);

}