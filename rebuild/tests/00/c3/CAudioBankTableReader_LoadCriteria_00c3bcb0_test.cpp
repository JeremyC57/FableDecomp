#include <assert.h>
#include <stdio.h>
#include <string.h>
// BEGIN exact source snapshot SHA256 7e0a1f288d58fcb5c6a2fe0f13ec46a22a3523dd136d6df76d86dfceaceb88ab
// 00c3bcb0 AudioCriteriaLoader_00c3bcb0::LoadCriteria (owner uncertain)
// Flags: /O2 /Oy /EHsc
// Layout: reader pointer this+0; callback slot0 receives context,text,count,driver array; disasm00c3bcb0.
#include <new>
void* __cdecl operator new[](unsigned int size);   // MSVCR71 ??_U, import thunk 0x00BFEB22
void __cdecl operator delete[](void* p);            // MSVCR71 ??_V, import thunk 0x00BFEB1C
struct CriteriaText{char*data;CriteriaText(const char*);~CriteriaText();};
// Layout: error factory00c30840 allocates12-byte entry and returns its string field+4. Descriptive helper names; original names unresolved.
struct CriteriaLog{char*data;};
struct CriteriaContext{CriteriaLog*CreateErrorText();bool Continue();};
CriteriaLog* __fastcall AppendErrorText(CriteriaLog*,const char*);
struct CriteriaReader{int FindSegment(const CriteriaText&);int Read(void*,int);};
struct CriteriaCallback{virtual void Entry(CriteriaContext*,const char*,unsigned,const unsigned*)=0;};
struct AudioCriteriaLoader_00c3bcb0{CriteriaReader*reader;void LoadCriteria(CriteriaContext*,CriteriaCallback*);};
void AudioCriteriaLoader_00c3bcb0::LoadCriteria(CriteriaContext*context,CriteriaCallback*callback){if(reader->FindSegment(CriteriaText("LHAudioBankCriteiaInfo"))<0){AppendErrorText(context->CreateErrorText(),"Could not find the criteria info segment");return;}unsigned count;if(reader->Read(&count,4)<4){AppendErrorText(context->CreateErrorText(),"Could not read the number of criteria entries!");return;}for(unsigned i=0;i<count;++i){unsigned length;if(reader->Read(&length,4)<4){AppendErrorText(context->CreateErrorText(),"Could not read the text length");return;}char*text=new char[length+1];text[length]=0;if(reader->Read(text,length)<(int)length){AppendErrorText(context->CreateErrorText(),"Could not read text");operator delete(text);return;}unsigned number;if(reader->Read(&number,4)<4){AppendErrorText(context->CreateErrorText(),"Could not read the number of variations");operator delete(text);return;}unsigned*drivers=0;if(number){drivers=new unsigned[number];if(reader->Read(drivers,number*4)<(int)(number*4)){AppendErrorText(context->CreateErrorText(),"Could not read the driver list");operator delete(text);delete[]drivers;return;}}callback->Entry(context,text,number,drivers);operator delete(text);if(drivers)delete[]drivers;if(!context->Continue())return;}}

// END exact source snapshot
CriteriaText::CriteriaText(const char*s){data=new char[strlen(s)+1];strcpy(data,s);}CriteriaText::~CriteriaText(){delete[]data;}
unsigned char bytes[32];int position=0,size=0;bool found=true,keep=true;CriteriaLog log;const char*error=0;
CriteriaLog*CriteriaContext::CreateErrorText(){keep=false;return &log;}bool CriteriaContext::Continue(){return keep;}CriteriaLog* __fastcall AppendErrorText(CriteriaLog*p,const char*s){error=s;return p;}
int CriteriaReader::FindSegment(const CriteriaText&t){assert(strcmp(t.data,"LHAudioBankCriteiaInfo")==0);return found?0:-1;}
int CriteriaReader::Read(void*p,int n){int got=n;if(got>size-position)got=size-position;memcpy(p,bytes+position,got);position+=got;return got;}
struct Callback:CriteriaCallback{int entries;void Entry(CriteriaContext*,const char*t,unsigned n,const unsigned*d){assert(strcmp(t,"A")==0&&n==1&&d[0]==9);++entries;}};
int main(){unsigned count=1,length=1,n=1,driver=9;memcpy(bytes,&count,4);memcpy(bytes+4,&length,4);bytes[8]='A';memcpy(bytes+9,&n,4);memcpy(bytes+13,&driver,4);CriteriaReader r;CriteriaContext c;Callback cb;cb.entries=0;AudioCriteriaLoader_00c3bcb0 a={&r};size=17;a.LoadCriteria(&c,&cb);assert(cb.entries==1&&!error);int cuts[5]={0,4,8,9,13};for(int i=0;i<5;++i){position=0;size=cuts[i];error=0;a.LoadCriteria(&c,&cb);assert(error);}found=false;position=0;error=0;a.LoadCriteria(&c,&cb);assert(error);puts("00c3bcb0_TEST PASS");}