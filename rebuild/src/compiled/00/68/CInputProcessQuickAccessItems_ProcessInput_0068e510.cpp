// CInputProcessQuickAccessItems::ProcessInput -- charge or dispatch a quick-access item input.
// Leaf: ?ProcessInput@CInputProcessQuickAccessItems@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Flags: /GS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_QUICKACCESS_PASS26
#define FABLE_RETAIL_CINPUTPROCESSQUICKACCESSITEMS_UPDATE
#define FABLE_RETAIL_CINPUTPROCESSQUICKACCESSITEMS_PROCESS_PASS26
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_ISOPEN_PASS26
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CLOTHING_DEFS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_REGIONLOAD_ACCESSORS
#define FABLE_RETAIL_CWORLD_DEFINITION_REFERENCE_PASS23
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CGAMEEVENT_DEFAULT_CONSTRUCTOR_PASS26
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CGAMEEVENT_INLINE_ADDMORE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CInputProcessQuickAccessItems.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CTCInventory.h"
#include "engine/CInventoryItemDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CGameEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/ThingAlive.h"
#include "engine/CMainGameComponent.h"

// The native same-TU owner at 0068E3C0 stays out of line. FableWin 021E61E0 confirms
// the counted off state, original game-event members and the main component dispatch.
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CInputProcessQuickAccessItems::CancelQuickAccessCharge()
{
 const CThingPlayerCreature* phero=MyPlayer->PeekPControlledCreature();
 if (IS_THING_ALIVE(phero)) {
  CGameEvent game_event(0x29);
  game_event.SetPlayer(phero->GetPlayerNumber());
  game_event.AddSLONG(ItemIndex);
  game_event.AddUBYTE(0);
  CMainGameComponent::Get()->AddGameEvent(game_event);
 }
}

void CInputProcessQuickAccessItems::ProcessInput(const CInputEvent& event,CProcessedInput& processed_input)
{
 if(IsEventButtonPressed(event)) {
  long item_index=-1;
  bool charge_guild_seal=IsEventGameAction(GAME_ACTION_CHARGE_GUILD_SEAL,event);
  if(charge_guild_seal)item_index=-2;
  if(IsEventQuickAccessItemButton(event,item_index)||charge_guild_seal) {
   long button_index=item_index;
   const CThingPlayerCreature* phero=MyPlayer->PeekPControlledCreature();
   if(IS_THING_ALIVE(phero)&&!IsOtherQuickAccessItemButtonHeld(item_index)&&CPlayerGui::Get().IsOpen()) {
    if(!charge_guild_seal)item_index+=GetGamePlayerInterface().GetNumberOfContextSensitiveItemButtons();
    if(ItemIndex!=-1)CancelQuickAccessCharge();
    ItemIndex=-1;
    ButtonIndex=-1;
    const CTCInventory* ptc_inventory=0;
    phero->PeekTC(&ptc_inventory);
    bool guild_seal=charge_guild_seal;
    if(charge_guild_seal)guild_seal=true;
    else {
     long quick_access_item_def_index=ptc_inventory->GetQuickAccessItem(item_index);
     if(quick_access_item_def_index>0) {
      long guild_seal_def_index=World->PeekDefinitionManager().GetDefGlobalIndexFromName("OBJECT_GUILD_SEAL_1");
      if(quick_access_item_def_index==guild_seal_def_index)guild_seal=true;
     }
    }
    float activation_time=2.0f;
    if(guild_seal) {
     long guild_seal_def_index=World->PeekDefinitionManager().GetDefGlobalIndexFromName("OBJECT_GUILD_SEAL_1");
     if(ptc_inventory->GetNumberOfItemsOfTypeInInventory(guild_seal_def_index)<=0)return;
     if(!World->IsTeleportingEnabled())return;
    }else {
     CDefPointer<const CInventoryItemDef> pitem_def=ptc_inventory->GetQuickAccessItemDef(item_index);
     if(pitem_def.IsNull())return;
     activation_time=pitem_def->ActivationTime;
    }
    CGameEvent game_event;
    game_event.Replacement = false;
    game_event.Valid = true;
    game_event.EndPos = 0;
    if (activation_time > 0.0001f) {
     game_event.Type = 0x29;
     game_event.SetPlayer(phero->GetPlayerNumber());
     game_event.AddSLONG(item_index);
     game_event.AddUBYTE(1);
     processed_input.AddGameEvent(game_event);
     ItemIndex = item_index;
     ButtonIndex = button_index;
    } else {
     game_event.Type = 0x27;
     game_event.SetPlayer(phero->GetPlayerNumber());
     game_event.AddSLONG(item_index);
     processed_input.AddGameEvent(game_event);
    }
   }
  }else if(IsEventContextSensitiveItemButton(event,item_index)) {
   item_index%=4;
   const CThingPlayerCreature* phero=MyPlayer->PeekPControlledCreature();
   if(IS_THING_ALIVE(phero)) {
    CGameEvent game_event(0x27);
    game_event.SetPlayer(phero->GetPlayerNumber());
    game_event.AddSLONG(item_index);
    processed_input.AddGameEvent(game_event);
   }
  }
 }
}