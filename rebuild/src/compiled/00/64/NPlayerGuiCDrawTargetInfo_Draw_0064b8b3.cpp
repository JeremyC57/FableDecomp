// 0064B8B3 NPlayerGui::CDrawTargetInfo::Draw
// Leaf: ?Draw@CDrawTargetInfo@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /O1 /Oy-
// Inline-budget: retail folds HasTC into Draw, while the independent HasTC and bitset::test bodies at 004118C0 and 00410DE0 use speed optimization.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_FORCEINLINE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_VSLOTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_SPEED_OWNER_PASS27
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYER_ISTARGETLOCKED
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_COMPONENT_QUERY_PASS27
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES
#define FABLE_RETAIL_TARGET_INFO_PASS30
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD_MAP_TABLE_QUERY
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ENGINEGRAPHIC
#define FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISOPENING_PASS26
#define FABLE_RETAIL_CTCTROPHY_ISMINIGAMEINPROGRESS
#define FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_ISUSABLE
#define FABLE_RETAIL_NTARGETING_MELEE
#define FABLE_RETAIL_CPLAYERGUI_GETINPUTINTERFACE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CTCHERORANGEDATTACKS
#define FABLE_RETAIL_CTCTROPHY
#define FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHING_HASTC_SPEED_OWNER_PASS27
#define FABLE_RETAIL_STLP_THING_BITSET_SPEED_OWNER_PASS26
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#ifdef _MSC_VER
#pragma optimize("s", off)
#pragma optimize("ty", on)
#endif
#include "stlport/fable_thing_bitset.h"
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
#include "engine/CDefString.h"
#include "engine/EGameAction.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCEnemy.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCTrophy.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCActionUseBase.h"
#include "engine/CTCActionUseScriptedHook.h"
#include "engine/NTargeting.h"
#include "engine/CWorld.h"
#include "engine/NPlayerGui_CDrawTargetInfo.h"

