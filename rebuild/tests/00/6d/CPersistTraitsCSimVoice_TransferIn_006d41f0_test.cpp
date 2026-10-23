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

#include <cstdio>
#include <cstring>
#include <new>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace N006d41f0 {
#pragma pack(push,1)
struct Block {CBasicString Base;unsigned int Length,Capacity;bool Fast;int Refs;};
#pragma pack(pop)
static int liveBlocks,liveTexts,destroyed,ctorCount,dtorCount,scalarCalls,stringCalls;
static unsigned tokenIndex;static const char* tokens[8];static const char* stringValue;static unsigned number;static CPersistContext* expectedContext;static Parser* expectedParser;static bool badArguments;
static Block* Owner(CBasicString* p){return reinterpret_cast<Block*>(p);}
static void Release(CBasicString* p){if(p&&--Owner(p)->Refs<=0){if(p->Data){delete[] p->Data;--liveTexts;}delete Owner(p);--liveBlocks;++destroyed;}}
CCharString::CCharString():Data(0){++ctorCount;}CCharString::CCharString(const CCharString& o):Data(o.Data){if(Data)++Owner(Data)->Refs;++ctorCount;}CCharString::~CCharString(){Release(Data);++dtorCount;}
static void Set(CCharString& out,const char* text){Release(out.Data);out.Data=0;if(!text)return;Block* p=new Block;p->Length=(unsigned)std::strlen(text);p->Capacity=p->Length+1;p->Fast=true;p->Refs=1;char* raw=new char[p->Capacity];std::memcpy(raw,text,p->Capacity);p->Base.Data=raw;out.Data=&p->Base;++liveBlocks;++liveTexts;}
bool CBasicString::Equal(const char* s)const{const signed char* p=(const signed char*)Data;const signed char* q=(const signed char*)s;while(*p&&*p==*q){++p;++q;}return *p==*q;}
int __fastcall Compare(const char* a,const char* b){const signed char* p=(const signed char*)a;const signed char* q=(const signed char*)b;while(*p&&*p==*q){++p;++q;}return *p<*q?-1:(*p>*q?1:0);}
CCharString Parser::ReadAsString(){::N006d41f0::CCharString result;Set(result,tokens[tokenIndex++]);return result;}
void __fastcall ReadInteger(CPersistContext* c,Parser* p,unsigned int* out){++scalarCalls;if(c!=expectedContext||p!=expectedParser||c->State!=42)badArguments=true;*out=number;}
void StringTraits::TransferIn(CPersistContext* c,Parser* p,CCharString* out){++stringCalls;if(c!=expectedContext||p!=expectedParser||c->State!=42)badArguments=true;Set(*out,stringValue);}
void SubTraits::TransferIn(CPersistContext*,Parser*,SubValue*){}
static void Reset(const char* opening,const char* comma,const char* closing){tokenIndex=0;tokens[0]=opening;tokens[1]=comma;tokens[2]=closing;scalarCalls=stringCalls=0;badArguments=false;}
}
int main(){using namespace N006d41f0;if(sizeof(Block)!=17)return 1;Block layout;if((unsigned)((char*)&layout.Refs-(char*)&layout)!=13)return 2;
{Parser p;std::memset(&p,0,sizeof p);CPersistContext c;c.State=42;expectedContext=&c;expectedParser=&p;Element e;Set(e.first,"old");e.second=111;CPersistTraits<Element> t;stringValue="actual payload";number=23;Reset("(",",",")");t.TransferIn(&c,&p,&e);if(!e.first.Data||std::strcmp(e.first.Data->Data,stringValue)||e.second!=23||tokenIndex!=3||scalarCalls!=1||stringCalls!=1||badArguments||liveBlocks!=1||liveTexts!=1)return 3;
CBasicString* saved=e.first.Data;Reset("?",",",")");e.second=111;t.TransferIn(&c,&p,&e);if(e.first.Data!=saved||e.second!=111||tokenIndex!=1||scalarCalls||stringCalls)return 4;
Reset(0,",",")");t.TransferIn(&c,&p,&e);if(tokenIndex!=1||stringCalls||scalarCalls||e.first.Data!=saved)return 5;
Reset("(","?",")");t.TransferIn(&c,&p,&e);if(tokenIndex!=2||stringCalls!=1||scalarCalls||e.second!=111)return 6;
Reset("(",0,")");t.TransferIn(&c,&p,&e);if(tokenIndex!=2||scalarCalls||e.second!=111)return 7;
Reset("(",",","?");number=71;t.TransferIn(&c,&p,&e);if(tokenIndex!=3||e.second!=71||badArguments)return 8;
Reset("(",",",0);number=93;t.TransferIn(&c,&p,&e);if(tokenIndex!=3||e.second!=93||badArguments)return 9;
CCharString root;Set(root,"shared");CCharString* copies=(CCharString*)::operator new(300*sizeof(CCharString));for(int i=0;i<300;++i)new(copies+i)CCharString(root);if(Owner(root.Data)->Refs!=301)return 10;for(int j=0;j<300;++j)copies[j].~CCharString();if(Owner(root.Data)->Refs!=1)return 11;::operator delete(copies);
}if(liveBlocks||liveTexts||ctorCount!=dtorCount)return 12;std::printf("006d41f0_TEST PASS\n");return 0;}