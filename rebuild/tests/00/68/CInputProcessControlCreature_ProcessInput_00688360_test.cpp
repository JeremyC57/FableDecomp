#include <stdio.h>
#define FABLE_RETAIL_CONTROL_CREATURE_INPUT_PASS27
#include "engine/EGameEventType.h"
#include "engine/EGameAction.h"
#include "engine/EPlayerMode.h"
#include "engine/ETCInterfaceType.h"
struct CInputEvent{unsigned buttons;bool actions[GAME_ACTION_COUNT];};
struct CGameEvent{
 long type;unsigned long number;unsigned char byte;int writes;
 CGameEvent(long n):type(n),number(0),byte(0),writes(0){}
 void AddULONG(unsigned long v){number=v;++writes;}
 void AddUBYTE(unsigned char v){byte=v;++writes;}
};
struct CProcessedInput{int events,performed;CGameEvent value;
 CProcessedInput():events(0),performed(0),value(-1){}
 void AddGameEvent(const CGameEvent& e){++events;value=e;}void SetAsPerformedEvent(){++performed;}};
struct CTCInventory{int shortcut;void SetScreenToShortcut(long n){shortcut=n;}};
struct CThingPlayerCreature{
 bool menu,inventory;CTCInventory inv;
 bool HasTC(ETCInterfaceType k)const{return k==TCI_IN_GAME_MENU&&menu;}
 bool GetTC(CTCInventory** ptr){if(inventory)*ptr=&inv;return inventory;}
};
struct CPlayerGui{bool live;unsigned toggles,maps;static CPlayerGui& Get(){static CPlayerGui g;return g;}
 bool IsLiveGUIMode()const{return live;}void ToggleLiveGui(){live=!live;++toggles;}void ToggleMiniMapCentering(){++maps;}};
struct CPlayer{CThingPlayerCreature hero;bool spell,shift;CThingPlayerCreature* GetPControlledCreature(){return &hero;}
 bool IsSpellMode()const{return spell;}void SetSpellMode(bool b){spell=b;}
 bool IsExpressionShiftMode()const{return shift;}void SetExpressionShiftMode(bool b){shift=b;}};
struct CGamePlayerInterface{bool toggleSpell,toggleShift;
 bool IsToggleSpells()const{return toggleSpell;}bool IsToggleExpressionShift()const{return toggleShift;}};
struct CInputProcessControlCreature{
 CPlayer* MyPlayer;CGamePlayerInterface input;
 bool IsEventButtonPressed(const CInputEvent& event)const{return (event.buttons&1)!=0;}
 bool IsEventButtonReleased(const CInputEvent& event)const{return (event.buttons&2)!=0;}
 bool IsEventButtonHeld(const CInputEvent& event)const{return (event.buttons&4)!=0;}
 bool IsEventGameAction(EGameAction a,const CInputEvent& event)const{return event.actions[a];}
 const CGamePlayerInterface& GetGamePlayerInterface()const{return input;}
 void ProcessInput(const CInputEvent&,CProcessedInput&);
};
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

