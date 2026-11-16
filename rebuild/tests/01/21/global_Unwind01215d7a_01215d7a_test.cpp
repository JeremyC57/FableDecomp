// Self-contained shared-header behavior test; actual parent body pasted unchanged.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#else
// Fixture resolves the external debugger API locally, without invoking Wine's debugger.
// Only the import decoration is omitted; the parent and helper bodies are unchanged.
#define __declspec(x)
#endif
// 00c4aeb0 PKCBufferedReader::WaitForSlot
// Flags: /EHsc
// Leaf: ?WaitForSlot@PKCBufferedReader@@QAE_NK@Z
#define FABLE_RETAIL_PKCBUFFEREDREADER_RETURN_CONTEXT_PASS23
#define FABLE_RETAIL_PKCBUFFEREDREADER_WAIT_PASS23
#define FABLE_RETAIL_PKCBUFFEREDREADER_POLL_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "libpk/PKCBufferedReader.h"
#include "engine/CString.h"
// Inline-budget: native00c4af85 calls the actual13-byte CDebugBreak constructor.
// Its real imported DebugBreak body stays visible but is not expanded here.
#if defined(_MSC_VER)
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
#pragma auto_inline(off)
#else
extern "C" void DebugBreak();
#endif
CDebugBreak::CDebugBreak(){DebugBreak();}
#if defined(_MSC_VER)
#pragma auto_inline(on)
#endif

// Complete genuine native C4AC70 context: assertion traces, optional ready out-flag,
// blocking mailbox result, modulo slot-owner clearing and outstanding decrement.
// This existing helper is context only; it is not an additional reserved row.
void PKCBufferedReader::ReturnBuffer(bool* more)
{
 if(m_outstanding==0){
  {CString line;line << ".\\PKCBufferedReader.cpp" << "(" << (long)56 << ") : " << "Error: There are no requests pending!" << "\n";
   if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);}
  CDebugBreak();
 }
 if(more)*more=m_mailbox.GetCount()!=0;
 unsigned long request=m_mailbox.GetResult();
 if(request>=m_slotCount){
  {CString line;line << ".\\PKCBufferedReader.cpp" << "(" << (long)62 << ") : " << "Error: returned buffer out of scope" << "\n";
   if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);}
  CDebugBreak();
 }
 if(m_slotOwners[request]!=request){
  {CString line;line << ".\\PKCBufferedReader.cpp" << "(" << (long)63 << ") : " << "Error: Request ID out of sync" << "\n";
   if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);}
  CDebugBreak();
 }
 for(unsigned long i=0;i<m_slotCount;++i){
  unsigned char& owner=m_slotOwners[(i+request)%m_slotCount];
  if(owner==request)owner=0xff;else i=m_slotCount;
 }
 --m_outstanding;
}

// Naming: descriptive libpk method/locals; actual layout and ABI are documented.
bool PKCBufferedReader::WaitForSlot(unsigned long slot)
{
 const unsigned long slotCount=m_slotCount;
 bool immediate=true;
 if(slot>=slotCount) {
  {
   CString line;
   line << ".\\PKCBufferedReader.cpp" << "(" << (long)84 << ") : " << "Error: Request number (" << slot << ") out of scope (" << slotCount << ")" << "\n";
   if(g_pfnAssertHook)g_pfnAssertHook(line.m_pchData);
  }
  CDebugBreak();
 }
 {
 bool more;
 while(m_slotOwners[slot]!=0xff) {
  ReturnBuffer(&more);
  if(!more)immediate=false;
 }
 }
 return immediate;
}

