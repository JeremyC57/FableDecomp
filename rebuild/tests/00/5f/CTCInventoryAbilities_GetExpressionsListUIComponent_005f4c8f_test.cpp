// Behaviour test for 005f4c8f CTCInventoryAbilities::GetExpressionsListUIComponent(long&, NUISystem::CComponent*): the function body from src/005f4c8f.cpp verbatim over self-contained
// C++03 stand-ins (std containers for STLport, a recording NUISystem model logging every UI call), then checks.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <utility>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; std::printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

// ---- recording model of the engine types the body touches (self-contained C++03 stand-ins) ----
static std::vector<std::string> g_log;
static void Log(const std::string& s) { g_log.push_back(s); }
static int LogFind(const std::string& s, int from)
{
    for (int i = from; i < (int)g_log.size(); ++i)
        if (g_log[i] == s) return i;
    return -1;
}
static int LogFind(const std::string& s) { return LogFind(s, 0); }
static int LogCount(const std::string& s)
{
    int n = 0;
    for (unsigned long i = 0; i < g_log.size(); ++i)
        if (g_log[i] == s) ++n;
    return n;
}
static std::string Num(long n) { char b[32]; std::sprintf(b, "%ld", n); return b; }
static void DumpLog() { for (unsigned long i = 0; i < g_log.size(); ++i) std::printf("  %lu %s\n", i, g_log[i].c_str()); }

class CCharString { public:
    std::string S;
    CCharString() {}
    CCharString(const char* s) : S(s) {}
    bool operator!=(const char* s) const { return S != s; }
    CCharString operator+(const char* s) const { CCharString r(*this); r.S += s; return r; }
};
class CWideString { public:
    std::wstring S;
    CWideString() {}
    CWideString(const wchar_t* s) : S(s) {}
    CWideString& operator+=(const CWideString& o) { S += o.S; return *this; }
    CWideString& operator+=(const wchar_t* s) { S += s; return *this; }
    std::string Narrow() const { std::string r; for (unsigned long i = 0; i < S.size(); ++i) r += (char)S[i]; return r; }
};
static CWideString Wide(const std::string& s) { CWideString w; for (unsigned long i = 0; i < s.size(); ++i) w.S += (wchar_t)s[i]; return w; }
CWideString GFIntToWideString(long n) { return Wide(Num(n)); }
class CRGBColour { public: unsigned char B, G, R, A; };

template <class T> class CCountedPointer { public:
    T* Data;
    CCountedPointer() : Data(0) {}
    explicit CCountedPointer(T* data) : Data(data) {}
    T* Get() const { return Data; }
    T* operator->() const { return Data; }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other) { Data = static_cast<T*>(other.Get()); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* Get() const { return Object; }
    T* operator->() const { return Object; }
};

