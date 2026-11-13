// Behaviour test for CTCInventoryAbilities::ConstructPCMagicList @ 0x005F1B10: the PC skills screen, magic page.
// Subject: the row's own statements (GetCategory / GetAbilityPDef / GetMiscThingsDef / GetAbilityLevel / GetAbilityGraphicIndex and
// ConstructPCMagicList, copied unchanged from src/005f1b10.cpp) over a mock engine that records every UI call. Scenarios: (A) four
// owned spells in three categories, a received palette shorter than the palette (padded with empty slots), expression-item quick
// slots (empty / spell item / expression item), spell quick slots (assigned / empty / past the received order), menu entries with
// level captions, unassigned icons, descriptions, category title counts; (B) received order longer than the palette (truncated), no
// experience TC (empty captions), no frame, no viewport, one category empty (menu entry made unavailable, arrow deleted).
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
// ---- strings ----
struct CCharString {
    Str s;
    CCharString() { }
    CCharString(const char* p) : s(p) { }
};
static CCharString operator+(const char* a, const CCharString& b) { CCharString r; r.s = Str(a) + b.s; return r; }
struct CWideString {
    Str s;
    CWideString() { }
    template<class C> CWideString(const C* w) { while (*w) s += (char)*w++; }
    CWideString(const Str& x, int) : s(x) { }
    CWideString& operator+=(const CWideString& o) { s += o.s; return *this; }
};
static CWideString operator+(const CWideString& a, const CWideString& b) { return CWideString(a.s + b.s, 0); }
static CCharString GFIntToCharString(long v) { return CCharString(Num(v).c_str()); }
static CWideString GFIntToWideString(long v) { return CWideString(Num(v), 0); }
namespace NGameText {
struct CDataBank { CWideString GetTextBySymbol(const CCharString& sym) const { return CWideString("<" + sym.s + ">", 0); } };
}
enum EHeroAbility { HERO_ABILITY_NULL = 0 };
enum ETCInterfaceType { TCI_INVENTORY = 7, TCI_INVENTORY_EXPERIENCE = 22 };
// ---- UI ----
namespace NUISystem {
enum EType { UI_TYPE_SPRITE = 0, UI_TYPE_TEXT = 1 };
enum { UI_EVENT_MOUSE_MOVEMENT = 7, UI_EVENT_COMPONENT_DROPPED = 9 };
enum EActionType {
    UI_ACTION_TYPE_NONE, UI_ACTION_TYPE_OBSERVE_EVENT, UI_ACTION_TYPE_IGNORE_EVENT, UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC,
    UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC, UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC, UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_PC,
    UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC, UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC,
    UI_ACTION_TYPE_ASSIGN_SPELL_PC, UI_ACTION_TYPE_UNASSIGN_SPELL_PC, UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_SWAPPING_PC
};
struct CActionParam { virtual ~CActionParam() { } virtual Str Show() const = 0; };
struct CActionParamSLONG : CActionParam { long V; CActionParamSLONG(long v) : V(v) { } Str Show() const { return "s" + Num(V); } };
struct CActionParamULONG : CActionParam { unsigned long V; CActionParamULONG(unsigned long v) : V(v) { } Str Show() const { return "u" + Num((long)V); } };
struct CAction;
template<class T> struct CCountedPointer {
    T* P;
    CCountedPointer() : P(0) { }
    CCountedPointer(T* p) : P(p) { }
    T* Get() const { return P; }
    T* operator->() const { return P; }
    bool IsNull() const { return P == 0; }
    void Reset(T* p) { P = p; }
    template<class U> void SetStaticCast(const CCountedPointer<U>& o) { P = static_cast<T*>(o.Get()); }
};
struct CAction {
    EActionType ActionType; std::list<CActionParam*> UIActionParams; std::vector<CActionParam*> Params; bool SelfParam;
    CAction() : ActionType(UI_ACTION_TYPE_NONE), SelfParam(false) { }
    void AddParam(CActionParam* p) { Params.push_back(p); }
    void AddParam(CActionParam* p, CCountedPointer<CAction>& self) { Params.push_back(p); SelfParam = (self.Get() == this); }
    Str Show() const;
};
struct CDeletion { enum EMethod { UI_DELETE_NONE, UI_DELETE_THIS_INSTANCE }; EMethod Method; CDeletion() : Method(UI_DELETE_NONE) { } };
struct CComponent : CActionParam {
    Str Name; long Graphic; Str Text; long TextId; bool Unavailable; bool SnapBack; long Offsets; int DeletionMethod; EType Type;
    std::vector<CComponent*> Children, PositionChildren, Fake, Scrolling, NonScrolling;
    std::vector<CAction*> Actions, OnSelected, OnDropped, OnDroppedNowhere, OnInactive;
    CComponent() : Graphic(-1), TextId(-1), Unavailable(false), SnapBack(true), Offsets(-1), DeletionMethod(-1), Type(UI_TYPE_TEXT) { }
    Str Show() const { return "c:" + Name; }
    CComponent* FindChild(const CCharString& name);
    void AddPositionChild(CComponent* c) { PositionChildren.push_back(c); }
    void AddChild(const CCountedPointer<CComponent>& c) { Children.push_back(c.Get()); }
    void AddAction(const CCountedPointer<CAction>& a) { Actions.push_back(a.Get()); }
    void AddActionOnSelected(const CCountedPointer<CAction>& a) { OnSelected.push_back(a.Get()); }
    void AddActionOnDropped(const CCountedPointer<CAction>& a) { OnDropped.push_back(a.Get()); }
    void AddActionOnDroppedNowhere(const CCountedPointer<CAction>& a) { OnDroppedNowhere.push_back(a.Get()); }
    void AddActionOnInactive(const CCountedPointer<CAction>& a) { OnInactive.push_back(a.Get()); }
    void MakeUnavailable() { Unavailable = true; }
    void SetDeletion(const CDeletion& d) { DeletionMethod = d.Method; }
    EType GetType() const { return Type; }
};
Str CAction::Show() const
{
    Str s = Num(ActionType) + (SelfParam ? " self" : "");
    for (size_t i = 0; i < Params.size(); ++i) s += " " + Params[i]->Show();
    for (std::list<CActionParam*>::const_iterator i = UIActionParams.begin(); i != UIActionParams.end(); ++i) s += " " + (*i)->Show();
    return s;
}
struct CDraggable : CComponent { void SetSnapBackIfDroppedNowhere(bool b) { SnapBack = b; } };
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
struct CMenuEntry : CText { };
static std::vector<Str> gMissing;
static std::vector<CComponent*> gCreated;
static bool Missing(const Str& n) { return std::find(gMissing.begin(), gMissing.end(), n) != gMissing.end(); }
static CComponent* Make(const Str& name) { CMenuEntry* c = new CMenuEntry; c->Name = name; if (name == "PC_ICON") c->Type = UI_TYPE_SPRITE; gCreated.push_back(c); return c; }
CComponent* CComponent::FindChild(const CCharString& name)
{
    if (Missing(name.s)) return 0;
    for (size_t i = 0; i < Children.size(); ++i) if (Children[i]->Name == name.s) return Children[i];
    CComponent* c = Make(name.s); c->Text = "T(" + name.s + ")"; Children.push_back(c); return c;
}
struct CManager {
    static CManager* GetInstance() { static CManager m; return &m; }
    CComponent* CreateComponent(const CCharString& name, bool) { return Make(name.s); }
    CComponent* CreateMenuEntryComponent(const CWideString& text, float, float, const CCharString& templ, float, const CCharString, float, CCharString button)
    { CComponent* c = Make("ENTRY " + templ.s + " " + button.s); c->Text = text.s; return c; }
};
struct CUIMiscThingsDef { CWideString SpaceSeparator; unsigned long TotalSpellsInPalettes; };
}
using NUISystem::CComponent;
using NUISystem::CAction;
using NUISystem::CActionParamSLONG;
using NUISystem::CActionParamULONG;
using namespace NUISystem;
// ---- defs ----
template<class T> struct CDefPointer { T* P; CDefPointer() : P(0) { } CDefPointer(T* p) : P(p) { } T* operator->() const { return P; } bool IsNull() const { return P == 0; } };
struct CHeroAbilityDef { long IconGraphicIndex; std::vector<long> LevDescription; };
struct CAbilityDef { EHeroAbility Ability; };
struct CThingObjectDef {
    CAbilityDef* Ability;
    bool GetSubDef(CDefPointer<const CAbilityDef>* out) const { if (!Ability) return false; out->P = Ability; return true; }
};
static CUIMiscThingsDef gMisc;
static std::map<long, CThingObjectDef> gThingDefs;
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<const CUIMiscThingsDef>* r) const { if (index != 77) return false; r->P = &gMisc; return true; }
    bool GetDef(long index, CDefPointer<const CThingObjectDef>* r) const
    { std::map<long, CThingObjectDef>::const_iterator f = gThingDefs.find(index); if (f == gThingDefs.end()) return false; r->P = &f->second; return true; }
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    long GetGlobalIndexFromClassIndex(const CCharString& name, long i) const { return (name.s == "UI_MISC_THINGS_DEF" && i == 1) ? 77 : 0; }
};
struct CMainGameComponent {
    NGameText::CDataBank Bank;
    const NGameText::CDataBank& GetTextBank() const { return Bank; }
    const CDefinitionManager& PeekDefinitionManager() const { return CGameDefinitionManager::Get(); }
};
// ---- thing / TCs ----
struct CTCInventory {
    std::vector<long> QuickAccessItems;
    long GetQuickAccessItemGraphicIndex(unsigned long n) const { return 1000 + QuickAccessItems[n]; }
};
struct CTCInventoryExperience { CWideString GetExperienceAbilityName(EHeroAbility a) const { return CWideString("Spell" + Num(a), 0); } };
struct CTCInventoryStats { static CWideString GetXString() { return CWideString(" x", 0); } };
struct CThing {
    CTCInventory Inventory; CTCInventoryExperience Experience; bool HasExperience;
    template<class T> bool GetTC(T** out);
    bool GetTC(ETCInterfaceType id, CTCInventoryExperience** out) { if (id != 22 || !HasExperience) return false; *out = &Experience; return true; }
};
template<> bool CThing::GetTC<CTCInventory>(CTCInventory** out) { *out = &Inventory; return true; }
// ---- the row's class ----
class CTCInventoryAbilities {
public:
    enum EUISpellCategory { SPELL_CATEGORY_ATTACK = 0, SPELL_CATEGORY_SURROUND = 1, SPELL_CATEGORY_PHYSICAL = 2 };
    CThing* ParentThing; CMainGameComponent Main;
    std::vector<EHeroAbility> ReceivedAbilityOrder;
    std::vector<std::pair<long, CDefPointer<const CHeroAbilityDef> > > Abilities;
    std::vector<EHeroAbility> AssignedAbilities, AssignedAbilitiesSave;
    std::map<EUISpellCategory, CComponent*> CategoriesLists;
    std::map<EHeroAbility, CComponent*> UnassignedIcons;
    std::map<EHeroAbility, CComponent*> AbilitiesMenuEntries;
    std::vector<Str> Calls;
    const CMainGameComponent& PeekComponent() const { return Main; }
    CComponent* GetMenuEntryFromCategory(EUISpellCategory c, CComponent* root)
    { Calls.push_back("menu " + Num(c) + " " + root->Name); return root->FindChild(CCharString(("CATEGORY_ENTRY_" + Num(c)).c_str())); }
    EUISpellCategory GetCategory(EHeroAbility ability);
    CDefPointer<const CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability) const;
    const CDefPointer<const NUISystem::CUIMiscThingsDef> GetMiscThingsDef() const;
    long GetAbilityLevel(EHeroAbility ability) const;
    long GetAbilityGraphicIndex(EHeroAbility ability) const;
    CComponent* ConstructPCMagicList(CComponent* parent);
};
// ==== the row's statements (unchanged from src/005f1b10.cpp) ====
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
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CDefPointer<const NUISystem::CUIMiscThingsDef> def;
    CGameDefinitionManager::Get().GetDef(index, &def);
    return def;
}
#pragma auto_inline(on)
long CTCInventoryAbilities::GetAbilityLevel(EHeroAbility ability) const
{
    return Abilities[ability].first;
}
long CTCInventoryAbilities::GetAbilityGraphicIndex(EHeroAbility ability) const
{
    return Abilities[ability].second->IconGraphicIndex;
}

