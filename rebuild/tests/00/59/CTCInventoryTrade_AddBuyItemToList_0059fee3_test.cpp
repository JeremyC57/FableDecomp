// Behaviour test for row 0x0059FEE3 CTCInventoryTrade::AddBuyItemToList (links the row's object). The CCountedPointer, STLport
// list / map / vector-map, PeekTC and CActionParamSLONG bodies are the row's own COMDATs; the test supplies the out-of-line
// callees with the row's decorated names (CAction ctor and AddParam, CActionParamString ctor, the strings, CManager, the player
// chain, the trade's private / public checks, bitset::test, the map's _M_insert) and gives the menu entry and the list fake
// vtables whose used slots record the calls (CComponent slots 0 / 67 / 69 / 92, CMenuEntry 151 / 152 / 153, CList 152 / 153).
// Cases (each condition both ways): affordable with room (IsSpecialCase short-circuited), affordable without room but a special
// case, without room and not special, not affordable (the inventory is not asked), quantity 0 and -1, the hero already owning
// the special, the inventory component missing (the space check runs on a null component), and the static / dynamic list add.
// Every case checks the template chosen, MakeUnavailable, the returned flag, the ItemsToBuy record, the seven actions (type,
// pushed SLONG / string parameters, AddParam targets: entry, viewport), AddUIActionParam, the list slot and the final release.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---- strings -------------------------------------------------------------------------------------------------------------
static int g_strings_live;
class CWideString {
public:
    CWideString(const unsigned short* text);
    ~CWideString();
    const unsigned short* Text;
};
CWideString::CWideString(const unsigned short* text) : Text(text) { ++g_strings_live; }
CWideString::~CWideString() { --g_strings_live; }
class CCharString {
public:
    CCharString(const char* text, long length);
    ~CCharString();
    const char* Text;
};
static int g_string_length_bad;
CCharString::CCharString(const char* text, long length) : Text(text) { ++g_strings_live; if (length != -1) ++g_string_length_bad; }
CCharString::~CCharString() { --g_strings_live; }

static int WideEquals(const unsigned short* a, const char* b)
{
    while (*b) if (*a++ != (unsigned short)*b++) return 0;
    return *a == 0;
}

// ---- counted pointers, actions, parameters ------------------------------------------------------------------------------
template<class T> class CCountedPointer { public: T* Data; struct CCPInfo { unsigned long RefCount; }* Info; };
struct ListNode { ListNode* Next; ListNode* Prev; void* Data; };

namespace NUISystem {
enum EActionParamType { UI_ACTION_PARAM_TYPE_NONE = 0, UI_ACTION_PARAM_TYPE_SLONG = 3, UI_ACTION_PARAM_TYPE_STRING = 4 };
class CComponent;
class CList;
class CViewport;
class CActionParam {
public:
    CActionParam(EActionParamType type) : ActionParamType(type) { }
    virtual ~CActionParam() { }
    EActionParamType ActionParamType;
};
class CActionParamSLONG : public CActionParam {
public:
    virtual ~CActionParamSLONG();
    long Value;
};
CActionParamSLONG::~CActionParamSLONG() { }
class CActionParamString : public CActionParam {
public:
    CActionParamString(CWideString& value);
    virtual ~CActionParamString() { }
    const unsigned short* ValueText;                // the CWideString value's text (kept as a pointer: the test never frees params)
};
CActionParamString::CActionParamString(CWideString& value) : CActionParam(UI_ACTION_PARAM_TYPE_STRING), ValueText(value.Text) { }

class CAction {
public:
    CAction();
    void AddParam(CActionParam* param, CCountedPointer<CAction>& self);
    long ActionType;
    ListNode* UIActionParams;
    ListNode* ThingActionParams;
    bool Valid;
};
static ListNode* NewListHead()
{
    ListNode* head = (ListNode*)malloc(sizeof(ListNode));
    head->Next = head->Prev = head;
    return head;
}
CAction::CAction() : ActionType(0), UIActionParams(NewListHead()), ThingActionParams(NewListHead()), Valid(true) { }

// AddParam calls wait here until the action is handed to the entry.
struct PendingParam { CAction* Action; CActionParam* Param; };
static PendingParam g_pending[16];
static int g_npending, g_addparam_bad_self;
void CAction::AddParam(CActionParam* param, CCountedPointer<CAction>& self)
{
    if (self.Data != this) ++g_addparam_bad_self;
    g_pending[g_npending].Action = this;
    g_pending[g_npending].Param = param;
    ++g_npending;
}
}
using namespace NUISystem;

