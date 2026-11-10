// Behaviour test for 005f0dc5 CTCInventoryAbilities::AssignAbilityPC(long, unsigned long, NUISystem::CComponent*): the function body from src/005f0dc5.cpp verbatim over self-contained
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

// ---- source body (verbatim from src/005f0dc5.cpp) ----
void CTCInventoryAbilities::AssignAbilityPC(long ability, unsigned long slot, NUISystem::CComponent* container)
{
    EHeroAbility current = AssignedAbilities[slot];
    bool assignedElsewhere = false;
    for (unsigned long i = 0; i < AssignedAbilities.size(); ++i) {
        if (AssignedAbilities[i] == current && i != slot)
            assignedElsewhere = true;
    }
    // Slots pair up (0/1, 2/3, ...): the same spell may not sit in both halves of a pair.
    if (!(slot & 1) && slot < AssignedAbilities.size() - 1) {
        if (AssignedAbilities[slot + 1] == ability)
            return;
    } else if (slot > 0) {
        if (AssignedAbilities[slot - 1] == ability)
            return;
    }
    if (!assignedElsewhere && ability != current && current != 0) {
        CCountedPointer<NUISystem::CComponent> icon(NUISystem::CManager::GetInstance()->CreateComponent("PC_UNASSIGNED_SPELL_ICON", false));
        AbilitiesMenuEntries[current]->AddChildDynamically(icon);
        UnassignedIcons[current] = icon.Get();
    }
    AssignedAbilities[slot] = static_cast<EHeroAbility>(ability);
    std::map<EHeroAbility, NUISystem::CComponent*>::iterator found = UnassignedIcons.find(static_cast<EHeroAbility>(ability));
    if (found != UnassignedIcons.end()) {
        NUISystem::CComponent* icon = found->second->FindChild("PC_UNASSIGNED_SPELL_ICON");
        if (icon) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            icon->SetDeletion(deletion);
        }
        UnassignedIcons.erase(found);
    }
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    // The old contents: an empty slot holds the empty spell item, an assigned one the assignment icon. Each arm looks up and
    // deletes its own child; VC7.1 cross-jumps the two arms into the single lookup at 0x005F0F91 (`jne ; push A ; jmp ;
    // push edi`), with the result in ebx as retail (one shared `old` assigned in the arms leaves it memory-only).
    if (current == 0) {
        NUISystem::CComponent* old = container->FindChild("PC_SPELL_INSIDE_CONTAINER");
        if (old) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            old->SetDeletion(deletion);
        }
    } else {
        NUISystem::CComponent* old = container->FindChild("PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER");
        if (old) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            old->SetDeletion(deletion);
        }
    }
    CCountedPointer<NUISystem::CComponent> item(manager->CreateComponent("PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER", false));
    NUISystem::CComponent* sprite = item->FindChild("PC_SPELL_ASSIGNMENT_ICON_SPRITE");
    if (sprite)
        static_cast<NUISystem::CSprite*>(sprite)->SetGraphicIndex(Abilities[ability].second->IconGraphicIndex);
    CCountedPointer<NUISystem::CAction> unassign(new NUISystem::CAction);
    unassign->ActionType = NUISystem::UI_ACTION_TYPE_UNASSIGN_SPELL_PC;
    unassign->AddParam(container, unassign);
    unassign->UIActionParams.push_back(new NUISystem::CActionParamULONG(slot));
    item->AddActionOnDroppedNowhere(unassign);
    item->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    CCountedPointer<NUISystem::CAction> swap(new NUISystem::CAction);
    swap->ActionType = NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_SWAPPING_PC;
    swap->UIActionParams.push_back(new NUISystem::CActionParamSLONG(ability));
    swap->UIActionParams.push_back(new NUISystem::CActionParamSLONG(slot));
    swap->AddParam(container);
    item->AddActionOnDropped(swap);
    container->AddChildDynamically(item);
}

