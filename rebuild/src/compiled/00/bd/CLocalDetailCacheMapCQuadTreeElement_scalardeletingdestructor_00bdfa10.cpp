// CLocalDetailCacheMap::CQuadTreeElement::ScalarDeletingDestructor(unsigned) -- destroy then conditionally return storage.
// Leaf: ScalarDeletingDestructor
// Native0x00bdfa10 corrected length38; rtti VT012a7df0 slot0, actual destructor00bddfb0; not landscape patch.
// Manager Deallocate00bd9b90 and engine global01436ea8 manager+6b0 independently audited.
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CMovableResource.h"
#define CLocalDetailCacheMap CLocalDetailCacheMapDonor
#include "engine/CLocalDetailCacheMap.h"
#undef CLocalDetailCacheMap
// EngineGlobal is a native renderer view; donor MemoryManager+0xf58 differs from retail+0x6b0.
struct LandscapeMemory : CEngineLandscapeMemoryManager { void Deallocate(void*); }; //0x00bd9b90
struct EngineGlobal { unsigned char EngineState[0x6b0]; LandscapeMemory* LandscapeMemoryManager; };
extern EngineGlobal* gEngine; //0x01436ea8
class CLocalDetailCacheMap;
// Native RTTI: QuadTreeElement -> CMovableResource; four genuine slots.
struct MovableNative {virtual ~MovableNative(){} virtual void RelocateData(long,void*,void*,long)=0;virtual bool CanRelocateData(long)const;virtual unsigned long Verify()const;};
struct QuadTreeElement:MovableNative {
 QuadTreeElement(CLocalDetailCacheMap*,long,long,long,long); // PDB00a8f6f0 donor signature
 virtual ~QuadTreeElement(); //0x00bddfb0, VT012a7df0 slot0
 virtual void RelocateData(long,void*,void*,long); //native slot1, PDB A8FA10
 virtual bool CanRelocateData(long)const; //native slot2, PDB A8F350
 unsigned char QuadTreeState[0x48]; // PDB total0x4c; no members accessed by this thunk
 void* ScalarDeletingDestructor(unsigned);
};
void* QuadTreeElement::ScalarDeletingDestructor(unsigned flags) {
 this->QuadTreeElement::~QuadTreeElement();
 if(flags&1) gEngine->LandscapeMemoryManager->Deallocate(this);
 return this;
}