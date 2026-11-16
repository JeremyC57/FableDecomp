// Self-contained behavior test; source below is pasted, never path-included.
#ifndef _MSC_VER
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// 00c319b0 PKSerialiseCParser::ValidateReader
// Flags: /EHsc
// Leaf: ?ValidateReader@PKSerialiseCParser@@QAEPAUIDataStream@@XZ
#define FABLE_RETAIL_PKSERIALISECPARSER_READER_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "engine/PKSerialiseCParser.h"
#include "engine/CString.h"
// Naming: ValidateReader is descriptive; original libpk symbols are unavailable.
IDataStream* PKSerialiseCParser::ValidateReader()
{
 if(!pReader) {
  {
   CString line;
   line << ".\\PKSerialiseCParser.cpp" << "(" << (long)11 << ") : " << "Null reader" << "\n";
   if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);
  }
  CDebugBreak();
 }
 return pReader;
}

static char empty_text[]="";
char* _afxPchNil=empty_text;
PFNASSERTHOOK g_pfnAssertHook=0;
static char buffer[256];
static int appended,destroyed,logged,broken,hook_change,break_change,throw_at;
static PKSerialiseCParser* active;
static char token_a,token_b;
static IDataStream* first(){return reinterpret_cast<IDataStream*>(&token_a);}
static IDataStream* second(){return reinterpret_cast<IDataStream*>(&token_b);}
static void check(bool ok){if(!ok){fputs("failed parser assertion\n",stderr);exit(1);}}
static void next_append(){
 ++appended;
#ifndef _MSC_VER
 if(throw_at==appended)throw 11;
#endif
}
CString& __fastcall CString::operator<<(const char* text){
 next_append();
 if(m_pchData==empty_text){buffer[0]=0;m_pchData=buffer;}
 strcat(buffer,text);return *this;
}
CString& __fastcall CString::operator<<(long value){
 next_append();check(value==11);strcat(buffer,"11");return *this;
}
CString::~CString(){++destroyed;}
static void __fastcall report(const char* text){
 check(destroyed==0&&broken==0&&appended==6);
 check(strcmp(text,".\\PKSerialiseCParser.cpp(11) : Null reader\n")==0);
 ++logged;
 if(hook_change)active->pReader=first();
#ifndef _MSC_VER
 if(throw_at==7)throw 11;
#endif
}
CDebugBreak::CDebugBreak(){
 check(destroyed==1);++broken;
 if(break_change)active->pReader=second();
#ifndef _MSC_VER
 if(throw_at==8)throw 11;
#endif
}
static void reset(){appended=destroyed=logged=broken=0;buffer[0]=0;}
int main(){
 int cases=0;
 for(int initial=0;initial!=2;++initial)
 for(int hook=0;hook!=2;++hook)
 for(hook_change=0;hook_change!=2;++hook_change)
 for(break_change=0;break_change!=2;++break_change){
  PKSerialiseCParser p;memset(&p,0,sizeof(p));p.pReader=initial?first():0;p.pWriter=second();active=&p;
  reset();throw_at=0;g_pfnAssertHook=hook?report:0;
  IDataStream* result=p.ValidateReader();
  IDataStream* expected=initial?first():(break_change?second():((hook&&hook_change)?first():0));
  check(result==expected&&p.pReader==expected&&p.pWriter==second());
  check(appended==(initial?0:6)&&destroyed==(initial?0:1)&&broken==(initial?0:1));
  check(logged==(!initial&&hook?1:0));++cases;
 }
 check(cases==16);
#ifndef _MSC_VER
 for(throw_at=1;throw_at<=8;++throw_at){
  PKSerialiseCParser p;memset(&p,0,sizeof(p));active=&p;reset();hook_change=break_change=0;g_pfnAssertHook=report;
  bool caught=false;try{p.ValidateReader();}catch(int e){caught=(e==11);}
  check(caught&&destroyed==1);check(broken==(throw_at==8?1:0));
 }
#endif
 puts("01214130_TEST PASS");return 0;
}