// ---- the fake menu entry / list ---------------------------------------------------------------------------------------------
struct FakeObject { void** VTable; unsigned char Body[0x40]; };   // CComponent: CActionParam base at +0x18
enum { ON_SELECTED = 1, ON_UNSELECTED, PRE_ACTION, ON_VISIBLE };
struct ActionRecord {
    int Where;
    FakeObject* Entry;
    long Type;
    int NParams;
    long ParamTypes[4];
    long ParamValues[4];
    const unsigned short* ParamTexts[4];
    int NAdded;
    void* Added[4];
};
static ActionRecord g_actions[16];
static int g_nactions, g_trap, g_unavailable, g_deleted, g_ui_params, g_list_adds, g_list_dynamic;
static void* g_ui_param;
static FakeObject* g_list_added;
static unsigned long g_list_refcount;
static int g_order[8], g_norder;                // 1 MakeUnavailable, 2 ItemsToBuy insert, 3 first action, 4 list add

static void Snapshot(int where, FakeObject* entry, CCountedPointer<CAction>* action)
{
    if (g_nactions == 0 && g_norder < 8) g_order[g_norder++] = 3;
    ActionRecord& r = g_actions[g_nactions++];
    memset(&r, 0, sizeof(r));
    r.Where = where;
    r.Entry = entry;
    r.Type = action->Data->ActionType;
    ListNode* head = action->Data->UIActionParams;
    for (ListNode* n = head->Next; n != head && r.NParams < 4; n = n->Next) {
        CActionParam* p = (CActionParam*)n->Data;
        r.ParamTypes[r.NParams] = p->ActionParamType;
        if (p->ActionParamType == UI_ACTION_PARAM_TYPE_SLONG)
            r.ParamValues[r.NParams] = ((CActionParamSLONG*)p)->Value;
        else
            r.ParamTexts[r.NParams] = ((CActionParamString*)p)->ValueText;
        ++r.NParams;
    }
    int kept = 0;
    for (int i = 0; i < g_npending; ++i) {
        if (g_pending[i].Action == action->Data && r.NAdded < 4)
            r.Added[r.NAdded++] = g_pending[i].Param;
        else
            g_pending[kept++] = g_pending[i];
    }
    g_npending = kept;
}
static void __fastcall Trap(FakeObject*, int) { g_trap = 1; }
static void* __fastcall EntryDelete(FakeObject* self, int, unsigned int flags) { if (flags == 1) ++g_deleted; return self; }
static void __fastcall EntryOnSelected(FakeObject* self, int, CCountedPointer<CAction>* a) { Snapshot(ON_SELECTED, self, a); }
static void __fastcall EntryPreAction(FakeObject* self, int, CCountedPointer<CAction>* a) { Snapshot(PRE_ACTION, self, a); }
static void __fastcall EntryMakeUnavailable(FakeObject*, int) { ++g_unavailable; if (g_norder < 8) g_order[g_norder++] = 1; }
static void __fastcall EntryAddUIActionParam(FakeObject*, int, CActionParam* param) { ++g_ui_params; g_ui_param = param; }
static void __fastcall EntryOnUnselected(FakeObject* self, int, CCountedPointer<CAction>* a) { Snapshot(ON_UNSELECTED, self, a); }
static void __fastcall EntryOnVisible(FakeObject* self, int, CCountedPointer<CAction>* a) { Snapshot(ON_VISIBLE, self, a); }
static void ListAdd(CCountedPointer<CComponent>* entry, int dynamic)
{
    ++g_list_adds;
    g_list_dynamic = dynamic;
    g_list_added = (FakeObject*)entry->Data;
    g_list_refcount = entry->Info ? entry->Info->RefCount : 0;
    if (g_norder < 8) g_order[g_norder++] = 4;
}
static void __fastcall ListAddStatic(FakeObject*, int, CCountedPointer<CComponent>* entry) { ListAdd(entry, 0); }
static void __fastcall ListAddDynamic(FakeObject*, int, CCountedPointer<CComponent>* entry) { ListAdd(entry, 1); }

