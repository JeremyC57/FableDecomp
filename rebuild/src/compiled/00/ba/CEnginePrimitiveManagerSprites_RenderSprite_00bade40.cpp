// CEnginePrimitiveManagerSprites::RenderSprite(CEngineInternalPrimitiveSprite& primitive) -- fade the sprite's alpha by its distance from the camera (nothing past the maximum draw distance), refresh its bounding-sphere radius, cull it against the camera frustum, start loading its graphic frame and queue a sorted sprite sub-primitive
// Leaf: ?RenderSprite@CEnginePrimitiveManagerSprites@@IAEXAAVCEngineInternalPrimitiveSprite@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BADE40, 793 bytes, `ret 4` (frame 0x20, FPO).
// Retail-asm: 0x00BADF02 `fld dword ptr [esp+10h]` + 0x00BADF06 `fistp dword ptr [esp+14h]` -- the engine's inline-assembly round helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h, FABLE_PORTABLE twin there); no asm in this file.
// Identity: Ego_r.pdb / Ego_d.pdb / FableWin.pdb ?RenderSprite@CEnginePrimitiveManagerSprites@@IAEXAAVCEngineInternalPrimitiveSprite@@@Z
//   (protected; overlay opt-in FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSPRITES_RENDERSPRITE; caller RenderPrimitive). Locals exactly as
//   Ego_d.pdb (debug length 865): world_pos (const C3DVector&), size_y, distance, size_x (floats), alpha (unsigned long),
//   capture_block (CRenderStateScopedCaptureBlock), subprim (CEngineSubPrimitiveSprite*).
// Shape: Ego_d.pdb's C13 line table (ref/00bade40/egod_lines.tsv, source lines 334..366; Ego_d is a VC10 /Od build, so a float compare
//   line is 2 bytes longer, a float argument 3, a float return spill 6, an assert line ~280):
//   335 capture_block; 337 / 338 / 339 size_x, size_y, world_pos (9 each, = ours); 341 alpha (13 = ours); 344 distance =
//   CalcDistanceFromCamera(world_pos) (11 = ours: a real call there, `mov ecx,[world_pos] ; call ; fstp`); 345 / 347 the draw-distance
//   test and return; 349 / 351 the fade; 355 the radius (117 bytes; this text 90 at VC7.1 /Od + four float arguments and the float return spills of VC10; the C2DVector text 77; see Radius); 356 / 358 the clip
//   test and return; 361 IsParticle (20 = ours); 362 the BeginLoading line (45 = ours); 363 new; 364 a debug-only assert (280 bytes, no
//   retail code); 365 the queue call (33 bytes = the five-argument renderer call with `this`, VC7.1 29 + 3 float + 1 uchar; the
//   four-argument CEnginePrimitiveManager::AddSubPrimitive wrapper would be 26); 366 `}`.
// Radius: `sqrt(GFSqr(fabs(sx * ofx) + sx) + GFSqr(fabs(sy * ofy) + sy))` through the float overloads of <math.h>. The C2DVector(..).
//   GetMagnitude() spelling gives the same instructions but one bracket tuple fewer (sqrtf directly, no `(sqrtf(_X))`), and c2's list
//   scheduler then emits `fst [esi+70h] ; fstp [radius copy] ; mov ecx,[GPCamera]`; retail 0x00BADF38 has the camera load between the
//   two stores. Measured with the c2 scheduler dump (tools/sched t20.py) and the corrected model (sim2.py: an x87 -> integer edge costs
//   two cycles more than x87 -> x87): exactly one no-code tuple between the store and the argument copy gives retail's order.
// Queue call: the direct CEngineSubPrimitiveRenderer::AddSubPrimitive(subprim, distance, alpha, 1, this) (0x00B846E0). Through the inline
//   four-argument manager wrapper the instructions are the same except the distance's squares (Y, Z, X instead of retail's Z, Y, X at
//   0x00BADEA3): 793v793 DIFFER, and no C3DVector / CalcDistanceFromCamera text moves that (60 probes, NOTES.md).
// Inline helpers expanded by retail (bodies as the landed siblings 0x00BBDDD0 / 0x00B9A4A0 define them; Ego_d line tables agree):
//   CEnginePrimitiveRenderUtil::GetCurrentCamera (Ego_d 10 bytes: the GPCamera load) and CalcDistanceFromCamera(const C3DVector&) (Ego_d
//   45 bytes, one 32-byte line, parameter world_pos); CGraphicDataBank::PeekGraphic / PeekGraphicInfoFrame in Ego_d's statement form
//   (PeekGraphic 554 / 556 / 558 = `if` 19, return 20, `return 0` 2; PeekGraphicInfoFrame 567 / 569 / 571 = 16 / 25 / 2; parameters
//   index, frame), FABLE_FORCEINLINE as in the landed 0x00BAD670 (no out-of-line copy in retail).
// Callees: 0x00A05840 CRenderStateManager::RestoreCaptureBlock (three exits), 0x009FE620 CGraphicInfoFrame::BeginLoading(0, 0), 0x00BEB540
//   operator new(24), 0x0099A2F0 CBaseClassNonCopyable::CBaseClassNonCopyable, 0x00B846E0 CEngineSubPrimitiveRenderer::AddSubPrimitive.
// Globals: 0x013CB508 GRenderStateManager, 0x01436EA0 NEngineInternal::GPCamera, 0x01436E98 GPResourceManager, 0x01436E74
//   GPSubPrimitiveRenderer, 0x0122DCB4 the 2^32 float bias of the unsigned alpha conversion, 0x012A5430 CEngineSubPrimitiveSprite vftable.
// Overlays (NEW opt-ins, hdr/): engine/CEnginePrimitiveManagerSprites.h FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSPRITES_RENDERSPRITE (the
//   protected declaration), engine/CEngineSubPrimitiveSprite.h (new class header, twin of CEngineSubPrimitiveSpriteTrail.h).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSPRITES_CLASS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSPRITES_RENDERSPRITE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#include "fable_compiler.h"
#include <math.h>
#include "engine/EGraphicBankNo.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CEnginePrimitiveManagerSprites.h"
#include "engine/CEngineInternalPrimitiveSprite.h"
#include "engine/CEngineSubPrimitiveSprite.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFWorldDistance.h"

