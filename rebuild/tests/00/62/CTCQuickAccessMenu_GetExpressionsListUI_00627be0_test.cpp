// Behavior fixture for the reconstructed expression submenu.
// Produce the category entries for available quick-access expressions.
// Leaf: ?GetExpressionsListUI@CTCQuickAccessMenu@@AAEPAVCComponent@NUISystem@@AAJPAV23@@Z
// Flags:
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
#define FABLE_RETAIL_CTCQUICKACCESSMENU_EXPRESSIONS
#define FABLE_RETAIL_QUICKACCESSMENU_ACTIVATE_PASS27
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CEXPRESSIONSUBDEF
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CEXPRESSIONSUBDEF_STATGETTYPENAME
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define private public
#define protected public
#include "fable_assert.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCQuickAccessMenu.h"
#define FABLE_RETAIL_INVENTORY_EXPRESSIONS_PASS26
#include "engine/CExpressionDef.h"
#include "engine/CExpressionSubDef.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

#undef private
#undef protected

using namespace NUISystem;

template<> __declspec(noinline) inline bool CDefinitionManager::GetDef<CThingBaseDef>(long index,CDefPointer<const CThingBaseDef>* out) const
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

template<> __declspec(noinline) inline bool CDefinitionManager::GetDef<CExpressionDef>(long index,CDefPointer<const CExpressionDef>* out) const
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
#pragma optimize("",on)

CComponent* CTCQuickAccessMenu::GetExpressionsListUI(long& tot_num_of_exp, CComponent* pparent_list)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

template <class T> union Storage {
    unsigned __int64 align;
    unsigned char bytes[sizeof(T)];
    T* Get() { return reinterpret_cast<T*>(bytes); }
};
template<class T> static void Init(T& value) { new (&value) T; }
typedef std::pair<long,long> Item;
typedef std::vector<Item> Items;

static void Assert(bool condition, int line)
{
    if (!condition) {
        printf("check failed on line %d\n", line);
        fflush(stdout);
        abort();
    }
}
#define Check(condition) Assert((condition), __LINE__)

enum { ThingDefStart = 1, SubDefStart = 11, ExpressionDefStart = 21 };

static CTCInventory* inventory;
static NUISystem::CList* submenu;
static NUISystem::CComponent* parent;
static CThingBaseDef* thing_defs[8];
static CExpressionSubDef* subdefs[8];
static CExpressionDef* expressions[8];
static CDefClassBase::CSubDefInfo info[8];
static NUISystem::CManager* manager;
static CGameDefinitionManager* game_def;
static NGameText::CDataBank* text_bank;
static unsigned calls, lists, offsets, text_calls, rows, missing_subdef, missing_expression;
static long indexes[8];
static unsigned items, count[4], ids[4][8], values[4][8];
static const char* symbols[]={
    "TEXT_GUI_MENU_SOCIAL","TEXT_GUI_MENU_RENOWN","TEXT_GUI_MENU_ALIGNMENT","TEXT_GUI_MENU_STEALTH"
};
static const wchar_t* labels[]={L"Social",L"Renown",L"Alignment",L"Stealth"};

CMainGameComponent* CMainGameComponent::PMainGameComponent;

CCharString::CCharString() : PStringData(0) {}
CCharString::CCharString(const char* text, long length) : PStringData((CCharStringData*)text)
{
    Check(length == -1);
}
CCharString::~CCharString() {}
CWideString::CWideString() : PStringData(0) {}
CWideString::CWideString(const CWideString& text) : PStringData(text.PStringData) {}
CWideString::CWideString(const wchar_t* text) : PStringData((CWideStringData*)text) {}
CWideString::~CWideString() {}
const CWideString& CWideString::operator=(const CWideString& other)
{
    PStringData = other.PStringData;
    return *this;
}
void CIVCountedPointeeBase::Delete() const { abort(); }
bool FABLE_FASTCALL GFAssertFailed(const char*, const char*, const char*, unsigned long) { abort(); return false; }