static void* g_entry_vtable[160];
static void* g_list_vtable[160];
static FakeObject g_entries[4];
static int g_nentries;
static FakeObject g_list, g_viewport;
static const char* g_template;

namespace NUISystem {
// The `const` on the by-value text parameter is in the retail declaration (its mangling spells the last parameter out as V5@ instead of
// a back-reference to the earlier by-value CCharString).
class CManager {
public:
    static CManager* __fastcall GetInstance();
    CComponent* CreateMenuEntryComponent(const CWideString& name, float a, float b, const CCharString& definition, float c,
                                         const CCharString text, float d, CCharString button);
};
}
static int g_menu_args_bad;
static CManager* g_manager = (CManager*)&g_list;
CManager* __fastcall CManager::GetInstance() { return g_manager; }
CComponent* CManager::CreateMenuEntryComponent(const CWideString& name, float a, float b, const CCharString& definition, float c,
                                               const CCharString text, float d, CCharString button)
{
    if (this != g_manager || !WideEquals(name.Text, "INVALID NAME") || a != 0.0f || b != 1.0f || c != 0.0f || d != 0.0f
        || strcmp(text.Text, "") != 0 || strcmp(button.Text, "PC_BUTTON_BASTARD") != 0)
        ++g_menu_args_bad;
    g_template = definition.Text;
    FakeObject* entry = &g_entries[g_nentries++];
    memset(entry, 0, sizeof(*entry));
    entry->VTable = g_entry_vtable;
    return (CComponent*)entry;
}

// ---- the player chain and the inventory component ---------------------------------------------------------------------------
enum ETCInterfaceType { TC_TEST_INVENTORY = 77, TC_TEST_OTHER = 5 };
class CTCInventoryItem { public: static ETCInterfaceType __fastcall GetInventoryType(long def_index); };
static long g_type_asked;
ETCInterfaceType __fastcall CTCInventoryItem::GetInventoryType(long def_index) { g_type_asked = def_index; return TC_TEST_INVENTORY; }

namespace NInventory {
class CTCInventoryBase { public: bool IsSpaceInInventoryForItem(long def_index) const; bool Room; };
}
static int g_space_calls;
static const NInventory::CTCInventoryBase* g_space_this;
bool NInventory::CTCInventoryBase::IsSpaceInInventoryForItem(long) const
{
    ++g_space_calls;
    g_space_this = this;
    return this != 0 && Room;
}
static NInventory::CTCInventoryBase g_inventory, g_other_component;

struct TCPair { long Type; void* Component; };
struct FakeThing {
    unsigned char Pad[0x20];
    unsigned long Bits[9];                          // +0x20 std::bitset<274>
    TCPair* Begin;                                  // +0x44 the sorted {type, component} vector map
    TCPair* End;                                    // +0x48
    TCPair* Capacity;                               // +0x4C
    TCPair Pairs[2];
};
static FakeThing g_thing;
namespace std {
template<unsigned int N> class bitset { public: bool test(unsigned int pos) const; unsigned long Words[(N + 31) / 32]; };
template<> bool bitset<274>::test(unsigned int pos) const { return (Words[pos >> 5] >> (pos & 31) & 1) != 0; }
}
class CThingPlayerCreature;
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; int Dummy; };
const CThingPlayerCreature* CPlayer::PeekPControlledCreature() const { return (const CThingPlayerCreature*)&g_thing; }
class CPlayerManager { public: const CPlayer& GetMainPlayer() const; CPlayer MainPlayer; };
const CPlayer& CPlayerManager::GetMainPlayer() const { return MainPlayer; }
class CMainGameComponent { protected: static CMainGameComponent* PMainGameComponent; };
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;
struct GameAccess : CMainGameComponent { static void Set(void* game) { PMainGameComponent = (CMainGameComponent*)game; } };
struct FakeGame { unsigned char Pad[0x1C]; CPlayerManager* PlayerManager; };   // PeekPlayerManager reads +0x1C
static CPlayerManager g_player_manager;
static FakeGame g_game;

