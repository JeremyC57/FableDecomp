// 0068f470 CInputProcessCombat::ProcessCombatInput
// Leaf: ?ProcessCombatInput@CInputProcessCombat@@AAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Flags: /GS
#define FABLE_RETAIL_COMBAT_INPUT_PASS26
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISSPELLMODE
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_WEAPONTRAIL
#define FABLE_RETAIL_CTCCOMBAT_ISMELEEBLOCKCOUNTERVALID
#define FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CPROCESSEDINPUT_ADDEVENT
#include "stlport/fable_containers.h"
#include "engine/EGameAction.h"
#include "engine/ETargetingType.h"
#include "engine/EInputButtonEventType.h"
#include "engine/EGameEventType.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCTargeted.h"
#include "engine/ThingAlive.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGameEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CInputEvent.h"
#include "engine/CInputProcessCombat.h"

void CInputProcessCombat::ProcessCombatInput(const CInputEvent& event,CProcessedInput& processed_input)
{
    if(MyPlayer->IsSpellMode())
        return;
    const CThingPlayerCreature* phero=MyPlayer->GetPControlledCreature();
    const CTCCombat* ptc_combat=0;
    phero->PeekTC(&ptc_combat);
    const CTCTargetingBase* ptc_targ=0;
    phero->PeekTC(&ptc_targ);
    CThing* pbest_thing_for_attack=ptc_targ->GetPBestThingForMeleeAttack();
    bool allow_melee_attacks=true;
    const CTCCarrying* ptc_carrying=0;
    if(phero->PeekTC(&ptc_carrying)) {
        const CThing* pcarried_thing=ptc_carrying->GetThingInPrimarySlot();
        const CTCWeapon* ptc_weapon=0;
        if(IS_THING_ALIVE(pcarried_thing) && pcarried_thing->PeekTC(&ptc_weapon) && ptc_weapon->IsProjectileWeapon()) {
            allow_melee_attacks=false;
            if(IS_THING_ALIVE(pbest_thing_for_attack)) {
                const CTCTargeted* ptc_targeted=0;
                if(pbest_thing_for_attack->PeekTC(&ptc_targeted) && (ptc_targeted->GetTargetingFlags() & TARGETING_STAB))
                    allow_melee_attacks=true;
            }
        }
    }
    std::vector<EGameAction> gas;
    gas.push_back(GAME_ACTION_INTERACT);
    gas.push_back(GAME_ACTION_ATTACK);
    bool interact_and_attack_on_same_button=GetGamePlayerInterface().IsControlInputSharedBetweenGameActions(gas);
    if(interact_and_attack_on_same_button) {
        CThing* ptargeted_thing=ptc_targ->GetPTargetedThing();
        if(!(ptc_targ->IsTargetedThingOfType(TARGETING_MELEE) && ptc_targ->IsTargetedThingInRange(TARGETING_MELEE))) {
            if((ptc_targ->IsTargetedThingOfType(TARGETING_TALKABLE) && ptc_targ->IsTargetedThingInRange(TARGETING_TALKABLE)) ||
               (ptc_targ->IsTargetedThingOfType(TARGETING_USABLE) && ptc_targ->IsTargetedThingInRange(TARGETING_USABLE)))
                allow_melee_attacks=false;
        }
    }
    if(IsEventButtonPressed(event)) {
        if(allow_melee_attacks) {
            bool flourish_prerequisite=true;
            if(GetGamePlayerInterface().IsRequireAttackButtonHeldForFlourish() &&
               !IsGameActionEventInQueue(GAME_ACTION_ATTACK,INPUT_BUTTON_TYPE_HELD))
                flourish_prerequisite=false;
            if(flourish_prerequisite && IsEventGameAction(GAME_ACTION_SPECIAL_ATTACK,event) && ptc_combat->IsMeleeFlourishValid()) {
                CGameEvent action(GAME_EVENT_CONTROLLED_CREATURE_USE_ABILITY);
                action.AddUWORD((unsigned short)DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_FLOURISH"));
                float pressure=GetButtonPressureForEvent(event);
                action.AddFloat(pressure);
                processed_input.AddGameEvent(action);
            } else if(IsEventGameAction(GAME_ACTION_SPECIAL_ATTACK,event) && ptc_combat->IsMeleeBlockCounterValid()) {
                CGameEvent action(GAME_EVENT_CONTROLLED_CREATURE_USE_ABILITY);
                action.AddUWORD((unsigned short)DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_BREAK_BLOCK"));
                float pressure=GetButtonPressureForEvent(event);
                action.AddFloat(pressure);
                processed_input.AddGameEvent(action);
            } else if(IsEventGameAction(GAME_ACTION_ATTACK,event)) {
                CGameEvent action(GAME_EVENT_CONTROLLED_CREATURE_USE_ABILITY);
                action.AddUWORD((unsigned short)DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_ATTACK"));
                float pressure=GetButtonPressureForEvent(event);
                action.AddFloat(pressure);
                processed_input.AddGameEvent(action);
            }
        }
        if(IsEventGameAction(GAME_ACTION_UNSHEATHE_RANGED_WEAPON,event)) {
            CGameEvent unsheathe_event(GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_RANGED_WEAPON);
            unsheathe_event.AddUBYTE(true);
            processed_input.AddGameEvent(unsheathe_event);
        } else if(IsEventGameAction(GAME_ACTION_UNSHEATHE_MELEE_WEAPON,event)) {
            CGameEvent unsheathe_event(GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_MELEE_WEAPON);
            unsheathe_event.AddUBYTE(true);
            processed_input.AddGameEvent(unsheathe_event);
        } else if(IsEventGameAction(GAME_ACTION_SHEATHE_MELEE_WEAPON,event) || IsEventGameAction(GAME_ACTION_SHEATHE_RANGED_WEAPON,event)) {
            processed_input.AddGameEvent(CGameEvent(GAME_EVENT_CONTROLLED_CREATURE_SHEATHE_WEAPON));
        }
    }
}