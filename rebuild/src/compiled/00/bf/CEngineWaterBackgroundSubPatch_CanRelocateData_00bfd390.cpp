// CEngineWaterBackgroundSubPatch::CanRelocateData(long handle) const -- the sub-patch's blocks may be moved unless its landscape map (+0x04) is locked from memory freeing (CEngineLandscapeMap +0x5C)
// Leaf: ?CanRelocateData@CEngineWaterBackgroundSubPatch@@EBE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFD390, thiscall, `ret 4`.
// Identity (NF279, 2026-11-22): the private virtual ?CanRelocateData@CEngineWaterBackgroundSubPatch@@EBE_NJ@Z (Ego_r.pdb 00a4bac0 + FableWin.pdb
//   02e06380), slot 2 of the CEngineWaterBackgroundSubPatch vtable 0x012A8334 (RTTI; slot 1 RelocateData 0x00BFD5D0, slot 0 ??_G 0x00BFDC90),
//   which the destructor row 0x00BFD480's emitted vtable binds. No call site (exe scan: the .rdata slot only). Was the stand-in ?Can@C@@QAE_NH@Z
//   on `struct S` / `struct C` placeholders (owner rule 2026-11-05 stand-in); the same body as the landed CLandscapeLayerMesh twin 0x00BFDDB0.
// Layout: engine/CEngineWaterBackgroundSubPatch.h (VIRTUALS opt-in on the retail CMovableResource base), Map +0x04. CEngineLandscapeMap's only
//   header is quarantined (header-less for the local-model rule, EXTRA_RULES 2026-11-20): local model in the shape of the landed 0x00BF7F70 /
//   0x00BFDDB0 (MemoryFreeingLocked at retail +0x5C, Ego_r +0x60). The body reads the member directly, as 0x00BFDDB0 does.
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22          // the class on its retail base CMovableResource
#define FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_VIRTUALS        // the virtual destructor and the two private overrides
#include "engine/CEngineWaterBackgroundSubPatch.h"
#include "engine/CMovableResource.h"

class CEngineLandscapeMap { public:
    unsigned char _pad00[0x5c];
    bool          MemoryFreeingLocked;          // +0x5c
};

bool CEngineWaterBackgroundSubPatch::CanRelocateData(long handle) const
{
    return Map->MemoryFreeingLocked == false;
}
