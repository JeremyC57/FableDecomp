// Behaviour test for 005f45b5 CTCInventoryAbilities::ConstructPCExpressionCategoryList(CWideString&, NUISystem::CList*, vector<pair<long,long> >&, NUISystem::CComponent*, const CCharString&, const CCharString&): the function body from src/005f45b5.cpp verbatim over self-contained
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
static bool Expect(const char* const* lines, int n, int from)
{
    for (int i = 0; i < n; ++i)
        if (from + i >= (int)g_log.size() || g_log[from + i] != lines[i]) { std::printf("  mismatch at %d: want %s\n", from + i, lines[i]); return false; }
    return true;
}
// stub for the PC item-row builder (0x005F1526, its own row): record its arguments
void CTCInventoryAbilities::ConstructPCItemLists(std::vector<std::pair<long, long> >& items, NUISystem::CList* list, NUISystem::CComponent* parent,
                                                 CCountedPointer<NUISystem::CComponent>& actions)
{
    Log("PCItemLists " + list->Name + " n" + Num((long)items.size()) + " " + parent->Name + " " + actions->Name);
}
// ---- source body (verbatim from src/005f45b5.cpp) ----
void CTCInventoryAbilities::ConstructPCExpressionCategoryList(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long, long> >& items,
                                                              NUISystem::CComponent* parent, const CCharString& ringPicture, const CCharString& guideSymbol)
{
    NUISystem::CManager* ui = NUISystem::CManager::GetInstance();
    NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(parent->FindChild("PC_DESCRIPTION_VIEWPORT"));
    NUISystem::CComponent* frame = parent->FindChild("PC_UI_FRAME");
    if (frame && viewport)
        frame->AddPositionChild(viewport);
    CWideString caption(title);
    if (!items.empty()) {
        caption += CTCInventoryStats::GetXString();
        caption += GFIntToWideString(items.size());
    }
    CCountedPointer<NUISystem::CComponent> entry(ui->CreateMenuEntryComponent(caption, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f,
                                                                              CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
    list->AddScrollingChildAndSetOffsets(entry);
    CCountedPointer<NUISystem::CComponent> description(ui->CreateComponent("PC_DESCRIPTION", false));
    NUISystem::CText* descriptionText = static_cast<NUISystem::CText*>(description.Get());
    descriptionText->SetValue(PeekComponent().GetTextBank().GetTextBySymbol(guideSymbol), true, true);
    entry->AddChild(description);
    viewport->AddFakeChild(description.Get());
    if (frame)
        frame->AddPositionChild(description.Get());
    CCountedPointer<NUISystem::CComponent> subtitle(ui->CreateComponent("PC_SUBTITLE", false));
    static_cast<NUISystem::CText*>(subtitle.Get())->SetValue(caption, true, true);
    entry->AddChild(subtitle);
    if (frame)
        frame->AddPositionChild(subtitle.Get());
    if (ringPicture != "") {
        CCountedPointer<NUISystem::CComponent> ring(ui->CreateComponent(ringPicture, false));
        entry->AddChild(ring);
        if (frame)
            frame->AddPositionChild(ring.Get());
    }
    {   // the category's sub list and its headers
        CCountedPointer<NUISystem::CComponent> subList(ui->CreateComponent("PC_SUB_LIST", false));
        NUISystem::CList* sub = static_cast<NUISystem::CList*>(subList.Get());
        if (ringPicture != "") {
            CCountedPointer<NUISystem::CComponent> ringHeader(ui->CreateComponent(ringPicture + "_FOR_LIST", false));
            sub->AddNonScrollingChild(ringHeader);
            if (frame)
                frame->AddPositionChild(ringHeader.Get());
        }
        if (items.empty()) {
            entry->MakeUnavailable();
        } else {
            entry->AddChild(subList);
            sub->InitialiseOffsets(items.size());
            CCountedPointer<NUISystem::CComponent> listTitle(ui->CreateComponent("PC_TITLE_EXPRESSIONS", false));
            NUISystem::CText* listTitleText = static_cast<NUISystem::CText*>(listTitle.Get());
            CWideString titleText(listTitleText->GetValue());
            titleText += L": ";
            titleText += title;
            listTitleText->SetValue(titleText, true, true);
            listTitle->SetName(TITLE_NAME);
            sub->AddNonScrollingChild(listTitle);
            if (frame)
                frame->AddPositionChild(listTitle.Get());
            CCountedPointer<NUISystem::CComponent> actions(NUISystem::CManager::GetInstance()->CreateComponent("PC_LIST_ITEMS_ACTIONS", false));
            NUISystem::CList* actionsList = static_cast<NUISystem::CList*>(actions.Get());
            CCountedPointer<NUISystem::CComponent> actionsTitle(ui->CreateComponent("PC_TITLE_EXPRESSIONS", false));
            static_cast<NUISystem::CText*>(actionsTitle.Get())->SetValue(titleText, true, true);
            actionsTitle->SetName(TITLE_NAME);
            actionsList->AddNonScrollingChild(actionsTitle);
            if (frame)
                frame->AddPositionChild(actionsTitle.Get());
            ConstructPCItemLists(items, sub, parent, actions);
        }
    }
}

// ---- driver ----
int main()
{
    using NUISystem::CComponent;
    NUISystem::g_autoValue["PC_TITLE_EXPRESSIONS"] = "Expr";
    CTCInventoryAbilities inv;
    NUISystem::CList list("LIST");
    CComponent parent("PARENT"), viewport("PC_DESCRIPTION_VIEWPORT"), frame("PC_UI_FRAME");
    parent.Attach(&viewport);
    parent.Attach(&frame);
    std::vector<std::pair<long, long> > items;
    items.push_back(std::make_pair(4L, 1L)); items.push_back(std::make_pair(5L, 1L));
    CWideString title(L"Social");

    inv.ConstructPCExpressionCategoryList(title, &list, items, &parent, "RING", "GUIDE");
#define E "PC_MENU_ENTRY_BRANCH_TEMPLATE[Social x2]"
    static const char* const lines[] = {
        "AddPositionChild PC_UI_FRAME <- PC_DESCRIPTION_VIEWPORT",
        "Create " E, "MenuEntryArgs |PC_BUTTON_BASTARD", "AddScrolling LIST <- " E,
        "Create PC_DESCRIPTION", "SetValue PC_DESCRIPTION = T:GUIDE", "AddChild " E " <- PC_DESCRIPTION",
        "AddFakeChild PC_DESCRIPTION_VIEWPORT <- PC_DESCRIPTION", "AddPositionChild PC_UI_FRAME <- PC_DESCRIPTION",
        "Create PC_SUBTITLE", "SetValue PC_SUBTITLE = Social x2", "AddChild " E " <- PC_SUBTITLE", "AddPositionChild PC_UI_FRAME <- PC_SUBTITLE",
        "Create RING", "AddChild " E " <- RING", "AddPositionChild PC_UI_FRAME <- RING",
        "Create PC_SUB_LIST", "Create RING_FOR_LIST", "AddNonScrolling PC_SUB_LIST <- RING_FOR_LIST", "AddPositionChild PC_UI_FRAME <- RING_FOR_LIST",
        "AddChild " E " <- PC_SUB_LIST", "InitialiseOffsets PC_SUB_LIST 2",
        "Create PC_TITLE_EXPRESSIONS", "SetValue PC_TITLE_EXPRESSIONS = Expr: Social", "SetName PC_TITLE_EXPRESSIONS -> TITLE_NAME",
        "AddNonScrolling PC_SUB_LIST <- TITLE_NAME", "AddPositionChild PC_UI_FRAME <- TITLE_NAME",
        "Create PC_LIST_ITEMS_ACTIONS",
        "Create PC_TITLE_EXPRESSIONS", "SetValue PC_TITLE_EXPRESSIONS = Expr: Social", "SetName PC_TITLE_EXPRESSIONS -> TITLE_NAME",
        "AddNonScrolling PC_LIST_ITEMS_ACTIONS <- TITLE_NAME", "AddPositionChild PC_UI_FRAME <- TITLE_NAME",
        "PCItemLists PC_SUB_LIST n2 PARENT PC_LIST_ITEMS_ACTIONS" };
#undef E
    CHECK(Expect(lines, 34, 0));
    CHECK(g_log.size() == 34);
    CHECK(title.Narrow() == "Social");

    // an empty category, no ring, a parent with only the viewport (no frame): no position registrations, the entry made
    // unavailable last, no item rows.
    CComponent bare("BARE"), vp2("PC_DESCRIPTION_VIEWPORT");
    bare.Attach(&vp2);
    std::vector<std::pair<long, long> > none;
    CWideString renown(L"Renown");
    g_log.clear();
    inv.ConstructPCExpressionCategoryList(renown, &list, none, &bare, "", "G2");
    CHECK(LogFind("Create PC_MENU_ENTRY_BRANCH_TEMPLATE[Renown]") == 0);
    CHECK(LogFind("SetValue PC_DESCRIPTION = T:G2") >= 0);
    for (unsigned long i = 0; i < g_log.size(); ++i) CHECK(g_log[i].find("AddPositionChild") != 0);
    CHECK(LogFind("MakeUnavailable PC_MENU_ENTRY_BRANCH_TEMPLATE[Renown]") == (int)g_log.size() - 1);
    CHECK(LogCount("Create PC_TITLE_EXPRESSIONS") == 0);

    if (fails) DumpLog();
    FreeCreated();
    if (fails) { std::printf("005f45b5_TEST FAIL (%d)\n", fails); return 1; }
    std::printf("005f45b5_TEST PASS\n");
    return 0;
}