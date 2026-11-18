// CTCInventory::ConstructItemCategoryList -- construct the category menu, merge entries by definition and build the action list.
// Grade: RELOCATION_MATCH
// Leaf: ?ConstructItemCategoryList@CTCInventory@@QAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCComponent@4@ABVCCharString@@4_N@Z
// Retail: Fable.exe 0x0058F016, 1587 bytes, thiscall.
// Flags: none
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCOMPONENT
#define FABLE_RETAIL_NUISYSTEM_CCOMPONENT_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CTCINVENTORY_CONSOLEMENUS
#define FABLE_RETAIL_CTCINVENTORY_CONSOLEMENU_HELPERS
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_CTCTARGETINGBASE
#define FABLE_RETAIL_CTHING_GETTC_SIMPLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_CTOR_NOINLINE
#define FABLE_RETAIL_STLP_MAP_FORCEINLINE_DTOR
#define FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryStats.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CThing.h"
#include "engine/CTCTargetingBase.h"
#include "engine/ThingAlive.h"

#pragma optimize("y", off)
#pragma optimize("s", on)
using namespace NUISystem;
CWideString FABLE_FASTCALL GFIntToWideString(long);
namespace { CCharString TITLE_NAME("ITEMS TITLE"); }
void CTCInventory::ConstructItemCategoryList(CWideString& menuparam, CList* plist, std::vector<std::pair<long, long> >& items, CComponent* ptop_list,
                                             const CCharString& ring_pic_name, const CCharString& text_tag_name, bool is_gifts)
{
    CManager* pmanager = CManager::GetInstance();
    CViewport* pviewport_real = (CViewport*)ptop_list->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    CCountedPointer<CComponent> psub_title;
    CWideString menu_title(menuparam);
    if (!items.empty()) {
        menu_title += CTCInventoryStats::GetXString();
        long number_of_items = 0;
        for (unsigned long i = 0; i < items.size(); ++i)
            number_of_items += items[i].second;
        menu_title += GFIntToWideString(number_of_items);
    }
    psub_title.Reset(pmanager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    CText* psub_title_real = static_cast<CText*>(psub_title.Get());
    psub_title_real->SetValue(menu_title, true, true);
    CCountedPointer<CComponent> pmenu_entry_cat(pmanager->CreateMenuEntryComponent(menu_title, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                                   0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    pmenu_entry_cat->AddChild(psub_title);
    plist->AddScrollingChildAndSetOffsets(pmenu_entry_cat);
    CCountedPointer<CComponent> pdesc = CreateMenuDesc(text_tag_name);
    pmenu_entry_cat->AddChild(pdesc);
    pviewport_real->AddFakeChild(pdesc.Get());
    if (ring_pic_name != "") {
        CCountedPointer<CComponent> plist_pic_component(pmanager->CreateComponent(ring_pic_name, false));
        pmenu_entry_cat->AddChild(plist_pic_component);
    }
    if (items.empty()) {
        CComponent* entryObject = pmenu_entry_cat.Get();
        CComponent* parrow = entryObject->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (parrow) {
            CDeletion delete_arrow;
            delete_arrow.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            parrow->SetDeletion(delete_arrow);
        }
        pmenu_entry_cat->MakeUnavailable();
    } else {
        std::map<long, long> thingmap;
        for (long i = 0; i < static_cast<long>(items.size()); ++i) {
            const std::pair<long,long>& item = items[i];
            long def = item.first;
            std::map<long, long>::iterator findit = thingmap.find(def);
            if (findit == thingmap.end())
                thingmap.insert(std::make_pair(def, item.second));
            else {
                long& item_count = findit->second;
                item_count += item.second;
            }
        }
        CCountedPointer<CComponent> plist_cat_component(pmanager->CreateComponent("UI_SUB_LIST", false));
        CList* plist_cat = static_cast<CList*>(plist_cat_component.Get());
        CCountedPointer<CComponent> pbackbutton(pmanager->CreateComponent("UI_BACK_FOR_LIST", false));
        plist_cat->AddNonScrollingChild(pbackbutton);
        pmenu_entry_cat->AddChild(plist_cat_component);
        plist_cat->InitialiseOffsets(thingmap.size());
        CCountedPointer<CComponent> ptitle_for_cat(pmanager->CreateComponent("UI_ITEM_TITLE", false));
        CText* ptext = static_cast<CText*>(ptitle_for_cat.Get());
        CWideString tit(ptext->GetValue());
        tit += L": ";
        tit += menuparam;
        ptext->SetValue(tit, true, true);
        ptitle_for_cat->SetName(TITLE_NAME);
        plist_cat->AddNonScrollingChild(ptitle_for_cat);
        bool can_use_gift = true;
        const CTCTargetingBase* ptc_targeting;
        if (is_gifts && static_cast<const CThing*>(ParentThing)->PeekTC(&ptc_targeting)) {
            CThing* ptarget = ptc_targeting->GetPTargetedThing();
            if (!IS_THING_ALIVE(ptarget))
                can_use_gift = false;
        }
        CCharString action_list_component_name(can_use_gift ? "UI_LIST_ITEMS_ACTIONS" : "UI_LIST_ITEMS_ACTIONS_CANT_USE");
        CCountedPointer<CComponent> pactions_list(CManager::GetInstance()->CreateComponent(action_list_component_name, false));
        CList* pactions_list_real = static_cast<CList*>(pactions_list.Get());
        pactions_list_real->SetDefaultChild(0L);
        if (!can_use_gift) {
            CComponent* puse_menu_entry = pactions_list_real->FindChild("UI_MENU_ENTRY_ITEM_ACTION_NOT_USE");
            puse_menu_entry->MakeUnavailable();
            CComponent* pbutton = puse_menu_entry->FindChild("UI_ACTION_ICON");
            if (pbutton) {
                CDeletion delete_button;
                delete_button.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                pbutton->SetDeletion(delete_button);
            }
        }
        CCountedPointer<CComponent> pbackbutton_foraction(pmanager->CreateComponent("UI_BACK_FOR_LIST", false));
        CCountedPointer<CComponent> pselectbutton_foraction(pmanager->CreateComponent("UI_SELECT_FOR_LIST", false));
        pactions_list_real->AddNonScrollingChild(pbackbutton_foraction);
        pactions_list_real->AddNonScrollingChild(pselectbutton_foraction);
        CCountedPointer<CComponent> pactiontitle(pmanager->CreateComponent("UI_ITEM_TITLE", false));
        static_cast<CText*>(pactiontitle.Get())->SetValue(tit, true, true);
        pactiontitle->SetName(TITLE_NAME);
        pactions_list_real->AddNonScrollingChild(pactiontitle);
        ContructItemLists(thingmap, plist_cat, ptop_list, pactions_list);
    }
}