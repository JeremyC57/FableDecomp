// 00627be0 CTCQuickAccessMenu::GetExpressionsListUI
// Leaf: ?GetExpressionsListUI@CTCQuickAccessMenu@@AAEPAVCComponent@NUISystem@@AAJPAV23@@Z
// Flags:
// Same-TU: both reserved members and the two category builders are owned by tc_quick_access_menu.obj.
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_STLP_NUISYSTEM_LIST_SIZE_OWNERS_PASS27
#define FABLE_RETAIL_STLP_ACTION_PARAM_LIST_SIZE_OWNERS_PASS26
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_QUICK_MENU_PAIR_CONSTRUCT_PASS27
#define FABLE_RETAIL_QUICK_MENU_PAIR_ALLOCATE_PASS27

// Fidelity: scaffolding, not retail. The size-mode definitions reproduce the original
// out-of-line library copies while the UI members retain their speed optimization.
#define FABLE_RETAIL_QUICKACCESSMENU_ACTIVATE_PASS27
#define FABLE_RETAIL_CTCQUICKACCESSMENU_ISACTIVE
#define FABLE_RETAIL_CTCQUICKACCESSMENU_ISQUITTING_ACTIVATE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_INVENTORY_EXPRESSIONS_PASS26
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CACTION_ORIGINAL_DTOR_DECL_PASS26
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY
#define FABLE_RETAIL_CTCQUICKACCESSMENU_EXPRESSIONS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CEXPRESSIONSUBDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CEXPRESSIONSUBDEF_STATGETTYPENAME
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_USEBUTTONACTION
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_ACTIONLISTS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_assert.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCQuickAccessMenu.h"
#include "engine/CTCInventoryStats.h"
#include "stlport/fable_action_params_list.h"
#include "stlport/fable_nuisystem_list_owners.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/GFGetTime.h"

#include "stlport/fable_map.h"
#include "engine/CExpressionDef.h"
#include "engine/CExpressionSubDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);

#include "engine/CTCInventoryItem.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CDeletion.h"
#if defined(_MSC_VER)
#define FABLE_QUICK_MENU_NOINLINE __declspec(noinline)
#else
#define FABLE_QUICK_MENU_NOINLINE
#endif
using namespace NUISystem;
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
inline NUISystem::CAction::CAction() : ActionType(UI_ACTION_TYPE_NONE), Valid(true) {}
inline NUISystem::CDeletion::CDeletion() {}
FABLE_QUICK_MENU_NOINLINE inline NUISystem::CDeletion::CDeletion(const CDeletion& other) : Method(other.Method), AssociatedParents(other.AssociatedParents) {}

#ifdef _MSC_VER
#pragma optimize("",on)
#endif
inline bool CTCQuickAccessMenu::IsActive() const { return Active; }