static void check(bool ok){if(!ok){fputs("WaitForSlot assertion failed\n",stderr);exit(1);}}
static char empty_text[]="";char* _afxPchNil=empty_text;PFNASSERTHOOK g_pfnAssertHook=0;
static char text_buffer[256],expected_text[256];
static int appended,destroyed,logged,broken,throw_at,remaining,calls,mask,swap_arrays,queued[16],queue_at,getcounts,repair;
static unsigned long active_slot;static unsigned char* alternate;static PKCBufferedReader* active;
PKCCriticalSection::PKCCriticalSection(){memset(m_section,0,sizeof(m_section));}PKCCriticalSection::~PKCCriticalSection(){}
PKCSemaphore::PKCSemaphore(long initial,long maximum):m_hSemaphore(0){check(initial==0&&maximum==0);}PKCSemaphore::~PKCSemaphore(){}
static void next_append(){++appended;
#ifndef _MSC_VER
 if(throw_at==appended)throw 23;
#endif
}
CString& __fastcall CString::operator<<(const char* text){if(m_pchData==empty_text){text_buffer[0]=0;m_pchData=text_buffer;}next_append();strcat(text_buffer,text);return *this;}
CString& __fastcall CString::operator<<(long value){next_append();char digits[32];sprintf(digits,"%ld",value);strcat(text_buffer,digits);return *this;}
CString& __fastcall CString::operator<<(unsigned long value){next_append();char digits[32];sprintf(digits,"%lu",value);strcat(text_buffer,digits);return *this;}
CString::~CString(){++destroyed;}
static void __fastcall report(const char* text){check(destroyed==0&&strcmp(text,expected_text)==0);++logged;
#ifndef _MSC_VER
 if(throw_at==11)throw 23;
#endif
}
extern "C" void __stdcall DebugBreak(){check(destroyed==1);++broken;if(repair==1)active->m_outstanding=1;else if(repair==2)active->m_slotCount=2;else if(repair==3)active->m_slotOwners[queued[0]]=(unsigned char)queued[0];
#ifndef _MSC_VER
 if(throw_at==12)throw 23;
#endif
}

