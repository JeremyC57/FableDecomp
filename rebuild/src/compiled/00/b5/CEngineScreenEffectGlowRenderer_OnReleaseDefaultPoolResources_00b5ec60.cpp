// Leaf: ?OnReleaseDefaultPoolResources@CEngineScreenEffectGlowRenderer_Methods@@QAEXXZ
#include "engine/CEngineScreenEffectGlowRenderer.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineScreenEffectGlowRenderer_Methods : CEngineScreenEffectGlowRenderer {
    void OnReleaseDefaultPoolResources();
};
void CEngineScreenEffectGlowRenderer_Methods::OnReleaseDefaultPoolResources(){ this->DynamicTexturePageHandle = 0xffffffff; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnReleaseDefaultPoolResources@CEngineScreenEffectGlowRenderer@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
