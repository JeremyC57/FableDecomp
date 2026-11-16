// Standalone behaviour fixture; the actual parent source is pasted below.
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define FABLE_RETAIL_CCODA_HOST_ALLOCATION_PASS23
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
// 01215b80 CCodaFrameStreamer::SubmitFrame generated cleanup
// Leaf: ?SubmitFrame@CCodaFrameStreamer@@UAEXPAVCCodaFrame@@@Z
// Flags: /EHsc
// Required parent00c49470,415 bytes; generated cleanup01215b80,8 bytes.
#define FABLE_RETAIL_CCODAFRAMESTREAMER_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#include "libpk/CCodaFrameStreamer.h"
#include "libpk/PKCProtectionInstance.h"
#include "engine/CString.h"
#include "engine/CSalSwBaseVoice.h"
extern PKCCriticalSection g_soundLock;
// Original CODA.h signatures, corroborated by the native wrappers and callers.
extern "C" void* __stdcall CODA_BufferCreate(const _CBUFFERDESC*);
extern "C" int __stdcall CODA_SourceQueueBuffer(void*,void*,CREFTIME);
extern "C" int __stdcall CODA_SourceIsPlaying(void*);
extern "C" void __stdcall CODA_SourcePlay(void*,int);
void CCodaFrameStreamer::SubmitFrame(CCodaFrame* frame)
{
 if(m_framesOut==0){
  {
   CString s;
   s<<".\\CCodaFrameStreamer.cpp"<<"("<<20L<<") : "<<"Must have got the frame from here!?!"<<"\n";
   if(g_pfnAssertHook)g_pfnAssertHook(s.m_pchData);
  }
  CDebugBreak();
 }
 --m_framesOut;
 if(frame->m_samples==0 || frame->m_channels!=m_channels){
  CCodaFrame* returned=frame;
  m_freeFrames.ProcessMessage(&returned);
  return;
 }

  _CBUFFERDESC desc;
  desc.nChannels=m_channels;
  desc.cbSize=frame->m_channels*frame->m_samples*2;
  desc.nSamplesPerSec=m_rate;
  desc.ffFormat=1;
  desc.cbLoopStart=0;desc.cbLoopEnd=0;
  desc.pvData=frame->GetData();
  PKCProtectionInstance lock(&g_soundLock);
  frame->m_hBuffer=CODA_BufferCreate(&desc);
  CODA_SourceQueueBuffer(m_pSource,frame->m_hBuffer,0);
  m_inFlight.Push(frame);
  if(!CODA_SourceIsPlaying(m_pSource))CODA_SourcePlay(m_pSource,1);
}

