// Self-contained actual shared-header test; parent implementation pasted below.
#ifndef _MSC_VER
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// 00c4b160 PKCBufferedReader::PollReadCompletion
// Flags: /EHsc
// Leaf: ?PollReadCompletion@PKCBufferedReader@@QAE_NXZ
#define FABLE_RETAIL_PKCBUFFEREDREADER_POLL_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "libpk/PKCBufferedReader.h"
#include "engine/CString.h"
// Naming: PollReadCompletion is descriptive; libpk original symbols unavailable.
bool PKCBufferedReader::PollReadCompletion()
{
 while(m_outstanding!=0) {
  if(m_mailbox.GetCount()==0)break;
  bool more;
  ReturnBuffer(&more);
  if(!more) {
   {
    CString line;
    line << ".\\PKCBufferedReader.cpp" << "(" << (long)482 << ") : " << "Error: Should not be blocked here..." << "\n";
    if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);
   }
   CDebugBreak();
  }
  Refill();
 }
 return m_outstanding==0;
}

static char empty_text[]="";
char* _afxPchNil=empty_text;
PFNASSERTHOOK g_pfnAssertHook=0;
static char buffer[256];
static int appended,destroyed,logged,broken,throw_at,queue_count,more_mask,returned,refilled,queries,add_once;
static PKCBufferedReader* active;
static void check(bool ok){if(!ok){fputs("buffer polling assertion failed\n",stderr);exit(1);}}
PKCCriticalSection::PKCCriticalSection(){memset(m_section,0,sizeof(m_section));}
PKCCriticalSection::~PKCCriticalSection(){}
PKCSemaphore::PKCSemaphore(long initialCount,long maximumCount):m_hSemaphore(0){check(initialCount==0&&maximumCount==0);}
PKCSemaphore::~PKCSemaphore(){}
long PKCMailbox::GetCount(){check(this==&active->m_mailbox);++queries;return queue_count;}
void PKCBufferedReader::ReturnBuffer(bool* more){
 check(this==active&&more&&queue_count>0&&m_outstanding>0);
 *more=(more_mask&(1<<returned))!=0;
 ++returned;--queue_count;--m_outstanding;
}
void PKCBufferedReader::Refill(){
 check(this==active&&returned==refilled+1&&destroyed==broken);
 ++refilled;if(add_once){++m_outstanding;add_once=0;}
}
static void next_append(){
 ++appended;
#ifndef _MSC_VER
 if(throw_at==appended)throw 17;
#endif
}
CString& __fastcall CString::operator<<(const char* text){
 if(m_pchData==empty_text){buffer[0]=0;m_pchData=buffer;appended=0;}
 next_append();strcat(buffer,text);return *this;
}
CString& __fastcall CString::operator<<(long value){next_append();check(value==482);strcat(buffer,"482");return *this;}
CString::~CString(){++destroyed;}
static void __fastcall report(const char* text){
 check(destroyed==broken&&appended==6);
 check(strcmp(text,".\\PKCBufferedReader.cpp(482) : Error: Should not be blocked here...\n")==0);++logged;
#ifndef _MSC_VER
 if(throw_at==7)throw 17;
#endif
}
CDebugBreak::CDebugBreak(){check(destroyed==broken+1);++broken;
#ifndef _MSC_VER
 if(throw_at==8)throw 17;
#endif
}
static void reset(){appended=destroyed=logged=broken=returned=refilled=queries=0;buffer[0]=0;}
int main(){
 const int masks[4]={0,1,5,31};int cases=0;
 for(int initial=0;initial<=4;++initial)
 for(int available=0;available<=4;++available)
 for(int mi=0;mi<4;++mi)
 for(int hook=0;hook!=2;++hook)
 for(int add=0;add!=2;++add){
  PKCBufferedReader p={{0},{PKCCriticalSection(),0,PKCSemaphore(0,0)},0};
  memset(p._unknown00,0xa5,sizeof(p._unknown00));p.m_outstanding=initial;active=&p;reset();
  queue_count=available;more_mask=masks[mi];g_pfnAssertHook=hook?report:0;add_once=add;throw_at=0;
  bool result=p.PollReadCompletion();
  int added=(initial>0&&available>0)?add:0;
  int n=initial==0?0:((initial+added<available)?initial+added:available);
  int remaining=initial+added-n;int errors=0;
  for(int k=0;k<n;++k)if(!(masks[mi]&(1<<k)))++errors;
  check(result==(remaining==0)&&p.m_outstanding==remaining&&queue_count==available-n);
  check(returned==n&&refilled==n&&queries==(initial? n+(remaining>0?1:0):0));
  check(destroyed==errors&&broken==errors&&logged==(hook?errors:0));
  for(unsigned int x=0;x<sizeof(p._unknown00);++x)check(p._unknown00[x]==0xa5);
  check(p.m_mailbox.m_pQueue==0);++cases;
 }
 check(cases==400);
#ifndef _MSC_VER
 for(throw_at=1;throw_at<=8;++throw_at){
  PKCBufferedReader p={{0},{PKCCriticalSection(),0,PKCSemaphore(0,0)},2};active=&p;reset();queue_count=2;more_mask=0;add_once=0;g_pfnAssertHook=report;
  bool caught=false;try{p.PollReadCompletion();}catch(int e){caught=(e==17);}
  check(caught&&destroyed==1&&refilled==0&&returned==1&&p.m_outstanding==1);
  check(broken==(throw_at==8?1:0));
 }
#endif
 puts("01215d9e_TEST PASS");return 0;
}