long PKCMailbox::GetCount(){check(this==&active->m_mailbox&&queue_at<remaining);++getcounts;return(mask&(1<<queue_at))?1:0;}
unsigned long PKCMailbox::GetResult(){check(this==&active->m_mailbox&&queue_at<remaining);if(swap_arrays){active->m_slotOwners=alternate;swap_arrays=0;}++calls;return queued[queue_at++];}
static void reset(){appended=destroyed=logged=broken=calls=queue_at=getcounts=0;text_buffer[0]=0;}
int main(){unsigned cases=0;const int masks[]={0,1,85,255};
 for(unsigned long count=1;count<=8;++count)for(unsigned long target=0;target<count;++target)for(unsigned long rotation=0;rotation<count;++rotation)for(int mi=0;mi<4;++mi)for(int sw=0;sw<2;++sw)for(int hook=0;hook<2;++hook){
  unsigned char owners[8],other[8];memset(owners,0xff,8);memset(other,0xff,8);for(unsigned long k=0;k<count;++k)owners[k]=other[k]=(unsigned char)k;
  PKCBufferedReader p={{0},owners,{0},count,{0},{PKCCriticalSection(),0,PKCSemaphore(0,0)},(long)count};
  active=&p;active_slot=target;alternate=other;remaining=(int)count;mask=masks[mi];swap_arrays=sw;throw_at=0;g_pfnAssertHook=hook?report:0;reset();
  int expected_calls=0;bool expected=true;
  for(unsigned long k=0;k<count;++k){queued[k]=(int)((rotation+k)%count);if((unsigned long)queued[k]==target)expected_calls=(int)k+1;}
  for(int k=0;k<expected_calls;++k)if(!(mask&(1<<k)))expected=false;
  bool result=p.WaitForSlot(target);
  check(result==expected&&calls==expected_calls&&getcounts==expected_calls&&queue_at==expected_calls&&p.m_slotOwners[target]==0xff&&p.m_outstanding==(long)count-expected_calls&&p.m_slotCount==count);
  for(unsigned long k=0;k<count;++k){bool freed=false;for(int j=0;j<expected_calls;++j)if((unsigned long)queued[j]==k)freed=true;check(p.m_slotOwners[k]==(freed?0xff:k));}
  check(appended==0&&destroyed==0&&logged==0&&broken==0);++cases;
 }
 check(cases==3264);
 // Empty slots: normal fast path and out-of-range trace without calling ReturnBuffer.
 for(unsigned long index=0;index<5;++index)for(unsigned long limit=0;limit<5;++limit)for(int hook=0;hook<2;++hook){
  unsigned char owners[8];memset(owners,0xff,8);PKCBufferedReader p={{0},owners,{0},limit,{0},{PKCCriticalSection(),0,PKCSemaphore(0,0)},17};active=&p;remaining=0;throw_at=0;g_pfnAssertHook=hook?report:0;reset();
  sprintf(expected_text,".\\PKCBufferedReader.cpp(84) : Error: Request number (%lu) out of scope (%lu)\n",index,limit);
  check(p.WaitForSlot(index)&&calls==0&&p.m_outstanding==17);int err=index>=limit?1:0;check(appended==10*err&&destroyed==err&&broken==err&&logged==(hook?err:0));++cases;
 }
 // Complete ReturnBuffer context: contiguous runs crossing the ring end; optional out flag.
 for(unsigned long count=1;count<=8;++count)for(unsigned long first=0;first<count;++first)for(unsigned long width=1;width<=count;++width)for(int out=0;out<2;++out)for(int ready=0;ready<2;++ready){
  unsigned char owners[8];memset(owners,0xff,8);for(unsigned long k=0;k<width;++k)owners[(first+k)%count]=(unsigned char)first;
  PKCBufferedReader p={{0},owners,{0},count,{0},{PKCCriticalSection(),0,PKCSemaphore(0,0)},1};active=&p;remaining=1;queued[0]=(int)first;mask=ready;swap_arrays=0;throw_at=0;g_pfnAssertHook=0;reset();bool more=!ready;p.ReturnBuffer(out?&more:0);
  check(calls==1&&getcounts==out&&p.m_outstanding==0&&(!out||more==(ready!=0)));for(int k=0;k<8;++k)check(owners[k]==0xff);check(appended==0&&destroyed==0&&broken==0);++cases;
 }

 check(cases==4130);
 // Each genuine assertion branch with a returning debugger hook that repairs the diagnosed state.
 for(int fault=1;fault<=3;++fault)for(int hook=0;hook<2;++hook)for(int out=0;out<2;++out){
  unsigned char owners[8];memset(owners,0xff,8);owners[1]=fault==3?0:1;
  PKCBufferedReader p={{0},owners,{0},fault==2?1UL:2UL,{0},{PKCCriticalSection(),0,PKCSemaphore(0,0)},fault==1?0:1};active=&p;remaining=1;queued[0]=1;mask=1;swap_arrays=0;throw_at=0;repair=fault;g_pfnAssertHook=hook?report:0;reset();
  const char* message=fault==1?"Error: There are no requests pending!":fault==2?"Error: returned buffer out of scope":"Error: Request ID out of sync";
  sprintf(expected_text,".\\PKCBufferedReader.cpp(%d) : %s\n",fault==1?56:fault==2?62:63,message);bool more=false;p.ReturnBuffer(out?&more:0);
  check(calls==1&&getcounts==out&&p.m_outstanding==0&&owners[1]==0xff&&appended==6&&destroyed==1&&broken==1&&logged==hook&&(!out||more));++cases;repair=0;
 }
 check(cases==4142);

#ifndef _MSC_VER
 for(throw_at=1;throw_at<=12;++throw_at){unsigned char owners[8];memset(owners,0xff,sizeof(owners));PKCBufferedReader p={{0},owners,{0},0,{0},{PKCCriticalSection(),0,PKCSemaphore(0,0)},17};active=&p;remaining=0;g_pfnAssertHook=report;reset();strcpy(expected_text,".\\PKCBufferedReader.cpp(84) : Error: Request number (0) out of scope (0)\n");bool caught=false;try{p.WaitForSlot(0);}catch(int e){caught=e==23;}check(caught&&destroyed==1&&calls==0&&broken==(throw_at==12?1:0));}
#endif
 puts("01215d7a_TEST PASS");return 0;
}