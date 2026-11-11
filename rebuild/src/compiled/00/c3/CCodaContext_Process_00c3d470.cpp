// CCodaContext::Process() -- commit pending audio changes and process ready output frames
// Leaf: ?Process@CCodaContext@@QAEXXZ
// Retail: 00c3d470,408 bytes,thiscall.
// Flags: none
#define FABLE_RETAIL_CCODACONTEXT_PROCESS
#define FABLE_RETAIL_CCODACONTEXT_SETERROR
#include "engine/CCodaContext.h"
#include "engine/CCodaListener.h"
#include "engine/CCoda3DSource.h"
#include "engine/CSalPlatform.h"
FABLE_STATIC_ASSERT(sizeof(CCodaContext)==168);
FABLE_STATIC_ASSERT(sizeof(CCodaSource)==252);
FABLE_STATIC_ASSERT(sizeof(CCoda3DSource)==476);
FABLE_STATIC_ASSERT(sizeof(CCodaListener)==76);
FABLE_STATIC_ASSERT(sizeof(CCodaEnvironment)==128);
FABLE_STATIC_ASSERT(sizeof(CSalPlatform)==472);
void CCodaContext::Process()
{
 bool bCommit=(m_ffUpdate>>31)&1;
 unsigned int ffCommit;
 if(bCommit){
  m_ffUpdate&=0x7fffffff;
  for(CCodaObject* pObject=m_Listeners.GetObjects();pObject;){
   CCodaListener* pListener=static_cast<CCodaListener*>(pObject);
   CCodaObject* pNext=pObject->GetNext();
   pListener->Commit();
   pObject=pNext;
  }
  ffCommit=CCodaListener::GetCommit()|m_ffUpdate|CCodaEnvironment::GetSourceCommit();
 }else ffCommit=0;
 bool bProcessSources=true;
 while(m_pSal->ReadyToProcess()){
  if(bProcessSources){
   CREFTIME time=m_pSal->GetReferenceTime();
   bProcessSources=false;
   for(CCodaObject* pObject=m_3DSources.GetObjects();pObject;){
    CCodaSource* pSource=static_cast<CCodaSource*>(pObject);
    CCodaObject* pNext=pObject->GetNext();
    if(bCommit)pSource->Get3D()->Commit(m_Listeners,m_Environments,ffCommit);
    if(pSource->IsPlaying()){pSource->Process(time);bProcessSources=true;}
    pObject=pNext;
   }
   m_pSal->ProcessXtc();
   for(CCodaObject* pObject=m_2DSources.GetObjects();pObject;){
    CCodaSource* pSource=static_cast<CCodaSource*>(pObject);
    CCodaObject* pNext=pObject->GetNext();
    if(pSource->IsPlaying()){pSource->Process(time);bProcessSources=true;}
    pObject=pNext;
   }
  }
  for(CCodaObject* pObject=m_Environments.GetObjects();pObject;){
   CCodaEnvironment* pEnvironment=static_cast<CCodaEnvironment*>(pObject);
   CCodaObject* pNext=pObject->GetNext();
   if(bCommit)pEnvironment->Commit();
   pEnvironment->Process();
   pObject=pNext;
  }
  m_pSal->ProcessOutput();
  CCodaListener::ClearCommit();
  CCodaEnvironment::ClearSourceCommit();
  m_ffUpdate=0;
 }
}