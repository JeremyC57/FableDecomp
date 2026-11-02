// CTCQuickAccessMenu::ConstructItemCategoryList(CWideString&, CList*, vector<pair<long,long> >&, CComponent*, bool) -- build a category and its item actions
// Leaf: ?ConstructItemCategoryList@CTCQuickAccessMenu@@AAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCComponent@4@_N@Z
#define FABLE_RETAIL_QUICK_ACCESS
#define FABLE_RETAIL_VISIBLE_LOWERBOUND
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWIDESTRING_CTOR
#define FABLE_RETAIL_CWIDESTRING_COPY
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#include "engine/CWideString.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCTargetingBase.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCQuickAccessMenu.h"
#pragma auto_inline(off)
namespace std {
template<> list<NUISystem::CComponent*>::~list() {}
template<> void _List_base<NUISystem::CComponent*,allocator<NUISystem::CComponent*> >::clear() {
    _List_node<NUISystem::CComponent*>* cur=(_List_node<NUISystem::CComponent*>*)_M_node._M_data->_M_next;
    while(cur!=_M_node._M_data) {
        _List_node<NUISystem::CComponent*>* old=cur;
        cur=(_List_node<NUISystem::CComponent*>*)cur->_M_next;
        _M_node.deallocate(old,1);
    }
    _M_node._M_data->_M_next=_M_node._M_data;
    _M_node._M_data->_M_prev=_M_node._M_data;
}
}
#pragma auto_inline(on)
inline CWideString __fastcall GFIntToWideString(long value)
{
    CWideString text;
    text.Sprintf(L"%d",value);
    return text;
}
template<class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && !thing->IsDead(); }
void CTCQuickAccessMenu::ConstructItemCategoryList(CWideString& menuparam,NUISystem::CList* plist,std::vector<std::pair<long,long> >& items,NUISystem::CComponent* ptop_list,bool is_gifts)
{
    using namespace NUISystem;
    CManager* pmanager=CManager::GetInstance();
    CWideString menu_title(menuparam);
    unsigned long total_amount=0;
    for(unsigned long count=0;count<items.size();++count) total_amount+=items[count].second;
    if(!items.empty()) {
        menu_title+=CTCInventoryStats::GetXString();
        menu_title+=GFIntToWideString(total_amount);
    }
    CCountedPointer<CComponent> pmenu_entry_cat(pmanager->CreateMenuEntryComponent(menu_title,0.0f,1.0f,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW_FOR_QUICK_ACCESS",0.0f,"",0.0f,"PC_BUTTON_BASTARD"));
    plist->AddScrollingChildAndSetOffsets(pmenu_entry_cat);
    if(items.empty()) {
        CComponent* arrow=pmenu_entry_cat->FindChild("UI_SELECTION_ARROW_RIGHT");
        if(arrow) {
            CDeletion delete_arrow;
            delete_arrow.Method=CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(delete_arrow);
        }
        pmenu_entry_cat->MakeUnavailable();
    } else {
        std::map<long,long> thingmap;
        for(long count=0;count<(long)items.size();++count) {
            long key=items[count].first;
            std::map<long,long>::iterator found=thingmap.find(key);
            if(found==thingmap.end()) thingmap.insert(std::pair<long,long>(key,items[count].second));
            else found->second+=items[count].second;
        }
        CCountedPointer<CComponent> plist_cat_component(pmanager->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS",false));
        CList* plist_cat=static_cast<CList*>(plist_cat_component.Get());
        pmenu_entry_cat->AddChild(plist_cat_component);
        plist_cat->InitialiseOffsets(thingmap.size());
        bool usable=true;
        const CTCTargetingBase* targeting;
        if(is_gifts && ParentThing.PeekTC(&targeting) && !IS_THING_ALIVE(targeting->GetPTargetedThing())) usable=false;
        CCharString action_list_component_name(usable?"UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS":"UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS_CANT_USE");
        CCountedPointer<CComponent> pactions_list(CManager::GetInstance()->CreateComponent(action_list_component_name,false));
        CList* pactions_list_ptr=static_cast<CList*>(pactions_list.Get());
        pactions_list_ptr->SetDefaultChild(0L);
        if(!usable) {
            CComponent* unavailable=pactions_list_ptr->FindChild("UI_MENU_ENTRY_QUICK_ACCESS_ITEM_ACTION_NOT_USE");
            CComponent* arrow=unavailable->FindChild("UI_SELECTION_ARROW_RIGHT_FOR_QUICK_ACCESS");
            if(arrow) {
                CDeletion delete_arrow;
                delete_arrow.Method=CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(delete_arrow);
            }
            unavailable->MakeUnavailable();
        }
        ContructItemLists(thingmap,plist_cat,ptop_list,pactions_list);
    }
}