static void check(bool x){if(!x){fputs("frame submit assertion failed\n",stderr);exit(1);}}
static char emptyText[]="",trace[256];
char* _afxPchNil=emptyText;
PFNASSERTHOOK g_pfnAssertHook=0;
static int appended,destroyed,reported,broken,entered,left,got,created,queued,pushed,queried,played,returned;
static int isPlaying,failAt,locked;
static void* createResult;
static CCodaFrameStreamer* active;
static CCodaFrame* current;
static unsigned long expectedSamples,expectedChannels;
static long initialOut;
static int events[32],eventCount;
static void step(int id){events[eventCount++]=id;
#ifndef _MSC_VER
 if(failAt==id)throw id;
#endif
}
PKCCriticalSection::PKCCriticalSection(){memset(m_section,0,sizeof(m_section));}
PKCCriticalSection::~PKCCriticalSection(){}
PKCCriticalSection g_soundLock;
PKCSemaphore::PKCSemaphore(long initial,long maximum):m_hSemaphore(0){check(initial==0&&maximum==3);}
PKCSemaphore::~PKCSemaphore(){}
template<class T> PKCStaticQueue<T>::PKCStaticQueue(long capacity):m_capacity(capacity),m_pSlots(0),m_used(0),m_writeIndex(0){}
template<class T> T PKCStaticQueue<T>::Pop(){check(false);return T();}
template<class T> void PKCStaticQueue<T>::Push(const T*){check(false);}
template<class T> long PKCStaticQueue<T>::GetCount(){check(false);return 0;}
template<class T> long PKCStaticQueue<T>::IsFull(){check(false);return 0;}
CMailboxQueue::CMailboxQueue(void* queue,long capacity):m_pQueue(static_cast<PKCStaticQueue<CCodaFrame*>*>(queue)),m_semaphore(0,capacity){}
CCodaFrameStreamer::CCodaFrameStreamer(unsigned long channels,long rate,unsigned long frames,unsigned long frameSamples,float volume):m_freeQueue(frames),m_freeFrames(&m_freeQueue,frames){
 check(frameSamples==16&&volume==1.0f);m_pSource=this;m_frames=frames;m_rate=rate;m_channels=channels;m_framesOut=0;
 m_inFlight.m_capacity=frames;m_inFlight.m_pSlots=0;m_inFlight.m_count=0;m_inFlight.m_tail=0;
}
CCodaFrameStreamer::~CCodaFrameStreamer(){}
long CCodaFrameStreamer::GetFreeFrameCount(){check(false);return 0;}
long CCodaFrameStreamer::GetQueuedFrameCount(){check(false);return 0;}
CCodaFrame* CCodaFrameStreamer::GetFrame(){check(false);return 0;}
long CCodaFrameStreamer::GetSampleRate(){return m_rate;}
void CCodaFrameStreamer::SetSourceVolume(float){check(false);}
CString& __fastcall CString::operator<<(const char* text){
 check(initialOut==0&&active->m_framesOut==0&&!locked);
 if(m_pchData==emptyText){trace[0]=0;m_pchData=trace;}
 ++appended;step(1);strcat(trace,text);return *this;
}
CString& __fastcall CString::operator<<(long n){check(n==20);++appended;step(1);strcat(trace,"20");return *this;}
CString::~CString(){++destroyed;}
static void __fastcall report(const char* text){
 check(strcmp(text,".\\CCodaFrameStreamer.cpp(20) : Must have got the frame from here!?!\n")==0);
 check(destroyed==0&&appended==6);++reported;step(2);
}
CDebugBreak::CDebugBreak(){check(destroyed==1&&appended==6);++broken;step(3);}
short* CCodaFrame::GetData(){check(this==current&&!locked&&active->m_framesOut==initialOut-1);++got;step(4);return m_pData;}
PKCProtectionInstance::PKCProtectionInstance(PKCCriticalSection* section):m_pSection(section),m_tick(0){
 check(section==&g_soundLock&&!locked&&got==1);++entered;locked=1;step(5);
}
PKCProtectionInstance::~PKCProtectionInstance(){check(m_pSection==&g_soundLock&&locked);locked=0;++left;}
extern "C" void* __stdcall CODA_BufferCreate(const _CBUFFERDESC* desc){
 check(locked&&desc->ffFormat==1&&desc->nChannels==expectedChannels&&desc->nSamplesPerSec==(unsigned int)active->m_rate);
 check(desc->cbSize==expectedChannels*expectedSamples*2&&desc->cbLoopStart==0&&desc->cbLoopEnd==0&&desc->pvData==current->m_pData);
 ++created;step(6);return createResult;
}
extern "C" int __stdcall CODA_SourceQueueBuffer(void* source,void* buffer,CREFTIME when){
 check(locked&&source==active->m_pSource&&buffer==createResult&&current->m_hBuffer==createResult&&when==0);++queued;step(7);return 0;
}
void CLockedRingBuffer::Push(CCodaFrame* frame){check(locked&&this==&active->m_inFlight&&frame==current&&queued==1);++pushed;step(8);}
extern "C" int __stdcall CODA_SourceIsPlaying(void* source){check(locked&&source==active->m_pSource&&pushed==1);++queried;step(9);return isPlaying;}
extern "C" void __stdcall CODA_SourcePlay(void* source,int looping){check(locked&&source==active->m_pSource&&queried==1&&looping==1&&!isPlaying);++played;step(10);}
long CMailboxQueue::ProcessMessage(void* message){check(!locked&&this==&active->m_freeFrames&&*static_cast<CCodaFrame**>(message)==current&&active->m_framesOut==initialOut-1);++returned;step(11);return 0;}
static void reset(){appended=destroyed=reported=broken=entered=left=got=created=queued=pushed=queried=played=returned=locked=eventCount=0;trace[0]=0;}
int main(){
#ifdef _MSC_VER
 check(sizeof(CCodaFrame)==20&&sizeof(CCodaFrameStreamer)==116&&sizeof(CMailboxQueue)==32&&sizeof(CLockedRingBuffer)==40);
 check(offsetof(CCodaFrameStreamer,m_freeQueue)==8&&offsetof(CCodaFrameStreamer,m_freeFrames)==0x1c&&offsetof(CCodaFrameStreamer,m_inFlight)==0x48&&offsetof(CCodaFrameStreamer,m_framesOut)==0x70);
#endif
 short samples[32];memset(samples,0,sizeof(samples));int cases=0;const long outs[3]={0,1,3};const unsigned long counts[3]={0,1,8};
 for(int oi=0;oi<3;++oi)for(int ni=0;ni<3;++ni)for(int fc=1;fc<=2;++fc)for(int sc=1;sc<=2;++sc)
 for(int playing=0;playing<2;++playing)for(int hook=0;hook<2;++hook)for(int ri=0;ri<2;++ri)for(int nullBuffer=0;nullBuffer<2;++nullBuffer){
  CCodaFrameStreamer streamer(sc,ri?44100:22050,3,16,1.0f);CCodaFrame frame;memset(&frame,0,sizeof(frame));
  frame.m_channels=fc;frame.m_capacity=16;frame.m_samples=counts[ni];frame.m_hBuffer=&samples[0];frame.m_pData=samples;
  streamer.m_framesOut=outs[oi];active=&streamer;current=&frame;initialOut=outs[oi];expectedSamples=counts[ni];expectedChannels=fc;
  isPlaying=playing;createResult=nullBuffer?0:static_cast<void*>(&samples[16]);g_pfnAssertHook=hook?report:0;failAt=0;reset();
  streamer.SubmitFrame(&frame);const int valid=counts[ni]!=0&&fc==sc;
  check(streamer.m_framesOut==initialOut-1&&frame.m_channels==(unsigned long)fc&&frame.m_samples==counts[ni]&&frame.m_capacity==16&&frame.m_pData==samples);
  check(streamer.m_pSource==&streamer&&streamer.m_rate==(ri?44100:22050)&&streamer.m_channels==(unsigned long)sc&&streamer.m_frames==3);
  check(appended==(initialOut==0?6:0)&&destroyed==(initialOut==0)&&broken==(initialOut==0)&&reported==(initialOut==0&&hook));
  check(got==valid&&entered==valid&&left==valid&&created==valid&&queued==valid&&pushed==valid&&queried==valid&&played==(valid&&!playing)&&returned==!valid&&!locked);
  check(frame.m_hBuffer==(valid?createResult:static_cast<void*>(&samples[0])));
  int k=0;if(initialOut==0){for(int q=0;q<6;++q)check(events[k++]==1);if(hook)check(events[k++]==2);check(events[k++]==3);}
  if(valid){for(int q=4;q<=9;++q)check(events[k++]==q);if(!playing)check(events[k++]==10);}else check(events[k++]==11);check(k==eventCount);++cases;
 }
 check(cases==576);
#ifndef _MSC_VER
 const int failures[9]={1,2,3,4,6,7,8,9,10};
 for(int fi=0;fi<9;++fi){
  CCodaFrameStreamer streamer(2,44100,3,16,1.0f);CCodaFrame frame;memset(&frame,0,sizeof(frame));frame.m_channels=2;frame.m_samples=8;frame.m_pData=samples;
  active=&streamer;current=&frame;initialOut=0;expectedSamples=8;expectedChannels=2;isPlaying=0;createResult=samples;g_pfnAssertHook=report;reset();failAt=failures[fi];
  bool caught=false;try{streamer.SubmitFrame(&frame);}catch(int n){caught=n==failAt;}
  check(caught&&destroyed==1&&!locked&&left==(failAt>=6)&&entered==(failAt>=6));
  check(streamer.m_framesOut==(failAt<=3?0:-1));
 }
#endif
 puts("01215b80_TEST PASS");return 0;
}