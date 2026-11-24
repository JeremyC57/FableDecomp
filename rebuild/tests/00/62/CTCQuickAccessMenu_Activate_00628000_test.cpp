// Behavior fixture for the reconstructed quick access menu activation.
// CTCQuickAccessMenu::Activate
// Leaf: ?Activate@CTCQuickAccessMenu@@QAEXXZ
// Flags:
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
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CACTION_ORIGINAL_DTOR_DECL_PASS26
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define private public
#define protected public
#include "fable_assert.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCQuickAccessMenu.h"
#include "engine/CTCInventoryStats.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/GFGetTime.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);

#undef private
#undef protected

using namespace NUISystem;

inline bool CTCQuickAccessMenu::IsActive() const { return Active; }

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

#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

template<class T> union Storage {
    unsigned __int64 align;
    unsigned char bytes[sizeof(T)];
    T* Get() { return reinterpret_cast<T*>(bytes); }
};
template<class T> static void Init(T& object) { new (&object) T; }
static void Assert(bool condition, int line)
{
    if (!condition) {
        printf("check failed on line %d\n",line);
        fflush(stdout);
        abort();
    }
}
#define Check(condition) Assert(condition, __LINE__)

// Each scenario seeds an empty map with one node. No rotation is needed.
template<class Dummy> void __cdecl std::_Rb_global<Dummy>::_Rebalance(
    std::_Rb_tree_node_base* node, std::_Rb_tree_node_base*& root)
{
    Check(root == node && !node->_M_left && !node->_M_right);
    node->_M_color = true;
}

static CTCQuickAccessMenu* menu;
static CTCInventory* inventory;
static CManager* manager;
static CList* root_list;
static CComponent* submenu[2];
static CComponent* entries[2];
static CPlayerManager* player_manager;
static CPlayer* player;
static CPlayerGui* player_gui;
static NGameText::CDataBank* text_bank;
static bool available[2];
static long item_count[2];
static unsigned times, managers, parameters_cleared, inventory_set, root_creates, offset_count,
                text_read, x_strings, number_conversions, list_reads, entry_creates, backs, children,
                scroll_adds, initialized, changed, player_modes, gui_calls, delete_root, action_dtors;
static CComponent* current_entry;
static wchar_t captions[2][50];
static wchar_t strings[32][50];
static unsigned string_count;

CMainGameComponent* CMainGameComponent::PMainGameComponent;
template<> CCountedPointer<CPlayerGui> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass = CCountedPointer<CPlayerGui>();

static wchar_t* StringBuffer()
{
    Check(string_count < sizeof(strings)/sizeof(strings[0]));
    return strings[string_count++];
}

CCharString::CCharString() : PStringData(0) { }
CCharString::CCharString(const char* text, long length) : PStringData((CCharStringData*)text) { Check(length == -1); }
CCharString::CCharString(const CCharString& str) : PStringData(str.PStringData) { }
CCharString::~CCharString() { }
static const char* Text(const CCharString& str) { return reinterpret_cast<const char*>(str.PStringData); }

CWideString::CWideString() : PStringData(0) { }
CWideString::CWideString(const wchar_t* text) : PStringData((CWideStringData*)text) { }
CWideString::CWideString(const CWideString& str) : PStringData(str.PStringData) { }
CWideString::~CWideString() { }
static const wchar_t* Wide(const CWideString& str) { return reinterpret_cast<const wchar_t*>(str.PStringData); }

const CWideString& CWideString::operator+=(const CWideString& str)
{
    wchar_t* next = StringBuffer();
    Check(wcslen(Wide(*this)) + wcslen(Wide(str)) < 50);
    wcscpy(next, Wide(*this));
    wcscat(next, Wide(str));
    PStringData = (CWideStringData*)next;
    return *this;
}
const CWideString NGameText::CDataBank::GetTextBySymbol(const CCharString& tag) const
{
    Check(this == text_bank);
    ++text_read;
    if (strcmp(Text(tag), "TEXT_GUI_MENU_ITEMS") == 0)
        return CWideString(L"Items");
    Check(strcmp(Text(tag), "TEXT_GUI_MENU_EXPRESSIONS") == 0);
    return CWideString(L"Expressions");
}
CWideString FABLE_FASTCALL CTCInventoryStats::GetXString() { ++x_strings; return CWideString(L" x"); }
CWideString FABLE_FASTCALL GFIntToWideString(long n)
{
    ++number_conversions;
    wchar_t* next = StringBuffer();
    swprintf(next, L"%ld", n);
    return CWideString(next);
}
void CIVCountedPointeeBase::Delete() const { abort(); }
CAction::CAction() : ActionType(UI_ACTION_TYPE_NONE), Valid(true) { }
CAction::~CAction() { ++action_dtors; }

double FABLE_FASTCALL GFGetTime() { ++times; return 91.25; }
CManager* FABLE_FASTCALL CManager::GetInstance() { ++managers; return manager; }

