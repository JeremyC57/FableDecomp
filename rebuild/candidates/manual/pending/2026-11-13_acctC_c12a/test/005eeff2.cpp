// Behaviour test for CTCInventoryAbilities::GetMagicListUIComponent @ 0x005EEFF2: the console skills screen's magic section.
// Subject: the row's own statements (GetCategory / GetAbilityPDef / GetMiscThingsDef / GetAbilityLevel and GetMagicListUIComponent,
// copied unchanged from src/005eeff2.cpp) over a mock engine that records every UI call. Scenarios: (A) four owned spells in three
// categories, received order shorter than the palettes (padded with empty slots, the slot-1 swap, the per-container reorder into two
// spell containers), level captions, unassigned icons, menu-entry action params, descriptions, category title / subtitle counts;
// (B) received order longer than the palettes (truncated), no experience TC (empty captions), no viewport, only attack spells
// (surround / physical menu entries made unavailable, their arrows deleted).
#include <cstdio>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
typedef std::string Str;
static int gFail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++gFail; } }
static Str Num(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static std::vector<Str> gLog;
static void Log(const Str& s) { gLog.push_back(s); }
// ---- strings ----
struct CCharString { Str s; CCharString() { } CCharString(const char* p) : s(p) { } };
struct CWideString {
    Str s;
    CWideString() { }
    template<class C> CWideString(const C* w) { while (*w) s += (char)*w++; }
    CWideString(const Str& x, int) : s(x) { }
    CWideString& operator+=(const CWideString& o) { s += o.s; return *this; }
};
static CWideString operator+(const CWideString& a, const CWideString& b) { return CWideString(a.s + b.s, 0); }
static CWideString GFIntToWideString(long v) { return CWideString(Num(v), 0); }
namespace NGameText {
struct CDataBank { CWideString GetTextBySymbol(const CCharString& sym) const { return CWideString("<" + sym.s + ">", 0); } };
}
enum EHeroAbility { HERO_ABILITY_NULL = 0 };
enum ETCInterfaceType { TCI_INVENTORY_EXPERIENCE = 22 };
template<class T> struct CCountedPointer {
    T* P;
    CCountedPointer() : P(0) { }
    CCountedPointer(T* p) : P(p) { }
    T* Get() const { return P; }
    T* operator->() const { return P; }
    void Reset(T* p) { P = p; }
    template<class U> void SetStaticCast(const CCountedPointer<U>& o) { P = static_cast<T*>(o.Get()); }
};
// ---- UI ----
namespace NUISystem {
enum EType { UI_TYPE_SPRITE = 0, UI_TYPE_TEXT = 1, UI_TYPE_TITLE = 6 };
enum EActionType { UI_ACTION_TYPE_NONE, UI_ACTION_TYPE_GIVE_EXCLUSIVE_INPUT = 11, UI_ACTION_TYPE_SHOW_SPELL_SELECTION = 22,
    UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN = 33 };
struct CActionParam { virtual ~CActionParam() { } virtual Str Show() const = 0; };
struct CActionParamSLONG : CActionParam { long V; CActionParamSLONG(long v) : V(v) { } Str Show() const { return "s" + Num(V); } };
struct CActionParamULONG : CActionParam { unsigned long V; CActionParamULONG(unsigned long v) : V(v) { } Str Show() const { return "u" + Num((long)V); } };
struct CAction { EActionType ActionType; CAction() : ActionType(UI_ACTION_TYPE_NONE) { } };
struct CDeletion { enum EMethod { UI_DELETE_NONE, UI_DELETE_THIS_INSTANCE }; EMethod Method; CDeletion() : Method(UI_DELETE_NONE) { } };
struct CComponent : CActionParam {
    Str Name; long Graphic; Str Text; long TextId; bool Unavailable; long Offsets; int DeletionMethod; EType Type; long EmptyDef;
    std::vector<CComponent*> Children, Fake, Scrolling, NonScrolling, Slots;
    std::vector<CAction*> Actions; std::vector<Str> Params;
    CComponent() : Graphic(-1), TextId(-1), Unavailable(false), Offsets(-1), DeletionMethod(-1), Type(UI_TYPE_TEXT), EmptyDef(-1) { }
    Str Show() const { return "c:" + Name; }
    CComponent* FindChild(const CCharString& name);
    CComponent* FindFirstChildOfTypeNonRecursive(EType t);
    void AddChild(const CCountedPointer<CComponent>& c) { Children.push_back(c.Get()); }
    void AddAction(const CCountedPointer<CAction>& a) { Actions.push_back(a.Get()); }
    void MakeUnavailable() { Unavailable = true; }
    void SetDeletion(const CDeletion& d) { DeletionMethod = d.Method; }
    EType GetType() const { return Type; }
};
struct CDraggable : CComponent { };
struct CList : CDraggable {
    void AddNonScrollingChild(const CCountedPointer<CComponent>& c) { NonScrolling.push_back(c.Get()); }
    void AddScrollingChildAndSetOffsets(const CCountedPointer<CComponent>& c) { Scrolling.push_back(c.Get()); }
    void InitialiseOffsets(unsigned long n) { Offsets = (long)n; }
};
struct CViewport : CList { void AddFakeChild(CComponent* c) { Fake.push_back(c); } };
struct CSprite : CViewport { void SetGraphicIndex(long g) { Graphic = g; } };
struct CText : CSprite {
    const CWideString& GetValue() const { static CWideString v; v = CWideString(Text, 0); return v; }
    void SetValue(const CWideString& t, bool, bool) { Text = t.s; }
    void SetValue(unsigned long id, const CWideString& t, bool) { TextId = (long)id; Text = t.s; }
};
struct CMenuEntry : CText { void AddUIActionParam(CActionParam* p) { Params.push_back(p->Show()); } };
struct CSpellContainerList : CMenuEntry {
    CComponent* Selection; CComponent* Indication;
    CSpellContainerList() : Selection(0), Indication(0) { }
    void AddSpellSelectionChild(const CCountedPointer<CComponent>& c) { Selection = c.Get(); }
    void AddSpellIndicationChild(const CCountedPointer<CComponent>& c) { Indication = c.Get(); }
    void AddChildAndSetOffsets(const CCountedPointer<CComponent>& c) { Slots.push_back(c.Get()); }
};
struct CSpellContainer : CSpellContainerList { void SetEmptySlotDef(long d) { EmptyDef = d; } };
static std::vector<Str> gMissing;
static bool Missing(const Str& n) { return std::find(gMissing.begin(), gMissing.end(), n) != gMissing.end(); }
static long gIconType = 0;
static std::vector<CComponent*> gMade;
static CComponent* Make(const Str& name) { CSpellContainer* c = new CSpellContainer; c->Name = name; if (name == "UI_ICON_TRADE_TEMPLATE") c->Type = (EType)gIconType; Log("make " + name); gMade.push_back(c); return c; }
CComponent* CComponent::FindChild(const CCharString& name)
{
    if (Missing(name.s)) return 0;
    for (size_t i = 0; i < Children.size(); ++i) if (Children[i]->Name == name.s) return Children[i];
    CComponent* c = Make(name.s); c->Text = "T(" + name.s + ")"; Children.push_back(c); return c;
}
CComponent* CComponent::FindFirstChildOfTypeNonRecursive(EType t)
{
    if (t != UI_TYPE_TITLE || Missing("TITLE")) return 0;
    CComponent* c = Make("TITLE:" + Name); c->Text = "Title"; Children.push_back(c); return c;
}
struct CManager {
    static CManager* GetInstance() { static CManager m; return &m; }
    CComponent* CreateComponent(const CCharString& name, bool) { return Make(name.s); }
    CComponent* CreateMenuEntryComponent(const CWideString& text, float, float, const CCharString& templ, float, const CCharString, float, CCharString button)
    { CComponent* c = Make("ENTRY " + templ.s + " " + button.s); c->Text = text.s; return c; }
};
struct CUIMiscThingsDef { CWideString SpaceSeparator; unsigned long TotalSpellsInPalettes; unsigned long TotalSpellsInContainer; };
}
// ---- defs ----
template<class T> struct CDefPointer { T* P; CDefPointer() : P(0) { } CDefPointer(T* p) : P(p) { } T* operator->() const { return P; } };
struct CHeroAbilityDef { long IconGraphicIndex; std::vector<long> LevDescription; };
static NUISystem::CUIMiscThingsDef gMisc;
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<const NUISystem::CUIMiscThingsDef>* r) const { if (index != 77) return false; r->P = &gMisc; return true; }
    long GetDefGlobalIndexFromName(const CCharString& name) const { return name.s == "UI_SPELL_EMPTY_TEMPLATE" ? 555 : -1; }
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    long GetGlobalIndexFromClassIndex(const CCharString& name, long i) const { return (name.s == "UI_MISC_THINGS_DEF" && i == 1) ? 77 : 0; }
};
struct CMainGameComponent {
    NGameText::CDataBank Bank;
    const NGameText::CDataBank& GetTextBank() const { return Bank; }
};
// ---- thing / TCs ----
struct CTCInventoryExperience { CWideString GetExperienceAbilityName(EHeroAbility a) const { return CWideString("Spell" + Num(a), 0); } };
struct CTCInventoryStats { static CWideString GetXString() { return CWideString(" x", 0); } };
struct CThing {
    CTCInventoryExperience Experience; bool HasExperience;
    bool GetTC(ETCInterfaceType id, CTCInventoryExperience** out) { if (id != 22 || !HasExperience) return false; *out = &Experience; return true; }
};
// ---- the row's class ----
class CTCInventoryAbilities {
public:
    enum EUISpellCategory { SPELL_CATEGORY_ATTACK = 0, SPELL_CATEGORY_SURROUND = 1, SPELL_CATEGORY_PHYSICAL = 2 };
    CThing* ParentThing; CMainGameComponent Main;
    std::vector<EHeroAbility> ReceivedAbilityOrder;
    std::vector<std::pair<long, CDefPointer<const CHeroAbilityDef> > > Abilities;
    std::vector<EHeroAbility> AssignedAbilities, AssignedAbilitiesSave;
    std::map<EUISpellCategory, NUISystem::CComponent*> CategoriesLists;
    std::map<EHeroAbility, NUISystem::CComponent*> UnassignedIcons;
    std::map<EHeroAbility, NUISystem::CComponent*> AbilitiesMenuEntries;
    const CMainGameComponent& PeekComponent() const { return Main; }
    NUISystem::CComponent* GetMenuEntryFromCategory(EUISpellCategory c, NUISystem::CComponent* root)
    { Log("menu " + Num(c) + " " + root->Name); return root->FindChild(CCharString(("CATEGORY_ENTRY_" + Num(c)).c_str())); }
    EUISpellCategory GetCategory(EHeroAbility ability);
    CDefPointer<const CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability) const;
    const CDefPointer<const NUISystem::CUIMiscThingsDef> GetMiscThingsDef() const;
    long GetAbilityLevel(EHeroAbility ability) const;
    NUISystem::CComponent* GetMagicListUIComponent(long& count, NUISystem::CComponent* parent);
};
// ==== the row's statements (unchanged from src/005eeff2.cpp) ====
CTCInventoryAbilities::EUISpellCategory CTCInventoryAbilities::GetCategory(EHeroAbility ability)
{
    switch (ability) {
    case 1: case 2: case 5: case 6: case 10:
        return SPELL_CATEGORY_SURROUND;
    case 3: case 9: case 11: case 12: case 16:
        return SPELL_CATEGORY_ATTACK;
    case 4: case 8: case 13: case 14: case 15: case 17:
        return SPELL_CATEGORY_PHYSICAL;
    default:
        return SPELL_CATEGORY_ATTACK;
    }
}
CDefPointer<const CHeroAbilityDef> CTCInventoryAbilities::GetAbilityPDef(EHeroAbility ability) const
{
    return Abilities[ability].second;
}
const CDefPointer<const NUISystem::CUIMiscThingsDef> CTCInventoryAbilities::GetMiscThingsDef() const
{
    long index;
    {
        CCharString name("UI_MISC_THINGS_DEF");
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    {
        CDefPointer<const NUISystem::CUIMiscThingsDef> def;
        CGameDefinitionManager::Get().GetDef(index, &def);
        return def;
    }
}
#pragma auto_inline(on)
long CTCInventoryAbilities::GetAbilityLevel(EHeroAbility ability) const
{
    return Abilities[ability].first;
}

NUISystem::CComponent* CTCInventoryAbilities::GetMagicListUIComponent(long& count, NUISystem::CComponent* parent)
{
    const NGameText::CDataBank& bank = PeekComponent().GetTextBank();
    std::list<EHeroAbility> owned;
    NUISystem::CManager* ui = NUISystem::CManager::GetInstance();
    const CDefPointer<const NUISystem::CUIMiscThingsDef> misc = GetMiscThingsDef();
    for (long n = 1; n < 20; ++n)
        if (Abilities[n].first >= 0)
            owned.push_back(static_cast<EHeroAbility>(n));
    AssignedAbilities.clear();
    AssignedAbilitiesSave.clear();
    unsigned long received = ReceivedAbilityOrder.size() <= misc->TotalSpellsInPalettes ? ReceivedAbilityOrder.size() : misc->TotalSpellsInPalettes;
    for (unsigned long n = 0; n < received; ++n) {
        AssignedAbilities.push_back(ReceivedAbilityOrder[n]);
        AssignedAbilitiesSave.push_back(ReceivedAbilityOrder[n]);
    }
    unsigned long assigned = AssignedAbilities.size();
    if (assigned < misc->TotalSpellsInPalettes) {
        for (unsigned long n = 0; n < misc->TotalSpellsInPalettes - assigned; ++n) {
            EHeroAbility empty = static_cast<EHeroAbility>(0);
            EHeroAbility savedEmpty = static_cast<EHeroAbility>(0);
            AssignedAbilities.push_back(empty);
            AssignedAbilitiesSave.push_back(savedEmpty);
        }
    }
    for (unsigned long n = 0; n < AssignedAbilities.size() && n < AssignedAbilitiesSave.size(); ++n) {
        if (n % misc->TotalSpellsInContainer == 1 && n < AssignedAbilities.size() - 1 && n < AssignedAbilitiesSave.size() - 1) {
            EHeroAbility swapped = AssignedAbilities[n];
            AssignedAbilities[n] = AssignedAbilities[n + 1];
            AssignedAbilities[n + 1] = swapped;
            swapped = AssignedAbilitiesSave[n];
            AssignedAbilitiesSave[n] = AssignedAbilitiesSave[n + 1];
            AssignedAbilitiesSave[n + 1] = swapped;
        }
    }
    CCountedPointer<NUISystem::CComponent> palette(ui->CreateComponent("UI_SPELL_CONTAINER_LIST_TEMPLATE", false));
    CCountedPointer<NUISystem::CComponent> container(ui->CreateComponent("UI_SPELL_CONTAINER_TEMPLATE", false));
    CCountedPointer<NUISystem::CComponent> selection(ui->CreateComponent("UI_SPELL_SELECTION_TEMPLATE", false));
    CCountedPointer<NUISystem::CComponent> indication(ui->CreateComponent("UI_UNSELECT_FOR_MAGIC_ASSIGNMENT_LIST", false));
    NUISystem::CSpellContainerList* paletteList = static_cast<NUISystem::CSpellContainerList*>(palette.Get());
    paletteList->InitialiseOffsets(AssignedAbilities.size() / misc->TotalSpellsInContainer + 1);
    paletteList->AddSpellSelectionChild(selection);
    paletteList->AddSpellIndicationChild(indication);
    NUISystem::CSpellContainer* spellContainer = static_cast<NUISystem::CSpellContainer*>(container.Get());
    spellContainer->SetEmptySlotDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_SPELL_EMPTY_TEMPLATE"));
    std::vector<EHeroAbility> reordered(AssignedAbilities.size());
    for (unsigned long n = 0; n < AssignedAbilities.size(); ++n)
        reordered[AssignedAbilities.size() - (n / misc->TotalSpellsInContainer + 1) * misc->TotalSpellsInContainer + n % misc->TotalSpellsInContainer] = AssignedAbilities[n];
    for (unsigned long n = 0; n < reordered.size(); ++n) {
        if (n % misc->TotalSpellsInContainer == 0 && n != 0) {
            paletteList->AddChildAndSetOffsets(container);
            container.Reset(ui->CreateComponent("UI_SPELL_CONTAINER_TEMPLATE", false));
            spellContainer = static_cast<NUISystem::CSpellContainer*>(container.Get());
            spellContainer->SetEmptySlotDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_SPELL_EMPTY_TEMPLATE"));
        }
        CCountedPointer<NUISystem::CComponent> spell;
        if (reordered[n] != static_cast<EHeroAbility>(0)) {
            spell.Reset(ui->CreateComponent("UI_SPELL_TEMPLATE", false));
            static_cast<NUISystem::CSprite*>(spell.Get())->SetGraphicIndex(Abilities[reordered[n]].second->IconGraphicIndex);
        } else
            spell.Reset(ui->CreateComponent("UI_SPELL_EMPTY_TEMPLATE", false));
        // through the named container pointer: retail 0x005EF489 `mov ecx,[ebp+68h]` (spellContainer's frame word), not container.Get()
        spellContainer->AddChildAndSetOffsets(spell);
    }
    paletteList->AddChildAndSetOffsets(container);
    CCountedPointer<NUISystem::CComponent> actions(NUISystem::CManager::GetInstance()->CreateComponent("UI_LIST_MAGIC_ACTIONS", false));
    NUISystem::CComponent* assignment = actions->FindChild("UI_MENU_ENTRY_MAGIC_ACTION_ASSIGN");
    CCountedPointer<NUISystem::CAction> exclusive(new NUISystem::CAction);
    exclusive->ActionType = NUISystem::UI_ACTION_TYPE_GIVE_EXCLUSIVE_INPUT;
    CCountedPointer<NUISystem::CAction> show(new NUISystem::CAction);
    show->ActionType = NUISystem::UI_ACTION_TYPE_SHOW_SPELL_SELECTION;
    CCountedPointer<NUISystem::CAction> set(new NUISystem::CAction);
    set->ActionType = NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN;
    assignment->AddAction(exclusive);
    assignment->AddAction(show);
    assignment->AddAction(set);
    CategoriesLists.clear();
    unsigned long surroundCount = 0, physicalCount = 0, attackCount = 0;
    for (std::list<EHeroAbility>::iterator it = owned.begin(); it != owned.end(); ++it) {
        EUISpellCategory category = GetCategory(*it);
        if (category == SPELL_CATEGORY_SURROUND) ++surroundCount;
        if (category == SPELL_CATEGORY_PHYSICAL) ++physicalCount;
        if (category == SPELL_CATEGORY_ATTACK) ++attackCount;
    }
    if (surroundCount) {
        NUISystem::CList* list = static_cast<NUISystem::CList*>(ui->CreateComponent("UI_SUB_LIST", false));
        list->InitialiseOffsets(surroundCount);
        CCountedPointer<NUISystem::CComponent> ring(ui->CreateComponent("UI_RING_PIC_SPELL_ASSIGNMENT", false));
        list->AddNonScrollingChild(ring);
        CCountedPointer<NUISystem::CComponent> title(ui->CreateComponent("UI_SURROUND_CATEGORIES_TITLE", false));
        list->AddNonScrollingChild(title);
        CCountedPointer<NUISystem::CComponent> button(ui->CreateComponent("UI_BACK_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        button.Reset(ui->CreateComponent("UI_SELECT_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        CategoriesLists[SPELL_CATEGORY_SURROUND] = list;
    }
    if (attackCount) {
        NUISystem::CList* list = static_cast<NUISystem::CList*>(ui->CreateComponent("UI_SUB_LIST", false));
        list->InitialiseOffsets(attackCount);
        CCountedPointer<NUISystem::CComponent> ring(ui->CreateComponent("UI_RING_PIC_SPELL_ASSIGNMENT", false));
        list->AddNonScrollingChild(ring);
        CCountedPointer<NUISystem::CComponent> title(ui->CreateComponent("UI_ATTACK_CATEGORIES_TITLE", false));
        list->AddNonScrollingChild(title);
        CCountedPointer<NUISystem::CComponent> button(ui->CreateComponent("UI_BACK_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        button.Reset(ui->CreateComponent("UI_SELECT_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        CategoriesLists[SPELL_CATEGORY_ATTACK] = list;
    }
    if (physicalCount) {
        NUISystem::CList* list = static_cast<NUISystem::CList*>(ui->CreateComponent("UI_SUB_LIST", false));
        list->InitialiseOffsets(physicalCount);
        CCountedPointer<NUISystem::CComponent> ring(ui->CreateComponent("UI_RING_PIC_SPELL_ASSIGNMENT", false));
        list->AddNonScrollingChild(ring);
        CCountedPointer<NUISystem::CComponent> title(ui->CreateComponent("UI_PHYSICAL_CATEGORIES_TITLE", false));
        list->AddNonScrollingChild(title);
        CCountedPointer<NUISystem::CComponent> button(ui->CreateComponent("UI_BACK_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        button.Reset(ui->CreateComponent("UI_SELECT_FOR_LIST", false));
        list->AddNonScrollingChild(button);
        CategoriesLists[SPELL_CATEGORY_PHYSICAL] = list;
    }
    count = attackCount + physicalCount + surroundCount;
    NUISystem::CComponent* root = ui->CreateComponent("UI_MAGIC_CATEGORIES", false);
    NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(parent->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    if (viewport) {
        if (NUISystem::CComponent* description = root->FindChild("UI_ATTACK_SPELLS_DESC"))
            viewport->AddFakeChild(description);
        if (NUISystem::CComponent* description = root->FindChild("UI_PHYSICAL_SPELLS_DESC"))
            viewport->AddFakeChild(description);
        if (NUISystem::CComponent* description = root->FindChild("UI_SURROUND_SPELLS_DESC"))
            viewport->AddFakeChild(description);
    }
    UnassignedIcons.clear();
    AbilitiesMenuEntries.clear();
    for (std::list<EHeroAbility>::iterator it = owned.begin(); it != owned.end(); ++it) {
        CWideString caption;
        CWideString displayName;
        CTCInventoryExperience* experience;
        if (ParentThing->GetTC(static_cast<ETCInterfaceType>(22), &experience)) {
            caption = displayName = experience->GetExperienceAbilityName(*it);
            caption += misc->SpaceSeparator;
            caption += GFIntToWideString(GetAbilityLevel(*it) + 1);
            displayName += CWideString(L" - ") + bank.GetTextBySymbol("TEXT_GUI_MENU_LEVEL") + CWideString(L" ");
            displayName += GFIntToWideString(GetAbilityLevel(*it) + 1);
        }
        CCountedPointer<NUISystem::CComponent> entry(ui->CreateMenuEntryComponent(caption, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        AbilitiesMenuEntries[*it] = entry.Get();
        if (std::find(AssignedAbilities.begin(), AssignedAbilities.end(), *it) == AssignedAbilities.end()) {
            CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_UNASSIGNED_SPELL_ICON", false));
            entry->AddChild(icon);
            UnassignedIcons[*it] = icon.Get();
        }
        CCountedPointer<NUISystem::CMenuEntry> menuEntry;
        menuEntry.SetStaticCast(entry);
        entry->AddChild(palette);
        menuEntry->AddUIActionParam(new NUISystem::CActionParamSLONG(*it));
        menuEntry->AddUIActionParam(new NUISystem::CActionParamULONG(Abilities[*it].second->IconGraphicIndex));
        menuEntry->AddUIActionParam(palette.Get());
        CCountedPointer<NUISystem::CComponent> name(ui->CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW", false));
        CCountedPointer<NUISystem::CText> text;
        text.SetStaticCast(name);
        text->SetValue(displayName, true, true);
        entry->AddChild(name);
        CCountedPointer<NUISystem::CComponent> icon(ui->CreateComponent("UI_ICON_TRADE_TEMPLATE", false));
        if (icon->GetType() == static_cast<NUISystem::EType>(0))
            static_cast<NUISystem::CSprite*>(icon.Get())->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);
        entry->AddChild(icon);
        entry->AddChild(actions);
        CCountedPointer<NUISystem::CComponent> description(ui->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", false));
        NUISystem::CText* descriptionText = static_cast<NUISystem::CText*>(description.Get());
        descriptionText->SetValue(GetAbilityPDef(*it)->LevDescription[GetAbilityLevel(*it)], L"", true);
        entry->AddChild(description);
        if (viewport)
            viewport->AddFakeChild(description.Get());
        static_cast<NUISystem::CList*>(CategoriesLists[GetCategory(*it)])->AddScrollingChildAndSetOffsets(entry);
    }
    {
        std::map<EUISpellCategory, NUISystem::CComponent*>::iterator found = CategoriesLists.find(SPELL_CATEGORY_ATTACK);
        if (found != CategoriesLists.end()) {
            CCountedPointer<NUISystem::CComponent> children(found->second);
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_ATTACK, root);
            menu->AddChild(children);
            NUISystem::CText* title = static_cast<NUISystem::CText*>(menu->FindFirstChildOfTypeNonRecursive(static_cast<NUISystem::EType>(6)));
            if (title)
                title->SetValue(title->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(attackCount), true, true);
            NUISystem::CText* subtitle = static_cast<NUISystem::CText*>(menu->FindChild("UI_ATTACK_SUBTITLE"));
            if (subtitle)
                subtitle->SetValue(subtitle->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(attackCount), true, true);
        } else {
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_ATTACK, root);
            menu->MakeUnavailable();
            NUISystem::CComponent* arrow = menu->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    {
        std::map<EUISpellCategory, NUISystem::CComponent*>::iterator found = CategoriesLists.find(SPELL_CATEGORY_SURROUND);
        if (found != CategoriesLists.end()) {
            CCountedPointer<NUISystem::CComponent> children(found->second);
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_SURROUND, root);
            menu->AddChild(children);
            NUISystem::CText* title = static_cast<NUISystem::CText*>(menu->FindFirstChildOfTypeNonRecursive(static_cast<NUISystem::EType>(6)));
            if (title)
                title->SetValue(title->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(surroundCount), true, true);
            NUISystem::CText* subtitle = static_cast<NUISystem::CText*>(menu->FindChild("UI_SURROUND_SUBTITLE"));
            if (subtitle)
                subtitle->SetValue(subtitle->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(surroundCount), true, true);
        } else {
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_SURROUND, root);
            menu->MakeUnavailable();
            NUISystem::CComponent* arrow = menu->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    {
        std::map<EUISpellCategory, NUISystem::CComponent*>::iterator found = CategoriesLists.find(SPELL_CATEGORY_PHYSICAL);
        if (found != CategoriesLists.end()) {
            CCountedPointer<NUISystem::CComponent> children(found->second);
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_PHYSICAL, root);
            menu->AddChild(children);
            NUISystem::CText* title = static_cast<NUISystem::CText*>(menu->FindFirstChildOfTypeNonRecursive(static_cast<NUISystem::EType>(6)));
            if (title)
                title->SetValue(title->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(physicalCount), true, true);
            NUISystem::CText* subtitle = static_cast<NUISystem::CText*>(menu->FindChild("UI_PHYSICAL_SUBTITLE"));
            if (subtitle)
                subtitle->SetValue(subtitle->GetValue() + CTCInventoryStats::GetXString() + GFIntToWideString(physicalCount), true, true);
        } else {
            NUISystem::CComponent* menu = GetMenuEntryFromCategory(SPELL_CATEGORY_PHYSICAL, root);
            menu->MakeUnavailable();
            NUISystem::CComponent* arrow = menu->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    return root;
}
// ==== end of the row's statements ====
using namespace NUISystem;
typedef CTCInventoryAbilities A;
static CHeroAbilityDef gDefs[20];
static CThing gThing;
static void Setup(A& a, const long* levels)
{
    a.ParentThing = &gThing; a.Abilities.clear();
    for (long n = 0; n < 20; ++n) {
        gDefs[n].IconGraphicIndex = 100 + n; gDefs[n].LevDescription.clear();
        for (long l = 0; l < 3; ++l) gDefs[n].LevDescription.push_back(1000 + n * 10 + l);
        a.Abilities.push_back(std::make_pair(levels[n], CDefPointer<const CHeroAbilityDef>(&gDefs[n])));
    }
    gMade.clear(); gMissing.clear(); gLog.clear();
}
static CComponent* Find(const Str& name, int k = 0)
{ for (size_t i = 0; i < gMade.size(); ++i) if (gMade[i]->Name == name && k-- == 0) return gMade[i]; return 0; }
static Str Names(const std::vector<CComponent*>& v)
{ Str s; for (size_t i = 0; i < v.size(); ++i) s += (i ? "," : "") + v[i]->Name + (v[i]->Graphic >= 0 ? "#" + Num(v[i]->Graphic) : Str()); return s; }
static Str Abil(const std::vector<EHeroAbility>& v) { Str s; for (size_t i = 0; i < v.size(); ++i) s += (i ? "," : "") + Num(v[i]); return s; }
static CComponent* Child(CComponent* c, const Str& name) { for (size_t i = 0; i < c->Children.size(); ++i) if (c->Children[i]->Name == name) return c->Children[i]; return 0; }
static Str Join(const std::vector<Str>& v) { Str s; for (size_t i = 0; i < v.size(); ++i) s += (i ? " " : "") + v[i]; return s; }
static void ScenarioA()
{
    A a; long lv[20]; for (int i = 0; i < 20; ++i) lv[i] = -1;
    lv[1] = 0; lv[3] = 2; lv[4] = 1; lv[9] = 0;
    Setup(a, lv); gThing.HasExperience = true; gIconType = 0;
    gMisc.SpaceSeparator = CWideString(" ", 0); gMisc.TotalSpellsInPalettes = 6; gMisc.TotalSpellsInContainer = 3;
    a.ReceivedAbilityOrder.push_back((EHeroAbility)3); a.ReceivedAbilityOrder.push_back((EHeroAbility)1);
    a.AssignedAbilities.push_back((EHeroAbility)7); a.UnassignedIcons[(EHeroAbility)2] = 0;
    CComponent* parent = Make("PARENT");
    long count = -1;
    CComponent* root = a.GetMagicListUIComponent(count, parent);
    Check(count == 4, "A count");
    Check(root && root->Name == "UI_MAGIC_CATEGORIES", "A root");
    Check(Abil(a.AssignedAbilities) == "3,0,1,0,0,0", "A assigned (fill + slot-1 swap)");
    Check(Abil(a.AssignedAbilitiesSave) == "3,0,1,0,0,0", "A assigned save");
    CComponent* pal = Find("UI_SPELL_CONTAINER_LIST_TEMPLATE");
    Check(pal && pal->Offsets == 3, "A palette offsets");
    CSpellContainerList* pl = static_cast<CSpellContainerList*>(pal);
    Check(pl->Selection && pl->Selection->Name == "UI_SPELL_SELECTION_TEMPLATE", "A selection");
    Check(pl->Indication && pl->Indication->Name == "UI_UNSELECT_FOR_MAGIC_ASSIGNMENT_LIST", "A indication");
    Check(pal->Slots.size() == 2, "A two containers");
    if (pal->Slots.size() == 2) {
        Check(pal->Slots[0]->EmptyDef == 555 && pal->Slots[1]->EmptyDef == 555, "A empty slot defs");
        Check(Names(pal->Slots[0]->Slots) == "UI_SPELL_EMPTY_TEMPLATE,UI_SPELL_EMPTY_TEMPLATE,UI_SPELL_EMPTY_TEMPLATE", "A container 1");
        Check(Names(pal->Slots[1]->Slots) == "UI_SPELL_TEMPLATE#103,UI_SPELL_EMPTY_TEMPLATE,UI_SPELL_TEMPLATE#101", "A container 2");
    }
    CComponent* acts = Find("UI_LIST_MAGIC_ACTIONS"); CComponent* as = acts ? Child(acts, "UI_MENU_ENTRY_MAGIC_ACTION_ASSIGN") : 0;
    Check(as && as->Actions.size() == 3 && as->Actions[0]->ActionType == 11 && as->Actions[1]->ActionType == 22 && as->Actions[2]->ActionType == 33, "A actions");
    CComponent* at = a.CategoriesLists[A::SPELL_CATEGORY_ATTACK]; CComponent* su = a.CategoriesLists[A::SPELL_CATEGORY_SURROUND];
    CComponent* ph = a.CategoriesLists[A::SPELL_CATEGORY_PHYSICAL];
    Check(at && su && ph && at->Offsets == 2 && su->Offsets == 1 && ph->Offsets == 1, "A list offsets");
    Check(at && Names(at->NonScrolling) == "UI_RING_PIC_SPELL_ASSIGNMENT,UI_ATTACK_CATEGORIES_TITLE,UI_BACK_FOR_LIST,UI_SELECT_FOR_LIST", "A attack list head");
    Check(su && Names(su->NonScrolling) == "UI_RING_PIC_SPELL_ASSIGNMENT,UI_SURROUND_CATEGORIES_TITLE,UI_BACK_FOR_LIST,UI_SELECT_FOR_LIST", "A surround list head");
    CComponent* e3 = a.AbilitiesMenuEntries[(EHeroAbility)3]; CComponent* e9 = a.AbilitiesMenuEntries[(EHeroAbility)9];
    CComponent* e4 = a.AbilitiesMenuEntries[(EHeroAbility)4]; CComponent* e1 = a.AbilitiesMenuEntries[(EHeroAbility)1];
    Check(a.AbilitiesMenuEntries.size() == 4 && e1 && e3 && e4 && e9, "A menu entries");
    Check(at && at->Scrolling.size() == 2 && at->Scrolling[0] == e3 && at->Scrolling[1] == e9, "A attack entries");
    Check(su && su->Scrolling.size() == 1 && su->Scrolling[0] == e1 && ph && ph->Scrolling.size() == 1 && ph->Scrolling[0] == e4, "A other entries");
    Check(a.UnassignedIcons.size() == 2 && a.UnassignedIcons[(EHeroAbility)4] && a.UnassignedIcons[(EHeroAbility)9], "A unassigned icons");
    if (e3 && e4) {
        Check(e3->Name == "ENTRY UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW PC_BUTTON_BASTARD" && e3->Text == "Spell3 3", "A entry 3 caption");
        Check(Join(e3->Params) == "s3 u103 c:UI_SPELL_CONTAINER_LIST_TEMPLATE", "A entry 3 params");
        Check(Names(e3->Children) == "UI_SPELL_CONTAINER_LIST_TEMPLATE,UI_TEXT_TRADE_NAME_TEMPLATE_NEW,UI_ICON_TRADE_TEMPLATE#103,UI_LIST_MAGIC_ACTIONS,UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", "A entry 3 children");
        Check(Names(e4->Children) == "UI_UNASSIGNED_SPELL_ICON,UI_SPELL_CONTAINER_LIST_TEMPLATE,UI_TEXT_TRADE_NAME_TEMPLATE_NEW,UI_ICON_TRADE_TEMPLATE#104,UI_LIST_MAGIC_ACTIONS,UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", "A entry 4 children");
        if (e3->Children.size() == 5) {
            Check(e3->Children[1]->Text == "Spell3 - <TEXT_GUI_MENU_LEVEL> 3", "A entry 3 name text");
            Check(e3->Children[4]->TextId == 1032 && e3->Children[4]->Text == "", "A entry 3 description");
        }
        Check(e4->Text == "Spell4 2", "A entry 4 caption");
    }
    CComponent* vp = parent ? Child(parent, "UI_SCROLLING_VIEWPORT_TEXT_AREA") : 0;
    Check(vp && Names(vp->Fake) == "UI_ATTACK_SPELLS_DESC,UI_PHYSICAL_SPELLS_DESC,UI_SURROUND_SPELLS_DESC,UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW,UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW,UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW,UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", "A viewport");
    CComponent* m0 = Child(root, "CATEGORY_ENTRY_0"); CComponent* m1 = Child(root, "CATEGORY_ENTRY_1"); CComponent* m2 = Child(root, "CATEGORY_ENTRY_2");
    Check(m0 && m1 && m2 && Child(m0, "UI_SUB_LIST") == at && Child(m1, "UI_SUB_LIST") == su && Child(m2, "UI_SUB_LIST") == ph, "A category lists attached");
    if (m0 && m1 && m2) {
        Check(Child(m0, "TITLE:CATEGORY_ENTRY_0") && Child(m0, "TITLE:CATEGORY_ENTRY_0")->Text == "Title x2", "A attack title");
        Check(Child(m0, "UI_ATTACK_SUBTITLE") && Child(m0, "UI_ATTACK_SUBTITLE")->Text == "T(UI_ATTACK_SUBTITLE) x2", "A attack subtitle");
        Check(Child(m1, "UI_SURROUND_SUBTITLE") && Child(m1, "UI_SURROUND_SUBTITLE")->Text == "T(UI_SURROUND_SUBTITLE) x1", "A surround subtitle");
        Check(Child(m2, "TITLE:CATEGORY_ENTRY_2") && Child(m2, "TITLE:CATEGORY_ENTRY_2")->Text == "Title x1", "A physical title");
        Check(!m0->Unavailable && !m1->Unavailable && !m2->Unavailable, "A all available");
    }
    Check(Join(gLog).find("menu 0 UI_MAGIC_CATEGORIES") != Str::npos && Join(gLog).find("menu 2 UI_MAGIC_CATEGORIES") != Str::npos, "A menu lookups");
}
static void ScenarioB()
{
    A a; long lv[20]; for (int i = 0; i < 20; ++i) lv[i] = -1;
    lv[3] = 1; lv[9] = 0;
    Setup(a, lv); gThing.HasExperience = false; gIconType = 1; gMissing.push_back("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    gMisc.SpaceSeparator = CWideString(" ", 0); gMisc.TotalSpellsInPalettes = 4; gMisc.TotalSpellsInContainer = 2;
    const long order[] = { 9, 3, 12, 16, 5, 1, 2 };
    for (int i = 0; i < 7; ++i) a.ReceivedAbilityOrder.push_back((EHeroAbility)order[i]);
    CComponent* parent = Make("PARENT");
    long count = -1;
    CComponent* root = a.GetMagicListUIComponent(count, parent);
    Check(count == 2, "B count");
    Check(Abil(a.AssignedAbilities) == "9,12,3,16", "B assigned (truncated + swap)");
    CComponent* pal = Find("UI_SPELL_CONTAINER_LIST_TEMPLATE");
    Check(pal && pal->Offsets == 3 && pal->Slots.size() == 2, "B palette");
    if (pal && pal->Slots.size() == 2) {
        Check(Names(pal->Slots[0]->Slots) == "UI_SPELL_TEMPLATE#103,UI_SPELL_TEMPLATE#116", "B container 1");
        Check(Names(pal->Slots[1]->Slots) == "UI_SPELL_TEMPLATE#109,UI_SPELL_TEMPLATE#112", "B container 2");
    }
    Check(a.CategoriesLists.size() == 1 && a.CategoriesLists[A::SPELL_CATEGORY_ATTACK]->Offsets == 2, "B only attack list");
    Check(a.UnassignedIcons.empty(), "B no unassigned");
    CComponent* e3 = a.AbilitiesMenuEntries[(EHeroAbility)3];
    Check(e3 && e3->Text == "" && e3->Children.size() == 5 && e3->Children[1]->Text == "" && e3->Children[2]->Graphic == -1, "B entry 3 (no experience, text icon)");
    Check(e3 && e3->Children.size() == 5 && e3->Children[4]->TextId == 1031, "B entry 3 description");
    CComponent* m1 = Child(root, "CATEGORY_ENTRY_1"); CComponent* m2 = Child(root, "CATEGORY_ENTRY_2"); CComponent* m0 = Child(root, "CATEGORY_ENTRY_0");
    Check(m1 && m1->Unavailable && Child(m1, "UI_SELECTION_ARROW_RIGHT") && Child(m1, "UI_SELECTION_ARROW_RIGHT")->DeletionMethod == 1, "B surround unavailable");
    Check(m2 && m2->Unavailable && Child(m2, "UI_SELECTION_ARROW_RIGHT") && Child(m2, "UI_SELECTION_ARROW_RIGHT")->DeletionMethod == 1, "B physical unavailable");
    Check(m0 && !m0->Unavailable && Child(m0, "TITLE:CATEGORY_ENTRY_0") && Child(m0, "TITLE:CATEGORY_ENTRY_0")->Text == "Title x2", "B attack title");
    Check(!Child(parent, "UI_SCROLLING_VIEWPORT_TEXT_AREA"), "B no viewport");
}
int main()
{
    ScenarioA();
    ScenarioB();
    if (gFail == 0) std::printf("005eeff2_TEST PASS\n");
    return gFail;
}
