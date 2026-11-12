// CTCBase::RemoveFromGlobalTCs() -- clear IsInGlobalUpdateTCs (+0x09) and remove this component from the global update container.
// Leaf: ?RemoveFromGlobalTCs@CTCBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686DF0, thiscall.
// Name (agentNF32, 2026-11-12; the leaf was the free __fastcall ?CTCBase_RemoveFromGlobalTCs@@YIXPAVCTCBase@@@Z): Ego_r.pdb 0x006B21B0 and
//   FableWin.pdb 0x021919D0 ?RemoveFromGlobalTCs@CTCBase@@QAEXXZ (public member, no arguments); engine/CTCBase.h declares it
//   (FABLE_RETAIL_CTCBASE); the landed 0x005C00A0 binds this address under that symbol.
// The container (0x013BA8AC) and its remove helper 0x004CC150 keep this row's stand-in spellings (not proven here).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

// helper at 0x4cc150 is a real member (this in ecx, one stack arg, callee-clean).
struct GContainer {
    void Remove(CTCBase** pp);   // __fastcall member
};
extern GContainer g_container;

void CTCBase::RemoveFromGlobalTCs()
{
    IsInGlobalUpdateTCs = 0;
    CTCBase* local = this;
    g_container.Remove(&local);
}