namespace NEngineInternal
{
    extern CEngineCamera* GPCamera;                                      // 0x01436EA0
    extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer;          // 0x01436E74
    extern CEngineResourceManager* GPResourceManager;                    // 0x01436E98
}

inline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera()
{
    return *NEngineInternal::GPCamera;
}
inline float __fastcall CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const C3DVector& world_pos)
{
    return (world_pos - GetCurrentCamera().PeekCameraPos()).GetMagnitude();
}
FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    if (index < GraphicList.size())
        return &GraphicList[index];
    return 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long index, unsigned long frame) const
{
    if (PeekGraphic(index))
        return PeekGraphic(index)->PeekInfoFrame(frame);
    return 0;
}

void CEnginePrimitiveManagerSprites::RenderSprite(CEngineInternalPrimitiveSprite& primitive)
{
    CRenderStateScopedCaptureBlock capture_block;

    const float size_x = primitive.SizeX;
    const float size_y = primitive.SizeY;
    const C3DVector& world_pos = primitive.WorldPos;

    unsigned long alpha = primitive.Colour.A;

    float distance = CalcDistanceFromCamera(world_pos);
    if (distance > primitive.FadeDistance.MaxDrawDistance)
        return;

    if (distance > primitive.FadeDistance.FadeBeginDistance)
    {
        alpha = GFFloatToLongNear((primitive.FadeDistance.MaxDrawDistance - distance) * alpha /
                                  (primitive.FadeDistance.MaxDrawDistance - primitive.FadeDistance.FadeBeginDistance));
    }

    primitive.BSphereRadius = sqrt(GFSqr(fabs(size_x * primitive.OffsetFactorX) + size_x) + GFSqr(fabs(size_y * primitive.OffsetFactorY) + size_y));
    if (!GetCurrentCamera().ClipSphere(world_pos, primitive.BSphereRadius))
        return;

    primitive.IsParticle = false;
    NEngineInternal::GPResourceManager->GetGraphicBank(MAIN_GRAPHIC_BANK_NO)->PeekGraphicInfoFrame(primitive.BankIndex, primitive.Frame)->BeginLoading(0, 0);
    CEngineSubPrimitiveSprite* subprim = new CEngineSubPrimitiveSprite(&primitive);
    NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(subprim, distance, alpha, 1, this);
}