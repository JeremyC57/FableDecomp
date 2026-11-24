#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <map>
#include <vector>
#include "engine/NUISystem_EActionType.h"
using namespace NUISystem;
static bool money,space,owned,special,bad,branch;
static unsigned checks,unavailable,ordinary,moving,extra;
struct CWideString {const wchar_t* s;CWideString(const wchar_t* t):s(t){}};
struct CActionParam { int id;CActionParam(int i=0):id(i){}virtual ~CActionParam(){}};
struct CActionParamSLONG: CActionParam {CActionParamSLONG(long i):CActionParam((int)i){}};
struct CActionParamString: CActionParam {CActionParamString(CWideString& x):CActionParam(74){if(wcscmp(x.s,L"PC_MODEL"))bad=true;}};
template<class T> struct CCountedPointer {
 T* p;explicit CCountedPointer(T* t=0):p(t){}
 T* Get()const{return p;}T* operator->()const{return p;}void Reset(T* t){p=t;}
};
struct CAction {
 EActionType ActionType;std::vector<CActionParam*> UIActionParams;
 void AddParam(CActionParam* param,CCountedPointer<CAction>& self){if(self.Get()!=this)bad=true;UIActionParams.push_back(param);}
};
static std::vector<int> channels,actionTypes;
static std::vector< std::vector<int> > params;
static void note(int channel,CCountedPointer<CAction> a){
 channels.push_back(channel);actionTypes.push_back(a->ActionType);params.push_back(std::vector<int>());
 for(unsigned i=0;i<a->UIActionParams.size();++i){
  CActionParam* p=a->UIActionParams[i];params.back().push_back(p->id);
  if(p->id!=82 && p->id!=83 && p->id!=84)delete p;
 }
 delete a.Get();
}
struct CComponent: CActionParam {
 explicit CComponent(int i=0):CActionParam(i){}
 void MakeUnavailable(){++unavailable;}
 void AddActionOnBack(CCountedPointer<CAction> a){note(1,a);}
 void AddActionOnSelected(CCountedPointer<CAction> a){note(2,a);}
 void AddPreAction(CCountedPointer<CAction> a){note(3,a);}
};
struct CMenuEntry: CComponent {
 CMenuEntry():CComponent(82){}
 void AddActionOnUnselected(CCountedPointer<CAction> a){note(4,a);}
 void AddActionOnVisible(CCountedPointer<CAction> a){note(5,a);}
 void AddUIActionParam(CComponent* p){if(p!=this)bad=true;++extra;}
};
static CMenuEntry entry;
struct CViewport: CComponent {CViewport():CComponent(83){}};
struct CManager {static CManager* GetInstance(){static CManager instance;return &instance;}
 CComponent* CreateMenuEntryComponent(const wchar_t* name,float a,float b,const char* pattern,float c,const char* empty,float z,const char* button){
 branch=strcmp(pattern,"PC_MENU_ENTRY_BRANCH_TEMPLATE")==0;
 if(!branch && strcmp(pattern,"PC_MENU_ENTRY_TEMPLATE"))bad=true;
 if(wcscmp(name,L"INVALID NAME") || a!=0 || b!=1 || c!=0 || *empty || z!=0 || strcmp(button,"PC_BUTTON_BASTARD"))bad=true;
 return &entry;
 }};
struct CList {
 void AddScrollingChildDynamicallyAndSetOffsets(CCountedPointer<CComponent> p){if(p.Get()!=&entry)bad=true;++moving;}
 void AddScrollingChildAndSetOffsets(CCountedPointer<CComponent> p){if(p.Get()!=&entry)bad=true;++ordinary;}
};
namespace NInventory {struct CTCInventoryBase{bool IsSpaceInInventoryForItem(long i)const{if(i!=71)bad=true;++checks;return space;}};}
static NInventory::CTCInventoryBase inventory;
struct CThingPlayerCreature {void PeekTC(int t,const NInventory::CTCInventoryBase** out)const{if(t!=101)bad=true;*out=&inventory;}};
struct CPlayer {const CThingPlayerCreature* PeekPControlledCreature()const{static CThingPlayerCreature player;return &player;}};
struct CPlayerManager {const CPlayer& GetMainPlayer()const{static CPlayer p;return p;}};
struct CMainGameComponent {static CMainGameComponent* Get(){static CMainGameComponent w;return &w;}
 const CPlayerManager& PeekPlayerManager()const{static CPlayerManager m;return m;}};
