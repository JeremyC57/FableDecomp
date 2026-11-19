// CTCObjectAugmentations::AttemptToStartParticle(CIntelligentPointer<CThing>& pfx, long effectid, long effectidtoblendto, const CCharString& startpoint, const CCharString& endpoint) -- clear the effect; when both attachment points resolve on the owner's appearance (helper point, else dummy object) and an effect is set, create the effect and the effect to blend to at the owner, stretch the effect's emitter towards the blend emitter by the points' distance against the two spline lengths, attach the effect at the start point and kill the blend particle
// Leaf: ?AttemptToStartParticle@CTCObjectAugmentations@@AAEXAAV?$CIntelligentPointer@VCThing@@@@JJABVCCharString@@1@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00766660, 696 bytes, `ret 14h`.
// Identity: Ego_r.pdb / FableWin.pdb ?AttemptToStartParticle@CTCObjectAugmentations@@AAEXAAV?$CIntelligentPointer@VCThing@@@@JJABVCCharString@@1@Z
//   (private; the shared header's FABLE_RETAIL_AUGMENTATION_PARTICLES class declares it). Locals and scopes: Ego_d.pdb (debug length 1943):
//   pfx, effectid, effectidtoblendto, startpoint, endpoint; end_pos, start_pos, orientation; { blend_to { ptc_emitter1, ptc_emitter2,
//   pem1, pem2, dist_between_attachment_points_vector, dist_between_attachment_points, long_spline_len, short_spline_len,
//   distance_between_particle_points, interp, MAGIC_FACTOR } }.
// Shape: the attachment block of the landed CTCMultiStrike::ApplyEffectOnWeapon 0x007CCF50 (same Ego_d local names, same opt-in set:
//   Sleep / SetAlwaysUpdate / GetPEmitter inline, the expanded GetTC), here on the owner's appearance with the effect handed back
//   through the intelligent pointer, and the spline-length difference floored at DELTA_NOTIONAL_ZERO (0x007668B6 `fcomp [129BA3Ch]`).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtable 0x01238C6C),
//   CTCGraphicAppearance::GetHelperPoint 0x004C0560 / GetDummyObject 0x004C08C0, CThing::GetPos 0x004C73D0,
//   CTCDParticleEmitter::Create(long, const C3DVector&, bool) 0x006E0880, CVectorMap LowerBound 0x0040F020,
//   CParticleEmitter::GetApproximateTotalSplineLength 0x00AED050 / InterpolateTo 0x00AED600, CTCDParticleEmitter::AttachToThing
//   0x006E0BE0, CThing::Kill 0x004C9B80.
// DELTA_NOTIONAL_ZERO from engine/C3DVector.h (declared there under FABLE_RETAIL_C3DVECTOR; audit: the row's own extern removed, byte-neutral).
// Ego_d line table (C13 lines, carved; debug length 1943): the statements in the text's order (MAGIC_FACTOR 629 between interp 628 and
//   InterpolateTo 630). Unreproduced: lines 612 / 613 (488 / 490 B, around the two GetTC calls) and 617 (219 B, before Sleep) are far
//   larger than their retail code; not written.
#define FABLE_RETAIL_AUGMENTATION_PARTICLES
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_GETPEMITTER_INLINE
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CParticleEmitter.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCObjectAugmentations.h"

void CTCObjectAugmentations::AttemptToStartParticle(CIntelligentPointer<CThing>& pfx, long effectid, long effectidtoblendto,
                                                    const CCharString& startpoint, const CCharString& endpoint)
{
    pfx = NULL;
    C3DVector start_pos;
    C3DVector end_pos;
    CRightHandedSet orientation;
    if (!GetParentThing().GetTCGraphicAppearance()->GetHelperPoint(startpoint, 0, 0, start_pos, 1.0f) &&
        !GetParentThing().GetTCGraphicAppearance()->GetDummyObject(startpoint, 0, 0, start_pos, orientation, 1.0f))
        return;
    if (!GetParentThing().GetTCGraphicAppearance()->GetHelperPoint(endpoint, 0, 0, end_pos, 1.0f) &&
        !GetParentThing().GetTCGraphicAppearance()->GetDummyObject(endpoint, 0, 0, end_pos, orientation, 1.0f))
        return;
    if (effectid > 0)
    {
        pfx = CTCDParticleEmitter::Create(effectid, GetParentThing().GetPos(), true);
        CThing* blend_to = CTCDParticleEmitter::Create(effectidtoblendto, GetParentThing().GetPos(), true);
        if (IS_THING_ALIVE(pfx) && IS_THING_ALIVE(blend_to))
        {
            CTCDParticleEmitter* ptc_emitter1 = 0;
            CTCDParticleEmitter* ptc_emitter2 = 0;
            pfx->GetTC(&ptc_emitter1);
            blend_to->GetTC(&ptc_emitter2);
            NParticleEngine::CParticleEmitter* pem1 = ptc_emitter1->GetPEmitter();
            NParticleEngine::CParticleEmitter* pem2 = ptc_emitter2->GetPEmitter();
            pem2->Sleep();
            C3DVector dist_between_attachment_points_vector = start_pos - end_pos;
            float dist_between_attachment_points = dist_between_attachment_points_vector.GetMagnitude();
            float long_spline_len = pem2->GetApproximateTotalSplineLength();
            float short_spline_len = pem1->GetApproximateTotalSplineLength();
            float distance_between_particle_points = long_spline_len - short_spline_len;
            float interp = (dist_between_attachment_points - short_spline_len) / std::max(distance_between_particle_points, DELTA_NOTIONAL_ZERO);
            const float MAGIC_FACTOR = 2.0f;
            pem1->InterpolateTo(*pem2, interp * MAGIC_FACTOR);
            pem1->SetAlwaysUpdate(true);
            ptc_emitter1->AttachToThing(GetParentThing(), 10, startpoint, 0, 0.0f);
            blend_to->Kill(true);
        }
    }
}