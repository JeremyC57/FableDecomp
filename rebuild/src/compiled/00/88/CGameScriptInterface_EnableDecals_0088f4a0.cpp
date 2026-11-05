// Leaf: ?EnableDecals@CGameScriptInterface_Methods@@QAEXD@Z
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct GObj { char pad[0x1010]; unsigned char b0:1; unsigned char b1:1; };
extern GObj* GetDecalObj(void);

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void EnableDecals(char enable);
};

void CGameScriptInterface_Methods::EnableDecals(char enable)
{
    GObj* o = GetDecalObj();
    o->b1 = enable;
}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?EnableDecals@CGameScriptInterface@@UBEX_N@Z); evidence: RTTI vtable slot + PDB name set.
