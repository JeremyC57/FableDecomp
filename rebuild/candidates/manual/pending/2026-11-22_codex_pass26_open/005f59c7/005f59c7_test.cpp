#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

using std::vector;
typedef std::string CCharString;
typedef std::wstring CWideString;
typedef std::pair<long, long> Item;
typedef vector<Item> Items;
static CCharString TITLE_NAME("EXPRESSIONS TITLE");
static CCharString GFIntToCharString(unsigned long value)
{
    char text[32];
    sprintf(text, "%lu", value);
    return text;
}

static CWideString toWide(const CCharString& text)
{
    CWideString result;
    for (size_t i = 0; i != text.size(); ++i)
        result += static_cast<wchar_t>(text[i]);
    return result;
}

template<class T> struct CCountedPointer {
    T* Data;
    CCountedPointer(T* p) : Data(p) {}
    T* Get() const { return Data; }
    T* operator->() const { return Data; }
};

template<class To, class From> To* GFDebugPDynamicCast(From* p, To*)
{
    return static_cast<To*>(p);
}

namespace NUISystem {
class CComponent;
enum { UI_ACTION_TYPE_OBSERVE_EVENT, UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC,
       UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC, UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC };
enum { UI_EVENT_MOUSE_MOVEMENT = 10, UI_EVENT_COMPONENT_DROPPED };
struct CActionParam {
    long value;
    bool isSigned;
    CActionParam(long v, bool isSigned) : value(v), isSigned(isSigned) {}
};
struct CActionParamSLONG : CActionParam {
    CActionParamSLONG(long n) : CActionParam(n, true) {}
};
struct CActionParamULONG : CActionParam {
    CActionParamULONG(unsigned long n) : CActionParam(n, false) {}
};
struct CAction {
    static vector<CAction*> all;
    int ActionType;
    CComponent* target;
    bool prepended;
    vector<CActionParam*> UIActionParams;
    CAction() : ActionType(-1), target(0), prepended(false) { all.push_back(this); }
    ~CAction() { for (size_t i = 0; i != UIActionParams.size(); ++i) delete UIActionParams[i]; }
    void AddParam(CComponent* component) { target = component; }
    void AddParam(CComponent* component, CCountedPointer<CAction>&) { target = component; prepended = true; }
};
vector<CAction*> CAction::all;

class CComponent {
public:
    CCharString kind, name;
    vector<CComponent*> children, positionChildren;
    vector<CAction*> actions, nowhere, dropped;
    explicit CComponent(const CCharString& type) : kind(type), name(type) {}
    virtual ~CComponent() {}
    CComponent* FindChild(const CCharString& name) const
    {
        for (size_t i = 0; i != children.size(); ++i) {
            if (children[i]->name == name) return children[i];
            CComponent* result = children[i]->FindChild(name);
            if (result) return result;
        }
        return 0;
    }
    void SetName(const CCharString& text) { name = text; }
    void AddChild(CCountedPointer<CComponent> c) { children.push_back(c.Get()); }
    void AddPositionChild(CComponent* c) { positionChildren.push_back(c); }
    void AddAction(CCountedPointer<CAction> action) { actions.push_back(action.Get()); }
    void AddActionOnDroppedNowhere(CCountedPointer<CAction> action) { nowhere.push_back(action.Get()); }
    void AddActionOnDropped(CCountedPointer<CAction> action) { dropped.push_back(action.Get()); }
};
class CList : public CComponent {
public:
    int offsets;
    vector<CAction*> selected;
    CList() : CComponent("PC_SUB_LIST"), offsets(-1) {}
    void AddNonScrollingChild(CCountedPointer<CComponent> c) { AddChild(c); }
    void AddActionOnSelected(CCountedPointer<CAction> action) { selected.push_back(action.Get()); }
    void InitialiseOffsets(int value) { offsets = value; }
};
class CSprite : public CComponent {
public:
    int graphic;
    CSprite() : CComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE"), graphic(-1) {}
    void SetGraphicIndex(int value) { graphic = value; }
};
class CManager {
public:
    static CManager* instance;
    vector<CComponent*> all;
    vector<CCharString> creations;
    unsigned icons;
    CManager() : icons(0) { instance = this; }
    ~CManager()
    {
        for (size_t i = 0; i != all.size(); ++i) delete all[i];
        for (size_t i = 0; i != CAction::all.size(); ++i) delete CAction::all[i];
        CAction::all.clear();
        instance = 0;
    }
    static CManager* GetInstance() { return instance; }
    CComponent* Remember(CComponent* item) { all.push_back(item); return item; }
    CComponent* CreateComponent(const CCharString& name, bool)
    {
        creations.push_back(name);
        if (name == "PC_SUB_LIST") return Remember(new CList);
        CComponent* item = Remember(new CComponent(name));
        if (name == "PC_EXPRESSIONS_ITEMS_CONTAINMENT_CONTAINER") {
            for (unsigned slot = 1; slot <= 4; ++slot) {
                if (slot == 2) continue;
                item->AddChild(Remember(new CComponent("PC_EXPRESSION_ITEM_CONTAINMENT_" + GFIntToCharString(slot))));
            }
        }
        if (name == "PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER" && icons++ == 0)
            item->AddChild(Remember(new CSprite));
        return item;
    }
};
CManager* CManager::instance = 0;
class CUIMiscThingsDef {};
}
using namespace NUISystem;

