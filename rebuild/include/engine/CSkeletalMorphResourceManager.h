#ifndef FABLE_ENGINE_CSKELETALMORPHRESOURCEMANAGER_H
#define FABLE_ENGINE_CSKELETALMORPHRESOURCEMANAGER_H
// Chunk 2051 (row 0x004A6E30 CWorld::Init). RTTI .?AVCSkeletalMorphResourceManager@@ (vtable 0x01238F84, ctor 0x004AE2A0 landed),
// a CTBaseSingleton<CSkeletalMorphResourceManager> (Ego_r.pdb ??0?$CTBaseSingleton@VCSkeletalMorphResourceManager@@@@IAE@XZ,
// ?Get@?$CTBaseSingleton@VCSkeletalMorphResourceManager@@@@SIAAVCSkeletalMorphResourceManager@@XZ; the singleton's PClass
// CCountedPointer is 0x013BAB10, reset out of line through 0x004A9A80). Ego_r.pdb: CachedMorphs map<CCharString,
// CCountedPointer<C3DSkeletalMorphTarget> > +0x04 (retail STLport 0xC; sizeof 0x10, `push 10h` 0x004A76CD).
#include "rebuild_abi.h"
#include "engine/CTBaseSingleton.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
class C3DSkeletalMorphTarget;
class CSkeletalMorphResourceManager : public CTBaseSingleton<CSkeletalMorphResourceManager> {
public:
#ifdef FABLE_RETAIL_CSKELETALMORPHRESOURCEMANAGER_INLINE_CTOR
    // Opt-in (NF52, row 0x00786700): inline in the class -- CTCSkeletalMorph::CalculateSkeletalMorph expands it through the inline
    // CTBaseSingleton::Get() (0x0078673B: the vtable store, then the map header malloc); 0x004AE2A0 is its out-of-line COMDAT.
    CSkeletalMorphResourceManager() { }
#else
    CSkeletalMorphResourceManager();                                           // 0x004AE2A0
#endif
    virtual ~CSkeletalMorphResourceManager();                                  // ??_G 0x004AE3B0
    void PreloadBoneConfigs();                                                 // 0x006C37D0
    void UnloadBoneConfigs();                                                  // 0x006C35E0
#ifdef FABLE_RETAIL_CSKELETALMORPHRESOURCEMANAGER_GETMORPHTARGET
    // Opt-in (NF52, row 0x00786700): Ego_r.pdb / FableWin.pdb
    // ?GetMorphTarget@CSkeletalMorphResourceManager@@QAEAAV?$CCountedPointer@VC3DSkeletalMorphTarget@@@@ABVCCharString@@@Z. Declaration only.
    CCountedPointer<C3DSkeletalMorphTarget>& GetMorphTarget(const CCharString& name);   // 0x006C3620
#endif
private:
    std::map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> > CachedMorphs;   // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CSkeletalMorphResourceManager) == 0x10);
#endif
