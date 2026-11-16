// 00c49470 CCodaFrameStreamer::SubmitFrame (EH parent; its generated cleanup 0x01215B80 lands as a separate funclet row that includes this file)
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