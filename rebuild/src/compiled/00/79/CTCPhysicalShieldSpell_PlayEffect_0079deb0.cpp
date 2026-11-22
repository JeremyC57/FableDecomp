// 0079deb0 CTCPhysicalShieldSpell::PlayEffect
// Leaf: ?PlayEffect@CTCPhysicalShieldSpell@@IAEXPAV?$CIntelligentPointer@VCThing@@@@ABVCCharString@@1PAVCThingCreatureBase@@J@Z
// Flags: none
#define FABLE_RETAIL_PHYSICAL_SHIELD_EFFECT_PASS26
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_MESHSIZE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CCHARSTRING_EQ_CSTR
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CSpecialAbilitiesPhysicalShieldDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CTCPhysicalShieldSpell.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/ThingAlive.h"
#include "engine/CDefClassBase_GetSubDef.h"

void CTCPhysicalShieldSpell::PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& effect_name_low,
                                        const CCharString& effect_name_hi, CThingCreatureBase* creature, long flags)
{
    if (effect_name_hi == "") {
        CDefPointer<const CSpecialEffectsDef> pspecial_effects;
        long effect_index = 0;
        if (PDef->GetSubDef(&pspecial_effects) &&
            pspecial_effects->GetEffectFromString(effect_name_low, effect_index)) {
            if (!effect) {
                CThing* temporary_effect = CTCSpecialAbilities::CreateParticleEffect(effect_index, creature->GetPos());
                if (IS_THING_ALIVE(temporary_effect)) {
                    CTCDParticleEmitter* ptc_emitter;
                    if (!temporary_effect->GetTC(&ptc_emitter)) {
                        FABLE_RETAIL_ASSERT(false);
                    }
                    ptc_emitter->AttachToThing(*creature, flags, "", 0, 0.0f);
                }
            } else {
                *effect = CTCSpecialAbilities::CreateParticleEffect(effect_index, creature->GetPos());
                if (IS_THING_ALIVE((*effect))) {
                    CTCDParticleEmitter* ptc_emitter;
                    if (!(*effect)->GetTC(&ptc_emitter)) {
                        FABLE_RETAIL_ASSERT(false);
                    }
                    ptc_emitter->AttachToThing(*creature, flags, "", 0, 0.0f);
                }
            }
        }
    } else {
        CDefPointer<const CSpecialEffectsDef> pspecial_effects;
        long effect_index_lo = 0, effect_index_hi = 0;
        if (PDef->GetSubDef(&pspecial_effects) &&
            pspecial_effects->GetEffectFromString(effect_name_low, effect_index_lo) &&
            pspecial_effects->GetEffectFromString(effect_name_hi, effect_index_hi)) {
            float height_range = creature->GetApproxMaxMeshHeight() - creature->GetNominalMeshHeight();
            float interpolate = 0.0f;
            if (height_range > 0.0f)
                interpolate = (creature->GetCurrentMeshHeight() - creature->GetNominalMeshHeight()) / height_range;
            if (!effect) {
                CThing* temporary_effect = CTCDParticleEmitter::Create(effect_index_lo, effect_index_hi, interpolate, creature->GetPos(), false);
                if (IS_THING_ALIVE(temporary_effect)) {
                    CTCDParticleEmitter* ptc_emitter = 0;
                    if (!temporary_effect->GetTC(&ptc_emitter)) {
                        FABLE_RETAIL_ASSERT(false);
                    }
                    ptc_emitter->AttachToThing(*creature, flags, "", 0, 0.0f);
                    ptc_emitter->SetAlwaysUpdate(true);
                }
            } else {
                *effect = CTCDParticleEmitter::Create(effect_index_lo, effect_index_hi, interpolate, creature->GetPos(), false);
                if (IS_THING_ALIVE((*effect))) {
                    CTCDParticleEmitter* ptc_emitter = 0;
                    if (!(*effect)->GetTC(&ptc_emitter)) {
                        FABLE_RETAIL_ASSERT(false);
                    }
                    ptc_emitter->AttachToThing(*creature, flags, "", 0, 0.0f);
                    ptc_emitter->SetAlwaysUpdate(true);
                }
            }
        }
    }
}