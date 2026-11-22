#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "engine/EGameAction.h"
static void check(bool v){if(!v)std::abort();}
// Dependency double uses native-width Type and payload words on LP64.
// Constructor and builder statements mirror shared CGameEvent.h.
class CGameEvent {public:int Type;signed char Player;unsigned char Data[32],EndPos;bool Valid,Replacement;
 CGameEvent() : Player(-1), Valid(false) {}
 CGameEvent(long type):Type((int)type),Player(-1),EndPos(0),Valid(true),Replacement(false){}
 void SetPlayer(long player){Player=(signed char)player;}
 void AddData(long n,const void*p){check(EndPos+n<=32);std::memcpy(Data+EndPos,p,n);EndPos+=(unsigned char)n;}
 void AddSLONG(long value){int word=(int)value;AddData(sizeof(word),&word);}
 void AddUBYTE(unsigned char value){AddData(sizeof(value),&value);}
};
class CProcessedInput {public:std::vector<CGameEvent> Events;void AddGameEvent(const CGameEvent& e){Events.push_back(e);}};
class CInputEvent {public:bool Press,Charge,Quick,Context;long Index;CInputEvent():Press(true),Charge(false),Quick(true),Context(false),Index(1){}};
class CInventoryItemDef {public:float ActivationTime;};
static int DefReleases;
template<class T> class CDefPointer {const T* P;public:CDefPointer(const T*p):P(p){} CDefPointer(const CDefPointer& o):P(o.P){} ~CDefPointer(){if(P)++DefReleases;} bool IsNull()const{return !P;}const T*operator->()const{return P;}};
class CTCInventory {public:long QuickDef,Number;CInventoryItemDef Def;bool NullDef;mutable long LastIndex;CTCInventory():QuickDef(7),Number(1),NullDef(false),LastIndex(-100){Def.ActivationTime=0.0f;}long GetQuickAccessItem(long i)const{LastIndex=i;return QuickDef;}long GetNumberOfItemsOfTypeInInventory(long d)const{check(d==99);return Number;}CDefPointer<const CInventoryItemDef> GetQuickAccessItemDef(long i)const{LastIndex=i;return CDefPointer<const CInventoryItemDef>(NullDef?0:&Def);}};
class CThingPlayerCreature {public:bool Dead;long Player;const CTCInventory* Inventory;CThingPlayerCreature():Dead(false),Player(3),Inventory(0){}bool PeekTC(const CTCInventory**p)const{*p=Inventory;return Inventory!=0;}unsigned long GetPlayerNumber()const{return Player;}};
template<class T> bool IS_THING_ALIVE(const T& p){return p&&!p->Dead;}
class CPlayer {public:const CThingPlayerCreature* Hero;const CThingPlayerCreature* PeekPControlledCreature()const{return Hero;}};
class CPlayerGui {public:bool Open;static CPlayerGui&Get(){static CPlayerGui x;return x;}bool IsOpen()const{return Open;}};
class CGamePlayerInterface {public:long Count;long GetNumberOfContextSensitiveItemButtons()const{return Count;}};
class CGameDefinitionManager {public:mutable int Lookups;CGameDefinitionManager():Lookups(0){}long GetDefGlobalIndexFromName(const char* s)const{check(std::strcmp(s,"OBJECT_GUILD_SEAL_1")==0);++Lookups;return 99;}};
class CWorld {public:CGameDefinitionManager Manager;bool Enabled;const CGameDefinitionManager& PeekDefinitionManager()const{return Manager;}bool IsTeleportingEnabled()const{return Enabled;}};
class CMainGameComponent {
public:
 static CMainGameComponent* Get() { static CMainGameComponent component; return &component; }
 void AddGameEvent(const CGameEvent& event) { Events.push_back(event); }
 std::vector<CGameEvent> Events;
};
class CInputProcessQuickAccessItems {public:static long ItemIndex,ButtonIndex;CWorld*World;CPlayer*MyPlayer;CGamePlayerInterface*Interface;bool OtherHeld;CInputProcessQuickAccessItems():OtherHeld(false){} bool IsEventButtonPressed(const CInputEvent&e)const{return e.Press;}bool IsEventGameAction(EGameAction a,const CInputEvent&e)const{check(a==GAME_ACTION_CHARGE_GUILD_SEAL);return e.Charge;}bool IsEventQuickAccessItemButton(const CInputEvent&e,long&i)const{if(e.Quick)i=e.Index;return e.Quick;}bool IsEventContextSensitiveItemButton(const CInputEvent&e,long&i)const{if(e.Context)i=e.Index;return e.Context;}bool IsOtherQuickAccessItemButtonHeld(long)const{return OtherHeld;}const CGamePlayerInterface&GetGamePlayerInterface()const{return *Interface;}void CancelQuickAccessCharge();void ProcessInput(const CInputEvent&,CProcessedInput&);};
long CInputProcessQuickAccessItems::ItemIndex=-1;long CInputProcessQuickAccessItems::ButtonIndex=-1;

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
int main(){
 for(int scenario=0;scenario<20;++scenario){
  CInputProcessQuickAccessItems p;CWorld world;CPlayer player;CThingPlayerCreature hero;CTCInventory inventory;CGamePlayerInterface interface_;CInputEvent event;CProcessedInput output;
  world.Enabled=true;interface_.Count=4;hero.Inventory=&inventory;player.Hero=&hero;p.World=&world;p.MyPlayer=&player;p.Interface=&interface_;CPlayerGui::Get().Open=true;p.ItemIndex=p.ButtonIndex=-1;DefReleases=0;
  long expectedType=0x27,expectedIndex=5;bool queued=true,charging=false;int expectedCancel=0;
  switch(scenario){
   case 0:break;
   case 1:event.Press=false;queued=false;break;
   case 2:player.Hero=0;queued=false;break;
   case 3:hero.Dead=true;queued=false;break;
   case 4:p.OtherHeld=true;queued=false;break;
   case 5:CPlayerGui::Get().Open=false;queued=false;break;
   case 6:inventory.Def.ActivationTime=0.5f;expectedType=0x29;charging=true;break;
   case 7:inventory.Def.ActivationTime=0.0001f;break;
   case 8:inventory.Def.ActivationTime=0.00010001f;expectedType=0x29;charging=true;break;
   case 9:inventory.NullDef=true;queued=false;break;
   case 10:event.Charge=true;event.Quick=false;expectedIndex=-2;expectedType=0x29;charging=true;break;
   case 11:inventory.QuickDef=99;expectedType=0x29;charging=true;break;
   case 12:inventory.QuickDef=99;inventory.Number=0;queued=false;break;
   case 13:inventory.QuickDef=99;world.Enabled=false;queued=false;break;
   case 14:p.ItemIndex=8;p.ButtonIndex=6;expectedCancel=1;break;
   case 15:event.Quick=false;event.Context=true;event.Index=11;expectedIndex=3;break;
   case 16:event.Quick=false;event.Context=true;event.Index=-7;expectedIndex=-3;break;
   case 17:event.Quick=false;event.Context=false;queued=false;break;
   case 18:event.Quick=false;event.Context=true;hero.Dead=true;queued=false;break;
   case 19:inventory.QuickDef=-1;inventory.Def.ActivationTime=-2.0f;break;
  }
  CMainGameComponent::Get()->Events.clear();
  p.ProcessInput(event,output);check(output.Events.size()==(queued?1:0));
  const std::vector<CGameEvent>& canceled = CMainGameComponent::Get()->Events;
  check(canceled.size() == (unsigned int)expectedCancel);
  if (expectedCancel) {
   const CGameEvent& event = canceled[0];
   int index = 0;
   std::memcpy(&index, event.Data, sizeof(index));
   check(event.Type == 0x29 && event.Player == 3 && event.EndPos == 5 && event.Valid && !event.Replacement);
   check(index == 8 && event.Data[4] == 0);
  }
  if(queued){const CGameEvent&e=output.Events[0];check(e.Type==expectedType&&e.Player==3&&e.Valid&&!e.Replacement);check(e.EndPos==(charging?5:4));int index=0;std::memcpy(&index,e.Data,4);check(index==expectedIndex);if(charging)check(e.Data[4]==1);}
  if(charging)check(p.ItemIndex==expectedIndex&&p.ButtonIndex==(scenario==10?-2:1));else check(p.ItemIndex==-1&&p.ButtonIndex==-1);
  check(world.Manager.Lookups==((scenario>=11&&scenario<=13)?2:((scenario==10)?1:((queued&&scenario!=15&&scenario!=16&&scenario!=19)?1:0)))||scenario<6||scenario==9||scenario==17||scenario==18);
 }
 CMainGameComponent::Get()->Events.clear();
 CInputProcessQuickAccessItems input;
 CPlayer player;
 input.MyPlayer = &player;
 input.ItemIndex = 6;
 input.ButtonIndex = 3;
 player.Hero = 0;
 input.CancelQuickAccessCharge();
 CThingPlayerCreature hero;
 hero.Dead = true;
 player.Hero = &hero;
 input.CancelQuickAccessCharge();
 check(CMainGameComponent::Get()->Events.empty());
 check(input.ItemIndex == 6 && input.ButtonIndex == 3);
 puts("0068e510_TEST PASS");return 0;
}