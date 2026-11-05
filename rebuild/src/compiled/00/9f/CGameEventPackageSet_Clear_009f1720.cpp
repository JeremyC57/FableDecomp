// CGameEventPackageSet::Clear() -- empty the set of network game-event packages (the package count goes to zero)
// Leaf: ?Clear@CGameEventPackageSet@@QAEXXZ
// Retail: Fable.exe @ 0x009F1720, 7 bytes, __thiscall: `mov dword ptr [ecx],0 ; ret`.
// Name: Ego_r.pdb and FableWin.pdb both carry ?Clear@CGameEventPackageSet@@QAEXXZ; NoPackages is the class's first member
// (+0x00, PDB layout), and the only landed caller, CNetworkClient::GetLocalGameEventPackageSet 0x004AEAA0, clears the set
// right before AddPackage. The packages array (+0x04, CGameEventPackage[50]) is left as is: only the count is reset.
// History: landed 2026-10 as `??1T@@QAE@XZ`, a polymorphic stand-in destructor whose vptr store matched retail's literal 0
// only because the grader masks relocations (a reloc-masked false match). Re-landed 2026-11-05 as the real member, which
// emits retail's immediate.
#define FABLE_RETAIL_CGAMEEVENTPACKAGESET_CLEAR
#include "engine/CGameEventPackageSet.h"

void CGameEventPackageSet::Clear()
{
    NoPackages = 0;
}
