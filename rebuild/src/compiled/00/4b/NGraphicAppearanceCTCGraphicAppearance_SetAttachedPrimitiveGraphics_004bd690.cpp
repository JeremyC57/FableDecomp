// NGraphicAppearance::CTCGraphicAppearance::SetAttachedPrimitiveGraphics(CPrimitive* primitive, const CEnginePrimitive& engine_primitive, CEnginePrimitiveAttachmentInfo& info, const CPrimitive* parent) -- attach a primitive to its parent through the engine (colour-filter bypass and draw-after-water flags)
// Leaf: ?SetAttachedPrimitiveGraphics@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAVCPrimitive@2@ABVCEnginePrimitive@@AAVCEnginePrimitiveAttachmentInfo@@PBV32@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BD690, length 78 bytes. Callees: CTCGraphicAppearance::IsDrawingAfterWater
// @ 0x004BD3C0 (this chunk), the primitive manager's virtual slot 32 (+0x80; six arguments). The manager is +0x34 of the engine
// object the global @ 0x013B8A18 points to (the landed RefreshPrimitiveAnimationAndPos neighbour reaches it the same way).
// Shape: the flags are an `unsigned short` local built first (0x800 when ByPassColorFilter > 0, `| 8` when drawing after water)
// and widened to the 32-bit parameter at the push (`movzx eax,si ; push eax`); the two primitives are passed by their handle (+4) addresses;
// the manager and its vtable are fetched before the argument pushes.
// Layout (Ego_r.pdb CTCGraphicAppearance, 116 bytes; retail 0x5c: the two STLport maps 12 bytes, the two STLport lists one
// header-node pointer each): CTCBase +0x00..+0x0b (ParentThing +0x04), MeshEffects +0x0c, TintColours +0x18, PTCAnimation +0x24,
// CachedStaticFocalPosOffset +0x28, Scale +0x34, Primitives +0x38, Components +0x3c, LightingChannel +0x40, MainGraphic +0x44,
// ActivePersistenceFlags +0x52, FrameUpdatedWasRenderedFlag +0x54, UpdatePrimitiveReasons +0x58, the flag bits +0x59
// (SelfIlluminating 0, ForceResendEveryRenderFrame 1, ResendPrimsEveryRenderFrame 2, DisableOcclusionTesting 3, DrawAfterWater 4,
// CachedStaticFocalPos 5, RenderedLastFrame 6, PrimitivesDrawn 7), Alpha +0x5a, ByPassColorFilter +0x5b.
// No engine header exists for CTCGraphicAppearance: local model.
// Identity: Ego_r.pdb 0x004C0F30 (signature above: the engine primitive and attachment info by reference, the parent const). The engine
//   call is CIEngine slot 32 ?SetAttachedPrimitive (handle, primitive, attachment info, parent handle, bone -1, flags), reached through
//   the global 0x013B8A18 = g_pCurrentPrimitivePrepareInfo (descriptive; see row 0x004BCFE0) ->Engine (+0x34).
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
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_ISDRAWINGAFTERWATER
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETATTACHEDPRIMITIVEGRAPHICS
#include "fable_string.h"
#include "engine/CThing.h"
class CEnginePrimitive;
class CEnginePrimitiveAttachmentInfo;
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/NGraphicAppearance_CPrimitive.h"

extern const CEnginePrimitivePrepareInfo* g_pCurrentPrimitivePrepareInfo;   // 0x013B8A18

namespace NGraphicAppearance {

void CTCGraphicAppearance::SetAttachedPrimitiveGraphics(CPrimitive* primitive, const CEnginePrimitive& engine_primitive,
                                                        CEnginePrimitiveAttachmentInfo& info, const CPrimitive* parent)
{
    unsigned short flags = 0;
    if (ByPassColorFilter > 0)
        flags = 0x800;
    if (IsDrawingAfterWater())
        flags |= 8;
    g_pCurrentPrimitivePrepareInfo->Engine->SetAttachedPrimitive(primitive->PrimitiveHandle, engine_primitive, info, parent->PrimitiveHandle,
                                                                 -1, flags);
}

}
