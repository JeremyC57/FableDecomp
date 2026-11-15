// CCodaContext::Create2DSource(unsigned int) -- compiler-generated cleanup and frame handler.
// Retail: 0x01214FE0, 21 bytes; associated EH section for parent 0x00C3DA60.
// Leaf: .text$x
// Flags: /EHsc
// Extent: cleanup11bytes; frame handler10bytes; complete associated compiler section.
// The parent 0x00C3DA60 CCodaContext::Create2DSource (not landed yet) follows verbatim; only its EH section is graded here.
// 00C3DA60 CCodaContext::Create2DSource -- original parent of assigned01214FE0 EH section.
// Parent leaf: ?Create2DSource@CCodaContext@@QAEPAVCCoda2DSource@@I@Z
#define FABLE_RETAIL_CCODACONTEXT_PROCESS
#define FABLE_RETAIL_CCODACONTEXT_SETERROR
#define FABLE_RETAIL_CCODACONTEXT_CREATE3D
#define FABLE_RETAIL_CCODACONTEXT_CREATE2D
#include "engine/CCodaContext.h"
#include "engine/CCoda2DSource.h"
FABLE_STATIC_ASSERT(sizeof(CCodaContext)==168);
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
