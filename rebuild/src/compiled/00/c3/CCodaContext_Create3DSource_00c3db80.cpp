// CCodaContext::Create3DSource(unsigned int) -- acquire a supported 3D audio source from the pool or allocator
// Leaf: ?Create3DSource@CCodaContext@@QAEPAVCCoda3DSource@@I@Z
// Retail: 00c3db80,335 bytes,thiscall.
// Flags: /EHsc
#define FABLE_RETAIL_CCODACONTEXT_PROCESS
#define FABLE_RETAIL_CCODACONTEXT_SETERROR
#define FABLE_RETAIL_CCODACONTEXT_CREATE3D
#include "engine/CCodaContext.h"
#include "engine/CCoda3DSource.h"
#include "engine/CodaCaps.h"
FABLE_STATIC_ASSERT(sizeof(CCodaContext)==168);
FABLE_STATIC_ASSERT(sizeof(CCodaSource)==252);
FABLE_STATIC_ASSERT(sizeof(CCoda3DSource)==476);
CCoda3DSource* CCodaContext::Create3DSource(unsigned int ffCaps)
{
 unsigned int ffSupported=g_SalCaps.SW3DVoice.ffCaps|g_SalCaps.HW3DVoice.ffCaps;
 unsigned int ffInvalid=ffCaps&~ffSupported;
 if(ffInvalid){CODA.SetError(-8);return 0;}
 CCoda3DSource* pSource;
 if(m_nMax3DSources>0){
  CCodaObject* pObject=m_3DSources.GetFree();
  if(!pObject){CODA.SetError(-9);pSource=0;}
  else pSource=static_cast<CCodaSource*>(pObject)->Get3D();
 }else if(m_nMax3DSources==-1)pSource=new CCoda3DSource;
 else {CODA.SetError(-9);pSource=0;}
 if(!pSource)return pSource;
 if(!pSource->Create(ffCaps)){ReleaseSource(pSource);pSource=0;}
 if(m_nMax3DSources>0)m_3DSources.RemoveFree();
 m_3DSources.Add(pSource);
 return pSource;
}