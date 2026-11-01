// CLocalDetailCacheMap::CQuadTreeElement::ScalarDeletingDestructor(unsigned) -- destroy then conditionally return storage.
// Leaf: ScalarDeletingDestructor
// Naming wave 28 (family c697): the destructor 0x00BDDFB0 is ??1CQuadTreeElement@CLocalDetailCacheMap@@UAE@XZ (chunk 697: vtable
//   0x012A7DF0 RTTI .?AVCQuadTreeElement@CLocalDetailCacheMap@@; Ego_r.pdb + FableWin.pdb), the class nested in CLocalDetailCacheMap.
//   Was a global QuadTreeElement (??1QuadTreeElement@@UAE@XZ).
// Native0x00bdfa10 corrected length38; rtti VT012a7df0 slot0, actual destructor00bddfb0; not landscape patch.
// Manager Deallocate00bd9b90 and engine global01436ea8 manager+6b0 independently audited.
#define CEngineLandscapeMemoryManager CEngineLandscapeMemoryManagerLayout
#include "engine/CEngineLandscapeMemoryManager.h"
#undef CEngineLandscapeMemoryManager

class CEngineLandscapeMemoryManager : public CEngineLandscapeMemoryManagerLayout
{
public:
    void FreeVirtual(void*);   // 0x00BD9B90
};
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CMovableResource.h"
#define CLocalDetailCacheMap CLocalDetailCacheMapDonor
#include "engine/CLocalDetailCacheMap.h"
#undef CLocalDetailCacheMap
// EngineGlobal is a native renderer view; donor MemoryManager+0xf58 differs from retail+0x6b0.
struct LandscapeMemory : CEngineLandscapeMemoryManager { }; //0x00bd9b90
struct EngineGlobal { unsigned char EngineState[0x6b0]; LandscapeMemory* LandscapeMemoryManager; };
extern EngineGlobal* gEngine; //0x01436ea8
class CLocalDetailCacheMap { public:
 class CQuadTreeElement; // nested (RTTI .?AVCQuadTreeElement@CLocalDetailCacheMap@@; Ego_r / FableWin)
};
// Native RTTI: CLocalDetailCacheMap::CQuadTreeElement -> CMovableResource; four genuine slots.
struct MovableNative {virtual ~MovableNative(){} virtual void RelocateData(long,void*,void*,long)=0;virtual bool CanRelocateData(long)const;virtual unsigned long Verify()const;};
class CLocalDetailCacheMap::CQuadTreeElement:public MovableNative { public:
 CQuadTreeElement(CLocalDetailCacheMap*,long,long,long,long); // PDB00a8f6f0 donor signature
 virtual ~CQuadTreeElement(); //0x00bddfb0, VT012a7df0 slot0
 virtual void RelocateData(long,void*,void*,long); //native slot1, PDB A8FA10
 virtual bool CanRelocateData(long)const; //native slot2, PDB A8F350
 unsigned char QuadTreeState[0x48]; // PDB total0x4c; no members accessed by this thunk
 void* ScalarDeletingDestructor(unsigned);
};
void* CLocalDetailCacheMap::CQuadTreeElement::ScalarDeletingDestructor(unsigned flags) {
 this->CLocalDetailCacheMap::CQuadTreeElement::~CQuadTreeElement();
 if(flags&1) gEngine->LandscapeMemoryManager->FreeVirtual(this);
 return this;
}