// ---- the ItemsToBuy map ------------------------------------------------------------------------------------------------------
namespace std {
template<class T1, class T2> struct pair { T1 first; T2 second; };
template<class T> struct less { };
template<class P> struct _Select1st { };
template<class T> class allocator { };
template<class T> struct _Nonconst_traits { };
struct _Rb_tree_node_base { char _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
template<class V, class Tr> struct _Rb_tree_iterator { _Rb_tree_iterator(_Rb_tree_node_base* node) : _M_node(node) { } _Rb_tree_node_base* _M_node; };
template<class K, class V, class KoV, class C, class A> class _Rb_tree {
    _Rb_tree_iterator<V, _Nonconst_traits<V> > _M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const V& v, _Rb_tree_node_base* w);
public:
    _Rb_tree_node_base* _M_header;
    unsigned int _M_node_count;
};
}
typedef std::pair<CComponent* const, std::pair<long, long> > BuyValue;
typedef std::_Rb_tree<CComponent*, BuyValue, std::_Select1st<BuyValue>, std::less<CComponent*>, std::allocator<BuyValue> > BuyTree;
static int g_inserts;
static CComponent* g_insert_entry;
static long g_insert_def, g_insert_quantity;
template<> std::_Rb_tree_iterator<BuyValue, std::_Nonconst_traits<BuyValue> > BuyTree::_M_insert(std::_Rb_tree_node_base*,
    std::_Rb_tree_node_base* y, const BuyValue& v, std::_Rb_tree_node_base*)
{
    ++g_inserts;
    g_insert_entry = v.first; g_insert_def = v.second.first; g_insert_quantity = v.second.second;
    if (g_norder < 8) g_order[g_norder++] = 2;
    return std::_Rb_tree_iterator<BuyValue, std::_Nonconst_traits<BuyValue> >(y);   // the test keeps the tree empty
}

// ---- the trade ------------------------------------------------------------------------------------------------------------------
class CTCInventoryTrade {
public:
    bool AddBuyItemToList(long item_def, long quantity, CList* pbuyable_list, CViewport* pviewport, bool dynamic);
    bool IsItemAffordable(long item_def, long count);
    bool IsSpecialCase(long item_def);
    unsigned char Pad[0x144];
    BuyTree ItemsToBuy;                             // +0x144
    bool Affordable, Special, HaveSpecial;
private:
    bool AlreadyHaveSpecial(long item_def);
};
static int g_affordable_calls, g_special_calls, g_have_calls, g_check_args_bad;
static long g_item;
bool CTCInventoryTrade::IsItemAffordable(long item_def, long count)
{
    ++g_affordable_calls;
    if (item_def != g_item || count != 1) ++g_check_args_bad;
    return Affordable;
}
bool CTCInventoryTrade::IsSpecialCase(long item_def) { ++g_special_calls; if (item_def != g_item) ++g_check_args_bad; return Special; }
bool CTCInventoryTrade::AlreadyHaveSpecial(long item_def)
{
    ++g_have_calls;
    if (item_def != g_item || g_affordable_calls != 0) ++g_check_args_bad;   // asked first, before IsItemAffordable
    return HaveSpecial;
}

// ---- one call ---------------------------------------------------------------------------------------------------------------------
static int Fail(int test_case, const char* what)
{
    printf("0059fee3_TEST FAIL case %d: %s\n", test_case, what);
    return 0;
}

static int CheckAction(int test_case, int index, int where, long type, int nslong, long v0, long v1, const char* text, void* add0, void* add1)
{
    const ActionRecord& r = g_actions[index];
    int nparams = nslong + (text ? 1 : 0);
    int nadded = add1 ? 2 : 1;
    if (r.Where != where || r.Type != type || r.Entry != &g_entries[0]) return Fail(test_case, "action target / slot / type");
    if (r.NParams != nparams) return Fail(test_case, "action parameter count");
    if (nslong > 0 && (r.ParamTypes[0] != UI_ACTION_PARAM_TYPE_SLONG || r.ParamValues[0] != v0)) return Fail(test_case, "first SLONG parameter");
    if (nslong > 1 && (r.ParamTypes[1] != UI_ACTION_PARAM_TYPE_SLONG || r.ParamValues[1] != v1)) return Fail(test_case, "second SLONG parameter");
    if (text && (r.ParamTypes[nslong] != UI_ACTION_PARAM_TYPE_STRING || !WideEquals(r.ParamTexts[nslong], text)))
        return Fail(test_case, "string parameter");
    if (r.NAdded != nadded || r.Added[0] != add0 || (add1 && r.Added[1] != add1)) return Fail(test_case, "AddParam targets");
    return 1;
}

