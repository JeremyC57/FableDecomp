// CTCQuickAccessMenu::ConstructExpressionCategoryList(CWideString&, CList*, vector<pair<long,long> >&, CComponent*) -- build an expression category entry
// Retail 0x00627400, 742 bytes, thiscall ret 0x10, /O2 /Oy (no Flags), same TU as sibling 0x00626E40.
// Appends " x<count>" (GetXString + GFIntToWideString) to the title when items are present, creates the menu entry
// (UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW_FOR_QUICK_ACCESS) and adds it to plist. Empty: deletes the right arrow and
// makes the entry unavailable. Otherwise: creates the sub-list and the actions list and calls ContructItemLists.
// Callees: 0x0041E5F2 CManager::GetInstance, 0x00428494 CManager::CreateMenuEntryComponent, 0x0041DB1D CManager::CreateComponent,
// 0x00429C15 CCountedPointer<CComponent> ctor, 0x00428B10 DeleteData, 0x005CC772 CTCInventoryStats::GetXString,
// 0x0099BA70 CWideString::Sprintf, 0x0099AED0/0x0099B720/0x0099B8D0/0x0099B510 CWideString, 0x0099EBF0/0x0099EAE0 CCharString,
// 0x0042CD84 CDeletion copy ctor, 0x0042ABCA ~list<CComponent*>, 0x0042A18F list clear, 0x00626750 ContructItemLists(vector),
// vtable: CComponent FindChild +0x0C, AddChild +0xEC, SetDeletion +0xF8, MakeUnavailable +0x170;
// CList AddScrollingChildAndSetOffsets +0x260, InitialiseOffsets +0x270, SetDefaultChild +0x278.
// Leaf: ?ConstructExpressionCategoryList@CTCQuickAccessMenu@@AAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCComponent@4@@Z
#define FABLE_RETAIL_CTCQUICKACCESSMENU_EXPRESSIONS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
// STLport list opt-ins (shared header rebuild/include/stlport/fable_containers.h, real STLport 4.6 bodies, no local specialisation):
// retail calls ~list<NUISystem::CComponent*> out of line (`call 0x0042ABCA` at 0x0062758A, the landed row
// 00/42/stdlistNUISystemCComponent_list_0042abca.cpp: clear() then deallocate the sentinel, i.e. STLport's empty ~list plus the
// inline ~_List_base), and that COMDAT calls _List_base::clear out of line (0x0042A18F, landed as
// 00/42/CGuiControlTreePane_SortTreeRecursively_0042a18f.cpp, manifest std::list<NUISystem::CComponent*>::clear). Same opt-ins as
// the landed callers 0x005A887D (FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE) and 0x005FCBE0 (FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE).
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCTargetingBase.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCQuickAccessMenu.h"
// GFIntToWideString: the real body of the landed row 00/40/global_GFIntToWideString_00405f00.cpp (retail 0x00405F00, Ego_r
// ?GFIntToWideString@@YI?AVCWideString@@J@Z). Retail inlined it here: no call to 0x00405F00; instead `push 0x0122DE44` (L"%d")
// at 0x0062745B and `call 0x0099BA70` (CWideString::Sprintf) at 0x00627461 on a local CWideString (sibling 0x00626E40 inlines it
// the same way, 0x00626EB7/0x00626EBD). Its header declaration is not visible to this TU, so the body is given here as `inline`.
inline CWideString __fastcall GFIntToWideString(long value)
{
    CWideString text;
    text.Sprintf(L"%d",value);
    return text;
}
void CTCQuickAccessMenu::ConstructExpressionCategoryList(CWideString& menuparam,NUISystem::CList* plist,std::vector<std::pair<long,long> >& items,NUISystem::CComponent* ptop_list)
{
    using namespace NUISystem;
    CManager* pmanager=CManager::GetInstance();
    CWideString menu_title(menuparam);
    if(!items.empty()) {
        menu_title+=CTCInventoryStats::GetXString();
        menu_title+=GFIntToWideString(items.size());
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
        CCountedPointer<CComponent> plist_cat_component(pmanager->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS",false));
        CList* plist_cat=static_cast<CList*>(plist_cat_component.Get());
        pmenu_entry_cat->AddChild(plist_cat_component);
        plist_cat->InitialiseOffsets(items.size());
        CCountedPointer<CComponent> pactions_list(CManager::GetInstance()->CreateComponent("UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS",false));
        static_cast<CList*>(pactions_list.Get())->SetDefaultChild(0L);
        ContructItemLists(items,plist_cat,ptop_list,pactions_list);
    }
}