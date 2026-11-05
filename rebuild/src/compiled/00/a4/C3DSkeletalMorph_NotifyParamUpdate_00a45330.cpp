// C3DSkeletalMorph::NotifyParamUpdate() -- release the CombinedMorph counted pointer (+0x10): drop the shared count, on zero run the deleter on the data and free the count block, then clear the pointer
// Leaf: ?NotifyParamUpdate@C3DSkeletalMorph@@QAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a45330, 49 bytes, thiscall, plain `ret`.
// Identity (probable): Ego_r.pdb C3DSkeletalMorph::CombinedMorph (CCountedPointer<C3DSkeletalMorphTarget>, PDB +0x14, retail +0x10 after the 12-byte MorphParameters CArray); of C3DSkeletalMorph's two Ego_r.pdb `void ()` members (NotifyParamUpdate, Reset) this one only drops the cached combined morph, and its callers CTCSkeletalMorph::ForceSkeletalMorph 0x007864C0 / CalculateSkeletalMorph 0x00786700 (landed) call it right after adding morph parameters; bound only as the stand-ins ApplyParameters / ResetCombinedMorph.
// Callees: the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete 0x00BFE9BC.
// Shape (family L108): CCountedPointer's release with `--Info->RefCount` (Info reloaded after the decrement and for the
//   delete), then Info and Data cleared in that order (ReleaseData) or Data then Info (the destructor).
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
#include "engine/CCountedPointer.h"
class C3DSkeletalMorphTarget;
class C3DSkeletalMorph {
public:
    unsigned char _pad[0x10];                         // (members not touched here)
    CCountedPointer<C3DSkeletalMorphTarget> CombinedMorph;                        // +0x10
    void NotifyParamUpdate();
};
void C3DSkeletalMorph::NotifyParamUpdate()
{
    CombinedMorph.ReleaseData();
}