// space: 1 room, 0 no room, -1 no inventory component on the hero.
static int Run(int test_case, bool affordable, int space, bool special, long quantity, bool have_special, bool dynamic,
               bool expect_branch)
{
    static const long item = 1234;
    CTCInventoryTrade trade;
    memset(&trade, 0, sizeof(trade));
    std::_Rb_tree_node_base header = { 0, 0, 0, 0 };
    header._M_left = header._M_right = &header;
    trade.ItemsToBuy._M_header = &header;
    trade.Affordable = affordable; trade.Special = special; trade.HaveSpecial = have_special;
    g_item = item;

    memset(&g_thing, 0, sizeof(g_thing));
    g_thing.Pairs[0].Type = TC_TEST_OTHER; g_thing.Pairs[0].Component = &g_other_component;
    g_thing.Pairs[1].Type = TC_TEST_INVENTORY; g_thing.Pairs[1].Component = &g_inventory;
    g_thing.Begin = g_thing.Pairs; g_thing.End = g_thing.Pairs + 2; g_thing.Capacity = g_thing.Pairs + 2;
    g_thing.Bits[TC_TEST_OTHER >> 5] |= 1UL << (TC_TEST_OTHER & 31);
    if (space >= 0) g_thing.Bits[TC_TEST_INVENTORY >> 5] |= 1UL << (TC_TEST_INVENTORY & 31);
    g_inventory.Room = space > 0;

    g_nentries = g_nactions = g_npending = g_norder = 0;
    g_trap = g_unavailable = g_deleted = g_ui_params = g_list_adds = g_list_dynamic = g_inserts = 0;
    g_affordable_calls = g_special_calls = g_have_calls = g_space_calls = 0;
    g_space_this = (const NInventory::CTCInventoryBase*)1;
    g_template = 0; g_list_added = 0; g_ui_param = 0; g_type_asked = 0;

    bool result = trade.AddBuyItemToList(item, quantity, (CList*)&g_list, (CViewport*)&g_viewport, dynamic);

    FakeObject* entry = &g_entries[0];
    void* entry_param = (unsigned char*)entry + 0x18;
    void* viewport_param = (unsigned char*)&g_viewport + 0x18;
    if (g_trap) return Fail(test_case, "unexpected virtual slot called");
    if (result != expect_branch) return Fail(test_case, "returned flag");
    if (g_nentries != 1 || g_menu_args_bad || g_string_length_bad) return Fail(test_case, "menu entry creation arguments");
    if (strcmp(g_template, expect_branch ? "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW" : "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW") != 0)
        return Fail(test_case, "menu entry template");
    if (g_unavailable != (expect_branch ? 0 : 1)) return Fail(test_case, "MakeUnavailable");
    if (g_have_calls != 1 || g_affordable_calls != 1 || g_check_args_bad) return Fail(test_case, "AlreadyHaveSpecial / IsItemAffordable calls");
    if (g_type_asked != item) return Fail(test_case, "inventory type of the item");
    if (g_space_calls != (affordable ? 1 : 0)) return Fail(test_case, "IsSpaceInInventoryForItem short-circuit");
    if (affordable && g_space_this != (space >= 0 ? &g_inventory : 0)) return Fail(test_case, "inventory component looked up");
    if (g_special_calls != (affordable && space <= 0 ? 1 : 0)) return Fail(test_case, "IsSpecialCase short-circuit");
    if (g_inserts != 1 || g_insert_entry != (CComponent*)entry || g_insert_def != item || g_insert_quantity != quantity)
        return Fail(test_case, "ItemsToBuy record");
    if (g_nactions != 7 || g_npending != 0 || g_addparam_bad_self) return Fail(test_case, "action count");
    int ok = CheckAction(test_case, 0, ON_SELECTED, 2000, 1, item, 0, "UI_ITEM_MODEL_TEMPLATE_NEW", entry_param, 0)
        && CheckAction(test_case, 1, ON_UNSELECTED, 2015, 0, 0, 0, 0, entry_param, 0)
        && CheckAction(test_case, 2, ON_SELECTED, 2009, 1, item, 0, 0, viewport_param, entry_param)
        && CheckAction(test_case, 3, ON_UNSELECTED, 2021, 0, 0, 0, 0, entry_param, 0)
        && CheckAction(test_case, 4, PRE_ACTION, 2010, 1, item, 0, 0, entry_param, 0)
        && CheckAction(test_case, 5, ON_UNSELECTED, 2022, 0, 0, 0, 0, entry_param, 0)
        && CheckAction(test_case, 6, ON_VISIBLE, 2006, 2, item, quantity, 0, entry_param, 0);
    if (!ok) return 0;
    if (g_ui_params != 1 || g_ui_param != entry_param) return Fail(test_case, "AddUIActionParam");
    if (g_list_adds != 1 || g_list_dynamic != (dynamic ? 1 : 0) || g_list_added != entry || g_list_refcount != 1)
        return Fail(test_case, "list add slot / entry");
    int o = 0;
    if (!expect_branch && g_order[o++] != 1) return Fail(test_case, "MakeUnavailable before the record");
    if (g_norder != o + 3 || g_order[o] != 2 || g_order[o + 1] != 3 || g_order[o + 2] != 4) return Fail(test_case, "call order");
    if (g_deleted != 1) return Fail(test_case, "menu entry released once at the end");
    if (g_strings_live != 0) return Fail(test_case, "string temporaries");
    return 1;
}

