// Leaf: ?OnReleaseDefaultPoolResources@CEngineScreenEffectRadialBlurRenderer_Methods@@QAEXXZ
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineScreenEffectRadialBlurRenderer_Methods : CEngineScreenEffectRadialBlurRenderer {
    void OnReleaseDefaultPoolResources();
};
void CEngineScreenEffectRadialBlurRenderer_Methods::OnReleaseDefaultPoolResources(){ this->DynamicTexturePageHandle = 0xffffffff; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnReleaseDefaultPoolResources@CEngineScreenEffectRadialBlurRenderer@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
