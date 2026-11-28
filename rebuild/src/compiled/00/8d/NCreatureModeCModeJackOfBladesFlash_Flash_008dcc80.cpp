// 008dcc80 NCreatureMode::CModeJackOfBladesFlash::Flash
// Leaf: ?Flash@CModeJackOfBladesFlash@NCreatureMode@@AAEXXZ
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_FLASH_SQRT_SEED_PASS30
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#define FABLE_RETAIL_FLASH_HIT_CTOR_OWNER_PASS30
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_FLASH_MAP_CTOR_DECL_PASS30
#define FABLE_RETAIL_STLP_MAP_DTOR_DECL_ONLY
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CTC_BURNT_EFFECT_FLASH_PASS30
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_NCREATUREMODE_UNCONSCIOUS_FLASH_PASS30
#define FABLE_RETAIL_JACK_FLASH_FRAME_PASS29
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_UPDATEAI_VECTOR_SUB_PASS29
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CJACKOFBLADESBATTLEDEF_TYPENAME_PASS26
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
#define FABLE_RETAIL_AI_CREATURE_ONHIT_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CWORLD
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/ThingAlive.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CWorld.h"

#include "engine/CThingBaseDef.h"
#include "engine/CJackOfBladesBattleDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCBurntEffect.h"
#include "engine/CHitParameters.h"
#include "engine/NCreatureMode.h"
#include "engine/NCreatureMode_CModeJackOfBladesFlash.h"

namespace NCreatureMode {
void CModeJackOfBladesFlash::Flash()
{
    CDefPointer<const CJackOfBladesBattleDef> p_jack_def;
    GetParentThing().PeekBaseDef().GetSubDef(&p_jack_def);
    float flash_damage = p_jack_def->BurnDamage;
    if (IS_THING_ALIVE(PTarget)) {
        CTCPerceiveThing* ptc_perceived_thing = 0;
        if (PCreature->GetTC(&ptc_perceived_thing)) {
            if (IS_THING_ALIVE(ptc_perceived_thing->GetPerceivedThing()) &&
                PTarget == ptc_perceived_thing->GetPerceivedThing()) {
                CTCBurntEffect* ptc_burnt = 0;
                bool has_burning_fx = PTarget->GetTC(&ptc_burnt);
                if (ptc_perceived_thing->IsThingLineOfSight()) {
                    if (!has_burning_fx)
                        PTarget->AddTC("CTCBurntEffect", false, 0);
                    long damage = 0;
                    if (BurnCounter > 5)
                        damage = (long)flash_damage;
                    ++BurnCounter;
                    C3DVector dir = PTarget->GetPos() - GetParentThing().GetPos();
                    dir.Normalise();
                    CHitParameters hit_params(0, &GetParentThing(), 0, PTarget->GetPos(), dir,
                                              false, false, false, false);
                    hit_params.SetPlayHitResponse(false);
                    hit_params.AddDamageWithAttribute(DAMAGE_FIRE, (float)damage);
                    PTarget->OnHit(hit_params);
                } else if (has_burning_fx) {
                    PTarget->RemoveTC(TCI_BURNT_EFFECT);
                }
            }
        }
    }
    std::vector<CThingAICreature*> creatures;
    GetWorld().PeekThingSearchTools().GetAllThings<CThingAICreature>(CThingFilter_IsNot(*PCreature), creatures);
    long num_things_to_burn = creatures.size();
    for (long n = 0; n < num_things_to_burn; ++n) {
        if (IS_THING_ALIVE(creatures[n]) && !IsUnconscious(*creatures[n])) {
            CTCBurntEffect* ptc_burnt = 0;
            bool has_burning_fx = creatures[n]->GetTC(&ptc_burnt);
            if (!has_burning_fx)
                creatures[n]->AddTC("CTCBurntEffect", false, 0);
            long damage = 0;
            if (BurnCounter > 5)
                damage = (long)flash_damage;
            C3DVector dir = creatures[n]->GetPos() - GetParentThing().GetPos();
            dir.Normalise();
            CHitParameters hit_params(0, &GetParentThing(), 0, creatures[n]->GetPos(), dir,
                                      false, false, false, false);
            hit_params.SetPlayHitResponse(false);
            hit_params.AddDamageWithAttribute(DAMAGE_FIRE, (float)damage);
            creatures[n]->OnHit(hit_params);
        }
    }
}
}