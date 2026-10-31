// 009fd3a0 CGraphicDataBank::UpdateAvailableMemory
// Codex parked-50 best attempt: DIFFER(59v59); residue: 59/59 bytes; AND destination register and available-memory store versus call-argument preparation are ordered differently.
// Leaf: ?UpdateAvailableMemory@CGraphicDataBank@@AAEXXZ
// Flags:
#include <stddef.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include "rebuild_abi.h"
#define CBankFile CBankFileFields
#include "engine/CBankFile.h"
#undef CBankFile
#define CResourceList CResourceListFields
#include "engine/CResourceList.h"
#undef CResourceList
#define CGraphicDataBank CGraphicDataBankFields
#include "engine/_quarantine/CGraphicDataBank.h"
#undef CGraphicDataBank
// Layout: RTTI proves both genuine base chains; CResourceBank begins at0x164.
// Unread bank storage is an explicit byte projection, not recovered member names.
#include "engine/CBaseClassNonCopyable.h"
class CBankFile:public CBaseClassNonCopyable{unsigned char unused[0x10c];};
class CBankFileAsync:public CBankFile{unsigned char unused[0x54];};
// Layout: original CResourceList derives from CFailedAllocationHandler (vptr0).
// Its private Head4/AllocatedMemory24/MaximumMemory28 agree with the shared header.
class CFailedAllocationHandler { public: virtual ~CFailedAllocationHandler(); virtual bool HandleFailedAllocation(unsigned long)=0; };
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResource.h"
class CResourceList : public CFailedAllocationHandler {
public:
 virtual ~CResourceList();
 virtual bool HandleFailedAllocation(unsigned long);
 __forceinline long GetFreeMemory() const {
  long available=MaximumMemory-AllocatedMemory;
  return available & ((long)(available<=0)-1);
 }
 unsigned long GetUnloadableMemory(unsigned long) const;
private:
 CResource Head;
 unsigned long ResourceCount;
 long AllocatedMemory,MaximumMemory;
 unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;
};
class CResourceBank : public CBaseClassNonCopyable {
public:
 CResourceList& GetResourceList(){return ResourceList;}
 const CResourceList& GetResourceList()const{return ResourceList;}
private:
 CResourceList ResourceList,LoadingQueue;
};
class CGraphicDataBank:public CBankFileAsync,public CResourceBank{
 unsigned char unused[0x8c];
private:unsigned long ReduceSizeFrameDelay,TexturePointersValidForFrames;long MinAvailablePreloadMemory,AvailableMemory;
private:unsigned char unusedTail[0x90];void UpdateAvailableMemory();
#ifdef PASS9_TEST
 friend int main();
#endif
};
// Layout: The used ResourceList fields at0x18c/0x190 and own0x26c/0x278
// are independently confirmed by native instructions and original field tables.
void CGraphicDataBank::UpdateAvailableMemory(){
 AvailableMemory=GetResourceList().GetFreeMemory();
 AvailableMemory+=GetResourceList().GetUnloadableMemory(ReduceSizeFrameDelay);
}
FABLE_STATIC_ASSERT(sizeof(CBankFile)==sizeof(CBankFileFields));
FABLE_STATIC_ASSERT(sizeof(CBankFileAsync)==0x164);
FABLE_STATIC_ASSERT(sizeof(CResourceList)==sizeof(CResourceListFields));
FABLE_STATIC_ASSERT(sizeof(CGraphicDataBank)==sizeof(CGraphicDataBankFields));
