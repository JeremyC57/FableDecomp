// 007C4430 CTCHealLife::StartHealingEffect() -- begin targeted or area healing.
// Leaf: ?StartHealingEffect@CTCHealLife@@QAEXXZ
// Flags: /Gr
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCHEALLIFE_START_ORIGINAL
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CPlayer.h"
#include "engine/CDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CSpecialAbilitiesHealLifeDef.h"
#include "engine/CHitParameters.h"
#include "engine/EHeroAbility.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CThingSearchTools.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NEntityEvents_CEventHitBy.h"
#include "engine/CTCEntityEvents.h"
#include "engine/ThingAlive.h"
#include "engine/CTCHealLife.h"

void CTCHealLife::StartHealingEffect()
{
    CThingCreatureBase& parent_creature = *GFDebugDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);
    CTCSoundPlayer* ptc_sound_player = 0;
    if (parent_creature.GetTC(&ptc_sound_player)) {
        switch (SpellLevel) {
        case 0: LoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_1", false, false, false, -1); break;
        case 1: LoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_2", false, false, false, -1); break;
        case 2: LoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_3", false, false, false, -1); break;
        case 3: LoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_HEAL_LIFE_CONTINUOUS_LEVEL_4", false, false, false, -1); break;
        }
    }
    long effect = 0;
    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    CCharString effectname = GetLevelEffectName();
    float search_radius = PDef->RadiusOfEffect[SpellLevel];
    PHealingTarget = &parent_creature;
    const CTCTargetingBase* ptc_targeting = 0;
    if (parent_creature.PeekTC(&ptc_targeting)) {
        CThing* ptarget = ptc_targeting->GetPTargetedThing();
        if (IS_THING_ALIVE(ptarget) && parent_creature.GetPlayer().IsZTargeting())
            PHealingTarget = ptarget;
    }
    if (CTCEnemy::AreThingsEnemies(*PHealingTarget, parent_creature))
        PHealingTarget = &parent_creature;
    CThingCreatureBase* phealingtarget_creature = 0;
    if (PHealingTarget->GetTypeFlags() & 8)
        phealingtarget_creature = GFDebugPDynamicCast(PHealingTarget.Get(), (CThingCreatureBase*)0);
    if (IS_THING_ALIVE(phealingtarget_creature)) {
        if (IS_THING_ALIVE(phealingtarget_creature)) {
            ThingsToHeal.push_back(phealingtarget_creature);
            HealingEffects.push_back(CIntelligentPointer<CThing>(0));
            HealthIncreaseAccum.push_back(0.0f);
            if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString(effectname, effect)) {
                CIntelligentPointer<CThing> fx(CTCSpecialAbilities::CreateParticleEffect(effect, phealingtarget_creature->GetPos()));
                if (IS_THING_ALIVE(fx)) {
                    CTCDParticleEmitter* ptc_emitter;
                    fx->GetTC(&ptc_emitter);
                    ptc_emitter->AttachToThing(*phealingtarget_creature, 0x28, "", 0, 0.0f);
                    HealingEffects.back() = fx;
                }
            }
            CTCEntityEvents* ptc_entity_events = 0;
            if (phealingtarget_creature->GetTC(&ptc_entity_events)) {
                CHitParameters hit_params;
                hit_params.SetThingHitting(parent_creature);
                ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit_params, HERO_ABILITY_HEAL_LIFE_SPELL));
            }
            if (phealingtarget_creature == &parent_creature) {
                C3DVector targetpos = phealingtarget_creature->GetPos();
                C2DBoxI search_area(targetpos.ToC2DCoordI(), (long)search_radius);
                PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(search_area,
                    GFFilter_Bind2(CThingFilter_IsNot(*phealingtarget_creature), CThingFilter_In2DRange(targetpos, search_radius)), ThingsToHeal);
                // Original donor02713298 has a constant-false arm before the iterator scope.
                if (false) {} else for (std::list<CIntelligentPointer<CThingCreatureBase> >::iterator it = ThingsToHeal.begin(), endit = ThingsToHeal.end(); it != endit; ++it) {
                    CIntelligentPointer<CThingCreatureBase> something = *it;
                    if (something == phealingtarget_creature)
                        continue;
                    HealingEffects.push_back(CIntelligentPointer<CThing>(0));
                    HealthIncreaseAccum.push_back(0.0f);
                    if (IS_THING_ALIVE(something) && !CTCEnemy::AreThingsEnemies(*phealingtarget_creature, *something)) {
                        if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString(effectname, effect)) {
                            CIntelligentPointer<CThing> fx(CTCSpecialAbilities::CreateParticleEffect(effect, something->GetPos()));
                            if (IS_THING_ALIVE(fx)) {
                                CTCDParticleEmitter* ptc_emitter = 0;
                                fx->GetTC(&ptc_emitter);
                                ptc_emitter->AttachToThing(*something, 0x28, "", 0, 0.0f);
                                HealingEffects.back() = fx;
                            }
                        }
                        CTCEntityEvents* ptc_loop_entity_events = 0;
                        if (something->GetTC(&ptc_loop_entity_events)) {
                            CHitParameters hit_params;
                            hit_params.SetThingHitting(parent_creature);
                            ptc_loop_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit_params, HERO_ABILITY_HEAL_LIFE_SPELL));
                        }
                    }
                }
            }
        }
    }
}