namespace NGameText {
class CDataBank {
public:
    mutable vector<CCharString> symbols;
    CWideString GetTextBySymbol(const CCharString& symbol) const
    {
        symbols.push_back(symbol);
        return toWide(symbol);
    }
};
}
struct MainComponent {
    NGameText::CDataBank bank;
    const NGameText::CDataBank& GetTextBank() const { return bank; }
};

template<class T> struct CDefPointer {
    T* pointer;
    CDefPointer() : pointer(0) {}
    T* operator->() const { return pointer; }
};
enum EExpressionInventoryType { EXPRESSION_INVENTORY_SOCIAL, EXPRESSION_INVENTORY_RENOWN,
    EXPRESSION_INVENTORY_ALIGNMENT, EXPRESSION_INVENTORY_STEALTH, OTHER_EXPRESSION };
struct CExpressionDef { EExpressionInventoryType ExpressionType; };
struct CExpressionSubDef { long ExpressionDef; };
struct CThingBaseDef {
    CExpressionSubDef sub;
    bool hasSub;
    bool GetSubDef(CDefPointer<const CExpressionSubDef>* result) const
    {
        if (hasSub) result->pointer = &sub;
        return hasSub;
    }
};
struct DefManager {
    CThingBaseDef things[7];
    CExpressionDef expressions[5];
    mutable vector<long> lookups;
    DefManager()
    {
        for (long i = 0; i != 7; ++i) {
            things[i].hasSub = i != 5;
            things[i].sub.ExpressionDef = i + 10;
            if (i < 5) expressions[i].ExpressionType = static_cast<EExpressionInventoryType>(i);
        }
    }
    bool GetDef(long id, CDefPointer<const CThingBaseDef>* result) const
    {
        result->pointer = &things[id - 1];
        return true;
    }
    bool GetDef(long id, CDefPointer<const CExpressionDef>* result) const
    {
        lookups.push_back(id);
        if (id >= 15) return false;
        result->pointer = &expressions[id - 10];
        return true;
    }
};
struct CTCInventory {
    Items items;
    vector<long> entries, icons, quick;
    vector<unsigned long> graphicRequests;
    void GetAllItems(Items& out) const { out = items; }
    vector<long>& GetItemsMenuEntries() { return entries; }
    vector<long>& GetAssignedIcons() { return icons; }
    vector<long>& GetQuickAccessItems() { return quick; }
    int GetQuickAccessItemGraphicIndex(unsigned long index)
    {
        graphicRequests.push_back(index);
        return 70 + static_cast<int>(index);
    }
};
struct CThing {
    CTCInventory* inventory;
    bool GetTC(CTCInventory** result) const { *result = inventory; return inventory != 0; }
};
struct Category {
    CWideString name;
    Items items;
    CCharString ring, guide;
};
class CTCInventoryAbilities {
    CThing parent;
public:
    MainComponent component;
    DefManager definitions;
    vector<Category> categories;
    mutable unsigned miscRequests;
    explicit CTCInventoryAbilities(CTCInventory* inventory) : miscRequests(0) { parent.inventory = inventory; }
    CThing& GetParentThing() { return parent; }
    const MainComponent& PeekComponent() const { return component; }
    const DefManager& GetDefinitionManager() const { return definitions; }
    CDefPointer<const CUIMiscThingsDef> GetMiscThingsDef() const
    {
        ++miscRequests;
        return CDefPointer<const CUIMiscThingsDef>();
    }
    void ConstructPCExpressionCategoryList(CWideString& name, CList*, Items& items, CComponent*,
                                            const CCharString& ring, const CCharString& guide)
    {
        Category c = {name, items, ring, guide};
        categories.push_back(c);
    }
    CComponent* ConstructPCExpressionsList(CComponent* ptop_list);
};