static void FABLE_FASTCALL ClearParams(CManager* self, void*)
{
    Check(self == manager && inventory->GetItemsMenuEntries().size() == 0 && inventory->GetAssignedIcons().size() == 0);
    ++parameters_cleared;
}
static void FABLE_FASTCALL SetInventory(CManager* self, void*, CTCInventory* selected)
{
    Check(self == manager && selected == inventory && parameters_cleared == 1);
    ++inventory_set;
}
CComponent* CManager::CreateComponent(const CCharString& name, bool attach)
{
    Check(this == manager && !attach && inventory_set == 1 && !strcmp(Text(name), "UI_LIST_QUICK_ACCESS_MENU"));
    ++root_creates;
    return root_list;
}
CComponent* CManager::CreateMenuEntryComponent(const CWideString& name, float angle, float scale,
    const CCharString& format, float a, const CCharString b, float c, CCharString d)
{
    Check(this == manager && a == 0.0f && c == 0.0f && !strcmp(Text(b), "") && !strcmp(Text(d), "PC_BUTTON_BASTARD"));
    Check(!strcmp(Text(format), "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS_ROOT"));
    unsigned kind = angle == 0.0f ? 0 : 1;
    Check(available[kind] && (kind == 0 ? scale == 1.0f : angle == 25.0f && scale == 2.0f/3.0f));
    wcscpy(captions[kind], Wide(name));
    ++entry_creates;
    current_entry = entries[kind];
    return current_entry;
}

CComponent* CTCQuickAccessMenu::GetItemsListUI(long& count, CComponent* root)
{
    Check(this == menu && root == root_list && list_reads == 0);
    ++list_reads;
    count = item_count[0];
    return available[0] ? submenu[0] : 0;
}
CComponent* CTCQuickAccessMenu::GetExpressionsListUI(long& count, CComponent* root)
{
    Check(this == menu && root == root_list && list_reads == 1);
    ++list_reads;
    count = item_count[1];
    return available[1] ? submenu[1] : 0;
}

static void FABLE_FASTCALL Offsets(CList* self, void*, unsigned long count)
{
    Check(self == root_list && count == unsigned(available[0]+available[1]) && count > 0);
    offset_count = count;
}
static void FABLE_FASTCALL Back(CComponent* self, void*, CCountedPointer<CAction>& action)
{
    Check(self == current_entry && action->ActionType == UI_ACTION_TYPE_CLOSE_QUICK_ACCESS_MENU);
    ++backs;
}
static void FABLE_FASTCALL Child(CComponent* self, void*, CCountedPointer<CComponent>& child)
{
    Check(self == current_entry && child.Get() == submenu[self == entries[0] ? 0 : 1]);
    ++children;
}
static void FABLE_FASTCALL Scroll(CList* self, void*, CCountedPointer<CComponent>& child)
{
    Check(self == root_list && child.Get() == current_entry);
    ++scroll_adds;
}
static void FABLE_FASTCALL Initialise(CList* self, void*) { Check(self == root_list); ++initialized; }
static void FABLE_FASTCALL Change(CList* self, void*, unsigned long state)
{
    Check(self == root_list && initialized == 1 && state == 5);
    ++changed;
}
static void* FABLE_FASTCALL DeleteComponent(CComponent* self, void*, unsigned long flags)
{
    Check((flags & 1) != 0);
    if (self == root_list)
        ++delete_root;
    return self;
}

CPlayerManager& CTCBase::GetPlayerManager() { return *player_manager; }
CPlayer& CPlayerManager::GetMainPlayer() { Check(this == player_manager); return *player; }
void CPlayer::AddMode(EPlayerMode mode, bool flag)
{
    Check(this == player && !flag && changed == 1 && mode == PLAYER_MODE_QUICK_ACCESS_MENU);
    ++player_modes;
}
void CPlayerGui::SetSpecialAbilityButtonsAsDisplaying(bool a, bool b)
{
    Check(this == player_gui && a && b && player_modes == 1);
    ++gui_calls;
}

static void Reset(bool active, bool items, bool expressions, long n_items, long n_expr)
{
    available[0]=items; available[1]=expressions; item_count[0]=n_items; item_count[1]=n_expr;
    times = managers = parameters_cleared = inventory_set = root_creates = offset_count = 0;
    text_read = x_strings = number_conversions = list_reads = entry_creates = backs = children = 0;
    scroll_adds = initialized = changed = player_modes = gui_calls = delete_root = action_dtors = string_count = 0;
    captions[0][0] = captions[1][0] = 0;
    menu->Active = active;
    menu->FirstRenderFrame = false;
    menu->LastRenderTime = 3.0;
    menu->Components.clear();
    inventory->GetItemsMenuEntries().insert(std::pair<const long, CComponent*>(7, submenu[0]));
    inventory->GetAssignedIcons().insert(std::pair<const long, CComponent*>(2, submenu[1]));
}