enum EHeroAbility { HERO_ABILITY_NULL = 0 };
namespace NUISystem {
enum EType { UI_TYPE_SPRITE = 0, UI_TYPE_OTHER = 1 };
enum EEvent { UI_EVENT_MOUSE_MOVEMENT = 25 };
enum EActionType { UI_ACTION_TYPE_OBSERVE_EVENT = 245, UI_ACTION_TYPE_UNASSIGN_SPELL_PC = 248, UI_ACTION_TYPE_IGNORE_EVENT = 249,
                   UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_PC = 253, UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_SWAPPING_PC = 328 };
class CDeletion { public:
    enum EMethod { UI_DELETE_NONE = 0, UI_DELETE_THIS_INSTANCE = 2 };
    EMethod Method;
    CDeletion() : Method(UI_DELETE_NONE) {}
};
class CActionParam { public:
    virtual ~CActionParam() {}
    virtual std::string Desc() const = 0;
};
class CActionParamULONG : public CActionParam { public:
    unsigned long Value;
    explicit CActionParamULONG(unsigned long v) : Value(v) {}
    std::string Desc() const { return "U" + Num((long)Value); }
};
class CActionParamSLONG : public CActionParam { public:
    long Value;
    explicit CActionParamSLONG(long v) : Value(v) {}
    std::string Desc() const { return "S" + Num(Value); }
};
class CAction { public:
    EActionType ActionType;
    std::list<CActionParam*> UIActionParams;
    CAction() : ActionType((EActionType)0) {}
    void AddParam(CActionParam* param) { UIActionParams.push_back(param); }
    void AddParam(CActionParam* param, CCountedPointer<CAction>& self) { if (self.Get() == this) UIActionParams.push_back(param); }
    std::string Desc() const {
        std::string r = "A" + Num(ActionType) + "(";
        for (std::list<CActionParam*>::const_iterator it = UIActionParams.begin(); it != UIActionParams.end(); ++it)
            r += (it == UIActionParams.begin() ? "" : ",") + (*it)->Desc();
        return r + ")";
    }
};
class CComponent : public CActionParam { public:
    std::string Name;
    CWideString Value;
    std::vector<CComponent*> Children;
    int Deleted;
    CRGBColour Colour;
    bool Unavailable;
    explicit CComponent(const std::string& name) : Name(name), Deleted(0), Unavailable(false) { Colour.B = Colour.G = Colour.R = Colour.A = 255; }
    std::string Desc() const { return Name; }
    void Attach(CComponent* child) { Children.push_back(child); }
    void AddChild(CCountedPointer<CComponent>& child) { Attach(child.Get()); Log("AddChild " + Name + " <- " + child->Name); }
    void AddChildDynamically(CCountedPointer<CComponent>& child) { Attach(child.Get()); Log("AddChildDynamically " + Name + " <- " + child->Name); }
    CComponent* FindChild(const char* name) {
        for (unsigned long i = 0; i < Children.size(); ++i)
            if (Children[i]->Name == name) return Children[i];
        return 0;
    }
    void SetDeletion(const CDeletion& deletion) { Deleted = (int)deletion.Method; Log("Delete " + Name + " " + Num(Deleted)); }
    CRGBColour GetColour() const { return Colour; }
    void SetColour(const CRGBColour& colour) { Colour = colour; Log("SetColour " + Name + " A" + Num(colour.A)); }
    void MakeUnavailable() { Unavailable = true; Log("MakeUnavailable " + Name); }
    void SetName(const CCharString& name) { Log("SetName " + Name + " -> " + name.S); Name = name.S; }
    EType GetType() const { return Name.find("ICON") != std::string::npos ? UI_TYPE_SPRITE : UI_TYPE_OTHER; }
    void AddPositionChild(CComponent* child) { Log("AddPositionChild " + Name + " <- " + child->Name); }
    void AddActionOnDropped(CCountedPointer<CAction>& action) { Log("OnDropped " + Name + " " + action->Desc()); }
    void AddActionOnDroppedNowhere(CCountedPointer<CAction>& action) { Log("OnDroppedNowhere " + Name + " " + action->Desc()); }
    void ObserveEvent(EEvent event) { Log("ObserveEvent " + Name + " " + Num(event)); }
};
class CText : public CComponent { public:
    explicit CText(const std::string& name) : CComponent(name) {}
    void SetValue(const CWideString& value, bool, bool) { Value = value; Log("SetValue " + Name + " = " + value.Narrow()); }
    const CWideString& GetValue() const { return Value; }
};
class CList : public CText { public:
    explicit CList(const std::string& name) : CText(name) {}
    void AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>& child) { Attach(child.Get()); Log("AddScrolling " + Name + " <- " + child->Name); }
    void AddNonScrollingChild(CCountedPointer<CComponent>& child) { Attach(child.Get()); Log("AddNonScrolling " + Name + " <- " + child->Name); }
    void InitialiseOffsets(unsigned long n) { Log("InitialiseOffsets " + Name + " " + Num((long)n)); }
    void SetDefaultChild(long n) { Log("SetDefaultChild " + Name + " " + Num(n)); }
    void AddActionOnSelected(CCountedPointer<CAction>& action) { Log("OnSelected " + Name + " " + action->Desc()); }
    void AddActionOnInactive(CCountedPointer<CAction>& action) { Log("OnInactive " + Name + " " + action->Desc()); }
};
class CViewport : public CList { public:
    explicit CViewport(const std::string& name) : CList(name) {}
    void AddFakeChild(CComponent* child) { Log("AddFakeChild " + Name + " <- " + child->Name); }
};
class CSprite : public CViewport { public:
    explicit CSprite(const std::string& name) : CViewport(name) {}
    void SetGraphicIndex(long index) { Log("SetGraphicIndex " + Name + " " + Num(index)); }
};
class CMenuEntry : public CSprite { public:
    explicit CMenuEntry(const std::string& name) : CSprite(name) {}
    void AddThingActionParam(long param) { Log("AddThingActionParam " + Name + " " + Num(param)); }
};
static std::vector<CComponent*> g_created;
static std::map<std::string, std::string> g_autoChild;   // template name -> the child its template carries (e.g. a sprite)
static std::map<std::string, std::string> g_autoValue;   // template name -> the text its template carries
class CManager { public:
    static CManager* GetInstance() { static CManager m; return &m; }
    CComponent* Make(const std::string& name) {
        CComponent* c = new CMenuEntry(name); g_created.push_back(c); Log("Create " + name);
        std::map<std::string, std::string>::iterator it = g_autoChild.find(name);
        if (it != g_autoChild.end()) { CComponent* child = new CMenuEntry(it->second); g_created.push_back(child); c->Attach(child); }
        std::map<std::string, std::string>::iterator v = g_autoValue.find(name);
        if (v != g_autoValue.end()) c->Value = Wide(v->second);
        return c;
    }
    CComponent* CreateComponent(const char* name, bool) { return Make(name); }
    CComponent* CreateComponent(const CCharString& name, bool) { return Make(name.S); }
    CComponent* CreateMenuEntryComponent(const CWideString& text, float, float, const char* name, float, const CCharString& sound, float,
                                         const CCharString& button) {
        CComponent* c = Make(std::string(name) + "[" + text.Narrow() + "]");
        Log("MenuEntryArgs " + sound.S + "|" + button.S);
        return c;
    }
};
}
static void FreeCreated() { for (unsigned long i = 0; i < NUISystem::g_created.size(); ++i) delete NUISystem::g_created[i]; NUISystem::g_created.clear(); }
class CHeroAbilityDef { public:
    long IconGraphicIndex;
};
namespace NGameText {
class CDataBank { public:
    CWideString GetTextBySymbol(const CCharString& symbol) const { return Wide("T:" + symbol.S); }
};
}
class CThingComponentModel { public:
    NGameText::CDataBank Bank;
    const NGameText::CDataBank& GetTextBank() const { return Bank; }
};
class CEngineGraphic { public: long BankIndex; };
class CTCInventoryItem { public:   // static item-def queries (by def index): 99 has no graphic
    static CWideString GetDescription(long index) { return Wide("Item" + Num(index)); }
    static CWideString GetDetails(long index) { return Wide("Det" + Num(index)); }
    static CEngineGraphic GetEngineGraphic(long index) { CEngineGraphic g; g.BankIndex = (index == 99) ? 0 : index * 10; return g; }
};
class CTCInventoryStats { public:
    static CWideString GetXString() { return CWideString(L" x"); }
};
class CTCInventory { public:
    std::map<long, NUISystem::CComponent*> ItemsMenuEntries;
    std::map<long, NUISystem::CComponent*> AssignedIcons;
    long QuickAccessItems[4];
    std::vector<std::pair<long, long> > Items;
    CTCInventory() { QuickAccessItems[0] = QuickAccessItems[1] = QuickAccessItems[2] = QuickAccessItems[3] = -1; }
    void GetAllItems(std::vector<std::pair<long, long> >& out) { out = Items; }
};
class CThing { public:
    CTCInventory* Inventory;
    CThing() : Inventory(0) {}
    bool GetTC(CTCInventory** out) { if (!Inventory) return false; *out = Inventory; return true; }
};
static CCharString TITLE_NAME("TITLE_NAME");
class CTCInventoryAbilities { public:
    CThing* ParentThing;
    CThingComponentModel Component;
    const CThingComponentModel& PeekComponent() const { return Component; }
    std::vector<std::pair<long, CDefPointer<const CHeroAbilityDef> > > Abilities;
    std::vector<EHeroAbility> AssignedAbilities, AssignedAbilitiesSave;
    std::map<EHeroAbility, NUISystem::CComponent*> UnassignedIcons;
    std::map<EHeroAbility, NUISystem::CComponent*> AbilitiesMenuEntries;
    CTCInventoryAbilities() : ParentThing(0) {}
    void UnAssignAbilityPC(unsigned long slot, NUISystem::CComponent* container);
    void AssignAbilityPC(long ability, unsigned long slot, NUISystem::CComponent* container);
    void ContructItemLists(std::vector<std::pair<long, long> >& items, NUISystem::CList* list, NUISystem::CComponent* owner,
                           CCountedPointer<NUISystem::CComponent>& actions);
    void ConstructPCItemLists(std::vector<std::pair<long, long> >& items, NUISystem::CList* list, NUISystem::CComponent* parent,
                              CCountedPointer<NUISystem::CComponent>& actions);
    void ConstructExpressionCategoryList(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long, long> >& items,
                                         NUISystem::CComponent* owner, const CCharString& ringPicture, const CCharString& guideSymbol);
    void ConstructPCExpressionCategoryList(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long, long> >& items,
                                           NUISystem::CComponent* parent, const CCharString& ringPicture, const CCharString& guideSymbol);
    NUISystem::CComponent* GetExpressionsListUIComponent(long& count, NUISystem::CComponent* owner);
};
enum EExpressionInventoryType { EXPRESSION_INVENTORY_TYPE_SOCIAL = 0, EXPRESSION_INVENTORY_TYPE_RENOWN = 1,
                                EXPRESSION_INVENTORY_TYPE_ALIGNMENT = 2, EXPRESSION_INVENTORY_TYPE_STEALTH = 3 };
