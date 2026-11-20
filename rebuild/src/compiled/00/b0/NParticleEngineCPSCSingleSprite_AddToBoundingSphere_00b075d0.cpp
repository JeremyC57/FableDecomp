// NParticleEngine::CPSCSingleSprite::AddToBoundingSphere(bsphere, interpolate, system, params) -- grow the bounding sphere by every trail point (system-transformed when the system stays with its emitter, radius half the trail width) and by the sprite at its position parameter (system-transformed, radius from the animated frame's aspect-scaled size)
// Leaf: ?AddToBoundingSphere@CPSCSingleSprite@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z
// Flags: none
// Retail-asm: 0x00B07892 `fld` / 0x00B07895 `fsub` / 0x00B07898 `fistp dword ptr [ebp+14h]` -- GFFloatToLongFloor (engine/GFFloatToLongFloor.h). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B075D0, 977 bytes, `ret 0x10`, ebp frame.
// Source shape: Ego_d.pdb locals and line table (lines 237..309) for the row; every expanded helper in its own Ego_d statement
// shape -- CMatrix3x4::TransformPoint (xx / yy / zz), CParticleSystem::TransformPosIfLocal, C3DVector::ToC3DVector,
// CGraphicDataBank::PeekGraphic (lines 552..559: test, `return &GraphicList[index];`, `return NULL;`),
// CParticleUpdateParamDesc::GetUpdateParam (lines 126..133: `nested < MaxRecursion`, forward with `nested + 1`, `return false`;
// the row passes 0 and retail pushes 1 at 0x00B0771F) -- and STLport's own list::size() (distance(begin(), end())).
// Measured (c2.dll 13.10.3077, tree canonicaliser 0x1075CC68): a commutative float node's operands are sorted by a 16-bit hash of
// the frame symbols' numbers (0x1075D388 / 0x1075D54E), and symbol numbers are handed out 32 at a time per pool (0x107603DB). With
// the helper bodies above the trail loop's E32 field symbol is the last of a block (#383) and E22 the first of the next (#480),
// which gives retail's `E22*y + E32*z` at 0x00B07676; any other count of inlined locals / return temporaries before it gives
// `E32*z + E22*y`. No statement of the row is fitted to that order.
// CParticleSystem::ScaleParticleDimensions(C2DVector&) (0x00B0794B `call 0x00AF8F00`): a header inline that VC7.1 does not expand here
// (Ego_d.pdb lines 243..250 of the particle-system header; shared opt-in FABLE_RETAIL_CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_C2DVECTOR_INLINE).
// The body must be visible (declaration only: 976v977); the COMDAT this object emits equals retail 0x00AF8F00 (117 bytes).
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TOC3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALE
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_C2DVECTOR_INLINE
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CPARTICLESYSTEM_BOUNDING_TRANSFORM_PASS24
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_ADDTOBOUNDS
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM_NESTED
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_GETUPDATEPARAM
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSINGLESPRITE_ADDTOBOUNDINGSPHERE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPSCSingleSprite.h"
#include "engine/CParticleSystem.h"
#include "engine/CParticleUpdateParameters.h"
#include "engine/GFGetInterpolatedValue.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/C2DVector.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicInfoFrame.h"
#include <math.h>

// Ego_d.pdb ?PeekGraphic@CGraphicDataBank@@QBEPBVCGraphicInfo@@K@Z line table 552..559: the bounds test (19 bytes), the indexed
// return (20), the null return (2); the debug-only assert of line 553 is not in retail.
inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    if (index < GraphicList.size())
        return &GraphicList[index];
    return 0;
}
inline unsigned long CGraphicInfoFrame::GetWidth()const{return GraphicInfo->Header.FrameWidth;}
inline unsigned long CGraphicInfoFrame::GetHeight()const{return GraphicInfo->Header.FrameHeight;}

namespace NParticleEngine {
long FABLE_FASTCALL GetTickRate();                           // 0x00AEA9F0
const CGraphicDataBank& FABLE_FASTCALL GetGraphicBank();     // 0x00AEAAA0

bool CPSCSingleSprite::AddToBoundingSphere(C3DBoundingSphere& bsphere, const float interpolate, CParticleSystem& system,
                                           const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    bool valid = false;
    if (TrailBankIndex != 0)
    {
        std::list<CEnginePrimitiveSpriteTrail::CSpriteTrailEntry>& trail = TrailUpdatePrimitive.Trail;
        long size = trail.size();
        if (size > 0)
        {
            const float half_trail_width = TrailWidth * 0.5f;
            for (std::list<CEnginePrimitiveSpriteTrail::CSpriteTrailEntry>::iterator it_trail_pos = trail.begin(); it_trail_pos != trail.end(); ++it_trail_pos)
            {
                C3DVector pt = it_trail_pos->Position.ToC3DVector();
                system.TransformPosIfLocal(pt);
                bsphere.AddToBounds(pt, half_trail_width);
            }
            valid = true;
        }
    }
    C3DVector pos;
    if (SpriteBankIndex != 0 && params.GetUpdateParam(pos, PositionParam, interpolate, false, 0))
    {
        C3DVector pt = pos.ToC3DVector();
        CMatrix3x4 trans;
        system.GetSystemTransform(trans);
        trans.TransformPoint(pt);
        float tick_rate = (float)GetTickRate();
        float life_timer = Lifetimer + interpolate;
        float normalised_time = system.GetNormalisedTime();
        if (normalised_time < 0.0f) normalised_time = 0.0f; else if (normalised_time > 1.0f) normalised_time = 1.0f;
        float anim_percent = 0.0f;
        if (!ForceAnimationTime)
        {
            anim_percent = normalised_time;
            if (anim_percent < 0.0f) anim_percent = 0.0f; else if (anim_percent > 1.0f - DELTA_NOTIONAL_ZERO) anim_percent = 1.0f - DELTA_NOTIONAL_ZERO;
        }
        else
        {
            float anim_step = 1.0f / (AnimationTimeSecs * tick_rate);
            anim_percent = life_timer * anim_step;
            anim_percent = fmod(anim_percent, 1.0f);
        }
        const CGraphicDataBank& bank = GetGraphicBank();
        long no_frames_in_graphic = bank.GetNoFramesInGraphic(SpriteBankIndex);
        long frame = GFFloatToLongFloor(no_frames_in_graphic * anim_percent);
        if (frame < 0) frame = 0; else if (frame > no_frames_in_graphic - 1) frame = no_frames_in_graphic - 1;
        const CGraphicInfoFrame* graphic_frame = bank.PeekGraphic(SpriteBankIndex)->PeekInfoFrame(frame);
        C2DVector size;
        size.X = GFGetInterpolatedValue(StartRenderSize, EndRenderSize, anim_percent);
        size.Y = (float)graphic_frame->GetHeight() / graphic_frame->GetWidth() * size.X;
        system.ScaleParticleDimensions(size);
        float factor = 0.5f;
        if (CrossedSprites && SpriteAlignment != 0)
            factor = 1.0f;
        bsphere.AddToBounds(pt, size.GetMagnitude() * factor);
        valid = true;
    }
    return valid;
}

}