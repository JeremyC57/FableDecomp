// Leaf: ?Create2DSource@CCodaContext@@QAEPAVCCoda2DSource@@I@Z
// 00c3da60 CCodaContext::Create2DSource
// Flags: /EHsc
#define FABLE_RETAIL_CCODACONTEXT_PROCESS
#define FABLE_RETAIL_CCODACONTEXT_SETERROR
#define FABLE_RETAIL_CCODACONTEXT_CREATE3D
#define FABLE_RETAIL_CCODACONTEXT_CREATE2D
#include "engine/CCodaContext.h"
#include "engine/CCoda2DSource.h"
#include "engine/CodaCaps.h"
FABLE_STATIC_ASSERT(sizeof(CCodaContext)==168);
FABLE_STATIC_ASSERT(sizeof(CCodaSource)==252);
FABLE_STATIC_ASSERT(sizeof(CCoda2DSource)==444);
CCoda2DSource* CCodaContext::Create2DSource(unsigned int ffCaps)
{
 CCoda2DSource* pSource;
 if(m_nMax2DSources>0){
  CCodaObject* pObject=m_2DSources.GetFree();
  if(!pObject){CODA.SetError(-9);pSource=0;}
  else pSource=static_cast<CCodaSource*>(pObject)->Get2D();
 }else if(m_nMax2DSources==-1)pSource=new CCoda2DSource;
 else {CODA.SetError(-9);pSource=0;}
 if(!pSource)return pSource;
 if(!pSource->Create(ffCaps)){ReleaseSource(pSource);pSource=0;}
 if(m_nMax2DSources>0)m_2DSources.RemoveFree();
 m_2DSources.Add(pSource);
 return pSource;
}