class CExpressionDef { public: EExpressionInventoryType ExpressionType; };
class CExpressionSubDef { public: long ExpressionDef; };
class CThingBaseDef { public:
    const CExpressionSubDef* Sub;
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const { if (!Sub) return false; out->Object = Sub; return true; }
};
// def tables by global index: item defs 10.., expression defs 100..
static std::map<long, CThingBaseDef> g_thingDefs;
static std::map<long, CExpressionDef> g_exprDefs;
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const {
        if (index <= 0 || g_thingDefs.find(index) == g_thingDefs.end()) return false;
        out->Object = &g_thingDefs[index]; return true;
    }
    bool GetDef(long index, CDefPointer<CExpressionDef>* out) const {
        if (index <= 0 || g_exprDefs.find(index) == g_exprDefs.end()) return false;
        out->Object = &g_exprDefs[index]; return true;
    }
};
CDefinitionManager& GetDefinitionManager() { static CDefinitionManager m; return m; }
static std::string Ids(const std::vector<std::pair<long, long> >& items)
{
    std::string r;
    for (unsigned long i = 0; i < items.size(); ++i) r += (i ? "," : "") + Num(items[i].first);
    return "[" + r + "]";
}
// stub for the console category builder (0x005F3F7B, its own row): record its arguments
void CTCInventoryAbilities::ConstructExpressionCategoryList(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long, long> >& items,
                                                            NUISystem::CComponent* owner, const CCharString& ringPicture, const CCharString& guideSymbol)
{
    Log("Category " + title.Narrow() + " " + Ids(items) + " " + ringPicture.S + " " + guideSymbol.S + " " + list->Name + " " + owner->Name);
}
// ---- source body (verbatim from src/005f4c8f.cpp) ----
NUISystem::CComponent* CTCInventoryAbilities::GetExpressionsListUIComponent(long& count, NUISystem::CComponent* owner)
{
    NUISystem::CManager* ui = NUISystem::CManager::GetInstance();
    CTCInventory* inventory;
    if (!ParentThing->GetTC(&inventory))
        return 0;
    inventory->ItemsMenuEntries.clear();
    inventory->AssignedIcons.clear();

    // sort the carried items into the four expression categories by their expression def's type
    std::vector<std::pair<long, long> > allItems;
    std::vector<std::pair<long, long> > socialItems;
    std::vector<std::pair<long, long> > renownItems;
    std::vector<std::pair<long, long> > alignmentItems;
    std::vector<std::pair<long, long> > stealthItems;
    inventory->GetAllItems(allItems);
    count = 0;
    for (unsigned long i = 0; i < allItems.size(); ++i) {
        CDefPointer<const CThingBaseDef> itemDef;
        GetDefinitionManager().GetDef(allItems[i].first, &itemDef);
        CDefPointer<const CExpressionSubDef> expressionSubDef;
        CDefPointer<CExpressionDef> expressionDef;
        if (itemDef->GetSubDef(&expressionSubDef)) {
            if (GetDefinitionManager().GetDef(expressionSubDef->ExpressionDef, &expressionDef)) {
                switch (expressionDef->ExpressionType) {
                case EXPRESSION_INVENTORY_TYPE_SOCIAL:    socialItems.push_back(allItems[i]);    break;
                case EXPRESSION_INVENTORY_TYPE_RENOWN:    renownItems.push_back(allItems[i]);    break;
                case EXPRESSION_INVENTORY_TYPE_ALIGNMENT: alignmentItems.push_back(allItems[i]); break;
                case EXPRESSION_INVENTORY_TYPE_STEALTH:   stealthItems.push_back(allItems[i]);   break;
                default:                                  socialItems.push_back(allItems[i]);    break;
                }
                ++count;
            }
        }
    }

    // the sub list: back / select / title headers and the four category entries
    NUISystem::CList* subList = static_cast<NUISystem::CList*>(ui->CreateComponent("UI_SUB_LIST", false));
    {   // the headers and the category title live in this block only: their frame slots reuse the sorting loop's
        // (retail select [ebp-0Ch] over the item def, title [ebp-14h] over the index, the title text in count's slot [ebp+8])
        CCountedPointer<NUISystem::CComponent> back(ui->CreateComponent("UI_BACK_FOR_LIST", false));
        CCountedPointer<NUISystem::CComponent> select(ui->CreateComponent("UI_SELECT_FOR_LIST", false));
        subList->AddNonScrollingChild(back);
        subList->AddNonScrollingChild(select);
        CCountedPointer<NUISystem::CComponent> title(ui->CreateComponent("UI_EXPRESSIONS_TITLE", false));
        title->SetName(TITLE_NAME);
        subList->AddNonScrollingChild(title);
        subList->InitialiseOffsets(4);

        const NGameText::CDataBank& textBank = PeekComponent().GetTextBank();
        CWideString categoryTitle;
        categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_SOCIAL");
        ConstructExpressionCategoryList(categoryTitle, subList, socialItems, owner, "UI_RING_PIC_EXPR_SOCIAL", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_SOCIAL");
        categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN");
        ConstructExpressionCategoryList(categoryTitle, subList, renownItems, owner, "UI_RING_PIC_EXPR_RENOWN", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_RENOWN");
        categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_ALIGNMENT");
        ConstructExpressionCategoryList(categoryTitle, subList, alignmentItems, owner, "UI_RING_PIC_EXPR_ALIGNMENT", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_ALIGNMENT");
        categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_STEALTH");
        ConstructExpressionCategoryList(categoryTitle, subList, stealthItems, owner, "UI_RING_PIC_EXPR_STEALTH", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_STEALTH");
    }
    return subList;
}

// ---- driver ----
int main()
{
    using NUISystem::CComponent;
    CExpressionSubDef subs[7];
    long types[7] = { 0, 1, 2, 3, 7, -1, -1 };
    for (int k = 0; k < 7; ++k) {
        subs[k].ExpressionDef = (k == 6) ? 0 : 100 + k;          // item 16's sub def names no expression def
        if (types[k] >= 0) g_exprDefs[100 + k].ExpressionType = (EExpressionInventoryType)types[k];
        g_thingDefs[10 + k].Sub = (k == 5) ? 0 : &subs[k];      // item 15 has no expression sub def
    }
    CTCInventory inventory;
    for (int k = 0; k < 7; ++k) inventory.Items.push_back(std::make_pair(10L + k, 1L));
    inventory.ItemsMenuEntries[3] = 0;
    inventory.AssignedIcons[1] = 0;
    CThing thing; thing.Inventory = &inventory;
    CTCInventoryAbilities inv; inv.ParentThing = &thing;
    CComponent owner("OWNER");
    long count = -1;

    CComponent* result = inv.GetExpressionsListUIComponent(count, &owner);
    CHECK(result != 0 && result->Name == "UI_SUB_LIST");
    CHECK(count == 5);
    CHECK(inventory.ItemsMenuEntries.empty() && inventory.AssignedIcons.empty());
    static const char* const lines[] = {
        "Create UI_SUB_LIST", "Create UI_BACK_FOR_LIST", "Create UI_SELECT_FOR_LIST",
        "AddNonScrolling UI_SUB_LIST <- UI_BACK_FOR_LIST", "AddNonScrolling UI_SUB_LIST <- UI_SELECT_FOR_LIST",
        "Create UI_EXPRESSIONS_TITLE", "SetName UI_EXPRESSIONS_TITLE -> TITLE_NAME", "AddNonScrolling UI_SUB_LIST <- TITLE_NAME",
        "InitialiseOffsets UI_SUB_LIST 4",
        "Category T:TEXT_GUI_MENU_SOCIAL [10,14] UI_RING_PIC_EXPR_SOCIAL TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_SOCIAL UI_SUB_LIST OWNER",
        "Category T:TEXT_GUI_MENU_RENOWN [11] UI_RING_PIC_EXPR_RENOWN TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_RENOWN UI_SUB_LIST OWNER",
        "Category T:TEXT_GUI_MENU_ALIGNMENT [12] UI_RING_PIC_EXPR_ALIGNMENT TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_ALIGNMENT UI_SUB_LIST OWNER",
        "Category T:TEXT_GUI_MENU_STEALTH [13] UI_RING_PIC_EXPR_STEALTH TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_STEALTH UI_SUB_LIST OWNER" };
    CHECK((int)g_log.size() == 13);
    for (int i = 0; i < 13 && i < (int)g_log.size(); ++i) CHECK(g_log[i] == lines[i]);

    // no inventory component: null, nothing built, count untouched
    CThing empty;
    inv.ParentThing = &empty;
    g_log.clear();
    count = 42;
    CHECK(inv.GetExpressionsListUIComponent(count, &owner) == 0);
    CHECK(g_log.empty() && count == 42);

    if (fails) DumpLog();
    FreeCreated();
    if (fails) { std::printf("005f4c8f_TEST FAIL (%d)\n", fails); return 1; }
    std::printf("005f4c8f_TEST PASS\n");
    return 0;
}