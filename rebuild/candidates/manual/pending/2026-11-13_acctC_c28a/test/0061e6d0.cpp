// Behaviour test for CTCInventoryQuests::ConstructQuestListPC @ 0x0061E6D0: the PC quest list (available / current / other).
// Subject: the row's own statements (src/0061e6d0.cpp from IsQuestCardAvailable to the end, unchanged) over the mock engine of the
// landed twin's test (0x0061B610), extended with the PC-only calls (AddPositionChild, ConvertVector, GetUIScale, the PC def fields).
// Checks what the list holds on six paths: frame choice, zoom limits and centre, text lines and colours, the refused / takeable /
// started entries, actions and parameters, sub-list offsets, icons, mini map, position children, world-map states, sort order.
#include <cstdio>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
typedef std::string Str;
#ifndef _MSC_VER
#define __fastcall
#endif
// ---- what a run records ----
static std::vector<Str> gTok, gSeg, gFacts;
static Str Num(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static Str Flt(float v) { char b[32]; std::sprintf(b, "%.2f", (double)v); return b; }
static void Tok(const Str& t) { gTok.push_back(t); }
static void Fact(const Str& f) { gFacts.push_back(f); }
static void Close(const char* op)
{
    std::sort(gTok.begin(), gTok.end());
    Str s;
    for (size_t i = 0; i < gTok.size(); ++i) { s += gTok[i]; s += ' '; }
    gSeg.push_back(s + op); gTok.clear();
}
// ---- strings: a CCharString built from a literal is logged (retail: the constructor 0x0099EBF0 with a literal argument) ----
class CCharString { public:
    Str s;
    CCharString() { }
    CCharString(const char* p) : s(p) { Tok("S:" + s); }
    CCharString(const Str& x, int) : s(x) { }
    bool operator==(const CCharString& o) const { return s == o.s; }
    CCharString GetAsUpperCase() const { Str u = s; for (size_t i = 0; i < u.size(); ++i) if (u[i] >= 'a' && u[i] <= 'z') u[i] -= 32; return CCharString(u, 0); }
};
static CCharString operator+(const char* a, const CCharString& b) { return CCharString(Str(a) + b.s, 0); }
class CWideString { public:
    Str s;
    CWideString() { }
    CWideString(const unsigned short* w) { while (*w) s += (char)*w++; }
#ifndef _MSC_VER
    CWideString(const wchar_t* w) { while (*w) s += (char)*w++; }      // g++ only: the row's wide literals are wchar_t there
#endif
    CWideString(const Str& x, int) : s(x) { }
    CWideString& operator+=(const CWideString& o) { s += o.s; return *this; }
    unsigned long GetLength() const { return (unsigned long)s.size(); }
};
static CWideString operator+(const CWideString& a, const CWideString& b) { return CWideString(a.s + b.s, 0); }
static CWideString operator+(const CWideString& a, const unsigned short* b) { return CWideString(a.s + CWideString(b).s, 0); }
static CWideString operator+(const unsigned short* a, const CWideString& b) { return CWideString(CWideString(a).s + b.s, 0); }
#ifndef _MSC_VER
static CWideString operator+(const CWideString& a, const wchar_t* b) { return CWideString(a.s + CWideString(b).s, 0); }
static CWideString operator+(const wchar_t* a, const CWideString& b) { return CWideString(CWideString(a).s + b.s, 0); }
#endif
static CWideString GFIntToWideString(long v) { return CWideString(Num(v), 0); }
// ---- small engine values ----
class C2DVector { public: float X, Y; C2DVector() : X(0), Y(0) { } C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); } };
class CRGBColour { public: unsigned char R, G, B, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : R(r), G(g), B(b), A(a) { } };
template<class T> class CCountedPointer { public:
    T* p;
    CCountedPointer() : p(0) { }
    CCountedPointer(T* x) : p(x) { }
    T* operator->() const { return p; }
    T* Get() const { return p; }
    void Reset(T* x) { p = x; }
    template<class U> void SetStaticCast(const CCountedPointer<U>& o) { p = static_cast<T*>(o.Get()); }
};
template<class T> class CDefPointer { public: T* p; CDefPointer() : p(0) { } T* operator->() const { return p; } };
template<class T> class CIntelligentPointer { public: T* p; CIntelligentPointer(T* x) : p(x) { } };
// ---- the UI: one mock component class stands for CComponent and the classes the row casts it to; every virtual logs its retail
// vtable offset (CComponent / CComponentContainer / CViewport / CSprite / CText / CZoomingComponent slots from the shared headers;
// the CList slots 0x240 / 0x244 / 0x260 / 0x270 / 0x278 are read off the retail calls of this row) ----
namespace NUISystem {
enum EActionType { UI_ACTION_TYPE_TAKE_QUEST = 6, UI_ACTION_TYPE_DELETE = 21, UI_ACTION_TYPE_CLOSE_MENU = 60, UI_ACTION_TYPE_TELEPORT_TO_BOAST = 207,
                   UI_ACTION_TYPE_POP_MAP_STATE = 216, UI_ACTION_TYPE_TAKE_QUEST_FOR_BOAST = 3004 };
enum EType { UI_TYPE_SPRITE = 0, UI_TYPE_OTHER = 1 };
class CActionParam { public: Str label; };
class CAction { public: EActionType ActionType; std::list<CActionParam*> UIActionParams; std::list<long> ThingActionParams; CAction() : ActionType((EActionType)0) { } };
class CDeletion { public: enum EDeletionMethod { UI_DO_NOT_DELETE = 0, UI_DELETE_THIS_INSTANCE = 1 }; EDeletionMethod Method; CDeletion() : Method(UI_DO_NOT_DELETE) { } };
enum EStateChangeType { UI_STATE_CHANGE_NONE = 0 };
class CUIStateDef { public: C2DVector Position; float UpdateTime; EStateChangeType StateChangeType; long StateChangeFlag;
    CUIStateDef() : UpdateTime(0), StateChangeType(UI_STATE_CHANGE_NONE), StateChangeFlag(0) { } };
class CUIState { public: CUIStateDef def; CUIState(const CUIStateDef& d) : def(d) { } };
static std::set<Str> gMissing;                 // FindChild names that return null
static EType gSpriteType = UI_TYPE_SPRITE;     // what GetType answers
class CComponent : public CActionParam {
public:
    Str name; std::vector<CCountedPointer<CComponent> > children; std::map<unsigned long, CUIState> states;
    CComponent(const Str& n) : name(n) { label = n; }
    static Str Act(const CCountedPointer<CAction>& a)
    {
        Str s = Num(a->ActionType);
        for (std::list<CActionParam*>::const_iterator i = a->UIActionParams.begin(); i != a->UIActionParams.end(); ++i) s += ":ui=" + (*i)->label;
        for (std::list<long>::const_iterator j = a->ThingActionParams.begin(); j != a->ThingActionParams.end(); ++j) s += ":thing=" + Num(*j);
        return s;
    }
    CComponent* FindChild(CCharString n) { Tok("vc"); if (gMissing.count(n.s)) return 0; return new CComponent(name + "/" + n.s); }
    std::vector<CCountedPointer<CComponent> >& GetChildren() { Tok("vd4"); return children; }
    void AddChild(const CCountedPointer<CComponent>& c) { Tok("vec"); children.push_back(c); Fact(name + ".child:" + c->name); }
    void SetDeletion(CDeletion d) { Tok("vf8"); Fact(name + ".deletion:" + Num(d.Method)); }
    void RemoveChild(unsigned long i) { Tok("v100"); Fact(name + ".remove:" + Num((long)i)); }
    EType GetType() const { Tok("v104"); return gSpriteType; }
    void AddAction(const CCountedPointer<CAction>& a) { Tok("v108"); Fact(name + ".action:" + Act(a)); }
    void AddActionOnBack(const CCountedPointer<CAction>& a) { Tok("v110"); Fact(name + ".back:" + Act(a)); }
    std::map<unsigned long, CUIState>& GetStates() { Tok("v148"); return states; }
    void MakeUnavailable() { Tok("v170"); Fact(name + ".unavailable"); }
    void AddTextLine(const CWideString& t, CRGBColour c, CCharString icon, long)
    { Tok("v238"); Fact("line:" + t.s + "|" + Num(c.R) + "," + Num(c.G) + "," + Num(c.B) + "," + Num(c.A) + "|" + icon.s); }
    void AddPositionChild(CComponent* c) { Tok("vpos"); Fact(name + ".pos:" + (c ? c->name : Str("null"))); }
    void AddFakeChild(CComponent* c) { Tok("v238"); Fact(name + ".fake:" + c->name); }
    void SetMinZoom(C2DVector& v) { Tok("v23c"); Fact("minzoom:" + Flt(v.X) + "," + Flt(v.Y)); }
    void SetMaxZoom(C2DVector& v) { Tok("v240"); Fact("maxzoom:" + Flt(v.X) + "," + Flt(v.Y)); }
    void SetZoomCenter(C2DVector& v) { Tok("v244"); Fact("zoomcenter:" + Flt(v.X) + "," + Flt(v.Y)); }
    void SetValue(const CWideString& t, bool a, bool b) { Tok("v240"); Fact(name + ".value:" + t.s + (a ? ":1" : ":0") + (b ? ":1" : ":0")); }
    void SetGraphicIndex(unsigned long g) { Tok("v244"); Fact(name + ".graphic:" + Num((long)g)); }
    void AddScrollingChild(const CCountedPointer<CComponent>& c) { Tok("v240"); Fact(name + ".scroll:" + c->name); }
    void AddNonScrollingChild(const CCountedPointer<CComponent>& c) { Tok("v244"); Fact(name + ".fixed:" + c->name); }
    void AddScrollingChildAndSetOffsets(const CCountedPointer<CComponent>& c) { Tok("v260"); Fact(name + ".scrolloffsets:" + c->name); }
    void InitialiseOffsets(unsigned long n) { Tok("v270"); Fact(name + ".offsets:" + Num((long)n)); }
    void SetDefaultChild(long n) { Tok("v278"); Fact(name + ".default:" + Num(n)); }
};
typedef CComponent CList; typedef CComponent CText; typedef CComponent CSprite; typedef CComponent CViewport;
typedef CComponent CZoomingComponent; typedef CComponent CComponentContainer;
class CUIMiscThingsDef { public: C2DVector RingCenter; C2DVector HeroDollFrameTL_PC; float HeroDollFrameEmulateListOffset; C2DVector PCRingCenter; };
static C2DVector gUIScale(1.0f, 1.0f);
class CUIScreenUnitConverter { public: static C2DVector ConvertVector(C2DVector v) { return C2DVector(v.X * 2.0f, v.Y * 4.0f); } };
static CComponent* gWorldMapComponent;   // the UI_WORLD_MAP component of the run (its states are checked)
struct CGraphicBank { unsigned long FindIndexBySymbol(const CCharString& n) { Close("FindIndexBySymbol"); return 7000 + (unsigned long)n.s.size(); } };
class CManager {
public:
    static CManager* GetInstance() { static CManager m; return &m; }
    CComponent* CreateComponent(const CCharString& n, bool)
    { Close("CreateComponent"); CComponent* c = new CComponent(n.s); if (n.s == "PC_WORLD_MAP") gWorldMapComponent = c; return c; }
    CComponent* CreateMenuEntryComponent(const CWideString& t, float a, float b, const CCharString& n, float c, const CCharString& d, float e, const CCharString& f)
    { Close("CreateMenuEntryComponent"); Fact("entry:" + t.s + "|" + Flt(a) + "," + Flt(b) + "," + Flt(c) + "," + Flt(e) + "|" + n.s + "|" + d.s + "|" + f.s); return new CComponent("entry:" + t.s); }
    C2DVector GetUIScale() const { return gUIScale; }
    CCountedPointer<CGraphicBank> GetGraphicsBank() { static CGraphicBank b; return CCountedPointer<CGraphicBank>(&b); }
};
}
using namespace NUISystem;
// ---- definitions, text, world ----
enum ETCInterfaceType { TCI_HERO_STATS = 4, TCI_INVENTORY_MAP = 27, TCI_QUEST_CARD = 108 };
enum EQuestCardPrerequisite { QUEST_CARD_PREREQUISITE_RENOWN = 0 };
enum QuestScreenType { QUEST_SCREEN_TYPE_AVAILABLE = 0, QUEST_SCREEN_TYPE_CURRENT = 1, QUEST_SCREEN_TYPE_OTHER = 2 };
static CUIMiscThingsDef gMiscDef;
class CDefinitionManager { public:
    template<class T> bool GetDef(long index, CDefPointer<const T>* r) const { if (index <= 0) return false; r->p = &gMiscDef; return true; }
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    long GetGlobalIndexFromClassIndex(const CCharString& n, long i) { Fact("defindex:" + n.s + ":" + Num(i)); return 55; }
};
namespace NGameText { class CDataBank { public: CWideString GetTextBySymbol(const CCharString& n) const { Close("GetTextBySymbol"); return CWideString("[" + n.s + "]", 0); } }; }
class CRegion { public:
    long number; Str name; C2DVector offset; bool onMap;
    CRegion() : number(0), onMap(false) { }
    const C2DVector GetWorldMapOffset() const { return offset; }
    bool IsOnWorldMap() const { return onMap; }
};
class CWorldMap { public:
    std::vector<CRegion> regions; long loaded;
    long GetRegionNumberFromName(const CCharString& n) const { for (size_t i = 0; i < regions.size(); ++i) if (regions[i].name == n.s) return regions[i].number; return 0; }
    long GetLoadedRegion() const { return loaded; }
    CRegion& GetRegion(long n) { for (size_t i = 0; i < regions.size(); ++i) if (regions[i].number == n) return regions[i]; return regions[0]; }
    const CRegion& GetRegion(long n) const { for (size_t i = 0; i < regions.size(); ++i) if (regions[i].number == n) return regions[i]; return regions[0]; }
};
class CWorld { public: CWorldMap map; CWorldMap& GetWorldMap() { return map; } const CWorldMap& PeekWorldMap() const { return map; } };
static CWorld gWorld;
class CMainGameComponent { public:
    NGameText::CDataBank bank;
    static CMainGameComponent* Get() { static CMainGameComponent m; return &m; }
    const NGameText::CDataBank& GetTextBank() const { return bank; }
    CWorld& GetWorld() { return gWorld; }
};
// ---- quests ----
class CDefString { public: Str s; CCharString GetString() const { return CCharString(s, 0); } };
class CQuestCardDef { public: CDefString RegionName; bool IsVignette; std::map<EQuestCardPrerequisite, long> Prerequisites; long NumBoasts; CQuestCardDef() : IsVignette(false), NumBoasts(0) { } };
class CTCQuestCard { public:
    CQuestCardDef def; bool core, optional; Str name, summary, objective, region, script; long gold, renown;
    CTCQuestCard() : core(false), optional(false), gold(0), renown(0) { }
    static ETCInterfaceType StatGetInterfaceType() { return TCI_QUEST_CARD; }
    bool IsCore() const { return core; }
    bool IsOptional() const { return optional; }
    CWideString GetQuestName() const { return CWideString(name, 0); }
    CWideString GetQuestSummary() const { return CWideString(summary, 0); }
    CWideString GetQuestObjective() const { return CWideString(objective, 0); }
    CCharString GetRegionName() const { return CCharString(region, 0); }
    CDefPointer<const CQuestCardDef> GetPDef() const { CDefPointer<const CQuestCardDef> p; p.p = &def; return p; }
    const CCharString GetScriptQuestName() const { return CCharString(script, 0); }
    long GetGoldReward() const { return gold; }
    long GetRenownReward() const { return renown; }
};
class CTCHeroStats { public: long renownLevel; long GetRenownLevel() const { return renownLevel; } };
class CTCInventoryMap { public:
    std::set<long> shown;
    void CreateMiniMapMarkers(CComponent* c, CRegion& r) { Fact("minimarkers:" + c->name + ":" + Num(r.number)); }
    void GetRegionsToDisplay(std::set<long>& r) { r = shown; }
    void CreateMapMarkers(CComponent* c, std::vector<CRegion>& r)
    { Str s = "markers:" + c->name; for (size_t i = 0; i < r.size(); ++i) s += ":" + Num(r[i].number); Fact(s); }
};
class CThing {
public:
    CTCQuestCard* card; CTCHeroStats* stats; CTCInventoryMap* map; long defIndex;
    CThing() : card(0), stats(0), map(0), defIndex(0) { }
    void Fetch(CTCQuestCard** r) { *r = card; } void Fetch(const CTCQuestCard** r) const { *r = card; }
    void Fetch(CTCHeroStats** r) { *r = stats; } void Fetch(CTCInventoryMap** r) { *r = map; }
    template<class T> bool GetTC(T** r) { T* p = 0; Fetch(&p); if (!p) return false; *r = p; return true; }
    template<class T> bool GetTC(ETCInterfaceType, T** r) { return GetTC(r); }
    template<class T> bool PeekTC(T** r) const { T* p = 0; Fetch(&p); if (!p) return false; *r = p; return true; }
    template<class T> bool PeekTC(ETCInterfaceType, T** r) const { return PeekTC(r); }
    long GetDefGlobalIndex() const { return defIndex; }
};
class CQuestManager { public:
    class CBoast { public: long ID, Cost, Reward; CCharString ScriptName; CWideString Text; bool Failed; };
    std::list<CBoast> boasts; std::set<Str> started;
    static CQuestManager& Get() { static CQuestManager m; return m; }
    std::list<CBoast>& GetQuestBoasts() { return boasts; }
    bool IsActiveQuestStartedInRegion(const CCharString& r) const { return started.count(r.s) != 0; }
};
static CThing gHero;
class CTCInventoryQuests {
public:
    std::map<CThing*, CComponent*> AvailableQuests;
    std::vector<CIntelligentPointer<CThing> > DisplayQuests;
    CThing& GetParentThing() { return gHero; }
    const CWorld& PeekWorld() const { return gWorld; }
    unsigned long GetMiniMapGraphic(CRegion& r) const { return 1000 + (unsigned long)r.number; }
    bool IsQuestCardAvailable(const CTCQuestCard& quest_card) const;
    void ConstructQuestListPC(CComponent* parent, const std::vector<CThing*>& quests, CList* list, QuestScreenType type, CList* frame_list);
};
// ---- the row's own statements (src/0061e6d0.cpp from IsQuestCardAvailable to the end, unchanged) ----
inline bool CTCInventoryQuests::IsQuestCardAvailable(const CTCQuestCard& quest_card) const
{
    if (CQuestManager::Get().IsActiveQuestStartedInRegion(quest_card.GetPDef()->RegionName.GetString()))
        return false;
    return true;
}