CComponent* CTCInventoryAbilities::ConstructPCMagicList(CComponent* parent)
{
    NUISystem::CManager* ui=NUISystem::CManager::GetInstance();
    CComponent* frame=parent->FindChild("PC_UI_FRAME");
    const NGameText::CDataBank& bank=PeekComponent().GetTextBank();
    std::list<EHeroAbility> owned;
    const CDefPointer<const NUISystem::CUIMiscThingsDef> misc=GetMiscThingsDef();
    for(long n=1;n<20;++n)
        if(Abilities[n].first>=0) owned.push_back(static_cast<EHeroAbility>(n));
    AssignedAbilities.clear();
    AssignedAbilitiesSave.clear();
    unsigned long count;
    if(ReceivedAbilityOrder.size()<=misc->TotalSpellsInPalettes) count=ReceivedAbilityOrder.size();
    else count=misc->TotalSpellsInPalettes;
    for(unsigned long n=0;n<count;++n) {
        AssignedAbilities.push_back(ReceivedAbilityOrder[n]);
        AssignedAbilitiesSave.push_back(ReceivedAbilityOrder[n]);
    }
    unsigned long assigned=AssignedAbilities.size();
    if(assigned<misc->TotalSpellsInPalettes) {
        for(unsigned long n=0;n<misc->TotalSpellsInPalettes-assigned;++n) {
            EHeroAbility empty=static_cast<EHeroAbility>(0);
            EHeroAbility savedEmpty=static_cast<EHeroAbility>(0);
            AssignedAbilities.push_back(empty);
            AssignedAbilitiesSave.push_back(savedEmpty);
        }
    }
    CategoriesLists.clear();
    unsigned long surroundCount=0,physicalCount=0,attackCount=0;
    for(std::list<EHeroAbility>::iterator it=owned.begin();it!=owned.end();++it) {
        EUISpellCategory category=GetCategory(*it);
        if(category==SPELL_CATEGORY_SURROUND) ++surroundCount;
        if(category==SPELL_CATEGORY_PHYSICAL) ++physicalCount;
        if(category==SPELL_CATEGORY_ATTACK) ++attackCount;
    }
    if(surroundCount) {
        NUISystem::CList* list=static_cast<NUISystem::CList*>(ui->CreateComponent("PC_SUB_LIST",false));
        list->InitialiseOffsets(surroundCount);
        CCountedPointer<CComponent> title(ui->CreateComponent("PC_TITLE_SURROUND",false));
        list->AddNonScrollingChild(title);
        if(frame) frame->AddPositionChild(title.Get());
        CategoriesLists[SPELL_CATEGORY_SURROUND]=list;
    }
    if(attackCount) {
        NUISystem::CList* list=static_cast<NUISystem::CList*>(ui->CreateComponent("PC_SUB_LIST",false));
        list->InitialiseOffsets(attackCount);
        CCountedPointer<CComponent> title(ui->CreateComponent("PC_TITLE_ATTACK",false));
        list->AddNonScrollingChild(title);
        if(frame) frame->AddPositionChild(title.Get());
        CategoriesLists[SPELL_CATEGORY_ATTACK]=list;
    }
    if(physicalCount) {
        NUISystem::CList* list=static_cast<NUISystem::CList*>(ui->CreateComponent("PC_SUB_LIST",false));
        list->InitialiseOffsets(physicalCount);
        CCountedPointer<CComponent> title(ui->CreateComponent("PC_TITLE_PHYSICAL",false));
        list->AddNonScrollingChild(title);
        if(frame) frame->AddPositionChild(title.Get());
        CategoriesLists[SPELL_CATEGORY_PHYSICAL]=list;
    }
    NUISystem::CList* root=static_cast<NUISystem::CList*>(ui->CreateComponent("PC_MAGIC_CATEGORIES",false));
    CCountedPointer<CComponent> bottomRight(ui->CreateComponent("PC_POSITION_DUMMY_BOTTOM_RIGHT",false));
    root->AddNonScrollingChild(bottomRight);
    bottomRight->AddPositionChild(root->FindChild("PC_SPELL_CONTAINMENT_CONTAINER"));
    CCountedPointer<CComponent> bottomLeft(ui->CreateComponent("PC_POSITION_DUMMY_BOTTOM_LEFT",false));
    root->AddNonScrollingChild(bottomLeft);
    bottomLeft->AddPositionChild(root->FindChild("PC_EXPRESSIONS_ITEMS_CONTAINMENT_CONTAINER"));
    // expression item quick slots
    CTCInventory* inventory;
    ParentThing->GetTC(&inventory);
    for(unsigned long n=0;n<inventory->QuickAccessItems.size();++n) {
        CCharString name="PC_EXPRESSION_ITEM_CONTAINMENT_"+GFIntToCharString(n+1);
        CComponent* slot=root->FindChild(name);
        if(slot) {
            CCountedPointer<CAction> observe(new CAction);
            observe->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observe->AddParam(slot,observe);
            observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
            root->AddActionOnSelected(observe);
            CCountedPointer<CAction> observeDrop(new CAction);
            observeDrop->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observeDrop->AddParam(slot,observeDrop);
            observeDrop->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_COMPONENT_DROPPED));
            root->AddActionOnSelected(observeDrop);
            CCountedPointer<CAction> assign(new CAction);
            assign->ActionType=NUISystem::UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC;
            assign->AddParam(slot,assign);
            assign->UIActionParams.push_back(new CActionParamULONG(n));
            slot->AddAction(assign);
            if(inventory->QuickAccessItems[n]==0) {
                CCountedPointer<CComponent> inside(ui->CreateComponent("PC_EXPRESSION_ITEM_INSIDE_CONTAINER",false));
                slot->AddChild(inside);
            } else {
                CCountedPointer<CComponent> icon(ui->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER",false));
                CComponent* sprite=icon->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
                if(sprite && inventory->QuickAccessItems[n]!=0)
                    static_cast<NUISystem::CSprite*>(sprite)->SetGraphicIndex(inventory->GetQuickAccessItemGraphicIndex(n));
                CCountedPointer<CAction> action(new CAction);
                action->ActionType=NUISystem::UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC;
                action->AddParam(slot,action);
                action->UIActionParams.push_back(new CActionParamULONG(n));
                icon->AddActionOnDroppedNowhere(action);
                CDefPointer<const CThingObjectDef> def;
                bool isSpell=false;
                PeekComponent().PeekDefinitionManager().GetDef(inventory->QuickAccessItems[n],&def);
                CDefPointer<const CAbilityDef> ability;
                if(!def.IsNull() && def->GetSubDef(&ability)) {
                    isSpell=true;
                    action.Reset(new CAction);
                    action->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC;
                    action->UIActionParams.push_back(new CActionParamSLONG(0));
                    icon->AddActionOnDroppedNowhere(action);
                } else {
                    action.Reset(new CAction);
                    action->ActionType=NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_PC;
                    action->UIActionParams.push_back(new CActionParamSLONG(0));
                    icon->AddActionOnDroppedNowhere(action);
                }
                CCountedPointer<CAction> follow(new CAction);
                follow->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                follow->AddParam(icon.Get(),follow);
                follow->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                root->AddActionOnSelected(follow);
                CCountedPointer<CAction> swap(new CAction);
                if(isSpell) {
                    swap->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC;
                    swap->UIActionParams.push_back(new CActionParamSLONG(ability->Ability));
                    swap->UIActionParams.push_back(new CActionParamSLONG(n));
                    swap->AddParam(slot);
                } else {
                    swap->ActionType=NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC;
                    swap->UIActionParams.push_back(new CActionParamSLONG(inventory->QuickAccessItems[n]));
                    swap->UIActionParams.push_back(new CActionParamSLONG(n));
                    swap->AddParam(slot);
                }
                icon->AddActionOnDropped(swap);
                slot->AddChild(icon);
            }
        }
    }
    // spell quick slots
    for(unsigned long n=0;n<misc->TotalSpellsInPalettes;++n) {
        CCharString name="PC_SPELL_CONTAINMENT_"+GFIntToCharString(n+1);
        CComponent* slot=root->FindChild(name);
        if(slot) {
            CCountedPointer<CAction> observe(new CAction);
            observe->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observe->AddParam(slot,observe);
            observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
            root->AddActionOnSelected(observe);
            CCountedPointer<CAction> observeDrop(new CAction);
            observeDrop->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observeDrop->AddParam(slot,observeDrop);
            observeDrop->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_COMPONENT_DROPPED));
            root->AddActionOnSelected(observeDrop);
            CCountedPointer<CAction> assign(new CAction);
            assign->ActionType=NUISystem::UI_ACTION_TYPE_ASSIGN_SPELL_PC;
            assign->AddParam(slot,assign);
            assign->UIActionParams.push_back(new CActionParamULONG(n));
            slot->AddAction(assign);
            if(n<ReceivedAbilityOrder.size()) {
                if(ReceivedAbilityOrder[n]==0) {
                    CCountedPointer<CComponent> inside(ui->CreateComponent("PC_SPELL_INSIDE_CONTAINER",false));
                    slot->AddChild(inside);
                } else {
                    CCountedPointer<CComponent> icon(ui->CreateComponent("PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER",false));
                    CComponent* sprite=icon->FindChild("PC_SPELL_ASSIGNMENT_ICON_SPRITE");
                    if(sprite)
                        static_cast<NUISystem::CSprite*>(sprite)->SetGraphicIndex(Abilities[ReceivedAbilityOrder[n]].second->IconGraphicIndex);
                    CCountedPointer<CAction> action(new CAction);
                    action->ActionType=NUISystem::UI_ACTION_TYPE_UNASSIGN_SPELL_PC;
                    action->AddParam(slot,action);
                    action->UIActionParams.push_back(new CActionParamULONG(n));
                    icon->AddActionOnDroppedNowhere(action);
                    action.Reset(new CAction);
                    action->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC;
                    action->UIActionParams.push_back(new CActionParamSLONG(0));
                    icon->AddActionOnDroppedNowhere(action);
                    CCountedPointer<CAction> follow(new CAction);
                    follow->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                    follow->AddParam(icon.Get(),follow);
                    follow->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                    root->AddActionOnSelected(follow);
                    CCountedPointer<CAction> swap(new CAction);
                    swap->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_SWAPPING_PC;
                    swap->UIActionParams.push_back(new CActionParamSLONG(ReceivedAbilityOrder[n]));
                    swap->UIActionParams.push_back(new CActionParamSLONG(n));
                    swap->AddParam(slot);
                    icon->AddActionOnDropped(swap);
                    slot->AddChild(icon);
                }
            }
        }
    }
    NUISystem::CViewport* viewport=static_cast<NUISystem::CViewport*>(parent->FindChild("PC_DESCRIPTION_VIEWPORT"));
    if(viewport) {
        CComponent* description=root->FindChild("PC_DESCRIPTION_ATTACK_SPELLS");
        if(description) viewport->AddFakeChild(description);
        description=root->FindChild("PC_DESCRIPTION_PHYSICAL_SPELLS");
        if(description) viewport->AddFakeChild(description);
        description=root->FindChild("PC_DESCRIPTION_SURROUND_SPELLS");
        if(description) viewport->AddFakeChild(description);
    }
    if(frame) {
        CComponent* child=root->FindChild("PC_TITLE_MAGIC_CATEGORIES");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_RING_PIC_MAGIC_ATTACK");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_SUBTITLE_ATTACK_SPELLS");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_DESCRIPTION_ATTACK_SPELLS");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_RING_PIC_MAGIC_SURROUND");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_SUBTITLE_SURROUND_SPELLS");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_DESCRIPTION_SURROUND_SPELLS");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_RING_PIC_MAGIC_PHYSICAL");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_SUBTITLE_PHYSICAL_SPELLS");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_DESCRIPTION_PHYSICAL_SPELLS");
        if(child) frame->AddPositionChild(child);
        if(viewport) frame->AddPositionChild(viewport);
    }
    UnassignedIcons.clear();
    AbilitiesMenuEntries.clear();
    for(std::list<EHeroAbility>::iterator it=owned.begin();it!=owned.end();++it) {
        CWideString caption,displayName;
        CTCInventoryExperience* experience;
        if(ParentThing->GetTC(static_cast<ETCInterfaceType>(22),&experience)) {
            caption=displayName=experience->GetExperienceAbilityName(*it);
            caption+=misc->SpaceSeparator;
            caption+=GFIntToWideString(GetAbilityLevel(*it)+1);
            displayName+=CWideString(L" - ")+bank.GetTextBySymbol("TEXT_GUI_MENU_LEVEL")+CWideString(L" ");
            displayName+=GFIntToWideString(GetAbilityLevel(*it)+1);
        }
        CCountedPointer<CComponent> entry(ui->CreateMenuEntryComponent(caption,0.0f,1.0f,"PC_MENU_ENTRY_TEMPLATE",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        CCountedPointer<CComponent> icon(ui->CreateComponent("PC_SPELL_ASSIGNMENT_ICON",false));
        CComponent* sprite=icon->FindChild("PC_SPELL_ASSIGNMENT_ICON_SPRITE");
        CCountedPointer<CComponent> faded(ui->CreateComponent("PC_SPELL_ICON_FADED",false));
        NUISystem::CSprite* fadedSprite=static_cast<NUISystem::CSprite*>(faded.Get());
        CCountedPointer<CAction> assign(new CAction);
        assign->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC;
        assign->UIActionParams.push_back(new CActionParamSLONG(*it));
        icon->AddActionOnDropped(assign);
        CCountedPointer<CAction> reset(new CAction);
        reset->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC;
        reset->UIActionParams.push_back(new CActionParamSLONG(0));
        icon->AddActionOnDroppedNowhere(reset);
        NUISystem::CDraggable* draggable=static_cast<NUISystem::CDraggable*>(icon.Get());
        if(draggable) draggable->SetSnapBackIfDroppedNowhere(false);
        static_cast<NUISystem::CSprite*>(sprite)->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);
        fadedSprite->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);
        entry->AddChild(icon);
        entry->AddChild(faded);
        AbilitiesMenuEntries[*it]=entry.Get();
        if(std::find(AssignedAbilities.begin(),AssignedAbilities.end(),*it)==AssignedAbilities.end()) {
            CCountedPointer<CComponent> unassigned(ui->CreateComponent("PC_UNASSIGNED_SPELL_ICON",false));
            entry->AddChild(unassigned);
            UnassignedIcons[*it]=unassigned.Get();
        }
        CCountedPointer<NUISystem::CMenuEntry> menuEntry;
        menuEntry.SetStaticCast(entry);
        CCountedPointer<CComponent> name(ui->CreateComponent("PC_SUBTITLE",false));
        CCountedPointer<NUISystem::CText> text;
        text.SetStaticCast(name);
        text->SetValue(displayName,true,true);
        entry->AddChild(name);
        if(frame) frame->AddPositionChild(name.Get());
        CCountedPointer<CComponent> abilityIcon(ui->CreateComponent("PC_ICON",false));
        if(abilityIcon->GetType()==static_cast<NUISystem::EType>(0))
            static_cast<NUISystem::CSprite*>(abilityIcon.Get())->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);
        entry->AddChild(abilityIcon);
        if(frame) frame->AddPositionChild(abilityIcon.Get());
        CCountedPointer<CComponent> ring;
        switch(GetCategory(*it)) {
        case SPELL_CATEGORY_PHYSICAL: ring.Reset(ui->CreateComponent("PC_RING_PIC_MAGIC_PHYSICAL",false)); break;
        case SPELL_CATEGORY_SURROUND: ring.Reset(ui->CreateComponent("PC_RING_PIC_MAGIC_SURROUND",false)); break;
        case SPELL_CATEGORY_ATTACK: ring.Reset(ui->CreateComponent("PC_RING_PIC_MAGIC_ATTACK",false)); break;
        }
        if(!ring.IsNull()) {
            entry->AddChild(ring);
            if(frame) frame->AddPositionChild(ring.Get());
        }
        CCountedPointer<CComponent> description(ui->CreateComponent("PC_DESCRIPTION",false));
        NUISystem::CText* descriptionText=static_cast<NUISystem::CText*>(description.Get());
        descriptionText->SetValue(GetAbilityPDef(*it)->LevDescription[GetAbilityLevel(*it)],L"",true);
        entry->AddChild(description);
        if(viewport) viewport->AddFakeChild(description.Get());
        if(frame) frame->AddPositionChild(description.Get());
        NUISystem::CList* list=static_cast<NUISystem::CList*>(CategoriesLists[GetCategory(*it)]);
        list->AddScrollingChildAndSetOffsets(entry);
        CCountedPointer<CAction> observe(new CAction);
        observe->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observe->AddParam(icon.Get(),observe);
        observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observe);
        CCountedPointer<CAction> ignore(new CAction);
        ignore->ActionType=NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignore->AddParam(icon.Get(),ignore);
        ignore->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnInactive(ignore);
    }
    {
        std::map<EUISpellCategory,CComponent*>::iterator found=CategoriesLists.find(SPELL_CATEGORY_ATTACK);
        if(found!=CategoriesLists.end()) {
            CCountedPointer<CComponent> children(found->second);
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_ATTACK,root);
            entry->AddChild(children);
            NUISystem::CText* title=static_cast<NUISystem::CText*>(entry->FindChild("UI_MENU_ENTRY_ATTACK_SPELLS_TEXT"));
            if(title) title->SetValue(title->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(attackCount),true,true);
            NUISystem::CText* subtitle=static_cast<NUISystem::CText*>(entry->FindChild("PC_SUBTITLE_ATTACK_SPELLS"));
            if(subtitle) subtitle->SetValue(subtitle->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(attackCount),true,true);
        } else {
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_ATTACK,root);
            entry->MakeUnavailable();
            CComponent* arrow=entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if(arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method=NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    {
        std::map<EUISpellCategory,CComponent*>::iterator found=CategoriesLists.find(SPELL_CATEGORY_SURROUND);
        if(found!=CategoriesLists.end()) {
            CCountedPointer<CComponent> children(found->second);
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_SURROUND,root);
            entry->AddChild(children);
            NUISystem::CText* title=static_cast<NUISystem::CText*>(entry->FindChild("UI_MENU_ENTRY_SURROUND_SPELLS_TEXT"));
            if(title) title->SetValue(title->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(surroundCount),true,true);
            NUISystem::CText* subtitle=static_cast<NUISystem::CText*>(entry->FindChild("PC_SUBTITLE_SURROUND_SPELLS"));
            if(subtitle) subtitle->SetValue(subtitle->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(surroundCount),true,true);
        } else {
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_SURROUND,root);
            entry->MakeUnavailable();
            CComponent* arrow=entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if(arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method=NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    {
        std::map<EUISpellCategory,CComponent*>::iterator found=CategoriesLists.find(SPELL_CATEGORY_PHYSICAL);
        if(found!=CategoriesLists.end()) {
            CCountedPointer<CComponent> children(found->second);
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_PHYSICAL,root);
            entry->AddChild(children);
            NUISystem::CText* title=static_cast<NUISystem::CText*>(entry->FindChild("UI_MENU_ENTRY_PHYSICAL_SPELLS_TEXT"));
            if(title) title->SetValue(title->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(physicalCount),true,true);
            NUISystem::CText* subtitle=static_cast<NUISystem::CText*>(entry->FindChild("PC_SUBTITLE_PHYSICAL_SPELLS"));
            if(subtitle) subtitle->SetValue(subtitle->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(physicalCount),true,true);
        } else {
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_PHYSICAL,root);
            entry->MakeUnavailable();
            CComponent* arrow=entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if(arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method=NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    return root;
}
// ==== end of the row's statements ====
static CComponent* Child(CComponent* c, const char* name) { for (size_t i = 0; i < c->Children.size(); ++i) if (c->Children[i]->Name == name) return c->Children[i]; return 0; }
static Str Names(const std::vector<CComponent*>& v) { Str s; for (size_t i = 0; i < v.size(); ++i) s += v[i]->Name + "|"; return s; }
static Str Acts(const std::vector<CAction*>& v) { Str s; for (size_t i = 0; i < v.size(); ++i) s += v[i]->Show() + "|"; return s; }
static Str Abil(const std::vector<EHeroAbility>& v) { Str s; for (size_t i = 0; i < v.size(); ++i) s += Num(v[i]) + " "; return s; }
static Str A(int type, const char* rest) { return Num(type) + rest; }
static CHeroAbilityDef gDefs[20];
static void Setup(CTCInventoryAbilities& a, CThing& thing)
{
    gMissing.clear(); gCreated.clear(); gThingDefs.clear();
    a.ParentThing = &thing;
    a.Abilities.clear();
    for (int n = 0; n < 20; ++n) {
        gDefs[n].IconGraphicIndex = 100 + n; gDefs[n].LevDescription.clear();
        for (int l = 0; l < 4; ++l) gDefs[n].LevDescription.push_back(500 + n * 10 + l);
        a.Abilities.push_back(std::make_pair(-1L, CDefPointer<const CHeroAbilityDef>(&gDefs[n])));
    }
    a.AssignedAbilities.push_back(static_cast<EHeroAbility>(17)); a.AssignedAbilitiesSave.push_back(static_cast<EHeroAbility>(17));
    a.CategoriesLists[CTCInventoryAbilities::SPELL_CATEGORY_PHYSICAL] = 0;
    a.UnassignedIcons[static_cast<EHeroAbility>(17)] = 0; a.AbilitiesMenuEntries[static_cast<EHeroAbility>(17)] = 0;
    gMisc.SpaceSeparator = CWideString(" ", 0);
}
static EHeroAbility H(int n) { return static_cast<EHeroAbility>(n); }
static CAbilityDef gSpellItem;
int main()
{
    // (A) owned 1 (surround, level 0), 3 (attack, level 2), 4 (physical, level 1), 9 (attack, level 0).
    {
        CTCInventoryAbilities a; CThing thing; Setup(a, thing); thing.HasExperience = true;
        a.Abilities[1].first = 0; a.Abilities[3].first = 2; a.Abilities[4].first = 1; a.Abilities[9].first = 0;
        a.ReceivedAbilityOrder.push_back(H(3)); a.ReceivedAbilityOrder.push_back(H(0)); a.ReceivedAbilityOrder.push_back(H(4));
        gMisc.TotalSpellsInPalettes = 5;
        thing.Inventory.QuickAccessItems.push_back(0); thing.Inventory.QuickAccessItems.push_back(101); thing.Inventory.QuickAccessItems.push_back(202);
        gSpellItem.Ability = H(9); gThingDefs[101].Ability = &gSpellItem; gThingDefs[202].Ability = 0;
        CComponent* parent = Make("PARENT");
        CComponent* root = a.ConstructPCMagicList(parent);
        CComponent* frame = Child(parent, "PC_UI_FRAME"); CComponent* viewport = Child(parent, "PC_DESCRIPTION_VIEWPORT");
        Check(root && root->Name == "PC_MAGIC_CATEGORIES", "A root");
        Check(Abil(a.AssignedAbilities) == "3 0 4 0 0 " && Abil(a.AssignedAbilitiesSave) == "3 0 4 0 0 ", "A palette padded");
        Check(a.CategoriesLists.size() == 3, "A three categories");
        CComponent* sur = a.CategoriesLists[CTCInventoryAbilities::SPELL_CATEGORY_SURROUND];
        CComponent* att = a.CategoriesLists[CTCInventoryAbilities::SPELL_CATEGORY_ATTACK];
        CComponent* phy = a.CategoriesLists[CTCInventoryAbilities::SPELL_CATEGORY_PHYSICAL];
        Check(sur && att && phy && sur->Offsets == 1 && att->Offsets == 2 && phy->Offsets == 1, "A category counts");
        Check(Names(sur->NonScrolling) == "PC_TITLE_SURROUND|" && Names(att->NonScrolling) == "PC_TITLE_ATTACK|" && Names(phy->NonScrolling) == "PC_TITLE_PHYSICAL|", "A category titles");
        Check(Names(root->NonScrolling) == "PC_POSITION_DUMMY_BOTTOM_RIGHT|PC_POSITION_DUMMY_BOTTOM_LEFT|", "A dummies");
        Check(Names(root->NonScrolling[0]->PositionChildren) == "PC_SPELL_CONTAINMENT_CONTAINER|"
              && Names(root->NonScrolling[1]->PositionChildren) == "PC_EXPRESSIONS_ITEMS_CONTAINMENT_CONTAINER|", "A dummy children");
        // expression-item quick slots
        CComponent* e1 = Child(root, "PC_EXPRESSION_ITEM_CONTAINMENT_1"); CComponent* e2 = Child(root, "PC_EXPRESSION_ITEM_CONTAINMENT_2");
        CComponent* e3 = Child(root, "PC_EXPRESSION_ITEM_CONTAINMENT_3");
        Check(e1 && e2 && e3 && !Child(root, "PC_EXPRESSION_ITEM_CONTAINMENT_4"), "A item slots");
        Check(Acts(e1->Actions) == A(UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC, " self c:PC_EXPRESSION_ITEM_CONTAINMENT_1 u0|"), "A item assign");
        Check(Names(e1->Children) == "PC_EXPRESSION_ITEM_INSIDE_CONTAINER|", "A empty item slot");
        CComponent* i2 = Child(e2, "PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER"); CComponent* i3 = Child(e3, "PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER");
        Check(i2 && i3, "A item icons");
        Check(Child(i2, "PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE")->Graphic == 1101 && Child(i3, "PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE")->Graphic == 1202, "A item graphics");
        Check(Acts(i2->OnDroppedNowhere) == A(UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC, " self c:PC_EXPRESSION_ITEM_CONTAINMENT_2 u1|") + A(UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC, " s0|"), "A spell item nowhere");
        Check(Acts(i3->OnDroppedNowhere) == A(UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC, " self c:PC_EXPRESSION_ITEM_CONTAINMENT_3 u2|") + A(UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_PC, " s0|"), "A expression item nowhere");
        Check(Acts(i2->OnDropped) == A(UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC, " c:PC_EXPRESSION_ITEM_CONTAINMENT_2 s9 s1|"), "A spell item swap");
        Check(Acts(i3->OnDropped) == A(UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC, " c:PC_EXPRESSION_ITEM_CONTAINMENT_3 s202 s2|"), "A expression item swap");
        // spell quick slots
        CComponent* s[6];
        for (int k = 1; k <= 5; ++k) s[k] = Child(root, ("PC_SPELL_CONTAINMENT_" + Num(k)).c_str());
        Check(s[1] && s[5] && !Child(root, "PC_SPELL_CONTAINMENT_6"), "A spell slots");
        Check(Acts(s[4]->Actions) == A(UI_ACTION_TYPE_ASSIGN_SPELL_PC, " self c:PC_SPELL_CONTAINMENT_4 u3|"), "A spell assign");
        Check(Names(s[2]->Children) == "PC_SPELL_INSIDE_CONTAINER|" && s[4]->Children.empty() && s[5]->Children.empty(), "A spell slot contents");
        CComponent* si = Child(s[3], "PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER");
        Check(si && Child(si, "PC_SPELL_ASSIGNMENT_ICON_SPRITE")->Graphic == 104, "A spell slot icon");
        Check(Acts(si->OnDroppedNowhere) == A(UI_ACTION_TYPE_UNASSIGN_SPELL_PC, " self c:PC_SPELL_CONTAINMENT_3 u2|") + A(UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC, " s0|"), "A spell slot nowhere");
        Check(Acts(si->OnDropped) == A(UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_SWAPPING_PC, " c:PC_SPELL_CONTAINMENT_3 s4 s2|"), "A spell slot swap");
        // root observers: 3 item slots x2 + 2 follows + 5 spell slots x2 + 2 follows = 20
        Check(root->OnSelected.size() == 20, "A root observers");
        Check(root->OnSelected[0]->Show() == A(UI_ACTION_TYPE_OBSERVE_EVENT, " self c:PC_EXPRESSION_ITEM_CONTAINMENT_1 u7")
              && root->OnSelected[1]->Show() == A(UI_ACTION_TYPE_OBSERVE_EVENT, " self c:PC_EXPRESSION_ITEM_CONTAINMENT_1 u9")
              && root->OnSelected[4]->Show() == A(UI_ACTION_TYPE_OBSERVE_EVENT, " self c:PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER u7"), "A observer order");
        // menu entries
        Check(a.AbilitiesMenuEntries.size() == 4 && a.UnassignedIcons.size() == 2 && a.UnassignedIcons.count(H(1)) && a.UnassignedIcons.count(H(9)), "A entries / unassigned");
        CComponent* en3 = a.AbilitiesMenuEntries[H(3)]; CComponent* en9 = a.AbilitiesMenuEntries[H(9)];
        Check(en3->Name == "ENTRY PC_MENU_ENTRY_TEMPLATE PC_BUTTON_BASTARD" && en3->Text == "Spell3 3", "A caption");
        Check(Names(en3->Children) == "PC_SPELL_ASSIGNMENT_ICON|PC_SPELL_ICON_FADED|PC_SUBTITLE|PC_ICON|PC_RING_PIC_MAGIC_ATTACK|PC_DESCRIPTION|", "A entry children");
        Check(Names(en9->Children) == "PC_SPELL_ASSIGNMENT_ICON|PC_SPELL_ICON_FADED|PC_UNASSIGNED_SPELL_ICON|PC_SUBTITLE|PC_ICON|PC_RING_PIC_MAGIC_ATTACK|PC_DESCRIPTION|", "A unassigned entry children");
        Check(a.UnassignedIcons[H(9)] == en9->Children[2], "A unassigned icon");
        Check(Child(en3, "PC_SUBTITLE")->Text == "Spell3 - <TEXT_GUI_MENU_LEVEL> 3", "A display name");
        Check(Child(en3, "PC_ICON")->Graphic == 103 && Child(en3, "PC_SPELL_ICON_FADED")->Graphic == 103, "A icons");
        CComponent* drag = Child(en3, "PC_SPELL_ASSIGNMENT_ICON");
        Check(Child(drag, "PC_SPELL_ASSIGNMENT_ICON_SPRITE")->Graphic == 103 && !drag->SnapBack, "A drag icon");
        Check(Acts(drag->OnDropped) == A(UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC, " s3|") && Acts(drag->OnDroppedNowhere) == A(UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC, " s0|"), "A drag actions");
        Check(Child(en3, "PC_DESCRIPTION")->TextId == 532 && Child(a.AbilitiesMenuEntries[H(4)], "PC_DESCRIPTION")->TextId == 541, "A descriptions");
        Check(Child(a.AbilitiesMenuEntries[H(1)], "PC_RING_PIC_MAGIC_SURROUND") && Child(a.AbilitiesMenuEntries[H(4)], "PC_RING_PIC_MAGIC_PHYSICAL"), "A rings");
        Check(Names(att->Scrolling) == Names(std::vector<CComponent*>(1, en3)) + Names(std::vector<CComponent*>(1, en9)) && sur->Scrolling.size() == 1 && phy->Scrolling.size() == 1, "A scrolling");
        Check(Acts(att->OnSelected) == A(UI_ACTION_TYPE_OBSERVE_EVENT, " self c:PC_SPELL_ASSIGNMENT_ICON u7|") + A(UI_ACTION_TYPE_OBSERVE_EVENT, " self c:PC_SPELL_ASSIGNMENT_ICON u7|")
              && Acts(phy->OnInactive) == A(UI_ACTION_TYPE_IGNORE_EVENT, " self c:PC_SPELL_ASSIGNMENT_ICON u7|"), "A list observers");
        // frame / viewport
        Check(Names(viewport->Fake) == "PC_DESCRIPTION_ATTACK_SPELLS|PC_DESCRIPTION_PHYSICAL_SPELLS|PC_DESCRIPTION_SURROUND_SPELLS|PC_DESCRIPTION|PC_DESCRIPTION|PC_DESCRIPTION|PC_DESCRIPTION|", "A viewport");
        Check(frame->PositionChildren.size() == 3 + 11 + 4 * 4 && frame->PositionChildren[0]->Name == "PC_TITLE_SURROUND" && frame->PositionChildren[13] == viewport
              && frame->PositionChildren[4]->Name == "PC_RING_PIC_MAGIC_ATTACK", "A frame");
        // category blocks
        Check(a.Calls.size() == 3 && a.Calls[0] == "menu 0 PC_MAGIC_CATEGORIES" && a.Calls[2] == "menu 2 PC_MAGIC_CATEGORIES", "A category menu entries");
        CComponent* ce0 = Child(root, "CATEGORY_ENTRY_0");
        Check(ce0 && ce0->Children[0] == att && !ce0->Unavailable, "A attack entry child");
        Check(Child(ce0, "UI_MENU_ENTRY_ATTACK_SPELLS_TEXT")->Text == "T(UI_MENU_ENTRY_ATTACK_SPELLS_TEXT) x2" && Child(ce0, "PC_SUBTITLE_ATTACK_SPELLS")->Text == "T(PC_SUBTITLE_ATTACK_SPELLS) x2", "A attack texts");
        Check(Child(Child(root, "CATEGORY_ENTRY_1"), "UI_MENU_ENTRY_SURROUND_SPELLS_TEXT")->Text == "T(UI_MENU_ENTRY_SURROUND_SPELLS_TEXT) x1", "A surround text");
        Check(Child(Child(root, "CATEGORY_ENTRY_2"), "PC_SUBTITLE_PHYSICAL_SPELLS")->Text == "T(PC_SUBTITLE_PHYSICAL_SPELLS) x1", "A physical text");
    }
    // (B) received order longer than the palette; no experience TC, no frame, no viewport; no physical spell; slot 2 missing.
    {
        CTCInventoryAbilities a; CThing thing; Setup(a, thing); thing.HasExperience = false;
        a.Abilities[3].first = 1; a.Abilities[1].first = 0;
        int rec[] = { 1, 3, 1, 3 };
        for (int k = 0; k < 4; ++k) a.ReceivedAbilityOrder.push_back(H(rec[k]));
        gMisc.TotalSpellsInPalettes = 2;
        gMissing.push_back("PC_UI_FRAME"); gMissing.push_back("PC_DESCRIPTION_VIEWPORT"); gMissing.push_back("PC_SPELL_CONTAINMENT_2");
        CComponent* parent = Make("PARENT");
        CComponent* root = a.ConstructPCMagicList(parent);
        Check(Abil(a.AssignedAbilities) == "1 3 " && Abil(a.AssignedAbilitiesSave) == "1 3 ", "B palette truncated");
        Check(a.CategoriesLists.size() == 2 && !a.CategoriesLists.count(CTCInventoryAbilities::SPELL_CATEGORY_PHYSICAL), "B two categories");
        Check(a.UnassignedIcons.empty() && a.AbilitiesMenuEntries.size() == 2, "B entries");
        Check(a.AbilitiesMenuEntries[H(1)]->Text == "" && Child(a.AbilitiesMenuEntries[H(1)], "PC_SUBTITLE")->Text == "", "B no captions");
        Check(Child(a.AbilitiesMenuEntries[H(3)], "PC_DESCRIPTION")->TextId == 531, "B description level");
        Check(root->OnSelected.size() == 3 && Child(root, "PC_SPELL_CONTAINMENT_1") && !Child(root, "PC_SPELL_CONTAINMENT_3"), "B spell slots");
        CComponent* s1 = Child(root, "PC_SPELL_CONTAINMENT_1");
        Check(Child(Child(s1, "PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER"), "PC_SPELL_ASSIGNMENT_ICON_SPRITE")->Graphic == 101, "B slot icon");
        CComponent* ce2 = Child(root, "CATEGORY_ENTRY_2");
        Check(ce2 && ce2->Unavailable && Child(ce2, "UI_SELECTION_ARROW_RIGHT")->DeletionMethod == CDeletion::UI_DELETE_THIS_INSTANCE && ce2->Children.size() == 1, "B empty category");
        Check(!Child(root, "CATEGORY_ENTRY_0")->Unavailable && Child(root, "CATEGORY_ENTRY_0")->Children[0] == a.CategoriesLists[CTCInventoryAbilities::SPELL_CATEGORY_ATTACK], "B attack category");
    }
    if (gFail == 0) std::printf("005f1b10_TEST PASS\n");
    return gFail;
}