void NPlayerGui::CDrawTargetInfo::Draw(const CEnginePrimitivePrepareInfo& info)
{
    AttackAlphaFade.Update(info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);
    BlockAlphaFade.Update(info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);
    const CThingPlayerCreature* phero = GetPlayer().GetPControlledCreature();
    if (IS_THING_ALIVE(phero)) {
        SetAsDrawing(true);
        for (long index = 0; index < CPlayerGui::PeekGuiDef().AbilityButtons.size(); ++index) {
            const CGamePlayerInterface& game_player_interface = PlayerGui.GetInputInterface();
            long attack_button = -1;
            long block_button = -1;
            long interact_button = -1;
            if (game_player_interface.IsButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].ControllerButton, GAME_ACTION_ATTACK) ||
                game_player_interface.IsMouseButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].MouseButton, GAME_ACTION_ATTACK))
                attack_button = index;
            if (game_player_interface.IsButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].ControllerButton, GAME_ACTION_BLOCK) ||
                game_player_interface.IsMouseButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].MouseButton, GAME_ACTION_BLOCK))
                block_button = index;
            if (game_player_interface.IsButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].ControllerButton, GAME_ACTION_INTERACT) ||
                game_player_interface.IsMouseButtonForGameEvent(CPlayerGui::PeekGuiDef().AbilityButtons[index].MouseButton, GAME_ACTION_INTERACT))
                interact_button = index;
            if (index == 0) interact_button = 1;
            if (interact_button > -1) PlayerGui.SetAbilityButton(interact_button, 0);
            CThing* ptarget = 0;
            if (PlayerGui.IsDrawingTargetingCentre()) {
                const CTCTargetingBase* ptc_targeting;
                phero->PeekTC(&ptc_targeting);
                ptarget = ptc_targeting->GetPTargetedThing();
            } else if (!phero->HasTC(TCI_BERSERK)) {
                const CTCTargetingBase* ptc_targeting;
                phero->PeekTC(&ptc_targeting);
                ptarget = ptc_targeting->GetPTargetedThing();
                if (interact_button > -1 && IS_THING_ALIVE(ptarget)) {
                    if (ptc_targeting->IsTargetedThingInRange(2) && !NTargeting::IsThingVulnerableToMeleeStrikesFromThing(*ptarget,*phero)) {
                        if (ptarget->GetDefName() == "CREATURE_DEMON_DOOR_FACE_01")
                            PlayerGui.SetAbilityButton(interact_button, CPlayerGui::PeekGuiDef().AbilityButtonActivateGraphic);
                        else if (ptarget->GetDefName() == "CREATURE_NW_DEMON_DOOR_FACE_01")
                            PlayerGui.SetAbilityButton(interact_button, CPlayerGui::PeekGuiDef().AbilityButtonActivateGraphic);
                        else {
                            bool carrying_trophy = false;
                            const CTCHeroRangedAttacks* ptc_ranged;
                            phero->PeekTC(&ptc_ranged);
                            bool in_process_of_firing = false;
                            if (ptc_ranged->IsInProcessOfFiringRangedWeapon()) in_process_of_firing = true;
                            const CTCCarrying* ptc_carrying;
                            phero->PeekTC(&ptc_carrying);
                            const CThing* pcarried_thing = ptc_carrying->GetThingInPrimarySlot();
                            if (IS_THING_ALIVE(pcarried_thing)) {
                                const CTCTrophy* ptc_trophy;
                                if (pcarried_thing->PeekTC(&ptc_trophy) && ptc_trophy->IsMiniGameInProgress()) {
                                    const CTCInventoryItem* ptc_inventory_item;
                                    pcarried_thing->PeekTC(&ptc_inventory_item);
                                    PlayerGui.SetAbilityButton(interact_button, ptc_inventory_item->GetEngineGraphic().BankIndex);
                                    carrying_trophy = true;
                                }
                            }
                            if (!carrying_trophy && !in_process_of_firing)
                                PlayerGui.SetAbilityButton(interact_button, CPlayerGui::PeekGuiDef().AbilityButtonTalkGraphic);
                        }
                    } else if (ptc_targeting->IsTargetedThingInRange(1)) {
                        bool targetable = true;
                        if (ptarget->HasTC(TCI_HERO_GUIDE) && !World.IsMapTableToShowQuestCardsWhenUsed()) targetable = false;
                        if (targetable) {
                            const CTCActionUseBase* ptc_action_use;
                            if (ptarget->PeekTC(&ptc_action_use)) {
                                if (!ptc_action_use->IsUsable()) targetable = false;
                            } else {
                                const CTCActionUseScriptedHook* ptc_action_use_hook = 0;
                                if (ptarget->PeekTC(&ptc_action_use_hook) && !ptc_action_use_hook->IsUsable()) targetable = false;
                            }
                        }
                        if (targetable) PlayerGui.SetAbilityButton(interact_button, CPlayerGui::PeekGuiDef().AbilityButtonActivateGraphic);
                    }
                }
            }
            if (attack_button > -1) {
                if (IS_THING_ALIVE(ptarget) && (CTCEnemy::AreThingsEnemies(*phero, *ptarget) || GetPlayer().IsTargetLocked()))
                    PlayerGui.SetButtonAlpha(attack_button, true, 1.0f);
                else if (IS_THING_ALIVE(ptarget) && ptarget->HasTC(TCI_SMASHABLE))
                    PlayerGui.SetButtonAlpha(attack_button, true, 1.0f);
                else if (AttackAlphaFade.IsOpening()) PlayerGui.SetButtonAlpha(attack_button, true, 1.0f);
                else if (!GetPlayer().IsAggressiveMode()) PlayerGui.SetButtonAlpha(attack_button, true, 1.0f);
                else PlayerGui.SetButtonAlpha(attack_button, true, 0.5f);
            }
            if (block_button > -1) {
                if (phero->IsBlocking()) PlayerGui.SetButtonAlpha(block_button, true, 1.0f);
                else if (phero->IsInMelee()) PlayerGui.SetButtonAlpha(block_button, true, 1.0f);
                else if (BlockAlphaFade.IsOpening()) PlayerGui.SetButtonAlpha(block_button, true, 1.0f);
                else PlayerGui.SetButtonAlpha(block_button, true, 0.5f);
            }
        }
    }
}