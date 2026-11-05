// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?DrawFindLastUnblockedPosition@CIDrawMap_Methods@@QAE_NHHH@Z
#include "engine/CIDrawMap.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIDrawMap_Methods : CIDrawMap {
    bool DrawFindLastUnblockedPosition(int a0, int a1, int a2);
};
bool CIDrawMap_Methods::DrawFindLastUnblockedPosition(int a0, int a1, int a2) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?DrawFindLastUnblockedPosition@CIDrawMap@@UBE_NPBVC3DVector@@0PAV2@@Z); evidence: RTTI vtable slot + PDB name set.