// The selected leaf body begins here. The harness supplies the collaborators above.
CComponent* CTCInventoryAbilities::ConstructPCExpressionsList(CComponent* ptop_list)
{
    CTCInventory* ptc_inventory;
    NUISystem::CManager* pmanager = NUISystem::CManager::GetInstance();
    CComponent* pframe = ptop_list->FindChild("PC_UI_FRAME");
    CDefPointer<const NUISystem::CUIMiscThingsDef> pmiscthingsdef = GetMiscThingsDef();

    if (!GetParentThing().GetTC(&ptc_inventory))
        return 0;
    ptc_inventory->GetItemsMenuEntries().clear();
    ptc_inventory->GetAssignedIcons().clear();

    std::vector<std::pair<long, long> > items, expressions_catA, expressions_catB, expressions_catC, expressions_catD;
    ptc_inventory->GetAllItems(items);
    long tot_num_of_expressions = 0;
    for (long count = 0; count < items.size(); ++count) {
        CDefPointer<const CThingBaseDef> pthing_def;
        GetDefinitionManager().GetDef(items[count].first, &pthing_def);
        CDefPointer<const CExpressionSubDef> pexpression_sub_def;
        CDefPointer<const CExpressionDef> pexpression_def;
        if (pthing_def->GetSubDef(&pexpression_sub_def) && GetDefinitionManager().GetDef(pexpression_sub_def->ExpressionDef, &pexpression_def)) {
            EExpressionInventoryType exp_type = pexpression_def->ExpressionType;
            switch (exp_type) {
            case EXPRESSION_INVENTORY_SOCIAL:    expressions_catA.push_back(items[count]);    break;
            case EXPRESSION_INVENTORY_RENOWN:    expressions_catB.push_back(items[count]);    break;
            case EXPRESSION_INVENTORY_ALIGNMENT: expressions_catC.push_back(items[count]); break;
            case EXPRESSION_INVENTORY_STEALTH:   expressions_catD.push_back(items[count]);   break;
            default:                                  expressions_catA.push_back(items[count]);    break;
            }
            ++tot_num_of_expressions;
        }
    }

    CComponent* pexpressions_list_component = 0;
    CList* pexpressions_list = 0;
    pexpressions_list_component = pmanager->CreateComponent("PC_SUB_LIST", false);
    pexpressions_list = GFDebugPDynamicCast(pexpressions_list_component,(CList*)0);
    CCountedPointer<CComponent> pexpressions_container(pmanager->CreateComponent("PC_EXPRESSIONS_ITEMS_CONTAINMENT_CONTAINER", false));
    pexpressions_list->AddNonScrollingChild(pexpressions_container);
    CCountedPointer<CComponent> pdummy_bottom_left(pmanager->CreateComponent("PC_POSITION_DUMMY_BOTTOM_LEFT", false));
    pexpressions_list->AddNonScrollingChild(pdummy_bottom_left);
    pdummy_bottom_left->AddPositionChild(pexpressions_container.Get());

    std::vector<long>& assigned_things = ptc_inventory->GetQuickAccessItems();
    for(unsigned long count=0;count<assigned_things.size();++count) {
        CCharString name="PC_EXPRESSION_ITEM_CONTAINMENT_"+GFIntToCharString(count+1);
        CComponent* pexpression_containment=pexpressions_list->FindChild(name);
        if(pexpression_containment) {
            CCountedPointer<CAction> paction_observe(new CAction);
            paction_observe->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            paction_observe->AddParam(pexpression_containment,paction_observe);
            paction_observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
            pexpressions_list->AddActionOnSelected(paction_observe);
            CCountedPointer<CAction> paction_observe_dropped_event(new CAction);
            paction_observe_dropped_event->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            paction_observe_dropped_event->AddParam(pexpression_containment,paction_observe_dropped_event);
            paction_observe_dropped_event->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_COMPONENT_DROPPED));
            pexpressions_list->AddActionOnSelected(paction_observe_dropped_event);
            CCountedPointer<CAction> paction_assign_expression(new CAction);
            paction_assign_expression->ActionType=NUISystem::UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC;
            paction_assign_expression->AddParam(pexpression_containment,paction_assign_expression);
            paction_assign_expression->UIActionParams.push_back(new CActionParamULONG(count));
            pexpression_containment->AddAction(paction_assign_expression);
            if(assigned_things[count]==0) {
                CCountedPointer<CComponent> pchild(pmanager->CreateComponent("PC_EXPRESSION_ITEM_INSIDE_CONTAINER",false));
                pexpression_containment->AddChild(pchild);
            } else {
                CCountedPointer<CComponent> pchild(pmanager->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER",false));
                CComponent* psprite_in_child=pchild->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
                if(psprite_in_child) {
                    NUISystem::CSprite* psprite_in_child_real = GFDebugPDynamicCast(psprite_in_child,(NUISystem::CSprite*)0);
                    if (assigned_things[count]!=0) psprite_in_child_real->SetGraphicIndex(ptc_inventory->GetQuickAccessItemGraphicIndex(count));
                }
                CCountedPointer<CAction> paction_on_nowhere(new CAction);
                paction_on_nowhere->ActionType=NUISystem::UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC;
                paction_on_nowhere->AddParam(pexpression_containment,paction_on_nowhere);
                paction_on_nowhere->UIActionParams.push_back(new CActionParamULONG(count));
                pchild->AddActionOnDroppedNowhere(paction_on_nowhere);
                CCountedPointer<CAction> paction_draggable_observe(new CAction);
                paction_draggable_observe->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                paction_draggable_observe->AddParam(pchild.Get(),paction_draggable_observe);
                paction_draggable_observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                pexpressions_list->AddActionOnSelected(paction_draggable_observe);
                CCountedPointer<CAction> paction_set_expression_to_assign(new CAction);
                paction_set_expression_to_assign->ActionType=NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC;
                paction_set_expression_to_assign->UIActionParams.push_back(new CActionParamSLONG(assigned_things[count]));
                paction_set_expression_to_assign->UIActionParams.push_back(new CActionParamSLONG(count));
                paction_set_expression_to_assign->AddParam(pexpression_containment);
                pchild->AddActionOnDropped(paction_set_expression_to_assign);
                pexpression_containment->AddChild(pchild);
            }
        }
    }

    CCountedPointer<CComponent> ptitle(pmanager->CreateComponent("PC_TITLE_EXPRESSIONS", false));
    ptitle->SetName(TITLE_NAME);
    pexpressions_list->AddNonScrollingChild(ptitle);
    if (pframe) {
        pframe->AddPositionChild(ptitle.Get());
    }
    pexpressions_list->InitialiseOffsets(4);

    const NGameText::CDataBank& rTextBank = PeekComponent().GetTextBank();
    CWideString menuparam;
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_SOCIAL");
    ConstructPCExpressionCategoryList(menuparam, pexpressions_list, expressions_catA, ptop_list, "PC_RING_PIC_EXPR_SOCIAL", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_SOCIAL");
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN");
    ConstructPCExpressionCategoryList(menuparam, pexpressions_list, expressions_catB, ptop_list, "PC_RING_PIC_EXPR_RENOWN", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_RENOWN");
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_ALIGNMENT");
    ConstructPCExpressionCategoryList(menuparam, pexpressions_list, expressions_catC, ptop_list, "PC_RING_PIC_EXPR_ALIGNMENT", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_ALIGNMENT");
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_STEALTH");
    ConstructPCExpressionCategoryList(menuparam, pexpressions_list, expressions_catD, ptop_list, "PC_RING_PIC_EXPR_STEALTH", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_STEALTH");
    return pexpressions_list;
}

static bool hasAction(const CAction* action, int type, CComponent* target, long value, bool prepended)
{
    return action->ActionType == type && action->target == target && action->prepended == prepended &&
           action->UIActionParams.size() == 1 && action->UIActionParams[0]->value == value &&
           !action->UIActionParams[0]->isSigned;
}

static int run(bool withFrame)
{
    CManager manager;
    CComponent top("top"), frame("PC_UI_FRAME");
    if (withFrame) top.AddChild(&frame);
    CTCInventory inventory;
    CTCInventoryAbilities abilities(&inventory);
    inventory.entries.push_back(9);
    inventory.icons.push_back(18);
    for (long i = 1; i <= 7; ++i) inventory.items.push_back(Item(i, i * 10));
    inventory.quick.push_back(0);
    inventory.quick.push_back(71);
    inventory.quick.push_back(72);
    inventory.quick.push_back(73);
    CList* list = static_cast<CList*>(abilities.ConstructPCExpressionsList(&top));
    if (!list || list->offsets != 4 || list->children.size() != 3) return 1;
    if (abilities.miscRequests != 1 || !inventory.entries.empty() || !inventory.icons.empty()) return 2;
    CComponent* container = list->children[0];
    CComponent* anchor = list->children[1];
    if (anchor->positionChildren.size() != 1 || anchor->positionChildren[0] != container) return 3;
    if (list->children[2]->name != TITLE_NAME || frame.positionChildren.size() != (withFrame ? 1U : 0U)) return 4;
    if (withFrame && frame.positionChildren[0] != list->children[2]) return 5;
    if (inventory.graphicRequests.size() != 1 || inventory.graphicRequests[0] != 2 || list->selected.size() != 8) return 6;
    if (container->children.size() != 3) return 7;
    const unsigned indices[] = {0, 2, 3};
    unsigned selected = 0;
    for (unsigned i = 0; i != 3; ++i) {
        unsigned slot = indices[i];
        CComponent* holder = container->children[i];
        if (holder->children.size() != 1 || holder->actions.size() != 1 ||
            !hasAction(holder->actions[0], UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC, holder, slot, true)) return 8;
        if (!hasAction(list->selected[selected++], UI_ACTION_TYPE_OBSERVE_EVENT, holder, UI_EVENT_MOUSE_MOVEMENT, true) ||
            !hasAction(list->selected[selected++], UI_ACTION_TYPE_OBSERVE_EVENT, holder, UI_EVENT_COMPONENT_DROPPED, true)) return 9;
        CComponent* icon = holder->children[0];
        if (i == 0) {
            if (icon->kind != "PC_EXPRESSION_ITEM_INSIDE_CONTAINER" || !icon->nowhere.empty() || !icon->dropped.empty()) return 10;
            continue;
        }
        if (icon->kind != "PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER" ||
            icon->nowhere.size() != 1 || icon->dropped.size() != 1 ||
            !hasAction(icon->nowhere[0], UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC, holder, slot, true) ||
            !hasAction(list->selected[selected++], UI_ACTION_TYPE_OBSERVE_EVENT, icon, UI_EVENT_MOUSE_MOVEMENT, true)) return 11;
        if (i == 1 && (icon->children.size() != 1 || static_cast<CSprite*>(icon->children[0])->graphic != 72)) return 12;
        if (i == 2 && !icon->children.empty()) return 13;
        const CAction* drop = icon->dropped[0];
        if (drop->ActionType != UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC || drop->target != holder || drop->prepended ||
            drop->UIActionParams.size() != 2 || drop->UIActionParams[0]->value != inventory.quick[slot] ||
            drop->UIActionParams[1]->value != slot || !drop->UIActionParams[0]->isSigned || !drop->UIActionParams[1]->isSigned) return 14;
    }
    const vector<Category>& categories = abilities.categories;
    const char* names[] = {"SOCIAL", "RENOWN", "ALIGNMENT", "STEALTH"};
    if (categories.size() != 4 || abilities.component.bank.symbols.size() != 4 || abilities.definitions.lookups.size() != 6) return 15;
    for (long i = 0; i != 5; ++i) if (abilities.definitions.lookups[i] != i + 10) return 16;
    if (abilities.definitions.lookups[5] != 16) return 17;
    for (unsigned i = 0; i != 4; ++i) {
        CCharString text = std::string("TEXT_GUI_MENU_") + names[i];
        if (categories[i].name != toWide(text) || abilities.component.bank.symbols[i] != text ||
            categories[i].ring != std::string("PC_RING_PIC_EXPR_") + names[i] ||
            categories[i].guide != std::string("TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_") + names[i] ||
            categories[i].items.size() != (i == 0 ? 2U : 1U) || categories[i].items[0] != inventory.items[i]) return 18;
    }
    if (categories[0].items[1] != inventory.items[4]) return 19;
    return 0;
}

int main()
{
    {
        CManager manager;
        CComponent top("top");
        CTCInventoryAbilities abilities(0);
        if (abilities.ConstructPCExpressionsList(&top) || abilities.miscRequests != 1 || !manager.creations.empty() ||
            !abilities.component.bank.symbols.empty() || !abilities.categories.empty()) return 1;
    }
    {
        CManager manager;
        CComponent top("top");
        CTCInventory inventory;
        CTCInventoryAbilities abilities(&inventory);
        CList* list = static_cast<CList*>(abilities.ConstructPCExpressionsList(&top));
        if (!list || !list->selected.empty() || !inventory.graphicRequests.empty() || abilities.categories.size() != 4) return 2;
        for (unsigned i = 0; i != 4; ++i) if (!abilities.categories[i].items.empty()) return 3;
    }
    int result = run(false);
    if (result) return 10 + result;
    result = run(true);
    if (result) return 30 + result;
    puts("005f59c7_TEST PASS");
    return 0;
}
