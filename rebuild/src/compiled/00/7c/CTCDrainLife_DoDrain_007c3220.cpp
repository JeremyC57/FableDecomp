// 007c3220 CTCDrainLife::DoDrain
// Leaf: ?DoDrain@CTCDrainLife@@AAEXPAVCThingCreatureBase@@@Z
// Flags: /Gr
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_DTOR_DECL_ONLY
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#include "fable_assert.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_DRAIN_PULSE_PASS29
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CDRAINLIFEDEF_PASS22
#define FABLE_RETAIL_CTCDRAINLIFE_PASS22
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR_BODY
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_SETCREATEDUST
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ISABLETOBEDEFLECTED
#define FABLE_RETAIL_CTHINGCREATUREBASE_MAKENOISE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"
#include "engine/CIntelligentPointer.h"
// The native CHitParameters constructor calls the size-built map owner at 00453554.
namespace std { template<> map<EDamageAttribute,float>::map(); }
// CHitParameters at 00789238 saves EBP and uses the size optimization profile.
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
#include "engine/CHitParameters.h"
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "engine/CThingCreatureBase.h"
#include "engine/CTCDrainLife.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialAbilitiesDrainLifeDataDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/ThingAlive.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NEntityEvents_CEventHitBy.h"
#include "engine/NThingShot_CTCShotDefinitionDrainLife.h"
#include "engine/EDamageAttribute.h"
void CTCDrainLife::DoDrain(CThingCreatureBase* pcreature)
{
    CThingCreatureBase& parent_creature = *GFDebugDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);
    parent_creature.MakeNoise("SIMPLE_NOISE");
    float suck = PDef->AmountOfHealthSucked[SpellLevel];
    CDefPointer<const CSpecialAbilitiesDrainLifeDataDef> psubdef;
    if (GetParentThing().GetPBaseDef()->GetSubDef(&psubdef))
        suck = psubdef->Damage;
    float health = pcreature->GetHealth();
    if (suck > health)
        suck = health;
    C3DVector hit_pos = pcreature->GetPos();
    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = pcreature->GetTCGraphicAppearance();
    if (ptc_graphic_appearance)
        if (!ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME,0,0,hit_pos,1.0f)) { FABLE_RETAIL_ASSERT(!"Thingy doesn't have a focus point for drainlife HIT"); }
    CHitParameters hit_params(0,&parent_creature,0,hit_pos,pcreature->GetPos()-parent_creature.GetPos(),false,false,false,false);
    hit_params.AddDamageWithAttribute(DAMAGE_DRAIN,suck);
    hit_params.SetPlayHitResponse(false);
    hit_params.SetCreateDustParticleEffectOnHit(false);
    pcreature->OnHit(hit_params);
    if (IS_THING_ALIVE(pcreature)) {
        CTCEntityEvents* ptc_entity_events;
        if (pcreature->GetTC(&ptc_entity_events))
            ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit_params,HERO_ABILITY_DRAIN_LIFE_SPELL));
    }
    if (!hit_params.IsBlocked() && !hit_params.IsHitNegated() && IS_THING_ALIVE(pcreature)) {
        CDefPointer<const CSpecialEffectsDef> pspecial_effects;
        long effect=0;
        CCharString effectname="DAMAGE_EFFECT";
        if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString(effectname,effect)) {
            CThing* fx=CTCSpecialAbilities::CreateParticleEffect(effect,pcreature->GetPos());
            CTCDParticleEmitter* emitter;
            if (IS_THING_ALIVE(fx) && fx->GetTC(&emitter))
                emitter->AttachToThing(*pcreature,12,MAIN_FOCAL_POSITION_NAME,0,0.0f);
        }
        if (pcreature->IsDamageable()) {
            long num_effects = PDef->NumEffects[SpellLevel];
            float per_effect_drain = suck/num_effects;
            long global_index = parent_creature.PeekDefinitionManager().GetDefGlobalIndexFromName(PDef->ShotName[SpellLevel]);
            if (false) {} else for (long i=0;i<num_effects;++i) {
                C3DVector creation_pos=pcreature->GetPos();
                if (ptc_graphic_appearance)
                    if (!ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME,0,0,creation_pos,1.0f)) { FABLE_RETAIL_ASSERT(!"Thingy doesn't have a focus point for drainlife"); }
                CThing* pshot=NThingShot::CreateShot(global_index,pcreature,0,&parent_creature,100.0f,1.0f,0.0f,
                    NULL_DEF,&creation_pos,&pcreature->GetPhysicsTC()->GetRHSet());
                if (IS_THING_ALIVE(pshot)) {
                    pshot->SetAsDrawable(false);
                    NThingShot::CTCShotDefinitionDrainLife* ptc_shot_drainlife;
                    if (pshot->GetTC(&ptc_shot_drainlife)) {
                        ptc_shot_drainlife->SetAmountToHeal(per_effect_drain);
                        ptc_shot_drainlife->SetSoulEffect(GetSoulEffect());
                    }
                    Shots.push_back(CIntelligentPointer<CThing>(pshot));
                }
            }
        }
    }
}