NUISystem::CManager* FABLE_FASTCALL NUISystem::CManager::GetInstance()
{
    ++calls;
    return manager;
}
NUISystem::CComponent* NUISystem::CManager::CreateComponent(const CCharString& name, bool register_root)
{
    Check(this == manager && !register_root && !strcmp((const char*)name.PStringData,
        "UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS"));
    ++lists;
    return submenu;
}
static void FABLE_FASTCALL CheckOffsets(NUISystem::CList* list, void*, unsigned long n)
{
    Check(list == submenu && n == 4);
    ++offsets;
}
const CWideString NGameText::CDataBank::GetTextBySymbol(const CCharString& symbol) const
{
    Check(this == text_bank && text_calls < 4 && !strcmp((const char*)symbol.PStringData, symbols[text_calls]));
    return CWideString(labels[text_calls++]);
}
void CTCQuickAccessMenu::ConstructExpressionCategoryList(CWideString& label, CList* list,
    Items& category, CComponent* parent_list)
{
    Check(parent_list == parent && list == submenu && rows < 4 && text_calls == rows + 1);
    Check(!wcscmp((const wchar_t*)label.PStringData, labels[rows]));
    count[rows] = category.size();
    for (unsigned j=0; j<category.size(); ++j) {
        ids[rows][j] = category[j].first - ThingDefStart;
        values[rows][j] = category[j].second;
    }
    ++rows;
}

const CGameDefinitionManager& CTCBase::GetDefinitionManager() const { return *game_def; }
void NInventory::CTCInventoryBase::GetAllItems(Items& out) const
{
    Check(this == static_cast<NInventory::CTCInventoryBase*>(inventory));
    for (unsigned i=0; i<items; ++i)
        out.push_back(Item(indexes[i], 10+i));
}
static const CDefClassBase::CSubDefInfo* FABLE_FASTCALL CheckSubDef(const CDefClassBase* def, void*,
    const CCharString& family)
{
    Check(!strcmp((const char*)family.PStringData, "CExpressionSubDef"));
    for (unsigned i=0; i<8; ++i)
        if (def == thing_defs[i])
            return i == missing_subdef ? 0 : &info[i];
    abort();
    return 0;
}
static CDefClassBase* LookupDefinition(long index)
{
    if (index >= ThingDefStart && index < ThingDefStart + 8)
        return thing_defs[index - ThingDefStart];
    if (index >= SubDefStart && index < SubDefStart + 8)
        return subdefs[index - SubDefStart];
    Check(index >= ExpressionDefStart && index < ExpressionDefStart + 8);
    unsigned offset = index - ExpressionDefStart;
    return offset == missing_expression ? 0 : expressions[offset];
}
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index) const
{
    Check(this == game_def);
    CDefPointer<const CDefClassBase> result;
    result.Reset(LookupDefinition(index));
    return result;
}
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index)
{
    Check(this == game_def);
    CDefPointer<CDefClassBase> result;
    result.Reset(LookupDefinition(index));
    return result;
}

static void Reset(unsigned n)
{
    items = n;
    calls = lists = offsets = rows = text_calls = 0;
    for (unsigned i=0; i<8; ++i)
        indexes[i] = ThingDefStart + i;
    missing_subdef = missing_expression = 99;
}

