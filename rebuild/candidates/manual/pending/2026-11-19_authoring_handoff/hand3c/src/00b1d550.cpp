// NParticleEngine::CPSCDecalRenderer::DecalCollides(system, it_decal, decal_size, it_collison_decal, collison_decal_size) -- find the first other active decal of the system whose normalised life (capped at 1) lies in the fade band [fade-in end, fade-out begin] and whose pooled size (render size interpolated from start to end by that life, times its size factor, capped at MaxPoolSize) overlaps the given decal grown by decal_size; return it and its size
// Leaf: ?DecalCollides@CPSCDecalRenderer@NParticleEngine@@IAE_NAAVCParticleSystem@2@ABU?$_List_iterator@PAVCDecal@NParticleEngine@@U?$_Nonconst_traits@PAVCDecal@NParticleEngine@@@std@@@std@@MAAU45@AAM@Z
// Flags: none
// Retail-asm: 0x00B1D5A5 `fld` + 0x00B1D5A9 `fistp dword ptr [esp+14h]` and 0x00B1D605 + 0x00B1D609 -- the engine's inline-assembly
//   GFFloatToLongNear inside the two GFUnpackFloat expansions (engine/GFPackFloat.h, GFRoundFloatNear). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1D550, 576 bytes, `ret 0x14`, esp frame.
// Shape: FableWin.pdb ?DecalCollides@CPSCDecalRenderer@NParticleEngine@@IAE_N...@Z (FableWin 0x02F567F0), statement for statement, with
//   the Ego_d.pdb locals (start_render_size, end_render_size, fade_out_begin, fade_in_end, it_this_decal, this_decal, life_timer,
//   decal_normalised_time, decal_anim_percent, decal, this_decal_size). The debug build's range ASSERT on decal_normalised_time is
//   not in retail; FableWin's `xor reg,reg ; je` before the loop is the debug for-scope guard.
// Callees: none out of line (GFUnpackFloat, GFGetInterpolatedValue, std::min, the CDecal accessors, GetSquaredMagnitude and GFSqr are
//   all expanded).
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
#define FABLE_RETAIL_CDECAL_GETSIZEFACTOR_NOARG
#define FABLE_RETAIL_CPSCDECALRENDERER_DECALCOLLIDES
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPSCDecalRenderer.h"
#include "engine/CDecal.h"
#include "engine/CParticleSystem.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/GFPackFloat.h"
#include "engine/GFGetInterpolatedValue.h"

namespace NParticleEngine {

bool CPSCDecalRenderer::DecalCollides(CParticleSystem& system, const std::list<CDecal*>::iterator& it_decal, float decal_size,
                                      std::list<CDecal*>::iterator& it_collison_decal, float& collison_decal_size)
{
    const float start_render_size = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
    const float end_render_size = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
    const float fade_out_begin = FadeOutBeginInteger * 0.001f;
    const float fade_in_end = FadeInEndInteger * 0.001f;
    for (std::list<CDecal*>::iterator it_this_decal = system.GetDecalList().begin(); it_this_decal != system.GetDecalList().end(); ++it_this_decal) {
        if (it_this_decal != it_decal) {
            CDecal* this_decal = *it_this_decal;
            if (this_decal->IsActive()) {
                const float life_timer = this_decal->GetInterpolatedLifetimer(0.0f);
                const float decal_normalised_time = std::min(life_timer / this_decal->GetMaxLife(), 1.0f);
                float decal_anim_percent = decal_normalised_time;
                if (decal_normalised_time >= fade_in_end && decal_normalised_time <= fade_out_begin) {
                    CDecal* decal = *it_decal;
                    float this_decal_size = std::min(GFGetInterpolatedValue(start_render_size, end_render_size, decal_anim_percent) * this_decal->GetSizeFactor(), MaxPoolSize);
                    if ((decal->GetPosition() - this_decal->GetPosition()).GetSquaredMagnitude() < GFSqr(decal_size + this_decal_size)) {
                        collison_decal_size = this_decal_size;
                        it_collison_decal = it_this_decal;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

}
