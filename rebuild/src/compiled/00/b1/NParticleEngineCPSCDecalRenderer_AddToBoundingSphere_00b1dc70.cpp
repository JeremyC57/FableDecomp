// NParticleEngine::CPSCDecalRenderer::AddToBoundingSphere(bsphere_list, interpolate, system, params) -- grow the bounding sphere by every active decal of the system (its position, radius half its rendered size at the animation point the system time or the forced animation time gives); clear expired inactive decals; false with no decal bank or no active decal
// Leaf: ?AddToBoundingSphere@CPSCDecalRenderer@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z
// Flags: none
// Retail-asm: 0x00B1DCD3 + 0x00B1DCD6, 0x00B1DD25 + 0x00B1DD28, 0x00B1DD84 + 0x00B1DD87 `fistp` -- GFFloatToLongNear in the three GFUnpackFloat
//   expansions -- and 0x00B1DEC0 `fld` / 0x00B1DEC3 `fsub` / 0x00B1DEC6 `fistp dword ptr [ebp-10h]` -- GFFloatToLongFloor (engine/GFPackFloat.h,
//   engine/GFFloatToLongFloor.h). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1DC70, 860 bytes, `ret 0x10`, ebp frame; vtable 0x012A09BC (CPSCBase slot).
// Shape: FableWin.pdb ?AddToBoundingSphere@CPSCDecalRenderer@NParticleEngine@@UAE_N...@Z (FableWin 0x02F55EB0), statement for statement, with
//   the Ego_d.pdb locals (start_render_size, end_render_size, anim_time_secs, pos, trail_pos2 (unused), valid, it_decal, decal,
//   tick_rate, life_timer, normalised_time, anim_percent, anim_step, bank, no_frames_in_graphic, frame, graphic_frame, size, factor).
// Inferred: the radius factor -- FableWin 0x02F5624D..0x02F5627D sets factor = 1.0f only when (Flags & 0x40) && (Flags & 0) (the second
//   mask is the literal 0, so retail folds it to the 0.5f); no enumerator name is recovered for either mask.
// Callees: NParticleEngine::GetTickRate 0x00AEA9F0, NParticleEngine::GetGraphicBank 0x00AEAAA0 (GetNoFramesInGraphic through vtable +0x50),
//   _CIfmod 0x00BFEB28, CParticleSystem::ScaleParticleDimensions 0x00AF8F00 (the folded copy; body visible noinline, as the landed
//   PreparePrimitives 0x00B1F200), C3DBoundingSphere::AddToBounds 0x00ADDD80, CParticleSystem::ClearDecal; DELTA_NOTIONAL_ZERO 0x0129BA3C.
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_GETDECALLIST
#define FABLE_RETAIL_CDECAL
#define FABLE_RETAIL_CDECAL_PREPARE
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CPARTICLESYSTEM_CLEARDECAL
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CDECAL_ISACTIVE_MAXLIFE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_ADDTOBOUNDS
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CDECAL_GETSIZEFACTOR_NOARG
#define FABLE_RETAIL_CPSCDECALRENDERER_DECALCOLLIDES
#define FABLE_RETAIL_CPSCDECALRENDERER_ADDTOBOUNDINGSPHERE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPSCDecalRenderer.h"
#include "engine/CDecal.h"
#include "engine/CParticleSystem.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/GFPackFloat.h"
#include "engine/GFGetInterpolatedValue.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/C2DVector.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicInfoFrame.h"
#include <math.h>

inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index)const{return index<GraphicList.size()?&GraphicList[index]:0;}
inline unsigned long CGraphicInfoFrame::GetWidth()const{return GraphicInfo->Header.FrameWidth;}
inline unsigned long CGraphicInfoFrame::GetHeight()const{return GraphicInfo->Header.FrameHeight;}
extern const float DELTA_NOTIONAL_ZERO;   // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB

namespace NParticleEngine {
// Inline in CParticleSystem's header (FableWin.pdb); retail calls the folded out-of-line copy 0x00AF8F00 (the landed PreparePrimitives
// 0x00B1F200 text).
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CParticleSystem::ScaleParticleDimensions(C2DVector& dim)const{
    if(ScaleParticles){float scale=((Scale.X>Scale.Y?Scale.X:Scale.Y)>Scale.Z?(Scale.X>Scale.Y?Scale.X:Scale.Y):Scale.Z);dim.X*=scale;dim.Y*=scale;}
}
long FABLE_FASTCALL GetTickRate();                           // 0x00AEA9F0
const CGraphicDataBank& FABLE_FASTCALL GetGraphicBank();     // 0x00AEAAA0

bool CPSCDecalRenderer::AddToBoundingSphere(C3DBoundingSphere& bsphere_list, const float interpolate, CParticleSystem& system,
                                            const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    if (DecalBankIndex == 0)
        return false;
    const float start_render_size = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
    const float end_render_size = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
    const float anim_time_secs = GFUnpackFloat(AnimationTimeSecs, 14, 0.1f, 100.0f, 2);
    C3DVector pos;
    C3DVector trail_pos2;
    bool valid = false;
    for (std::list<CDecal*>::iterator it_decal = system.GetDecalList().begin(); it_decal != system.GetDecalList().end(); ++it_decal) {
        const CDecal* decal = *it_decal;
        if (decal->IsActive()) {
            pos = decal->GetPosition();
            float tick_rate = (float)GetTickRate();
            float life_timer = decal->GetInterpolatedLifetimer(interpolate);
            float normalised_time = system.GetNormalisedTime();
            if (normalised_time < 0.0f)
                normalised_time = 0.0f;
            else if (normalised_time > 1.0f)
                normalised_time = 1.0f;
            float anim_percent = 0.0f;
            if (!ForceAnimationTime) {
                anim_percent = normalised_time;
                if (anim_percent < 0.0f)
                    anim_percent = 0.0f;
                else if (anim_percent > 1.0f - DELTA_NOTIONAL_ZERO)
                    anim_percent = 1.0f - DELTA_NOTIONAL_ZERO;
            } else {
                float anim_step = 1.0f / (anim_time_secs * tick_rate);
                anim_percent = life_timer * anim_step;
                anim_percent = fmod(anim_percent, 1.0f);
            }
            const CGraphicDataBank& bank = GetGraphicBank();
            long no_frames_in_graphic = bank.GetNoFramesInGraphic(DecalBankIndex);
            long frame = GFFloatToLongFloor(no_frames_in_graphic * anim_percent);
            if (frame < 0)
                frame = 0;
            else if (frame > no_frames_in_graphic - 1)
                frame = no_frames_in_graphic - 1;
            const CGraphicInfoFrame* graphic_frame = bank.PeekGraphic(DecalBankIndex)->PeekInfoFrame(frame);
            C2DVector size;
            size.X = GFGetInterpolatedValue(start_render_size, end_render_size, anim_percent);
            size.Y = (float)graphic_frame->GetHeight() / graphic_frame->GetWidth() * size.X;
            system.ScaleParticleDimensions(size);
            float factor = 0.5f;
            if ((Flags & 0x40) && (Flags & 0))
                factor = 1.0f;
            bsphere_list.AddToBounds(pos, size.GetMagnitude() * factor);
            valid = true;
        } else if (decal->GetInterpolatedLifetimer(interpolate) > decal->GetMaxLife())
            system.ClearDecal(it_decal);
    }
    return valid;
}

}