void CTCInventoryQuests::ConstructQuestListPC(CComponent* parent, const std::vector<CThing*>& quests, CList* list,
                                              QuestScreenType type, CList* frame_list)
{
    std::vector<CThing*> sorted_quests[3];
    CManager* manager = CManager::GetInstance();
    unsigned long i = 0;
    if (quests.size() != 0) {
        for (; i != quests.size(); ++i) {
            const CTCQuestCard* quest_card;
            if (!quests[i]->PeekTC(&quest_card))
                continue;
            if (type == QUEST_SCREEN_TYPE_AVAILABLE) {
                if (quest_card->IsCore())
                    sorted_quests[0].push_back(quests[i]);
                else if (quest_card->IsOptional() && !quest_card->GetPDef()->IsVignette)
                    sorted_quests[1].push_back(quests[i]);
                else if (quest_card->GetPDef()->IsVignette)
                    sorted_quests[2].push_back(quests[i]);
                else
                    sorted_quests[0].push_back(quests[i]);
            } else {
                sorted_quests[0].push_back(quests[i]);
            }
        }
    }

    CDefPointer<const CUIMiscThingsDef> misc_def;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &misc_def);
    const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
    CTCHeroStats* hero_stats = 0;
    GetParentThing().GetTC(TCI_HERO_STATS, &hero_stats);

    CList* frame_parent = frame_list ? frame_list : list;
    CComponent* frame;
    if (type == QUEST_SCREEN_TYPE_AVAILABLE)
        frame = frame_parent->FindChild("PC_UI_MAP_FRAME");
    else
        frame = frame_parent->FindChild("PC_UI_FRAME_QUESTS");
    CCountedPointer<CComponent> world_map(manager->CreateComponent("PC_WORLD_MAP", false));

    CZoomingComponent* zooming_map = static_cast<CZoomingComponent*>(world_map.Get());
    C2DVector zoom_center = CUIScreenUnitConverter::ConvertVector(misc_def->HeroDollFrameTL_PC);
    zoom_center.Y += CManager::GetInstance()->GetUIScale().Y * misc_def->HeroDollFrameEmulateListOffset;
    zoom_center.X += CManager::GetInstance()->GetUIScale().X * misc_def->PCRingCenter.X;
    zoom_center.Y += CManager::GetInstance()->GetUIScale().Y * misc_def->PCRingCenter.Y;
    C2DVector min_zoom(0.2f, 0.2f);
    C2DVector max_zoom(1.5f, 1.5f);
    zooming_map->SetMaxZoom(max_zoom);
    zooming_map->SetMinZoom(min_zoom);
    zooming_map->SetZoomCenter(zoom_center);

    CComponent* description_viewport = parent ? parent->FindChild("PC_DESCRIPTION_VIEWPORT")
                                              : frame_parent->FindChild("PC_DESCRIPTION_VIEWPORT");
    list->InitialiseOffsets(quests.size());
    CCountedPointer<CComponent> movie_viewport(manager->CreateComponent("PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT", false));
    list->AddNonScrollingChild(movie_viewport);
    frame->AddPositionChild(movie_viewport.Get());

    unsigned long state_index = 0;
    for (long category = 0; category != 3; ++category) {
        unsigned long j = 0;
        std::vector<CThing*>& category_quests = sorted_quests[category];
        std::vector<CThing*>::iterator first = category_quests.begin();
        unsigned long count = category_quests.size();
        if (count != 0)
        for (; j != count; ++j) {
            CThing* thing = first[j];
            CTCQuestCard* quest_card = 0;
            thing->GetTC(&quest_card);
            CWideString title = L"   " + quest_card->GetQuestName();
            CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(title, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                                                0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            if (type == QUEST_SCREEN_TYPE_AVAILABLE) {
                CCountedPointer<CAction> close_action(new CAction);
                close_action->ActionType = UI_ACTION_TYPE_CLOSE_MENU;
                entry->AddActionOnBack(close_action);
            }

            CWorldMap& world_map_data = CMainGameComponent::Get()->GetWorld().GetWorldMap();
            CCharString region_name = quest_card->GetRegionName();
            long region_number = world_map_data.GetRegionNumberFromName(region_name);
            if (region_number == 0)
                region_number = world_map_data.GetLoadedRegion();
            CRegion* region = &world_map_data.GetRegion(region_number);
            CCharString mini_map_name = "MINIMAP_" + region_name.GetAsUpperCase();
            CCountedPointer<CComponent> mini_map(manager->CreateComponent("PC_MAP_MENU_MINI_MAP_TEMPLATE", false));
            CViewport* viewport = static_cast<CViewport*>(movie_viewport.Get());
            CCountedPointer<CComponent> mini_map_sprite(manager->CreateComponent("PC_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE", false));
            frame->AddPositionChild(mini_map.Get());
            if (mini_map_sprite->GetType() == UI_TYPE_SPRITE)
                static_cast<CSprite*>(mini_map_sprite.Get())->SetGraphicIndex(GetMiniMapGraphic(*region));
            mini_map->AddChild(mini_map_sprite);
            viewport->AddFakeChild(mini_map.Get());
            CTCInventoryMap* inventory_map = 0;   // retail zeroes it before the TC lookup (`xor ecx,ecx` 0x0061F0A8)
            GetParentThing().GetTC(&inventory_map);
            inventory_map->CreateMiniMapMarkers(mini_map.Get(), *region);
            AvailableQuests.insert(std::map<CThing*, CComponent*>::value_type(thing, entry.Get()));
            DisplayQuests.push_back(CIntelligentPointer<CThing>(thing));

            CCountedPointer<CComponent> icon;
            unsigned long icon_index = manager->GetGraphicsBank()->FindIndexBySymbol("HUD_ORB_QUEST_CORE");
            if (quest_card->IsCore()) {
                icon.Reset(manager->CreateComponent("UI_QUEST_SPRITE_CORE", false));
                icon_index = manager->GetGraphicsBank()->FindIndexBySymbol("HUD_ORB_QUEST_CORE");
            } else if (quest_card->GetPDef()->IsVignette) {
                icon.Reset(manager->CreateComponent("UI_QUEST_SPRITE_VIGNETTE", false));
                icon_index = manager->GetGraphicsBank()->FindIndexBySymbol("HUD_ORB_QUEST_VIGNETTE");
            } else {
                icon.Reset(manager->CreateComponent("UI_QUEST_SPRITE_OPTIONAL", false));
                icon_index = manager->GetGraphicsBank()->FindIndexBySymbol("HUD_ORB_QUEST_OPTIONAL");
            }
            entry->AddChild(icon);
            list->AddScrollingChildAndSetOffsets(entry);

            CCountedPointer<CComponent> title_text(manager->CreateComponent("PC_TITLE_QUEST_TEXT", false));
            frame->AddPositionChild(title_text.Get());
            CCountedPointer<CText> title_as_text;
            title_as_text.SetStaticCast(title_text);
            title_as_text->SetValue(title, true, true);
            entry->AddChild(title_text);
            CCountedPointer<CComponent> quest_icon(manager->CreateComponent("PC_ICON_INDEPENDANT", false));
            frame->AddPositionChild(quest_icon.Get());
            static_cast<CSprite*>(quest_icon.Get())->SetGraphicIndex(icon_index);
            entry->AddChild(quest_icon);
            CCountedPointer<CComponent> description(manager->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT", false));
            frame->AddPositionChild(description.Get());
            CComponentContainer* container = static_cast<CComponentContainer*>(description.Get());

            CWideString text;
            if (quest_card->GetPDef()->Prerequisites.size() != 0) {
                long renown_needed = 0;
                std::map<EQuestCardPrerequisite, long>::const_iterator it = quest_card->GetPDef()->Prerequisites.find(QUEST_CARD_PREREQUISITE_RENOWN);
                if (it != quest_card->GetPDef()->Prerequisites.end())
                    renown_needed = it->second;
                if (hero_stats->GetRenownLevel() < renown_needed) {
                    text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_NOT_RENOWNED_ENOUGH") + CWideString(L"\n");
                    container->AddTextLine(text, CRGBColour(0xc8, 0x14, 0x14, 0xff), "", 0);
                }
            }
            text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_QUEST_SUMMARY");
            container->AddTextLine(text, CRGBColour(0xc8, 0xc8, 0x14, 0xff), "", 0);
            text = quest_card->GetQuestSummary() + L"\n";
            container->AddTextLine(text, CRGBColour(0xff, 0xff, 0xff, 0xff), "", 0);
            if (type == QUEST_SCREEN_TYPE_CURRENT) {
                if (quest_card->GetQuestObjective().GetLength() != 0) {
                    text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_CURRENT_OBJECTIVE");
                    container->AddTextLine(text, CRGBColour(0xc8, 0xc8, 0x14, 0xff), "", 0);
                    text = quest_card->GetQuestObjective() + L"\n";
                    container->AddTextLine(text, CRGBColour(0xff, 0xff, 0xff, 0xff), "", 0);
                }
                std::list<CQuestManager::CBoast>& boasts = CQuestManager::Get().GetQuestBoasts();
                CCharString script_name = quest_card->GetScriptQuestName();
                CWideString boast_text;
                bool any_boast = false;
                std::list<CQuestManager::CBoast>::iterator boast = boasts.begin();
                while (boast != boasts.end()) {
                    if (boast->ScriptName == script_name) {
                        boast_text += CWideString(L" - ");
                        boast_text += boast->Text;
                        boast_text += CWideString(L" (");
                        boast_text += text_bank.GetTextBySymbol("TEXT_GUI_MENU_WAGER");
                        boast_text += CWideString(L": ");
                        boast_text += GFIntToWideString(boast->Cost);
                        boast_text += CWideString(L", ");
                        boast_text += text_bank.GetTextBySymbol("TEXT_GUI_MENU_REWARD");
                        boast_text += CWideString(L": ");
                        boast_text += GFIntToWideString(boast->Reward);
                        boast_text += CWideString(L")");
                        any_boast = true;
                        if (boast->Failed) {
                            boast_text += CWideString(L" ");
                            boast_text += text_bank.GetTextBySymbol("TEXT_GUI_MENU_FAILED");
                        }
                        boast_text += CWideString(L"\n");
                    }
                    ++boast;
                }
                if (any_boast) {
                    text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_BOASTS");
                    container->AddTextLine(text, CRGBColour(0xc8, 0xc8, 0x14, 0xff), "", 0);
                    container->AddTextLine(boast_text, CRGBColour(0xff, 0xff, 0xff, 0xff), "", 0);
                }
            }
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_MONEY_REWARD") + CWideString(L": ") + GFIntToWideString(quest_card->GetGoldReward());
            container->AddTextLine(text, CRGBColour(0xff, 0xff, 0xff, 0xff), "HUD_ICON_MONEY_BAG", 0);
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN_REWARD") + CWideString(L": ") + GFIntToWideString(quest_card->GetRenownReward());
            container->AddTextLine(text, CRGBColour(0xff, 0xff, 0xff, 0xff), "HUD_ORB_QUEST_CORE", 0);
            if (type == QUEST_SCREEN_TYPE_AVAILABLE) {
                text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_NUM_BOASTS") + CWideString(L": ") + GFIntToWideString(quest_card->GetPDef()->NumBoasts);
                container->AddTextLine(text, CRGBColour(0xff, 0xff, 0xff, 0xff), "", 0);
            }
            entry->AddChild(description);
            static_cast<CViewport*>(description_viewport)->AddFakeChild(description.Get());

            CCountedPointer<CComponent> sub_list(manager->CreateComponent("PC_SUB_LIST", false));
            CCountedPointer<CList> sub_list_as_list;
            sub_list_as_list.SetStaticCast(sub_list);
            entry->AddChild(sub_list);
            if (type == QUEST_SCREEN_TYPE_AVAILABLE) {
                long renown_needed = 0;
                std::map<EQuestCardPrerequisite, long>::const_iterator it = quest_card->GetPDef()->Prerequisites.find(QUEST_CARD_PREREQUISITE_RENOWN);
                if (it != quest_card->GetPDef()->Prerequisites.end())
                    renown_needed = it->second;
                if (hero_stats->GetRenownLevel() < renown_needed || !IsQuestCardAvailable(*quest_card)) {
                    entry->MakeUnavailable();
                    entry->RemoveChild(entry->GetChildren().size() - 1);
                } else {
                    CWideString take_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TAKE_QUEST");
                    CCountedPointer<CComponent> take_entry(manager->CreateMenuEntryComponent(take_text, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                                         0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                    take_entry->AddChild(mini_map);
                    CCountedPointer<CAction> take_action(new CAction);
                    take_action->ActionType = UI_ACTION_TYPE_TAKE_QUEST;
                    take_action->ThingActionParams.push_back(thing->GetDefGlobalIndex());
                    take_entry->AddAction(take_action);
                    CCountedPointer<CAction> take_boast_action(new CAction);
                    take_boast_action->ActionType = UI_ACTION_TYPE_TAKE_QUEST_FOR_BOAST;
                    take_boast_action->ThingActionParams.push_back(thing->GetDefGlobalIndex());
                    CCountedPointer<CAction> pop_action(new CAction);
                    pop_action->ActionType = UI_ACTION_TYPE_POP_MAP_STATE;
                    pop_action->UIActionParams.push_back(world_map.Get());
                    take_entry->AddAction(pop_action);
                    pop_action->ThingActionParams.push_back(thing->GetDefGlobalIndex());
                    long num_boasts = quest_card->GetPDef()->NumBoasts;
                    long num_entries = (num_boasts != 0) + 1;
                    sub_list_as_list->InitialiseOffsets(num_entries);
                    sub_list_as_list->AddScrollingChildAndSetOffsets(take_entry);
                    CCountedPointer<CAction> delete_action(new CAction);
                    delete_action->ActionType = UI_ACTION_TYPE_DELETE;
                    delete_action->UIActionParams.push_back(entry.Get());
                    take_entry->AddAction(delete_action);
                    if (quest_card->GetPDef()->NumBoasts != 0) {
                        CWideString take_boast_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TAKE_QUEST_BOAST");
                        CCountedPointer<CComponent> take_boast_entry(manager->CreateMenuEntryComponent(take_boast_text, 0.0f, 1.0f,
                                                                     "PC_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                        take_boast_entry->AddChild(mini_map);
                        sub_list_as_list->AddScrollingChildAndSetOffsets(take_boast_entry);
                        take_boast_entry->AddAction(take_boast_action);
                        CCountedPointer<CAction> teleport_action(new CAction);
                        teleport_action->ActionType = UI_ACTION_TYPE_TELEPORT_TO_BOAST;
                        take_boast_entry->AddAction(teleport_action);
                    }
                }
            } else if (type == QUEST_SCREEN_TYPE_CURRENT) {
                CWideString zoom_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_ZOOMED_REGION");
                CCountedPointer<CComponent> zoom_entry(manager->CreateMenuEntryComponent(zoom_text, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                                     0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                sub_list_as_list->AddScrollingChild(zoom_entry);
                zoom_entry->AddChild(mini_map);
            } else if (type == QUEST_SCREEN_TYPE_OTHER) {
                CWideString zoom_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_ZOOMED_REGION");
                CCountedPointer<CComponent> zoom_entry(manager->CreateMenuEntryComponent(zoom_text, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                                     0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                sub_list_as_list->AddScrollingChild(zoom_entry);
                zoom_entry->AddChild(mini_map);
            }

            CCharString state_region_name = quest_card->GetRegionName();
            C2DVector region_offset(0.0f, 0.0f);
            if (region_number > 0)
                region_offset = region->GetWorldMapOffset();
            CUIStateDef state_def;
            C2DVector origin = CUIScreenUnitConverter::ConvertVector(misc_def->HeroDollFrameTL_PC);
            origin.Y += CManager::GetInstance()->GetUIScale().Y * misc_def->HeroDollFrameEmulateListOffset;
            state_def.Position = misc_def->PCRingCenter - region->GetWorldMapOffset();
            state_def.Position.X *= CManager::GetInstance()->GetUIScale().X;
            state_def.Position.Y *= CManager::GetInstance()->GetUIScale().Y;
            state_def.Position.X += origin.X;
            state_def.Position.Y += origin.Y;
            state_def.UpdateTime = 0.5f;
            state_def.StateChangeType = (EStateChangeType)3;
            state_def.StateChangeFlag = 0x67;
            std::pair<const unsigned long, CUIStateDef> state(state_index + 8, state_def);
            world_map->GetStates().insert(world_map->GetStates().begin(), state);
            ++state_index;
        }
    }

    if (type == QUEST_SCREEN_TYPE_AVAILABLE)
        list->SetDefaultChild(0L);
    else
        list->SetDefaultChild(quests.size() - 1);
    CTCInventoryMap* inventory_map;
    if (GetParentThing().GetTC(&inventory_map) && inventory_map) {
        std::set<long> regions;
        std::vector<CRegion> display_regions;
        inventory_map->GetRegionsToDisplay(regions);
        for (i = 0; i < quests.size(); ++i) {
            CTCQuestCard* quest_card = 0;
            quests[i]->GetTC(&quest_card);
            long region_number = PeekWorld().PeekWorldMap().GetRegionNumberFromName(quest_card->GetRegionName());
            if (region_number > 0)
                regions.insert(region_number);
        }
        for (std::set<long>::iterator it = regions.begin(); it != regions.end(); ++it) {
            if (PeekWorld().PeekWorldMap().GetRegion(*it).IsOnWorldMap())
                display_regions.push_back(PeekWorld().PeekWorldMap().GetRegion(*it));
        }
        inventory_map->CreateMapMarkers(world_map.Get(), display_regions);
    }
    list->AddNonScrollingChild(world_map);
}

// ---- harness ----
struct Quest { CThing thing; CTCQuestCard card; };
static std::vector<Quest*> gQuests;
static Quest* AddQuest(const char* name, const char* region, bool core, bool optional, bool vignette, long boasts, long index)
{
    Quest* q = new Quest; q->card.name = name; q->card.summary = Str(name) + " summary"; q->card.region = region; q->card.script = Str("Q_") + name;
    q->card.core = core; q->card.optional = optional; q->card.def.IsVignette = vignette; q->card.def.NumBoasts = boasts; q->card.def.RegionName.s = region;
    q->card.gold = 100 + index; q->card.renown = 200 + index; q->thing.card = &q->card; q->thing.defIndex = index; gQuests.push_back(q); return q;
}
static void AddRegion(long number, const char* name, float x, float y, bool onMap)
{ CRegion r; r.number = number; r.name = name; r.offset = C2DVector(x, y); r.onMap = onMap; gWorld.map.regions.push_back(r); }
static void AddBoast(const char* script, const char* text, long cost, long reward, bool failed)
{ CQuestManager::CBoast b; b.ID = 0; b.Cost = cost; b.Reward = reward; b.ScriptName = CCharString(script, 0); b.Text = CWideString(text, 0); b.Failed = failed; CQuestManager::Get().boasts.push_back(b); }
static CTCHeroStats gStats; static CTCInventoryMap gMap; static CTCInventoryQuests* gSubject; static CComponent* gList;
static void Reset()
{
    gTok.clear(); gSeg.clear(); gFacts.clear(); gQuests.clear(); gMissing.clear(); gSpriteType = UI_TYPE_SPRITE;
    gWorld.map.regions.clear(); gWorld.map.loaded = 1; CQuestManager::Get().boasts.clear(); CQuestManager::Get().started.clear();
    gStats.renownLevel = 5; gMap.shown.clear(); gHero.stats = &gStats; gHero.map = &gMap;
    gMiscDef.HeroDollFrameTL_PC = C2DVector(10.0f, 20.0f); gMiscDef.HeroDollFrameEmulateListOffset = 5.0f; gMiscDef.PCRingCenter = C2DVector(300.0f, 200.0f);
    gUIScale = C2DVector(2.0f, 3.0f);
    AddRegion(1, "Lookout", 10.0f, 20.0f, true); AddRegion(2, "Oakvale", 44.0f, 86.0f, true); AddRegion(3, "Cave", 1.0f, 2.0f, false);
}
static void Run(QuestScreenType type, bool withParent, bool withFrameList)
{
    std::vector<CThing*> quests;
    for (size_t i = 0; i < gQuests.size(); ++i) quests.push_back(&gQuests[i]->thing);
    gSubject = new CTCInventoryQuests; gList = new CComponent("list");
    CComponent* parent = withParent ? new CComponent("parent") : 0;
    CComponent* frameList = withFrameList ? new CComponent("framelist") : 0;
    gSubject->ConstructQuestListPC(parent, quests, gList, type, frameList);
    Close("end");
}
static int gFail = 0;
static void Check(bool ok, const char* what) { if (!ok) { ++gFail; std::printf("FAIL: %s\n", what); } }
static bool Has(const Str& f) { return std::find(gFacts.begin(), gFacts.end(), f) != gFacts.end(); }
static long Count(const Str& prefix) { long n = 0; for (size_t i = 0; i < gFacts.size(); ++i) if (gFacts[i].compare(0, prefix.size(), prefix) == 0) ++n; return n; }
static long Index(const Str& prefix) { for (size_t i = 0; i < gFacts.size(); ++i) if (gFacts[i].compare(0, prefix.size(), prefix) == 0) return (long)i; return -1; }
// state key -> ((PCRingCenter - region offset) * scale) + (ConvertVector(TL) + (0, scale.Y * list offset)), 0.5 s, change type 3, flag 0x67
static bool State(unsigned long key, float rx, float ry)
{
    std::map<unsigned long, CUIState>::iterator i = gWorldMapComponent->states.find(key);
    float x = (300.0f - rx) * 2.0f + 20.0f, y = (200.0f - ry) * 3.0f + (80.0f + 15.0f);
    return i != gWorldMapComponent->states.end() && i->second.def.Position.X == x && i->second.def.Position.Y == y && i->second.def.UpdateTime == 0.5f
        && (long)i->second.def.StateChangeType == 3 && i->second.def.StateChangeFlag == 0x67;
}
int main(int argc, char** argv)
{
    bool dump = argc > 1;
    // run 1: the available screen, one core quest the hero is not renowned enough for (prerequisite 9 v level 5)
    Reset(); AddQuest("Wasp", "Oakvale", true, false, false, 0, 11)->card.def.Prerequisites[QUEST_CARD_PREREQUISITE_RENOWN] = 9;
    Run(QUEST_SCREEN_TYPE_AVAILABLE, false, false);
    if (dump) for (size_t i = 0; i < gFacts.size(); ++i) std::printf("r1\t%s\n", gFacts[i].c_str());
    Check(Has("minzoom:0.20,0.20") && Has("maxzoom:1.50,1.50"), "run 1: zoom limits");
    // centre = ConvertVector(10,20) = (20,80); Y += 3 * 5; X += 2 * 300; Y += 3 * 200
    Check(Has("zoomcenter:620.00,695.00"), "run 1: zoom centre");
    Check(Index("minzoom:") > Index("maxzoom:") && Index("maxzoom:") >= 0, "run 1: SetMaxZoom before SetMinZoom");
    Check(Has("list.offsets:1") && Has("list.default:0"), "run 1: list offsets and default child");
    Check(Has("entry:   Wasp.back:60"), "run 1: close action on back");
    Check(Has("line:[TEXT_GUI_MENU_NOT_RENOWNED_ENOUGH]\n|200,20,20,255|"), "run 1: red not-renowned line");
    Check(Has("line:Wasp summary\n|255,255,255,255|"), "run 1: summary line");
    Check(Has("line:[TEXT_GUI_MENU_MONEY_REWARD]: 111|255,255,255,255|HUD_ICON_MONEY_BAG"), "run 1: money reward");
    Check(Has("line:[TEXT_GUI_MENU_NUM_BOASTS]: 0|255,255,255,255|"), "run 1: number of boasts");
    Check(Has("entry:   Wasp.unavailable") && Count("entry:   Wasp.remove:") == 1, "run 1: unavailable entry, last child removed once");
    Check(Count("entry:[TEXT_GUI_MENU_TAKE_QUEST]") == 0, "run 1: no take entry");
    Check(Has("PC_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE.graphic:1002") && Has("minimarkers:PC_MAP_MENU_MINI_MAP_TEMPLATE:2"), "run 1: mini map of region 2");
    Check(Has("list/PC_UI_MAP_FRAME.pos:PC_MAP_MENU_MINI_MAP_TEMPLATE") && Has("list/PC_UI_MAP_FRAME.pos:PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT"), "run 1: the available screen's frame gets the position children");
    Check(Has("PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT.fake:PC_MAP_MENU_MINI_MAP_TEMPLATE") && Has("list.fixed:PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT"), "run 1: movie viewport");
    Check(gSubject->AvailableQuests.size() == 1 && gSubject->DisplayQuests.size() == 1, "run 1: quest maps");
    Check(Has("markers:PC_WORLD_MAP:2") && Has("list.fixed:PC_WORLD_MAP"), "run 1: world map markers, world map on the list");
    Check(State(8, 44.0f, 86.0f) && gWorldMapComponent->states.size() == 1, "run 1: state 8");
    // run 2: the available screen, a vignette with one boast the hero can take, a parent and a frame list
    Reset(); AddQuest("Fete", "Lookout", false, false, true, 1, 12); gMap.shown.insert(2); gMap.shown.insert(3);
    Run(QUEST_SCREEN_TYPE_AVAILABLE, true, true);
    if (dump) for (size_t i = 0; i < gFacts.size(); ++i) std::printf("r2\t%s\n", gFacts[i].c_str());
    Check(Has("entry:[TEXT_GUI_MENU_TAKE_QUEST].action:6:thing=12"), "run 2: take action");
    Check(Has("entry:[TEXT_GUI_MENU_TAKE_QUEST].action:21:ui=entry:   Fete"), "run 2: delete action");
    Check(Has("PC_SUB_LIST.offsets:2") && Has("PC_SUB_LIST.scrolloffsets:entry:[TEXT_GUI_MENU_TAKE_QUEST]") && Has("PC_SUB_LIST.scrolloffsets:entry:[TEXT_GUI_MENU_TAKE_QUEST_BOAST]"), "run 2: sub list with the boast entry");
    Check(Has("entry:[TEXT_GUI_MENU_TAKE_QUEST_BOAST].action:3004:thing=12") && Has("entry:[TEXT_GUI_MENU_TAKE_QUEST_BOAST].action:207"), "run 2: boast and teleport actions");
    Check(Has("entry:[TEXT_GUI_MENU_TAKE_QUEST].child:PC_MAP_MENU_MINI_MAP_TEMPLATE") && Has("entry:[TEXT_GUI_MENU_TAKE_QUEST_BOAST].child:PC_MAP_MENU_MINI_MAP_TEMPLATE"), "run 2: the take entries carry the mini map");
    Check(Count("entry:   Fete.unavailable") == 0 && Count("line:[TEXT_GUI_MENU_NOT_RENOWNED_ENOUGH]") == 0, "run 2: available, no red line");
    Check(Has("framelist/PC_UI_MAP_FRAME.pos:PC_MAP_MENU_MINI_MAP_TEMPLATE") && Count("list/PC_UI_MAP_FRAME.pos:") == 0, "run 2: the frame comes from the frame list");
    Check(Has("parent/PC_DESCRIPTION_VIEWPORT.fake:PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT"), "run 2: the description viewport comes from the parent");
    Check(Has("markers:PC_WORLD_MAP:1:2") && State(8, 10.0f, 20.0f), "run 2: region 3 is not on the world map; state 8");
    // run 3: the same quest already started in its region: the entry is refused although the hero is renowned enough
    Reset(); AddQuest("Fete", "Lookout", false, false, true, 1, 12); CQuestManager::Get().started.insert("Lookout");
    Run(QUEST_SCREEN_TYPE_AVAILABLE, false, false);
    Check(Has("entry:   Fete.unavailable") && Count("entry:[TEXT_GUI_MENU_TAKE_QUEST]") == 0 && Count("line:[TEXT_GUI_MENU_NOT_RENOWNED_ENOUGH]") == 0, "run 3: started region refuses the quest");
    // run 4: the current screen, a quest with an objective, a boast of another quest and two boasts of this one
    Reset(); AddQuest("Hobbe", "Cave", false, true, false, 0, 13)->card.objective = "Find him";
    AddBoast("Q_Hobbe", "No armour", 50, 150, false); AddBoast("Q_Other", "Skip", 1, 2, false); AddBoast("Q_Hobbe", "Fist", 60, 160, true);
    Run(QUEST_SCREEN_TYPE_CURRENT, false, false);
    if (dump) for (size_t i = 0; i < gFacts.size(); ++i) std::printf("r4\t%s\n", gFacts[i].c_str());
    Check(Has("line:[TEXT_GUI_MENU_CURRENT_OBJECTIVE]|200,200,20,255|") && Has("line:Find him\n|255,255,255,255|"), "run 4: objective");
    Check(Has("line: - No armour ([TEXT_GUI_MENU_WAGER]: 50, [TEXT_GUI_MENU_REWARD]: 150)\n - Fist ([TEXT_GUI_MENU_WAGER]: 60, [TEXT_GUI_MENU_REWARD]: 160) [TEXT_GUI_MENU_FAILED]\n|255,255,255,255|"), "run 4: the quest's two boasts, not the other quest's");
    Check(Has("PC_SUB_LIST.scroll:entry:[TEXT_GUI_MENU_ZOOMED_REGION]") && Count("line:[TEXT_GUI_MENU_NUM_BOASTS]") == 0, "run 4: zoom entry, no boast count");
    Check(Has("list/PC_UI_FRAME_QUESTS.pos:PC_MAP_MENU_MINI_MAP_TEMPLATE") && Count("list/PC_UI_MAP_FRAME.pos:") == 0, "run 4: the quests frame on the current screen");
    Check(Count("entry:   Hobbe.back:") == 0, "run 4: no close action outside the available screen");
    Check(State(8, 1.0f, 2.0f), "run 4: state 8 from the cave's offset");
    // run 5: the other screen: no boast section at all, three quests, an unknown region (the loaded region), sprites that are not sprites
    Reset(); AddQuest("A", "Oakvale", true, false, false, 0, 21); AddQuest("B", "Nowhere", false, true, false, 0, 22); AddQuest("C", "Lookout", false, false, true, 0, 23);
    AddBoast("Q_A", "Naked", 5, 15, false); gSpriteType = UI_TYPE_OTHER;
    Run(QUEST_SCREEN_TYPE_OTHER, false, false);
    Check(Has("list.offsets:3") && Count("PC_SUB_LIST.scroll:entry:[TEXT_GUI_MENU_ZOOMED_REGION]") == 3, "run 5: three entries, three zoom entries");
    Check(Count("PC_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE.graphic:") == 0 && Has("minimarkers:PC_MAP_MENU_MINI_MAP_TEMPLATE:1"), "run 5: no sprite graphic, loaded region for the unknown one");
    Check(Count("line:[TEXT_GUI_MENU_BOASTS]") == 0 && Count("line: - Naked") == 0, "run 5: the boast section belongs to the current screen only");
    Check(gSubject->AvailableQuests.size() == 3 && gSubject->DisplayQuests.size() == 3, "run 5: quest maps");
    Check(State(8, 44.0f, 86.0f) && State(9, 10.0f, 20.0f) && State(10, 10.0f, 20.0f) && gWorldMapComponent->states.size() == 3, "run 5: states 8, 9, 10");
    // run 6: the available screen sorts core quests first, then optional, then vignettes
    Reset(); AddQuest("V", "Lookout", false, false, true, 0, 31); AddQuest("O", "Lookout", false, true, false, 0, 32); AddQuest("K", "Lookout", true, false, false, 0, 33);
    Run(QUEST_SCREEN_TYPE_AVAILABLE, false, false);
    Check(Index("entry:   K|") >= 0 && Index("entry:   K|") < Index("entry:   O|") && Index("entry:   O|") < Index("entry:   V|"), "run 6: core, optional, vignette order");
    if (gFail == 0) std::printf("0061e6d0_TEST PASS\n");
    return gFail;
}