int main(){
 const EGameAction slots[]={GAME_ACTION_OPEN_WEAPONS_MENU,GAME_ACTION_OPEN_CLOTHING_MENU,GAME_ACTION_OPEN_ITEMS_MENU,
  GAME_ACTION_OPEN_CURRENT_QUESTS_MENU,GAME_ACTION_OPEN_MAP_MENU,GAME_ACTION_OPEN_EXPRESSIONS_MENU,
  GAME_ACTION_OPEN_PERSONALITY_MENU,GAME_ACTION_OPEN_LOGBOOK_MENU,GAME_ACTION_OPEN_MAGIC_MENU};
 for(unsigned buttons=0;buttons<8;++buttons)for(unsigned id=0;id<GAME_ACTION_COUNT+2;++id)for(unsigned flags=0;flags<128;++flags){
  CInputEvent e;e.buttons=buttons;
  for(unsigned i=0;i<GAME_ACTION_COUNT;++i)e.actions[i]=(id==i);
  if(id==GAME_ACTION_COUNT){e.actions[GAME_ACTION_ACTIVATE_SPELL_MODE]=true;e.actions[GAME_ACTION_EXPRESSION_SHIFT]=true;}
  if(id==GAME_ACTION_COUNT+1){e.actions[GAME_ACTION_OPEN_ITEMS_MENU]=true;e.actions[GAME_ACTION_OPEN_MAP_MENU]=true;}
  CInputProcessControlCreature ctl;CPlayer player;ctl.MyPlayer=&player;CPlayerGui& gui=CPlayerGui::Get();
  bool spell=player.spell=(flags&1)!=0,shift=player.shift=(flags&2)!=0;
  ctl.input.toggleSpell=(flags&4)!=0;ctl.input.toggleShift=(flags&8)!=0;
  bool live=gui.live=(flags&16)!=0;player.hero.menu=(flags&32)!=0;player.hero.inventory=(flags&64)!=0;
  gui.toggles=gui.maps=0;player.hero.inv.shortcut=97;
  CProcessedInput out;ctl.ProcessInput(e,out);
  long type=-1;int performed=0,slot=97,toggles=0,maps=0;
  if(buttons&1){
   if(e.actions[GAME_ACTION_OPEN_IN_GAME_MENU]&&player.hero.menu)type=GAME_EVENT_OPEN_HERO_INFO_SCREEN;
   if(e.actions[GAME_ACTION_TOGGLE_LIVE_GUI]){toggles=performed=1;if(!live&&player.hero.inventory)slot=-1;}
   if(e.actions[GAME_ACTION_TOGGLE_MINI_MAP])maps=performed=1;
   for(unsigned k=0;k<9;++k)if(e.actions[slots[k]]&&!live){performed=toggles=1;if(player.hero.inventory)slot=k;break;}
   bool setSpell=e.actions[GAME_ACTION_ACTIVATE_SPELL_MODE]&&!ctl.input.toggleSpell;
   bool setShift=e.actions[GAME_ACTION_EXPRESSION_SHIFT]&&!ctl.input.toggleShift;
   spell=spell||setSpell;shift=shift||setShift;if(setSpell||setShift)++performed;
  }else if(buttons&2){
   if(e.actions[GAME_ACTION_TOGGLE_PASSIVE_AGGRESSIVE_MODE])type=GAME_EVENT_TOGGLE_PASSIVE_AGGRESSIVE_MODE;
   else if(e.actions[GAME_ACTION_TOGGLE_KILL_EVERYTHING_MODE])type=GAME_EVENT_TOGGLE_KILL_EVERYTHING_MODE;
   else{
    bool changeSpell=e.actions[GAME_ACTION_ACTIVATE_SPELL_MODE]&&ctl.input.toggleSpell;
    bool changeShift=e.actions[GAME_ACTION_EXPRESSION_SHIFT]&&ctl.input.toggleShift;
    if(changeSpell)spell=!spell;
    if(changeShift)shift=shift; // The two toggles in this release preserve the entering state.
    performed=changeSpell||changeShift;
   }
  }else if((buttons&4)&&e.actions[GAME_ACTION_CENTRE_CAMERA])type=GAME_EVENT_CENTRE_CAMERA;
  if(out.performed!=performed||gui.toggles!=unsigned(toggles)||gui.maps!=unsigned(maps)||gui.live!=(live!=(toggles!=0))||
     player.hero.inv.shortcut!=slot||player.spell!=spell||player.shift!=shift||out.events!=int(type!=-1)||out.value.type!=type)return 1;
  if(type==GAME_EVENT_OPEN_HERO_INFO_SCREEN){
   if(out.value.writes!=2||out.value.number!=PLAYER_MODE_NAVIGATE_IN_GAME_MENU||out.value.byte!=1)return 2;
  }else if(out.value.writes)return 3;
 }
 puts("00688360_TEST PASS");return 0;
}