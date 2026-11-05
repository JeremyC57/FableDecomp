// RemoveFromGlobalTCs @ 00686df0, module CTCBase, __fastcall(CTCBase*)
// Leaf: ?CTCBase_RemoveFromGlobalTCs@@YIXPAVCTCBase@@@Z
#include "engine/CTCBase.h"  // retyped onto the PDB layout; byte parity re-verified
class CTCBase;

// helper at 0x4cc150 is a real member (this in ecx, one stack arg, callee-clean).
struct GContainer {
    void Remove(CTCBase** pp);   // __fastcall member
};
extern GContainer g_container;


void __fastcall CTCBase_RemoveFromGlobalTCs(CTCBase* other)
{
    other->IsInGlobalUpdateTCs = 0;
    CTCBase* local = other;
    g_container.Remove(&local);
}