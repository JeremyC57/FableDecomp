// 00688360 CInputProcessControlCreature::ProcessInput
// Leaf: ?ProcessInput@CInputProcessControlCreature@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Flags: /GS
#define FABLE_RETAIL_CONTROL_CREATURE_INPUT_PASS27
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISSPELLMODE
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CGAMEEVENT_INLINE_ADDULONG
#define FABLE_RETAIL_CPROCESSEDINPUT_ADDEVENT
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_LIVEGUI
#define FABLE_RETAIL_CPLAYERGUI_TOGGLE_LIVE_PASS26
#define FABLE_RETAIL_STLP_MAP
#include "engine/EPlayerMode.h"
#include "engine/EGameAction.h"
#include "engine/EGameEventType.h"
#include "engine/CInputEvent.h"
#include "engine/CGameEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CInputProcessControlCreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CTCInventory.h"
#include "engine/CPlayerGui.h"

void CInputProcessControlCreature::ProcessInput(const CInputEvent& event,CProcessedInput& processed_input)
{
    CThingPlayerCreature* player_creature=MyPlayer->GetPControlledCreature();
    if(IsEventButtonPressed(event)){
        if(IsEventGameAction(GAME_ACTION_OPEN_IN_GAME_MENU,event)){
            if(player_creature->HasTC(TCI_IN_GAME_MENU)){
                CGameEvent action(GAME_EVENT_OPEN_HERO_INFO_SCREEN);
                action.AddULONG(PLAYER_MODE_NAVIGATE_IN_GAME_MENU);
                action.AddUBYTE(true);
                processed_input.AddGameEvent(action);
            }
        }else if(IsEventGameAction(GAME_ACTION_TOGGLE_LIVE_GUI,event)){
            if(!CPlayerGui::Get().IsLiveGUIMode()){
                CTCInventory* ptc_inventory;
                if(player_creature->GetTC(&ptc_inventory))ptc_inventory->SetScreenToShortcut(-1);
            }
            CPlayerGui::Get().ToggleLiveGui();
            processed_input.SetAsPerformedEvent();
        }else if(IsEventGameAction(GAME_ACTION_TOGGLE_MINI_MAP,event)){
            CPlayerGui::Get().ToggleMiniMapCentering();
            processed_input.SetAsPerformedEvent();
        }else if(IsEventGameAction(GAME_ACTION_OPEN_WEAPONS_MENU,event)){
            if(!CPlayerGui::Get().IsLiveGUIMode()){
                CTCInventory* ptc_inventory;
                if(player_creature->GetTC(&ptc_inventory))ptc_inventory->SetScreenToShortcut(0);
                CPlayerGui::Get().ToggleLiveGui();
                processed_input.SetAsPerformedEvent();
            }
        }else if(IsEventGameAction(GAME_ACTION_OPEN_CLOTHING_MENU,event)){
            if(!CPlayerGui::Get().IsLiveGUIMode()){
                CTCInventory* ptc_inventory;
                if(player_creature->GetTC(&ptc_inventory))ptc_inventory->SetScreenToShortcut(1);
                CPlayerGui::Get().ToggleLiveGui();
                processed_input.SetAsPerformedEvent();
            }
        }else if(IsEventGameAction(GAME_ACTION_OPEN_ITEMS_MENU,event)){
            if(!CPlayerGui::Get().IsLiveGUIMode()){
                CTCInventory* ptc_inventory;
                if(player_creature->GetTC(&ptc_inventory))ptc_inventory->SetScreenToShortcut(2);
                CPlayerGui::Get().ToggleLiveGui();
                processed_input.SetAsPerformedEvent();
            }
        }else if(IsEventGameAction(GAME_ACTION_OPEN_CURRENT_QUESTS_MENU,event)){
            if(!CPlayerGui::Get().IsLiveGUIMode()){
                CTCInventory* ptc_inventory;
                if(player_creature->GetTC(&ptc_inventory))ptc_inventory->SetScreenToShortcut(3);
                CPlayerGui::Get().ToggleLiveGui();
                processed_input.SetAsPerformedEvent();
            }
        }else if(IsEventGameAction(GAME_ACTION_OPEN_MAP_MENU,event)){
            if(!CPlayerGui::Get().IsLiveGUIMode()){
                CTCInventory* ptc_inventory;
                if(player_creature->GetTC(&ptc_inventory))ptc_inventory->SetScreenToShortcut(4);
                CPlayerGui::Get().ToggleLiveGui();
                processed_input.SetAsPerformedEvent();
            }
        }else if(IsEventGameAction(GAME_ACTION_OPEN_EXPRESSIONS_MENU,event)){
            if(!CPlayerGui::Get().IsLiveGUIMode()){
                CTCInventory* ptc_inventory;
                if(player_creature->GetTC(&ptc_inventory))ptc_inventory->SetScreenToShortcut(5);
                CPlayerGui::Get().ToggleLiveGui();
                processed_input.SetAsPerformedEvent();
            }
        }else if(IsEventGameAction(GAME_ACTION_OPEN_PERSONALITY_MENU,event)){
            if(!CPlayerGui::Get().IsLiveGUIMode()){
                CTCInventory* ptc_inventory;
                if(player_creature->GetTC(&ptc_inventory))ptc_inventory->SetScreenToShortcut(6);
                CPlayerGui::Get().ToggleLiveGui();
                processed_input.SetAsPerformedEvent();
            }
        }else if(IsEventGameAction(GAME_ACTION_OPEN_LOGBOOK_MENU,event)){
            if(!CPlayerGui::Get().IsLiveGUIMode()){
                CTCInventory* ptc_inventory;
                if(player_creature->GetTC(&ptc_inventory))ptc_inventory->SetScreenToShortcut(7);
                CPlayerGui::Get().ToggleLiveGui();
                processed_input.SetAsPerformedEvent();
            }
        }else if(IsEventGameAction(GAME_ACTION_OPEN_MAGIC_MENU,event)){
            if(!CPlayerGui::Get().IsLiveGUIMode()){
                CTCInventory* ptc_inventory;
                if(player_creature->GetTC(&ptc_inventory))ptc_inventory->SetScreenToShortcut(8);
                CPlayerGui::Get().ToggleLiveGui();
                processed_input.SetAsPerformedEvent();
            }
        }
        bool got_event=false;
        if(IsEventGameAction(GAME_ACTION_ACTIVATE_SPELL_MODE,event) && !GetGamePlayerInterface().IsToggleSpells()){
            MyPlayer->SetSpellMode(true);got_event=true;
        }
        if(IsEventGameAction(GAME_ACTION_EXPRESSION_SHIFT,event) && !GetGamePlayerInterface().IsToggleExpressionShift()){
            MyPlayer->SetExpressionShiftMode(true);got_event=true;
        }
        if(got_event)processed_input.SetAsPerformedEvent();
    }else if(IsEventButtonReleased(event)){
        if(IsEventGameAction(GAME_ACTION_TOGGLE_PASSIVE_AGGRESSIVE_MODE,event)){
            processed_input.AddGameEvent(CGameEvent(GAME_EVENT_TOGGLE_PASSIVE_AGGRESSIVE_MODE));
        }else if(IsEventGameAction(GAME_ACTION_TOGGLE_KILL_EVERYTHING_MODE,event)){
            processed_input.AddGameEvent(CGameEvent(GAME_EVENT_TOGGLE_KILL_EVERYTHING_MODE));
        }else{
            bool got_shifty_event=false;
            if(GetGamePlayerInterface().IsToggleSpells() && IsEventGameAction(GAME_ACTION_ACTIVATE_SPELL_MODE,event)){
                got_shifty_event=true;
                if(!MyPlayer->IsSpellMode())MyPlayer->SetSpellMode(true);
                else if(GetGamePlayerInterface().IsToggleSpells())MyPlayer->SetSpellMode(false);
            }
            if(GetGamePlayerInterface().IsToggleExpressionShift() && IsEventGameAction(GAME_ACTION_EXPRESSION_SHIFT,event)){
                got_shifty_event=true;
                MyPlayer->SetExpressionShiftMode(!MyPlayer->IsExpressionShiftMode());
                if(!MyPlayer->IsExpressionShiftMode())MyPlayer->SetExpressionShiftMode(true);
                else if(GetGamePlayerInterface().IsToggleExpressionShift())MyPlayer->SetExpressionShiftMode(false);
            }
            if(got_shifty_event)processed_input.SetAsPerformedEvent();
        }
    }else if(IsEventButtonHeld(event)){
        if(IsEventGameAction(GAME_ACTION_CENTRE_CAMERA,event))
            processed_input.AddGameEvent(CGameEvent(GAME_EVENT_CENTRE_CAMERA));
    }
}