// Leaf: ?OnReleaseDefaultPoolResources@CEngineScreenEffectDisplacementRenderer_Methods@@QAEXXZ
#include "engine/CEngineScreenEffectDisplacementRenderer.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineScreenEffectDisplacementRenderer_Methods : CEngineScreenEffectDisplacementRenderer {
    void OnReleaseDefaultPoolResources();
};
void CEngineScreenEffectDisplacementRenderer_Methods::OnReleaseDefaultPoolResources(){ this->DynamicTexturePageHandle = 0xffffffff; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnReleaseDefaultPoolResources@CEngineScreenEffectDisplacementRenderer@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
