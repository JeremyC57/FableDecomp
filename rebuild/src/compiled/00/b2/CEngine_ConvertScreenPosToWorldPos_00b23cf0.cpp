// Constant-return member: returns false. __fastcall this=ecx.
// Leaf: ?ConvertScreenPosToWorldPos@CEngine_Methods@@QAE_NHH@Z
#include "engine/CEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngine_Methods : CEngine {
    bool ConvertScreenPosToWorldPos(int a0, int a1);
};
bool CEngine_Methods::ConvertScreenPosToWorldPos(int a0, int a1) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?ConvertScreenPosToWorldPos@CEngine@@UBE_NABVC2DVector@@AAVC3DVector@@@Z); evidence: RTTI vtable slot + PDB name set.
