#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// BEGIN exact source snapshot SHA256 2818b8108e56a8bfc81dc8240d5cad693747635a41018d3f76844e78b3e3b578
// 00c3bb50 AudioBankTableReader_00c3bb50::Load
// Flags: /O2 /Oy /EHsc /GS
// Leaf: Load
// Layout: retail receiver+0 reader, resource stack block28c with enabled104. String LHAudioBankSampleTable identifies table format; owning class uncertain.
struct String{char* data;String(const char*);~String();};
struct Reader{int Find(const String&);int Read(void*,unsigned);};
struct Log{Log& __fastcall Text(const char*);Log& __fastcall Number(unsigned);};
struct Context{Log& CreateErrorText_00c30840();bool Good();};
struct Callback{virtual void Resource(Context*,const void*)=0;};
struct AudioBankTableReader_00c3bb50{Reader* reader;void Load(Context*,Callback*);};
void AudioBankTableReader_00c3bb50::Load(Context* context,Callback* callback){
 if(reader->Find(String("LHAudioBankSampleTable"))<0){context->CreateErrorText_00c30840().Text("Could not find the bank sample table segment");return;}
 unsigned count;if(reader->Read(&count,4)<4){context->CreateErrorText_00c30840().Text("Could not read the number of samples!");return;}
 count&=0xffff;char resource[0x28c];for(unsigned index=0;index<count;++index){
  if((unsigned)reader->Read(resource,sizeof(resource))<sizeof(resource)){context->CreateErrorText_00c30840().Text("Could not read in resource ").Number(index);return;}
  if(*(int*)(resource+0x104)){callback->Resource(context,resource);if(!context->Good())return;}
 }
}

// END exact source snapshot
String::String(const char* s){data=(char*)malloc(strlen(s)+1);strcpy(data,s);}String::~String(){free(data);}static int mode,reads,errors,calls;static bool good=true;static Log logObject;
int Reader::Find(const String& s){assert(!strcmp(s.data,"LHAudioBankSampleTable"));return mode==1?-1:0;}int Reader::Read(void* p,unsigned n){++reads;if(n==4){*(unsigned*)p=2;return mode==2?0:4;}if(mode==3)return 0;memset(p,0,n);*(int*)((char*)p+0x104)=1;return n;}Log& Context::CreateErrorText_00c30840(){++errors;good=false;return logObject;}bool Context::Good(){return good;}Log& __fastcall Log::Text(const char* text){assert(text && *text);return *this;}Log& __fastcall Log::Number(unsigned n){assert(n==0);return *this;}struct Handler:Callback{void Resource(Context*,const void*){++calls;}};
int main(){Reader r;AudioBankTableReader_00c3bb50 loader={&r};Context c;Handler h;loader.Load(&c,&h);assert(calls==2 && errors==0);for(mode=1;mode<=3;++mode){loader.Load(&c,&h);assert(errors==mode);}mode=0;good=false;calls=0;loader.Load(&c,&h);assert(calls==1);puts("00c3bb50_TEST PASS");return 0;}