// CInputProcessUseEnvironment::ProcessInput() -- dispatch the requested carried, talk or world interaction.
// Grade: DIFFER(1617v1471) -- The shared input, event and component interfaces preserve the native SpellMode and UseInfoDisplay gates. The compiler still differs in component lookups, event lifetimes and the protected input-body exits.
// Leaf: ?ProcessInput@CInputProcessUseEnvironment@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: 0x0068DD00, 1471 bytes, thiscall.
// Flags: /GS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISSPELLMODE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MAP_TABLE_QUERY
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_CINPUTPROCESSUSEENVIRONMENT_PASS24
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CGAMEEVENT_INLINE_ADDMORE
#define FABLE_RETAIL_CPROCESSEDINPUT_ADDEVENT
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS3
#define FABLE_RETAIL_CTCACTIONUSEBASE_TUTORIAL_PASS24
#define FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_ISUSABLE
#define FABLE_RETAIL_CTCACTIONUSEREADABLE_PASS24
#define FABLE_RETAIL_CREADABLEDEF_USEINFO_PASS24
#define FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE
#define FABLE_RETAIL_CTCHERO_TUTORIAL
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_NTARGETING_MELEE
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CInputEvent.h"
#include "engine/EGameAction.h"
#include "engine/CInputProcessUseEnvironment.h"
#include "engine/CGameEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCActionUseBase.h"
#include "engine/CTCActionUseReadable.h"
#include "engine/CTCActionUseScriptedHook.h"
#include "engine/ETargetingType.h"
#include "engine/CTCTargetingBase.h"
#include "engine/NTargeting.h"
#include "engine/CTCHero.h"
#include "engine/CTCSoundPlayer.h"

void CInputProcessUseEnvironment::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input)
{
    if (MyPlayer->IsSpellMode())
        return;
    CThingPlayerCreature* pplayer_creature = MyPlayer->GetPPlayerCharacter();
    if (!IsEventGameAction(GAME_ACTION_INTERACT, event))
        return;
    unsigned long activation_flags = IsEventButtonPressed(event) ? 1 : 0;
    activation_flags |= IsEventButtonReleased(event) ? 2 : 0;
    activation_flags |= IsEventButtonHeld(event) ? 4 : 0;
    if (!activation_flags)
        return;

    const CTCCarrying* ptc_carrying;
    pplayer_creature->PeekTC(&ptc_carrying);
    const CThing* pcarried_thing = ptc_carrying->GetThingInPrimarySlot();
    if (pcarried_thing && !pcarried_thing->IsDead() && pcarried_thing->HasTC(TCI_CARRIED_ON_ACTION_USE)) {
        const CTCActionUseBase* ptc_use;
        if (!pcarried_thing->PeekTC(&ptc_use) || !(activation_flags & ptc_use->GetWhenToActivate()) || !ptc_use->IsUsable())
            return;
        CGameEvent action(0x11);
        action.AddUHUGE(pplayer_creature->GetUniqueID());
        action.AddUHUGE(pcarried_thing->GetUniqueID());
        processed_input.AddGameEvent(action);
        return;
    }
    const CTCTargetingBase* ptc_targeting;
    pplayer_creature->PeekTC(&ptc_targeting);
    CThing* ptarget = ptc_targeting->GetPTargetedThing();
    if (ptarget && !ptarget->IsDead() && NTargeting::IsThingVulnerableToMeleeStrikesFromThing(*ptarget, *pplayer_creature))
        return;
    const CThing* ptalk_thing;
    if (ptc_targeting->IsTargetedThingOfType(TARGETING_TALKABLE) && ptc_targeting->IsTargetedThingInRange(TARGETING_TALKABLE) &&
        (ptalk_thing = ptc_targeting->GetPTargetedThing()) != 0) {
        if (!(activation_flags & 1))
            return;
        if (ptalk_thing->HasTC(TCI_EXPRESSION)) {
            CGameEvent action(0x13);
            processed_input.AddGameEvent(action);
            return;
        }
        CGameEvent action(0x23);
        action.AddUBYTE(0);
        processed_input.AddGameEvent(action);
        return;
    }
    if (!ptc_targeting->IsTargetedThingOfType(TARGETING_USABLE) || !ptc_targeting->IsTargetedThingInRange(TARGETING_USABLE))
        return;
    CThing* puse_thing = ptc_targeting->GetPTargetedThing();
    if (!puse_thing || puse_thing->IsDead())
        return;
    bool use = false;
    const CTCActionUseBase* ptc_use;
    if (puse_thing->PeekTC(&ptc_use) && ptc_use->IsUsable()) {
        if (ptc_use->GetTutorialCategory()) {
            CThingPlayerCreature* phero = World->PeekMyPlayer()->GetPControlledCreature();
            if (IS_THING_ALIVE(phero)) {
                CTCHero* ptc_hero;
                if (phero->GetTC(TCI_HERO, &ptc_hero) && ptc_hero && ptc_hero->GiveTutorialForCategory(ptc_use->GetTutorialCategory(), false)) {
                    processed_input.SetAsPerformedEvent();
                    return;
                }
            }
        }
        use = (activation_flags & ptc_use->GetWhenToActivate()) != 0;
    }
    const CTCActionUseScriptedHook* ptc_scripted_hook;
    if (((activation_flags & 1) && puse_thing->PeekTC(&ptc_scripted_hook) && ptc_scripted_hook->IsUsable()) || use) {
        CTCActionUseBase* action_use;
        if (puse_thing->GetTC(&action_use) && action_use->GetClassTypeName() == CCharString("CTCActionUseReadable")) {
            CTCActionUseReadable* ptc_readable = static_cast<CTCActionUseReadable*>(action_use);
            if (ptc_readable->UsesInfoDisplay())
                goto locked;
        }
        if (!(puse_thing->HasTC(TCI_HERO_GUIDE) && !World->IsMapTableToShowQuestCardsWhenUsed())) {
            CGameEvent action(0x11);
            action.AddUHUGE(pplayer_creature->GetUniqueID());
            action.AddUHUGE(puse_thing->GetUniqueID());
            processed_input.AddGameEvent(action);
            return;
        }
    }
locked:
    CTCSoundPlayer* ptc_sound_player;
    if (puse_thing->HasTC(TCI_DOOR) && (activation_flags & 1) && puse_thing->GetTC(&ptc_sound_player)) {
        ptc_sound_player->PlayAttachedCriteria("DOOR_OPEN_FAIL", false, false, false, -1);
        processed_input.SetAsPerformedEvent();
    }
}
