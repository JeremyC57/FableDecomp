// Exact selected row and queue predicates; external Play/SetError are controlled boundaries.
#include <assert.h>
#include <stdio.h>
#ifndef _MSC_VER
#define __stdcall
#endif
typedef void* CHANDLE;
typedef int CBOOL;
#define C_ERR_NOTALLOCATED (-7)
class CSalBuffer{};
struct CQUEUEFORMAT{unsigned int ffFormat,nChannels,nSamplesPerSec;};
class CCodaBufferQueue{
public:
 unsigned int m_nBuffers;CQUEUEFORMAT m_Format;
 unsigned int GetSize()const{return m_nBuffers;}
 bool IsCreated()const{return GetSize()!=0;}
 bool IsEmpty()const{return IsCreated() && m_Format.ffFormat==0;}
};
static int play_calls,last_error,error_calls,play_error;static bool last_loop;
class CCodaSource{
public:
 CCodaBufferQueue m_Queue;CSalBuffer* m_pBuffer;
 bool IsBufferSet()const{return m_pBuffer!=0 || (m_Queue.IsCreated() && !m_Queue.IsEmpty());}
 int Play(bool bLooping){++play_calls;last_loop=bLooping;return play_error;}
};
class CCodaContext{public:void SetError(int error){++error_calls;last_error=error;}};
CCodaContext CODA;
extern "C" CBOOL __stdcall CODA_SourcePlay(CHANDLE hSource,CBOOL bLooping){
 CCodaSource* pSource=static_cast<CCodaSource*>(hSource);
 if(!pSource->IsBufferSet()){CODA.SetError(C_ERR_NOTALLOCATED);return 0;}
 int error=pSource->Play(bLooping!=0);
 if(error){CODA.SetError(error);return 0;}
 return 1;
}

int main(){
 const int sizes[]={0,1,17},formats[]={0,1,0x1234},loops[]={0,1,-9},errors[]={0,-1,-7,19};int cases=0;
 for(int has=0;has<2;++has)for(int n=0;n<3;++n)for(int f=0;f<3;++f)for(int b=0;b<3;++b)for(int e=0;e<4;++e){
  CSalBuffer buffer;CCodaSource source;source.m_pBuffer=has?&buffer:0;source.m_Queue.m_nBuffers=sizes[n];source.m_Queue.m_Format.ffFormat=formats[f];
  source.m_Queue.m_Format.nChannels=2;source.m_Queue.m_Format.nSamplesPerSec=44100;
  assert(source.m_Queue.IsCreated()==(sizes[n]!=0));assert(source.m_Queue.IsEmpty()==(sizes[n]!=0&&formats[f]==0));
  bool ready=has||(sizes[n]!=0&&formats[f]!=0);assert(source.IsBufferSet()==ready);
  play_calls=error_calls=0;last_error=123;last_loop=false;play_error=errors[e];
  CBOOL result=CODA_SourcePlay(&source,loops[b]);
  assert(result==(ready&&errors[e]==0));assert(play_calls==(ready?1:0));
  if(ready)assert(last_loop==(loops[b]!=0));
  if(!ready){assert(error_calls==1&&last_error==C_ERR_NOTALLOCATED);}else if(errors[e]){assert(error_calls==1&&last_error==errors[e]);}else{assert(error_calls==0&&last_error==123);}
  ++cases;
 }
 assert(cases==216);puts("00c35dd0_TEST PASS");return 0;
}