int main()
{
    static Storage<CThing> owner_store;
    static Storage<CTCInventory> inventory_store;
    static Storage<CTCQuickAccessMenu> menu_store;
    static Storage<NUISystem::CManager> manager_store;
    static Storage<NUISystem::CComponent> parent_store;
    static Storage<NUISystem::CList> submenu_store;
    static Storage<CGameDefinitionManager> def_store;
    static Storage<CMainGameComponent> component_store;
    static Storage<NGameText::CDataBank> text_store;
    static Storage<CThingBaseDef> thing_store[8];
    static Storage<CExpressionSubDef> sub_store[8];
    static Storage<CExpressionDef> expression_store[8];
    static void* list_dispatch[0x270/4+1];
    static void* def_dispatch[15];
    inventory = inventory_store.Get();
    manager = manager_store.Get();
    parent = parent_store.Get();
    submenu = submenu_store.Get();
    game_def = def_store.Get();
    text_bank = text_store.Get();
    CMainGameComponent::PMainGameComponent = component_store.Get();
    *reinterpret_cast<NGameText::CDataBank**>(component_store.Get()->PTextBank) = text_bank;
    list_dispatch[0x270/4] = (void*)&CheckOffsets;
    def_dispatch[14] = (void*)&CheckSubDef;
    *reinterpret_cast<void***>(submenu) = list_dispatch;

    CThing* owner = owner_store.Get();
    CTCQuickAccessMenu* menu = menu_store.Get();
    menu->ParentThing = owner;
    Init(owner->UsingComponents);
    Init(owner->Components);
    owner->Components.push_back(std::make_pair(TCI_INVENTORY, static_cast<CTCBase*>(inventory)));
    for (unsigned j=0; j<8; ++j) {
        thing_defs[j]=thing_store[j].Get();
        subdefs[j]=sub_store[j].Get();
        expressions[j]=expression_store[j].Get();
        *reinterpret_cast<void***>(thing_defs[j]) = def_dispatch;
        thing_defs[j]->PDefManager = game_def;
        thing_defs[j]->IVRefCount = 1;
        subdefs[j]->IVRefCount = 1;
        expressions[j]->IVRefCount = 1;
        info[j].DefIndex = SubDefStart + j;
        subdefs[j]->ExpressionDef = ExpressionDefStart + j;
    }
    expressions[0]->ExpressionType = EXPRESSION_INVENTORY_SOCIAL;
    expressions[1]->ExpressionType = EXPRESSION_INVENTORY_RENOWN;
    expressions[2]->ExpressionType = EXPRESSION_INVENTORY_ALIGNMENT;
    expressions[3]->ExpressionType = EXPRESSION_INVENTORY_STEALTH;
    expressions[4]->ExpressionType = (EExpressionInventoryType)42;
    expressions[7]->ExpressionType = EXPRESSION_INVENTORY_STEALTH;

    Reset(0);
    long total = 432;
    Check(menu->GetExpressionsListUI(total, parent) == 0 && total == 432);
    Check(calls == 1 && lists == 0);

    owner->UsingComponents[TCI_INVENTORY] = true;
    Reset(0);
    Check(menu->GetExpressionsListUI(total, parent) == 0 && total == 0);
    Check(calls == 1 && lists == 0);

    Reset(2);
    missing_subdef = 0;
    missing_expression = 1;
    Check(menu->GetExpressionsListUI(total, parent) == 0 && total == 0);
    Check(calls == 1 && lists == 0);

    Reset(8);
    missing_subdef = 5;
    missing_expression = 6;
    Check(menu->GetExpressionsListUI(total, parent) == submenu && total == 6);
    Check(lists == 1 && offsets == 1 && text_calls == 4 && rows == 4);
    Check(count[0] == 2 && count[1] == 1 && count[2] == 1 && count[3] == 2);
    Check(ids[0][0] == 0 && ids[0][1] == 4 && values[0][1] == 14);
    Check(ids[1][0] == 1 && ids[2][0] == 2 && ids[3][0] == 3 && ids[3][1] == 7);
    Check(values[3][1] == 17 && thing_defs[7]->IVRefCount == 1 && expressions[7]->IVRefCount == 1);

    Reset(1);
    indexes[0] = ThingDefStart + 4;
    Check(menu->GetExpressionsListUI(total, parent) == submenu && total == 1);
    Check(lists == 1 && offsets == 1 && rows == 4 && count[0] == 1 && count[1] == 0 && count[2] == 0 && count[3] == 0);
    Check(ids[0][0] == 4);

    puts("00627be0_TEST PASS");
    return 0;
}