// ---- driver ----
int main()
{
    using NUISystem::CComponent;
    NUISystem::g_autoChild["PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER"] = "PC_SPELL_ASSIGNMENT_ICON_SPRITE";
    CHeroAbilityDef defs[8];
    CTCInventoryAbilities inv;
    for (int i = 0; i < 8; ++i) {
        defs[i].IconGraphicIndex = 100 + i;
        std::pair<long, CDefPointer<const CHeroAbilityDef> > p; p.first = 1; p.second.Object = &defs[i];
        inv.Abilities.push_back(p);
    }
    long start[4] = { 3, 0, 5, 3 };
    for (int i = 0; i < 4; ++i) inv.AssignedAbilities.push_back((EHeroAbility)start[i]);
    CComponent entry3("ENTRY3"), entry5("ENTRY5"), entry7("ENTRY7"), container("CONTAINER"), empty("PC_SPELL_INSIDE_CONTAINER");
    inv.AbilitiesMenuEntries[(EHeroAbility)3] = &entry3;
    inv.AbilitiesMenuEntries[(EHeroAbility)5] = &entry5;
    inv.AbilitiesMenuEntries[(EHeroAbility)7] = &entry7;
    container.Attach(&empty);

    // 1. 7 into the empty slot 1 (its pair partner slot 0 holds 3): the empty spell item is deleted, the assignment icon is
    //    built with 7's icon graphic, an unassign action (container, slot) on dropped-nowhere, mouse movement observed, the
    //    swap action (ability, slot, container) on dropped, and the item added to the container.
    inv.AssignAbilityPC(7, 1, &container);
    CHECK(inv.AssignedAbilities[1] == 7);
    CHECK(LogFind("Delete PC_SPELL_INSIDE_CONTAINER 2") == 0);
    CHECK(LogFind("Create PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER") == 1);
    CHECK(LogFind("SetGraphicIndex PC_SPELL_ASSIGNMENT_ICON_SPRITE 107") == 2);
    CHECK(LogFind("OnDroppedNowhere PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER A248(CONTAINER,U1)") == 3);
    CHECK(LogFind("ObserveEvent PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER 25") == 4);
    CHECK(LogFind("OnDropped PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER A328(S7,S1,CONTAINER)") == 5);
    CHECK(LogFind("AddChildDynamically CONTAINER <- PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER") == 6);
    CHECK(g_log.size() == 7);

    // 2. 3 into slot 0 (3 is also in slot 3, the partner holds 7): no unassigned icon; the old assignment icon is deleted.
    g_log.clear();
    inv.AssignAbilityPC(3, 0, &container);
    CHECK(inv.AssignedAbilities[0] == 3);
    CHECK(LogFind("Delete PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER 2") == 0);
    CHECK(LogCount("Create PC_UNASSIGNED_SPELL_ICON") == 0);
    CHECK(LogFind("OnDropped PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER A328(S3,S0,CONTAINER)") >= 0);

    // 3. the same spell in both halves of a pair is refused: 3 into slot 1 (slot 0 holds 3), 7 into slot 0 (slot 1 holds 7).
    g_log.clear();
    inv.AssignAbilityPC(3, 1, &container);
    inv.AssignAbilityPC(7, 0, &container);
    CHECK(g_log.empty() && inv.AssignedAbilities[1] == 7 && inv.AssignedAbilities[0] == 3);

    // 4. 7 over 5 in slot 2 (5 assigned nowhere else): 5 gets an unassigned icon on its menu entry.
    g_log.clear();
    inv.AssignAbilityPC(7, 2, &container);
    CHECK(inv.AssignedAbilities[2] == 7);
    CHECK(LogFind("Create PC_UNASSIGNED_SPELL_ICON") == 0 && LogFind("AddChildDynamically ENTRY5 <- PC_UNASSIGNED_SPELL_ICON") == 1);
    CHECK(inv.UnassignedIcons.size() == 1 && inv.UnassignedIcons[(EHeroAbility)5] == entry5.Children[0]);

    // 5. 5 back into slot 3 (over 3, which stays in slot 0): 5's unassigned icon is found, deleted and forgotten.
    CComponent* icon5 = entry5.Children[0];
    icon5->Attach(icon5);
    g_log.clear();
    inv.AssignAbilityPC(5, 3, &container);
    CHECK(inv.AssignedAbilities[3] == 5);
    CHECK(LogFind("Delete PC_UNASSIGNED_SPELL_ICON 2") == 0);
    CHECK(inv.UnassignedIcons.empty());
    CHECK(LogFind("SetGraphicIndex PC_SPELL_ASSIGNMENT_ICON_SPRITE 105") >= 0);

    if (fails) DumpLog();
    FreeCreated();
    if (fails) { std::printf("005f0dc5_TEST FAIL (%d)\n", fails); return 1; }
    std::printf("005f0dc5_TEST PASS\n");
    return 0;
}