struct CTCInventoryItem {static int GetInventoryType(long i){if(i!=71)bad=true;return 101;}};
template<class T,class U> U* GFDebugPDynamicCast(T* p,U*){return static_cast<U*>(p);}
struct CTCInventoryTrade {
 std::map<CComponent*, std::pair<long,long> > ItemsToBuy;
 bool AlreadyHaveSpecial(long i){if(i!=71)bad=true;return owned;}
 bool IsItemAffordable(long i,int q){if(i!=71||q!=1)bad=true;return money;}
 bool IsSpecialCase(long i){if(i!=71)bad=true;return special;}
 bool AddBuyItemToListPC(CComponent*,long,long,CList*,CViewport*,bool);
};
bool CTCInventoryTrade::AddBuyItemToListPC(CComponent* pframe, long item_def, long quantity, CList* pbuyable_list, CViewport* pviewport, bool dynamic)
{
    bool buyable = true;
    CManager* pmanager = CManager::GetInstance();
    CCountedPointer<CComponent> pmenu_entry;
    const CThingPlayerCreature* pplayer = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    const NInventory::CTCInventoryBase* ptc_inventory = 0;
    pplayer->PeekTC(CTCInventoryItem::GetInventoryType(item_def), &ptc_inventory);
    bool already_have_special = AlreadyHaveSpecial(item_def);
    if (IsItemAffordable(item_def, 1) && (ptc_inventory->IsSpaceInInventoryForItem(item_def) || IsSpecialCase(item_def)) &&
        quantity > 0) {
        if (!already_have_special) {
            pmenu_entry.Reset(pmanager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                      0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        } else {
            buyable = false;
            pmenu_entry.Reset(pmanager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                      0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            pmenu_entry->MakeUnavailable();
        }
    } else {
        buyable = false;
        pmenu_entry.Reset(pmanager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                      0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        pmenu_entry->MakeUnavailable();
    }
    if (IsSpecialCase(item_def)) {
        CCountedPointer<CAction> paction(new CAction);
        paction->ActionType = UI_ACTION_TYPE_CLOSE_ACTIVE_TRADE_MENU;
        pmenu_entry->AddActionOnBack(paction);
    }
    std::map<CComponent*, std::pair<long, long> >::value_type value_pair(pmenu_entry.Get(), std::pair<long, long>(item_def, quantity));
    ItemsToBuy.insert(value_pair);
    CMenuEntry* pmenu_entry_real = GFDebugPDynamicCast(pmenu_entry.Get(), (CMenuEntry*)0);
    CCountedPointer<CAction> paction_load_mesh(new CAction);
    paction_load_mesh->ActionType = UI_ACTION_ADD_MESH_CHILD;
    paction_load_mesh->UIActionParams.push_back(new CActionParamSLONG(item_def));
    paction_load_mesh->AddParam(pmenu_entry.Get(), paction_load_mesh);
    CWideString component_type(L"PC_MODEL");
    paction_load_mesh->UIActionParams.push_back(new CActionParamString(component_type));
    paction_load_mesh->AddParam(pframe, paction_load_mesh);
    pmenu_entry->AddActionOnSelected(paction_load_mesh);
    CCountedPointer<CAction> paction_unload_mesh(new CAction);
    paction_unload_mesh->ActionType = UI_ACTION_REMOVE_MESH_CHILD;
    paction_unload_mesh->AddParam(pmenu_entry.Get(), paction_unload_mesh);
    pmenu_entry_real->AddActionOnUnselected(paction_unload_mesh);
    CCountedPointer<CAction> paction_load_item_description(new CAction);
    paction_load_item_description->ActionType = UI_ACTION_LOAD_BUY_ITEM_DESC;
    paction_load_item_description->UIActionParams.push_back(new CActionParamSLONG(item_def));
    paction_load_item_description->AddParam(pviewport, paction_load_item_description);
    paction_load_item_description->AddParam(pmenu_entry.Get(), paction_load_item_description);
    paction_load_item_description->AddParam(pframe, paction_load_item_description);
    pmenu_entry->AddActionOnSelected(paction_load_item_description);
    CCountedPointer<CAction> paction_unload_item_description(new CAction);
    paction_unload_item_description->ActionType = UI_ACTION_UNLOAD_BUY_ITEM_DESC;
    paction_unload_item_description->AddParam(pmenu_entry.Get(), paction_unload_item_description);
    pmenu_entry_real->AddActionOnUnselected(paction_unload_item_description);
    CCountedPointer<CAction> paction_load_action_menu(new CAction);
    paction_load_action_menu->ActionType = UI_ACTION_LOAD_BUY_ACTION_MENU;
    paction_load_action_menu->UIActionParams.push_back(new CActionParamSLONG(item_def));
    paction_load_action_menu->AddParam(pmenu_entry.Get(), paction_load_action_menu);
    pmenu_entry->AddPreAction(paction_load_action_menu);
    CCountedPointer<CAction> paction_unload_action_menu(new CAction);
    paction_unload_action_menu->ActionType = UI_ACTION_UNLOAD_BUY_ACTION_MENU;
    paction_unload_action_menu->AddParam(pmenu_entry.Get(), paction_unload_action_menu);
    pmenu_entry_real->AddActionOnUnselected(paction_unload_action_menu);
    CCountedPointer<CAction> paction_load_item_name(new CAction);
    paction_load_item_name->ActionType = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
    paction_load_item_name->UIActionParams.push_back(new CActionParamSLONG(item_def));
    paction_load_item_name->AddParam(pmenu_entry.Get(), paction_load_item_name);
    paction_load_item_name->UIActionParams.push_back(new CActionParamSLONG(quantity));
    pmenu_entry_real->AddActionOnVisible(paction_load_item_name);
    pmenu_entry_real->AddUIActionParam(pmenu_entry.Get());
    if (dynamic)
        pbuyable_list->AddScrollingChildDynamicallyAndSetOffsets(pmenu_entry);
    else
        pbuyable_list->AddScrollingChildAndSetOffsets(pmenu_entry);
    return buyable;
}

int main(){
 for(int mask=0;mask<32;++mask)for(long q=-1;q<2;++q){
  money=(mask&1)!=0;space=(mask&2)!=0;special=(mask&4)!=0;owned=(mask&8)!=0;bool dyn=(mask&16)!=0;
  bad=branch=false;ordinary=moving=extra=checks=unavailable=0;channels.clear();actionTypes.clear();params.clear();
  CTCInventoryTrade trade;CComponent frame(84);CViewport viewport;CList list;
  bool got=trade.AddBuyItemToListPC(&frame,71,q,&list,&viewport,dyn);
  bool can=money&&(space||special)&&q>0&&!owned;
  if(bad||got!=can||branch!=can||unavailable!=unsigned(!can)||checks!=unsigned(money)||ordinary!=unsigned(!dyn)||
     moving!=unsigned(dyn)||extra!=1||trade.ItemsToBuy.size()!=1||trade.ItemsToBuy[&entry]!=std::make_pair(71L,q))return 1;
  int types[]={UI_ACTION_ADD_MESH_CHILD,UI_ACTION_REMOVE_MESH_CHILD,UI_ACTION_LOAD_BUY_ITEM_DESC,UI_ACTION_UNLOAD_BUY_ITEM_DESC,
   UI_ACTION_LOAD_BUY_ACTION_MENU,UI_ACTION_UNLOAD_BUY_ACTION_MENU,UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME};
  int places[]={2,4,2,4,3,4,5};int lens[]={4,1,4,1,2,1,3};
  int expected[][4]={{71,82,74,84},{82},{71,83,82,84},{82},{71,82},{82},{71,82,(int)q}};
  if(channels.size()!=unsigned(7+special) || (special && (channels[0]!=1||actionTypes[0]!=UI_ACTION_TYPE_CLOSE_ACTIVE_TRADE_MENU||!params[0].empty())))return 2;
  for(int j=0;j<7;++j){int n=j+special;if(channels[n]!=places[j]||actionTypes[n]!=types[j]||params[n].size()!=unsigned(lens[j]))return 3;
    for(int k=0;k<lens[j];++k)if(params[n][k]!=expected[j][k])return 4;}
 }
 puts("005a6ca1_TEST PASS");return 0;
}