#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
namespace std {
template<> FABLE_QUICK_MENU_NOINLINE inline pair<long,long>* FABLE_FASTCALL __uninitialized_copy(pair<long,long>* first, pair<long,long>* last, pair<long,long>* result, const __false_type&) {
    pair<long,long>* cur=result;
    for(; first!=last; ++first,++cur) _Construct(&*cur,*first);
    return cur;
}
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline void FABLE_FASTCALL CCountedPointer<NUISystem::CAction>::DeleteData(void* data) {delete static_cast<NUISystem::CAction*>(data);}
template<> inline CCountedPointer<NUISystem::CAction>::CCountedPointer(NUISystem::CAction* data):Data(data) { if (Data) Info = new CCPPointerInfo(1,DeleteData,(void*)Data); else Info=0; }
template<> inline CCountedPointer<NUISystem::CComponent>::CCountedPointer(NUISystem::CComponent* data):Data(data) { if (Data) Info = new CCPPointerInfo(1,DeleteData,(void*)Data); else Info=0; }
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
// These size-optimized specializations emit the original helper copies.
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
template<> inline void CCountedPointer<NUISystem::CAction>::ReleaseData()
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data = 0;
    Info = 0;
}
template<> inline void CCountedPointer<NUISystem::CComponent>::ReleaseData()
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data = 0;
    Info = 0;
}
template<> inline CCountedPointer<NUISystem::CAction>::~CCountedPointer()
{
    ReleaseData();
}
template<> inline CCountedPointer<NUISystem::CComponent>::~CCountedPointer()
{
    ReleaseData();
}
namespace std {
typedef pair<const long, NUISystem::CComponent*> QuickAccessComponentPair;
typedef _Rb_tree<long, QuickAccessComponentPair, _Select1st<QuickAccessComponentPair>,
                 less<long>, allocator<QuickAccessComponentPair> > QuickAccessComponentTree;
template<> FABLE_QUICK_MENU_NOINLINE inline void QuickAccessComponentTree::_M_erase(_Rb_tree_node<QuickAccessComponentPair>* node)
{
    while (node != 0) {
        _M_erase(static_cast<_Rb_tree_node<QuickAccessComponentPair>*>(node->_M_right));
        _Rb_tree_node<QuickAccessComponentPair>* left = static_cast<_Rb_tree_node<QuickAccessComponentPair>*>(node->_M_left);
        _Destroy(&node->_M_value_field);
        this->_M_header.deallocate(node, 1);
        node = left;
    }
}

template<> inline void QuickAccessComponentTree::clear()
{
    if (_M_node_count != 0) {
        _M_erase(static_cast<_Rb_tree_node<QuickAccessComponentPair>*>(this->_M_header._M_data->_M_parent));
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
        _M_node_count = 0;
    }
}
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
void CTCQuickAccessMenu::Activate()
{
    if (!IsActive()) {
        LastRenderTime = GFGetTime();
        CTCInventory* ptc_inventory = 0;
        FABLE_RETAIL_ASSERT(GetParentThing().GetTC(&ptc_inventory));
        GetParentThing().GetTC(&ptc_inventory);
        ptc_inventory->GetItemsMenuEntries().clear();
        ptc_inventory->GetAssignedIcons().clear();
        CManager::GetInstance()->ClearActionParams();
        CManager::GetInstance()->SetInventory(ptc_inventory);

        // Inline-budget: the original constructor stays out of line at 0x006280EF.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        CCountedPointer<CAction> paction_back_out(new CAction);
#ifdef _MSC_VER
#pragma inline_depth()
#endif

        paction_back_out->ActionType = UI_ACTION_TYPE_CLOSE_QUICK_ACCESS_MENU;
        CManager* pmanager = CManager::GetInstance();
        CComponent* plist_component = pmanager->CreateComponent("UI_LIST_QUICK_ACCESS_MENU", false);
        CList* plist = static_cast<CList*>(plist_component);
        long tot_num_of_items = 0;
        long tot_num_of_exp = 0;
        CComponent* pitems_list = GetItemsListUI(tot_num_of_items, plist_component);
        CComponent* pexpressions_list = GetExpressionsListUI(tot_num_of_exp, plist_component);
        long total = 0;
        if (pitems_list)
            ++total;
        if (pexpressions_list)
            ++total;
        if (total > 0)
            plist->InitialiseOffsets(total);

        if (pitems_list) {
            CWideString name = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_ITEMS");
            if (tot_num_of_items > 0) {
                name += CTCInventoryStats::GetXString();
                name += GFIntToWideString(tot_num_of_items);
            }
            CCountedPointer<CComponent> pmenu_entry(pmanager->CreateMenuEntryComponent(name, 0.0f, 1.0f,
                "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS_ROOT", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            CCountedPointer<CComponent> pitems_list_counted(pitems_list);
            pmenu_entry->AddActionOnBack(paction_back_out);
            pmenu_entry->AddChild(pitems_list_counted);
            plist->AddScrollingChildAndSetOffsets(pmenu_entry);
        }
        if (pexpressions_list) {
            CWideString name = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_EXPRESSIONS");
            if (tot_num_of_exp > 0) {
                name += CTCInventoryStats::GetXString();
                name += GFIntToWideString(tot_num_of_exp);
            }
            CCountedPointer<CComponent> pmenu_entry(pmanager->CreateMenuEntryComponent(name, 25.0f, 2.0f / 3.0f,
                "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS_ROOT", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            CCountedPointer<CComponent> pexpressions_list_counted(pexpressions_list);
            pmenu_entry->AddActionOnBack(paction_back_out);
            pmenu_entry->AddChild(pexpressions_list_counted);
            plist->AddScrollingChildAndSetOffsets(pmenu_entry);
        }
        if (pexpressions_list || pitems_list) {
            plist->Initialise();
            plist->ChangeState(5);
            Components.push_back(plist);
            Active = true;
            FirstRenderFrame = true;
            GetPlayerManager().GetMainPlayer().AddMode(PLAYER_MODE_QUICK_ACCESS_MENU, false);
            CPlayerGui::Get().SetSpecialAbilityButtonsAsDisplaying(true, true);
        } else {
            delete plist;
        }
    }
}

#ifdef _MSC_VER
#pragma optimize("y",off)
#endif
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
namespace std {
typedef pair<long, long> QuickAccessInventoryPair;
template<> FABLE_QUICK_MENU_NOINLINE inline void vector<QuickAccessInventoryPair, allocator<QuickAccessInventoryPair> >::_M_insert_overflow(
    iterator position, const QuickAccessInventoryPair& item, const __false_type&, size_type fillLength, bool atEnd)
{
    const size_type oldSize = size();
    const size_type length = oldSize + (max)(oldSize, fillLength);
    QuickAccessInventoryPair* newStart = this->_M_end_of_storage.allocate(length);
    QuickAccessInventoryPair* newFinish = __uninitialized_copy(this->_M_start, position, newStart, __false_type());
    if (fillLength == 1) {
        _Construct(newFinish, item);
        ++newFinish;
    } else {
        newFinish = __uninitialized_fill_n(newFinish, fillLength, item, __false_type());
    }
    if (!atEnd)
        newFinish = __uninitialized_copy(position, this->_M_finish, newFinish, __false_type());
    _M_clear();
    _M_set(newStart, newFinish, newStart + length);
}
}

#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#ifdef _MSC_VER
#pragma optimize("y",off)
#endif
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
namespace std {
typedef pair<long, long> QuickAccessInventoryPair;
template<> FABLE_QUICK_MENU_NOINLINE inline void vector<QuickAccessInventoryPair, allocator<QuickAccessInventoryPair> >::push_back(const QuickAccessInventoryPair& pair)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, pair);
        ++this->_M_finish;
    } else {
        _M_insert_overflow(this->_M_finish, pair, _IsPODType(), 1, true);
    }
}
}

#ifdef _MSC_VER
#pragma optimize("",on)
#endif
// Inline-budget: the two original GetDef instantiations are 98-byte size-optimized copies.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> FABLE_QUICK_MENU_NOINLINE inline bool CDefinitionManager::GetDef<CThingBaseDef>(long index,CDefPointer<const CThingBaseDef>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
        if (def.Get()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}

template<> FABLE_QUICK_MENU_NOINLINE inline bool CDefinitionManager::GetDef<CExpressionDef>(long index,CDefPointer<const CExpressionDef>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
        if (def.Get()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
void CTCQuickAccessMenu::ContructItemLists(std::vector<std::pair<long, long> >& items, NUISystem::CList* list, NUISystem::CComponent* owner,
                                           CCountedPointer<NUISystem::CComponent>& actions_list)
{
    using namespace NUISystem;
    CManager* manager = CManager::GetInstance();
    CTCInventory* inventory = 0;
    ParentThing->GetTC(&inventory);
    for (unsigned long i = 0; i < items.size(); ++i) {
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS",
                                                                            0.0f, "UI_QUICK_ACCESS_EXPRESSION_MENU_ENTRY_TEXT", 0.0f, "PC_BUTTON_BASTARD"));
        inventory->ItemsMenuEntries[items[i].first] = entry.Get();
        if (inventory->QuickAccessItems[2] == items[i].first) {
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_LEFT_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[2] = icon.Get();
        } else if (inventory->QuickAccessItems[3] == items[i].first) {

            // Inline-budget: the original constructor stays out of line at right icon in 0x00626750.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_RIGHT_ITEM_ICON", false));
#ifdef _MSC_VER
#pragma inline_depth()
#endif

            entry->AddChild(icon);
            inventory->AssignedIcons[3] = icon.Get();
        } else if (inventory->QuickAccessItems[0] == items[i].first) {

            // Inline-budget: the original constructor stays out of line at up icon in 0x00626750.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_UP_ITEM_ICON", false));
#ifdef _MSC_VER
#pragma inline_depth()
#endif

            entry->AddChild(icon);
            inventory->AssignedIcons[0] = icon.Get();
        } else if (inventory->QuickAccessItems[1] == items[i].first) {

            // Inline-budget: the original constructor stays out of line at down icon in 0x00626750.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_DOWN_ITEM_ICON", false));
#ifdef _MSC_VER
#pragma inline_depth()
#endif

            entry->AddChild(icon);
            inventory->AssignedIcons[1] = icon.Get();

        // Inline-budget: the down-icon exit calls the original counted-pointer destructor.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        }
#ifdef _MSC_VER
#pragma inline_depth()
#endif

        CMenuEntry* menu_entry = static_cast<CMenuEntry*>(entry.Get());
        CCountedPointer<CAction> load_name(new CAction);
        load_name->ActionType = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
        load_name->UIActionParams.push_back(new CActionParamSLONG(items[i].first));
        load_name->AddParam(entry.Get(), load_name);
        load_name->UIActionParams.push_back(new CActionParamSLONG(1));
        menu_entry->AddActionOnVisible(load_name);
        CCountedPointer<CAction> load_icon(new CAction);
        load_icon->ActionType = UI_ACTION_LOAD_QUICK_MENU_EXPRESSIONS_ICON;
        load_icon->UIActionParams.push_back(new CActionParamSLONG(items[i].first));
        load_icon->AddParam(entry.Get(), load_icon);
        menu_entry->AddActionOnVisible(load_icon);
        menu_entry->AddThingActionParam(items[i].first);
        list->AddScrollingChildAndSetOffsets(entry);
        entry->AddChild(actions_list);
    }
}

inline CWideString FABLE_FASTCALL GFIntToWideString(long value)
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

        // Inline-budget: the original constructor stays out of line at 0x006275C2.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        CCountedPointer<CComponent> plist_cat_component(pmanager->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS",false));
#ifdef _MSC_VER
#pragma inline_depth()
#endif

        CList* plist_cat=static_cast<CList*>(plist_cat_component.Get());
        pmenu_entry_cat->AddChild(plist_cat_component);
        plist_cat->InitialiseOffsets(items.size());

        // Inline-budget: the original constructor stays out of line at 0x00627623.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
        CCountedPointer<CComponent> pactions_list(CManager::GetInstance()->CreateComponent("UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS",false));
#ifdef _MSC_VER
#pragma inline_depth()
#endif

        static_cast<CList*>(pactions_list.Get())->SetDefaultChild(0L);
        ContructItemLists(items,plist_cat,ptop_list,pactions_list);
    }
}
FABLE_QUICK_MENU_NOINLINE CComponent* CTCQuickAccessMenu::GetExpressionsListUI(long& tot_num_of_exp, CComponent* pparent_list)
{
    CManager* pmanager = CManager::GetInstance();
    CTCInventory* ptc_inventory = 0;
    if (!GetParentThing().GetTC(&ptc_inventory))
        return 0;
    std::vector<std::pair<long, long> > items, expressions_catA, expressions_catB, expressions_catC, expressions_catD;
    ptc_inventory->GetAllItems(items);
    tot_num_of_exp = 0;
    for (long count = 0; count < items.size(); ++count) {
        CDefPointer<const CThingBaseDef> pthing_def;
        if (!GetDefinitionManager().GetDef(items[count].first, &pthing_def)) FABLE_RETAIL_ASSERT(false);
        CDefPointer<const CExpressionSubDef> pexpression_sub_def;
        CDefPointer<const CExpressionDef> pexpression_def;
        if (pthing_def->GetSubDef(&pexpression_sub_def) &&
            GetDefinitionManager().GetDef(pexpression_sub_def->ExpressionDef, &pexpression_def)) {
            EExpressionInventoryType exp_type = pexpression_def->ExpressionType;
            switch (exp_type) {
                case EXPRESSION_INVENTORY_SOCIAL: expressions_catA.push_back(items[count]); break;
                case EXPRESSION_INVENTORY_RENOWN: expressions_catB.push_back(items[count]); break;
                case EXPRESSION_INVENTORY_ALIGNMENT: expressions_catC.push_back(items[count]); break;
                case EXPRESSION_INVENTORY_STEALTH: expressions_catD.push_back(items[count]); break;
                default: expressions_catA.push_back(items[count]); break;
            }
            ++tot_num_of_exp;
        }
    }
    CComponent* pexpressions_list_component = 0;
    CList* pexpressions_list = 0;
    long num_submenus = 0;
    if (!expressions_catA.empty()) ++num_submenus;
    if (!expressions_catB.empty()) ++num_submenus;
    if (!expressions_catC.empty()) ++num_submenus;
    if (!expressions_catD.empty()) ++num_submenus;
    if (num_submenus > 0) {
        pexpressions_list_component = pmanager->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS", false);
        pexpressions_list = static_cast<CList*>(pexpressions_list_component);
        pexpressions_list->InitialiseOffsets(4);
        CWideString menuparam;
        menuparam = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_SOCIAL");
        ConstructExpressionCategoryList(menuparam, pexpressions_list, expressions_catA, pparent_list);
        menuparam = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_RENOWN");
        ConstructExpressionCategoryList(menuparam, pexpressions_list, expressions_catB, pparent_list);
        menuparam = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_ALIGNMENT");
        ConstructExpressionCategoryList(menuparam, pexpressions_list, expressions_catC, pparent_list);
        menuparam = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_STEALTH");
        ConstructExpressionCategoryList(menuparam, pexpressions_list, expressions_catD, pparent_list);
    }
    return pexpressions_list;
}