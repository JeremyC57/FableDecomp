// 0068ef50 CInputProcessUseRangedWeapon::ProcessRangedAttackInput
// Leaf: ?ProcessRangedAttackInput@CInputProcessUseRangedWeapon@@AAEXABVCInputEvent@@AAVCProcessedInput@@ABVCThing@@@Z
// Flags: /GS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_RANGED_ATTACK_INPUT_PASS29
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_COMBAT_INPUT_PASS26
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CPROCESSEDINPUT_ADDEVENT
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_BLOCKING
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CTCSPECIALABILITIES
#define FABLE_RETAIL_PLAYER_ISSPELLMODE

#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CGameEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/EGameEventType.h"
#include "engine/CInputProcessUseRangedWeapon.h"
#include "engine/ThingAlive.h"

void CInputProcessUseRangedWeapon::ProcessRangedAttackInput(const CInputEvent& event, CProcessedInput& processed_input, const CThing& weapon)
{
    const CTCCombat* ptc_combat = 0;
    CThing* phero = MyPlayer->GetPControlledCreature();
    if (IS_THING_ALIVE(phero))
        phero->PeekTC(&ptc_combat);

    if (IsEventGameAction(GAME_ACTION_FIRE_RANGED_WEAPON, event)) {
        CTCHeroRangedAttacks* ptc_ranged_attacks = 0;
        phero->GetTC(&ptc_ranged_attacks);
        const CTCSpecialAbilities* ptc_special_abilities;
        phero->PeekTC(&ptc_special_abilities);
        if (!ptc_combat->IsBlocking() && !MyPlayer->IsSpellMode() && !ptc_special_abilities->GetAbilityMutexLock()) {
            if (IsEventButtonHeld(event) || IsEventButtonPressed(event)) {
                if (IsEventButtonPressed(event))
                    ptc_ranged_attacks->AttemptToLoadRangedWeapon();
                float button_pressure = GetButtonPressureForEvent(event);
                ptc_ranged_attacks->SetButtonPressure(button_pressure);
                ptc_ranged_attacks->ContinueAimingRangedWeapon();
            } else {
                ptc_ranged_attacks->AttemptToFireRangedWeapon();
                processed_input.SetAsPerformedEvent();
            }
        } else {
            processed_input.SetAsPerformedEvent();
        }
    } else if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(GAME_ACTION_SHEATHE_RANGED_WEAPON, event)) {
            processed_input.AddGameEvent(CGameEvent(GAME_EVENT_CONTROLLED_CREATURE_SHEATHE_WEAPON));
        } else if (IsEventGameAction(GAME_ACTION_UNSHEATHE_MELEE_WEAPON, event)) {
            CGameEvent unsheathe_event(GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_MELEE_WEAPON);
            unsheathe_event.AddUBYTE(true);
            processed_input.AddGameEvent(unsheathe_event);
        }
    }
}