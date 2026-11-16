// NGraphicAppearance::CTCGraphicAppearance::SetPrimitiveGraphics(CPrimitive*, const CEnginePrimitive&, unsigned short) -- send an engine primitive to the scene manager: for a tracked primitive with the appearance's persistence/colour-filter/occlusion/after-water flags, else as a one-off
// Leaf: ?SetPrimitiveGraphics@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAVCPrimitive@2@ABVCEnginePrimitive@@G@Z
//
// Retail 0x004BD620, 112 bytes (`ret 0Ch`). Ego_r.pdb `?SetPrimitiveGraphics@CTCGraphicAppearance@
// NGraphicAppearance@@QAEXPAVCPrimitive@2@ABVCEnginePrimitive@@G@Z`. Callees: CTCGraphicAppearance::
// IsDrawingAfterWater() const @ 0x004BD3C0 (`this` still in ecx), the scene manager's virtual slots 23
// (+0x5c, primitive handle, primitive, flags: CIEngine::SetPersistentPrimitive) and 20 (+0x50, primitive only: CIEngine::AddPrimitive) of
// the engine read through the global 0x013B8A18 = g_pCurrentPrimitivePrepareInfo (descriptive; see row 0x004BCFE0) ->Engine (+0x34).
// Layout: retail CTCGraphicAppearance (PDB -0x18): ActivePersistenceFlags +0x52, the flag bit byte +0x59
// (bit 3 DisableOcclusionTesting, bit 4 DrawAfterWater), ByPassColorFilter +0x5b (signed char);
// CPrimitive's engine handle at +4.
// Shape: the flags are an `unsigned short` local (`xor esi,esi ; mov si ; or si`, the 32-bit `or`s, the
// `movzx` for the unsigned long parameter); each arm keeps its own `ret 0Ch`.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_ISDRAWINGAFTERWATER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/NGraphicAppearance_CPrimitive.h"

extern const CEnginePrimitivePrepareInfo* g_pCurrentPrimitivePrepareInfo;   // 0x013B8A18

namespace NGraphicAppearance {

void CTCGraphicAppearance::SetPrimitiveGraphics(CPrimitive* primitive, const CEnginePrimitive& engine_primitive, unsigned short flags)
{
    if (primitive)
    {
        unsigned short all_flags = ActivePersistenceFlags | flags;
        if (ByPassColorFilter > 0)
            all_flags |= 0x800;
        if (DisableOcclusionTesting)
            all_flags |= 0x4000;
        if (IsDrawingAfterWater())
            all_flags |= 8;
        g_pCurrentPrimitivePrepareInfo->Engine->SetPersistentPrimitive(primitive->PrimitiveHandle, engine_primitive, all_flags);
    }
    else
        g_pCurrentPrimitivePrepareInfo->Engine->AddPrimitive(engine_primitive);
}

}