static void CheckResult(unsigned count, unsigned number_suffixes)
{
    Check(menu->LastRenderTime == 91.25 && times == 1 && managers == 3);
    Check(parameters_cleared == 1 && inventory_set == 1 && root_creates == 1 && list_reads == 2 && action_dtors == 1);
    Check(offset_count == count && entry_creates == count && backs == count && children == count && scroll_adds == count);
    Check(text_read == count && x_strings == number_suffixes && number_conversions == number_suffixes);
    Check(menu->Active == (count != 0) && menu->FirstRenderFrame == (count != 0));
    Check(initialized == (count != 0) && changed == (count != 0) && player_modes == (count != 0) && gui_calls == (count != 0));
    Check(menu->Components.size() == (count != 0) && delete_root == (count == 0));
    if (count)
        Check(menu->Components.front() == root_list);
}

int main()
{
    static Storage<CThing> actor;
    static Storage<CTCInventory> inv;
    static Storage<CTCQuickAccessMenu> qmenu;
    static Storage<CList> root;
    static Storage<CComponent> subs[2], menu_entries[2];
    static Storage<CManager> m;
    static Storage<CMainGameComponent> gc;
    static Storage<NGameText::CDataBank> bank;
    static Storage<CPlayerManager> pm;
    static Storage<CPlayer> pp;
    static Storage<CPlayerGui> pg;
    static void* root_virtuals[0x270/4+1];
    static void* component_virtuals[0x110/4+1];
    static void* manager_virtuals[0x74/4+1];

    menu = qmenu.Get();
    inventory = inv.Get();
    root_list = root.Get();
    manager = m.Get();
    text_bank = bank.Get();
    player = pp.Get();
    player_gui = pg.Get();
    player_manager = pm.Get();
    CMainGameComponent::PMainGameComponent = gc.Get();
    *reinterpret_cast<NGameText::CDataBank**>(gc.Get()->PTextBank) = text_bank;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Data = player_gui;
    manager_virtuals[0x74/4] = (void*)&ClearParams;
    manager_virtuals[0x54/4] = (void*)&SetInventory;
    root_virtuals[0x270/4] = (void*)&Offsets;
    root_virtuals[0x260/4] = (void*)&Scroll;
    root_virtuals[0xAC/4] = (void*)&Initialise;
    root_virtuals[0xC0/4] = (void*)&Change;
    root_virtuals[0] = (void*)&DeleteComponent;
    component_virtuals[0x110/4] = (void*)&Back;
    component_virtuals[0xEC/4] = (void*)&Child;
    component_virtuals[0] = (void*)&DeleteComponent;
    *reinterpret_cast<void***>(manager) = manager_virtuals;
    *reinterpret_cast<void***>(root_list) = root_virtuals;
    for (unsigned j=0; j<2; ++j) {
        submenu[j] = subs[j].Get();
        entries[j] = menu_entries[j].Get();
        *reinterpret_cast<void***>(submenu[j]) = component_virtuals;
        *reinterpret_cast<void***>(entries[j]) = component_virtuals;
    }
    Init(menu->Components);
    Init(inventory->GetItemsMenuEntries());
    Init(inventory->GetAssignedIcons());
    menu->ParentThing = actor.Get();
    Init(actor.Get()->UsingComponents);
    Init(actor.Get()->Components);
    actor.Get()->UsingComponents[TCI_INVENTORY] = true;
    actor.Get()->Components.push_back(std::make_pair(TCI_INVENTORY, static_cast<CTCBase*>(inventory)));

    Reset(true, true, true, 7, 3);
    menu->Activate();
    Check(!times && !managers && inventory->GetItemsMenuEntries().size() == 1);
    Check(!menu->FirstRenderFrame && menu->LastRenderTime == 3.0);

    Reset(false, false, false, 0, 0);
    menu->Activate();
    CheckResult(0, 0);

    Reset(false, true, false, 7, 0);
    menu->Activate();
    CheckResult(1, 1);
    Check(wcscmp(captions[0], L"Items x7") == 0);

    Reset(false, false, true, 0, 3);
    menu->Activate();
    CheckResult(1, 1);
    Check(wcscmp(captions[1], L"Expressions x3") == 0);

    Reset(false, true, true, 0, 0);
    menu->Activate();
    CheckResult(2, 0);
    Check(wcscmp(captions[0], L"Items") == 0 && wcscmp(captions[1], L"Expressions") == 0);

    Reset(false, true, true, -7, 3);
    menu->Activate();
    CheckResult(2, 1);
    Check(wcscmp(captions[0], L"Items") == 0 && wcscmp(captions[1], L"Expressions x3") == 0);

    Reset(false, true, true, 7, -3);
    menu->Activate();
    CheckResult(2, 1);
    Check(wcscmp(captions[0], L"Items x7") == 0 && wcscmp(captions[1], L"Expressions") == 0);

    puts("00628000_TEST PASS");
    return 0;
}