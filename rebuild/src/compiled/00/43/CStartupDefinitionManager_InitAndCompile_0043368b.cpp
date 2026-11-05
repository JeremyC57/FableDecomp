// Empty virtual hook (CGameComponent::Init and friends): __fastcall this=ecx, no args,
// Leaf: ?InitAndCompile@CStartupDefinitionManager_Methods@@QAEXXZ
// no cleanup -> a bare `ret`.
#include "engine/CStartupDefinitionManager.h"  // retyped onto the PDB layout; byte parity re-verified
struct CStartupDefinitionManager_Methods : CStartupDefinitionManager {
    void InitAndCompile();
};
void CStartupDefinitionManager_Methods::InitAndCompile() {}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitAndCompile@CStartupDefinitionManager@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
