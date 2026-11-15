// CTCMultiStrike::ApplyEffectOnWeapon(CThing* w) -- apply the selected weapon effect to each attachment-point pair.
// Grade: MATCH(1088v1088) -- original declarations and generated special members restored.
// Leaf: ?ApplyEffectOnWeapon@CTCMultiStrike@@QAEXPAVCThing@@@Z
// Retail: Fable.exe TLC VC7.1 @ 0x007CCF50, 1088 bytes including four jump-table entries at 0x007CD380.
// Flags: /Gr
// Original local names recovered from Ego_r.pdb stream 366; other descriptive locals remain identified in NAMING.md.
#define FABLE_RETAIL_CTCMULTISTRIKE_ORIGINAL_DECLARATIONS_PASS22
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_PASS22_MULTI_STRIKE_WEAPON
#define FABLE_RETAIL_CTCMULTISTRIKE_MEMBERS
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
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
#include "engine/CDefPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CSpecialAbilitiesMultiStrikeDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CParticleEmitter.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCMultiStrike.h"

void CTCMultiStrike::ApplyEffectOnWeapon(CThing* w)
{
    if (IS_THING_ALIVE(w)) {
    long effectid = 0;
    long effectidtoblendto = 0;
    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    if (PDef->GetSubDef(&pspecial_effects)) {
        CCharString short_effect_name;
        CCharString long_effect_name;
        switch (SpellLevel) {
        case 1:
            short_effect_name = "WEAPON_EFFECT_SHORT_LEVEL2";
            long_effect_name = "WEAPON_EFFECT_LONG_LEVEL2";
            break;
        case 2:
            short_effect_name = "WEAPON_EFFECT_SHORT_LEVEL3";
            long_effect_name = "WEAPON_EFFECT_LONG_LEVEL3";
            break;
        case 3:
            short_effect_name = "WEAPON_EFFECT_SHORT_LEVEL4";
            long_effect_name = "WEAPON_EFFECT_LONG_LEVEL4";
            break;
        case 0:
            short_effect_name = "WEAPON_EFFECT_SHORT_LEVEL1";
            long_effect_name = "WEAPON_EFFECT_LONG_LEVEL1";
            break;
        default:
            short_effect_name = "WEAPON_EFFECT_SHORT_LEVEL1";
            long_effect_name = "WEAPON_EFFECT_LONG_LEVEL1";
            break;
        }
        if (!pspecial_effects->GetEffectFromString(short_effect_name, effectid))
            return;
        if (!pspecial_effects->GetEffectFromString(long_effect_name, effectidtoblendto))
            return;
        // the attachment points come in pairs: the short effect is attached at the first and stretched to the second
        long num_pairs = PDef->AttachmentPoint.size() / 2;
        for (long i = 0; i < num_pairs; ++i) {
            long index = i*2;
            CCharString startpoint = PDef->AttachmentPoint[index];
            CCharString endpoint = PDef->AttachmentPoint[index+1];
            C3DVector start_pos;
            C3DVector end_pos;
            CRightHandedSet orientation;
            if (!w->GetTCGraphicAppearance()->GetHelperPoint(startpoint, 0, 0, start_pos, 1.0f) &&
                !w->GetTCGraphicAppearance()->GetDummyObject(startpoint, 0, 0, start_pos, orientation, 1.0f))
                continue;
            if (!w->GetTCGraphicAppearance()->GetHelperPoint(endpoint, 0, 0, end_pos, 1.0f) &&
                !w->GetTCGraphicAppearance()->GetDummyObject(endpoint, 0, 0, end_pos, orientation, 1.0f))
                continue;
                CThing* pfx = CTCDParticleEmitter::Create(effectid, w->GetPos(), true);
                CThing* blend_to = CTCDParticleEmitter::Create(effectidtoblendto, w->GetPos(), true);
                if (IS_THING_ALIVE(pfx) && IS_THING_ALIVE(blend_to)) {
                    CTCDParticleEmitter* ptcEmitter1 = 0;
                    CTCDParticleEmitter* ptcEmitter2 = 0;
                    pfx->GetTC(&ptcEmitter1);
                    blend_to->GetTC(&ptcEmitter2);
                    NParticleEngine::CParticleEmitter* pem1 = ptcEmitter1->GetPEmitter();
                    NParticleEngine::CParticleEmitter* pem2 = ptcEmitter2->GetPEmitter();
                    pem2->Sleep();
                    C3DVector dist_between_attachment_points_vector = start_pos - end_pos;
                    float dist_between_attachment_points = dist_between_attachment_points_vector.GetMagnitude();
                    float long_spline_len = pem2->GetApproximateTotalSplineLength();
                    float short_spline_len = pem1->GetApproximateTotalSplineLength();
                    float distance_between_particle_points = long_spline_len-short_spline_len;
                    float interp = (dist_between_attachment_points-short_spline_len)/distance_between_particle_points;
                    const float MAGIC_FACTOR = 2.0f;
                    pem1->InterpolateTo(*pem2, interp*MAGIC_FACTOR);
                    pem1->SetAlwaysUpdate(true);
                    ptcEmitter1->AttachToThing(*w, 10, startpoint, 0, 0.0f);
                    blend_to->Kill(true);
                    ParticleEffects.push_back(pfx);
                }
        }
    }
    }
}