int main()
{
    int i;
    for (i = 0; i < 160; ++i) { g_entry_vtable[i] = (void*)&Trap; g_list_vtable[i] = (void*)&Trap; }
    g_entry_vtable[0] = (void*)&EntryDelete;
    g_entry_vtable[67] = (void*)&EntryOnSelected;            // CComponent::AddActionOnSelected  +0x10C
    g_entry_vtable[69] = (void*)&EntryPreAction;             // CComponent::AddPreAction         +0x114
    g_entry_vtable[92] = (void*)&EntryMakeUnavailable;       // CComponent::MakeUnavailable      +0x170
    g_entry_vtable[151] = (void*)&EntryAddUIActionParam;     // CMenuEntry::AddUIActionParam     +0x25C
    g_entry_vtable[152] = (void*)&EntryOnUnselected;         // CMenuEntry::AddActionOnUnselected +0x260
    g_entry_vtable[153] = (void*)&EntryOnVisible;            // CMenuEntry::AddActionOnVisible   +0x264
    g_list_vtable[152] = (void*)&ListAddStatic;              // CList::AddScrollingChildAndSetOffsets +0x260
    g_list_vtable[153] = (void*)&ListAddDynamic;             // CList::AddScrollingChildDynamicallyAndSetOffsets +0x264
    g_list.VTable = g_list_vtable;
    g_viewport.VTable = 0;
    g_game.PlayerManager = &g_player_manager;
    GameAccess::Set(&g_game);

    int ok = 1;
    //            case affordable space special quantity have  dynamic  branch
    ok &= Run(1, true,  1,  false, 2,  false, false, true);    // affordable, room: IsSpecialCase not asked
    ok &= Run(2, true,  0,  true,  1,  false, true,  true);    // no room but a special case
    ok &= Run(3, true,  0,  false, 1,  false, false, false);   // no room, not special
    ok &= Run(4, false, 1,  true,  5,  false, true,  false);   // not affordable: the inventory is not asked
    ok &= Run(5, true,  1,  false, 0,  false, false, false);   // none in stock
    ok &= Run(6, true,  1,  false, -1, false, true,  false);   // negative stock
    ok &= Run(7, true,  1,  false, 1,  true,  false, false);   // the hero already owns this special
    ok &= Run(8, true,  -1, true,  3,  false, true,  true);    // no inventory component: the space check runs on null
    ok &= Run(9, true,  -1, false, 3,  false, false, false);   // no inventory component, not special
    if (!ok) return 1;
    printf("0059fee3_TEST PASS\n");
    return 0;
}