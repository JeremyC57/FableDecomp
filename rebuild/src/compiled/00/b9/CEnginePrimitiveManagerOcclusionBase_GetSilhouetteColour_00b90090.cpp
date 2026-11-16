// CEnginePrimitiveManagerOcclusionBase::GetSilhouetteColour(CEngineInternalPrimitiveOcclusionBase&, CRGBColour&) -- the base has no silhouette: false.
// Leaf: ?GetSilhouetteColour@CEnginePrimitiveManagerOcclusionBase@@IAE_NAAVCEngineInternalPrimitiveOcclusionBase@@AAVCRGBColour@@@Z
// NF37 (2026-11-16): moved off the T::Get stand-in onto the shared engine/CEnginePrimitiveManagerOcclusionBase.h (SILHOUETTE opt-in, which
// names 0x00B90090; Ego_r.pdb 0x00A4FCE0 / FableWin.pdb 0x02ED7FD0, protected; Ego_r order GetSilhouetteColour / IsClippedByNearPlane
// 0x00A4FCE0 / 0x00A4FD00 = retail 0x00B90090 / 0x00B900A0).
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_SILHOUETTE
#include "engine/CEnginePrimitiveManagerOcclusionBase.h"

bool CEnginePrimitiveManagerOcclusionBase::GetSilhouetteColour(CEngineInternalPrimitiveOcclusionBase& primitive, CRGBColour& colour)
{
    return false;
}
