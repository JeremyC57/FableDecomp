// Leaf: ?Create2DSource@CCodaContext@@QAEPAVCCoda2DSource@@I@Z
// 00c3da60 CCodaContext::Create2DSource
// Flags: /EHsc
#define FABLE_RETAIL_CCODACONTEXT_PROCESS
#define FABLE_RETAIL_CCODACONTEXT_SETERROR
#define FABLE_RETAIL_CCODACONTEXT_CREATE3D
#define FABLE_RETAIL_CCODACONTEXT_CREATE2D
#define private public
#include "engine/CCodaContext.h"
#undef private
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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
CCodaContext CODA;
int CODA_Error;
static bool is3d, can_create, allocate;
static int takes, queries, creates, allocations, removals, adds, releases, callbacks;
static unsigned last_caps;
static CCoda2DSource* last_release;
static CCodaObject* last_add;
CCodaObjectBase::~CCodaObjectBase() {}
CCodaObject::~CCodaObject() {}
CCodaSource::CCodaSource() {}
CCodaSource::~CCodaSource() {}
CCoda2DSource::CCoda2DSource() {}
CCoda2DSource::~CCoda2DSource() {}
bool CCodaSource::IsCreated()const {return false;}
bool CCodaSource::Is3D()const {++queries; return is3d;}
bool CCodaSource::Create(unsigned int) {assert(false);return false;}
bool CCoda2DSource::Create(unsigned int caps) {++creates; last_caps=caps;return can_create;}
extern "C" void* __cdecl CodaMalloc(unsigned int size) {assert(size==sizeof(CCoda2DSource)); ++allocations; return allocate?malloc(size):0;}
extern "C" void __cdecl CodaFree(void* p) {free(p);}
CCodaObject* CCodaObjectList::GetFree()const {++takes;return m_pFree;}
void CCodaObjectList::RemoveFree(){++removals;}
void CCodaObjectList::Add(CCodaObject* p){++adds;last_add=p;}
void CCodaContext::ReleaseSource(CCodaSource* p){++releases; last_release=static_cast<CCoda2DSource*>(p);}
static void FABLE_STDCALL on_error(int e) {assert(e==-9);++callbacks;}
int main(){
 CCoda2DSource pooled;
 CCodaContext ctx;
 const int limits[]={2,0,-1,-2};
 for(int i=0;i<4;++i) for(unsigned mask=0;mask<64;++mask){
  const int limit=limits[i];const bool available=(mask&1)!=0;
  is3d=(mask&2)!=0;can_create=(mask&4)!=0;allocate=(mask&8)!=0;
  const bool callback=(mask&16)!=0;const bool latched=(mask&32)!=0;
  ctx.m_nMax2DSources=limit;ctx.m_2DSources.m_pFree=available?&pooled:0;
  CODA_Error=latched?17:0;CODA.m_pfnErrorCallback=callback?on_error:0;ctx.m_pfnErrorCallback=0;
  allocations=takes=queries=creates=removals=adds=releases=callbacks=0;last_release=0;last_add=0;last_caps=0;
  const unsigned caps=0x15a00|mask;
  CCoda2DSource* result=ctx.Create2DSource(caps);
  const bool has=(limit>0 && available && !is3d)||(limit==-1 && allocate);
  const bool error=(limit==0||limit < -1||(limit>0&&!available));
  assert(CODA_Error==(latched?17:(error?-9:0)));
  assert(callbacks==int(error&&callback&&!latched));
  assert(takes==int(limit>0)&&queries==int(limit>0&&available)&&allocations==int(limit==-1));
  assert(creates==int(has)&&releases==int(has&&!can_create));
  assert(adds==int(has)&&removals==int(has&&limit>0)&&last_add==result);
  assert((result!=0)==(has&&can_create)&&(!has||last_caps==caps));
  if(has&&limit>0) assert((can_create?result:last_release)==&pooled);
  if(has&&limit==-1) delete (can_create?result:last_release);
 }
 puts("00c3da60_TEST PASS");
}