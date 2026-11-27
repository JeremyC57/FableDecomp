// 008329b0 CThingAICreature::OnHit
// Leaf: ?OnHit@CThingAICreature@@UAEXAAVCHitParameters@@@Z
// Flags: none
#define FABLE_RETAIL_AI_CREATURE_ONHIT_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETATTACKEDBYCREATURE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_CRIMES
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFCLASSBASE_CREATURE_OWNER_SIZE_PASS26
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#define FABLE_RETAIL_CCREATUREDEF_RESPAWN_PASS24
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON
#define FABLE_RETAIL_CTCCARRYING_PLAYERHIT_PASS26
#define FABLE_RETAIL_CTCENEMY
#define FABLE_RETAIL_CTCENEMY_FACTION
#define FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CHitParameters.h"
#include "engine/NCreatureMode_CModeBriarRoseConcentrate.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCCarrying.h"
#include "engine/EOpinionPostDeedType.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventHitThing.h"

void CThingAICreature::OnHit(CHitParameters& hit_params)
{
    CTCCreatureModeManager* ptc_cmm;
    if (GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_cmm)) {
        if (ptc_cmm->IsModeActive(0x3f)) {
            NCreatureMode::CModeBriarRoseConcentrate* briar_mode = static_cast<NCreatureMode::CModeBriarRoseConcentrate*>(&ptc_cmm->GetCurrentMode());
            if (!briar_mode->IsImposter() && !briar_mode->IsAllowedToHitBriarRose()) {
                CThing* pimposter = briar_mode->GetRandomImposter();
                if (IS_THING_ALIVE(pimposter)) {
                    CRightHandedSet their_rhs(pimposter->GetPhysicsTC()->GetRHSet());
                    C3DVector their_pos(pimposter->GetPhysicsTC()->GetPosition());
                    pimposter->GetPhysicsTC()->ResetRHSet(GetPhysicsTC()->GetRHSet());
                    pimposter->GetPhysicsTC()->ResetPosition(GetPhysicsTC()->GetPosition());
                    GetPhysicsTC()->ResetRHSet(their_rhs);
                    GetPhysicsTC()->ResetPosition(their_pos);
                    if (pimposter->GetTypeFlags() & 4) {
                        CThingGameObject* pgo = static_cast<CThingGameObject*>(pimposter);
                        pgo->OnHit(hit_params);
                        return;
                    }
                }
            }
        }
        if (ptc_cmm->IsModeActive(0x51)) return;
    }
    bool hit_enemy = false;
    CThing* phitting_thing = hit_params.GetPThingHitting();
    if (IS_THING_ALIVE(phitting_thing) && (phitting_thing->GetTypeFlags() & 8)) {
        const unsigned long TIME_TO_LEAVE_SHOUT_MESSAGE = CMainGameComponent::GetConstantFPS() * 5;
        const float ON_HIT_SHOUT_RADIUS = 5.0f;
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_YELL_I_SEE_HIM, TIME_TO_LEAVE_SHOUT_MESSAGE, GetPos(), this, phitting_thing, 0.0f, ON_HIT_SHOUT_RADIUS, 0);
        hit_enemy = CTCEnemy::AreThingsEnemies(*this, *phitting_thing);
        CThingCreatureBase* phitting_creature = static_cast<CThingCreatureBase*>(phitting_thing);
        bool am_i_a_dying_troll = false;
        CDefPointer<const CCreatureDef> pcreature_def(NULL_DEF);
        if (PeekBaseDef().GetSubDef(&pcreature_def))
            am_i_a_dying_troll = GetHealth() <= 0.0f && ((pcreature_def->CreatureGroup & 0x10000) || (pcreature_def->CreatureGroup & 0x2000000) || (pcreature_def->CreatureGroup & 0x8000));
        const CThingCreatureBase* plast_attacker = PeekPLastAttackedByCreature();
        if ((IS_THING_DEAD(plast_attacker) || IsNewAttackerGreaterThreat(*plast_attacker,*phitting_creature)) && !am_i_a_dying_troll) {
            SetAttackedByCreature(phitting_creature);
            bool extra_data_relating_to_hits_killing = false;
            (void)extra_data_relating_to_hits_killing;
            CTCHeroOpinionDeedLog* ptc_deed_log;
            if (phitting_creature->GetTC(TCI_HERO_OPINION_DEED_LOG,&ptc_deed_log)) {
                const CTCEnemy* pmy_enemy_tc;
                if (PeekTC(TCI_ENEMY,&pmy_enemy_tc) && pmy_enemy_tc->GetPFaction()) {
                    CTCCreatureOpinionOfHero* ptc_opinion;
                    if (GetTC(TCI_CREATURE_OPINION_OF_HERO,&ptc_opinion)) {
                        EOpinionDeedType deed = hit_params.GetPWeapon() != phitting_thing ? OPINION_DEED_TYPE_CRIME_GBH : OPINION_DEED_TYPE_CRIME_ASSAULT;
                        bool is_crime = ptc_opinion->OnHit(deed == OPINION_DEED_TYPE_CRIME_GBH);
                        long old_crime = ptc_deed_log->FindCriminalDeed(deed,this);
                        if (old_crime == -1) {
                            long crime_handle = ptc_deed_log->PostDeed(deed,PeekPos(),this,this,OPINION_POST_DEED_TYPE_RECIPIENT,is_crime,false,pmy_enemy_tc->GetPFaction(),false,0);
                            if (ptc_deed_log->GetDeedFromHandle(crime_handle))
                                ptc_deed_log->PostDeed(OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH,PeekPos(),this,this,OPINION_POST_DEED_TYPE_WITNESSES,false,false,pmy_enemy_tc->GetPFaction(),false,is_crime ? crime_handle : -1);
                        } else {
                            long non_crime_handle = ptc_deed_log->PostDeed(deed,PeekPos(),this,this,OPINION_POST_DEED_TYPE_RECIPIENT,false,false,pmy_enemy_tc->GetPFaction(),false,0);
                            if (ptc_deed_log->GetDeedFromHandle(non_crime_handle))
                                ptc_deed_log->PostDeed(OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH,PeekPos(),this,this,OPINION_POST_DEED_TYPE_WITNESSES,false,false,pmy_enemy_tc->GetPFaction(),false,old_crime);
                        }
                    }
                }
            }
        }
    }
    CThingCreatureBase::OnHit(hit_params);
    CTCEntityEvents* ptc_entity_events;
    if (IS_THING_ALIVE(phitting_thing) && (phitting_thing->GetTypeFlags() & 8) && phitting_thing->GetTC(TCI_ENTITY_EVENTS,&ptc_entity_events)) {
        bool decapitate = hit_params.IsDecapitate();
        bool used_melee_weapon = CTCCarrying::IsThingCarryingMeleeWeapon(*phitting_thing);
        bool used_ranged_weapon = CTCCarrying::IsThingCarryingRangedWeapon(*phitting_thing);
        ptc_entity_events->AddEvent(new NEntityEvents::CEventHitThing(*phitting_thing,*this,hit_enemy,used_melee_weapon,used_ranged_weapon,hit_params.GetPWeapon(),hit_params.IsFlourish(),decapitate));
    }
}