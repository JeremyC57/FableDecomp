// CSpawnedFuncBase::RemovePredicate() -- release the PPredicate counted pointer (+0x2C): drop the shared count, on zero run the deleter on the data and free the count block, then clear the pointer
// Leaf: ?RemovePredicate@CSpawnedFuncBase@@QAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f35a30, 49 bytes, thiscall, plain `ret`.
// Identity (proven): bound ?RemovePredicate@CSpawnedFuncBase@@QAEXXZ (Ego_r.pdb); Ego_r.pdb CSpawnedFuncBase::PPredicate (CCountedPointer<const NScript::CPredicateBase>, +0x2C); callers CScriptBase::UpdateSpawnedFunction 0x00CB7950 (twice) and CScriptBase::SetPredicate 0x00CB7920's tail path.
// Callees: the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete 0x00BFE9BC.
// Shape (family L028): CCountedPointer's release with `--Info->RefCount` (Info reloaded after the decrement and for the
//   delete), then Info and Data cleared in that order (ReleaseData) or Data then Info (the destructor).
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
#include "engine/CCountedPointer.h"
namespace NScript { class CPredicateBase; }
class CSpawnedFuncBase {
public:
    unsigned char _pad[0x2C];                         // (members not touched here)
    CCountedPointer<const NScript::CPredicateBase> PPredicate;                        // +0x2C
    void RemovePredicate();
};
void CSpawnedFuncBase::RemovePredicate()
{
    PPredicate.ReleaseData();
}