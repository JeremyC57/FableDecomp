// Behaviour test for CTCInventoryMap::ConstructPCMapList @ 0x0060EEB0: the PC world-map / teleport list.
// Subject: the row's own statements (CTCInventoryMap::ConstructPCMapList and the row's inline GetGuildSealRecallRegion text,
// copied unchanged from src/0060eeb0.cpp) over a mock engine that records every UI call.  Scenarios: (A) inventory map: regions
// shown on the world map or holding the hero; (B) teleport screen: teleporter regions, quest-card teleporter regions (deduped
// by def name, not the loaded region), the guild-seal recall region moved to the front and the hero guild second, recall text,
// default children, teleport / recall actions, observers, map states and markers; (C) guild-only teleport list; (D) nothing to
// show; (E) check_only with and without a result pointer.
#include <cstdio>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
typedef std::string Str;
static std::vector<Str> gLog;
static void Log(const Str& s) { gLog.push_back(s); }
static Str Num(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
// ---- strings ----
struct CCharString {
    Str s;
    CCharString() { }
    CCharString(const char* p) : s(p) { }
    bool operator==(const CCharString& o) const { return s == o.s; }
    bool operator!=(const CCharString& o) const { return s != o.s; }
    bool operator!=(const char* p) const { return s != p; }
};
struct CWideString {
    Str s;
    CWideString() { }
    template<class C> CWideString(const C* w) { while (*w) s += (char)*w++; }
    CWideString(const Str& x, int) : s(x) { }
    CWideString& operator+=(const CWideString& o) { s += o.s; return *this; }
};
template<class C> static CWideString operator+(const C* a, const CWideString& b) { return CWideString(CWideString(a).s + b.s, 0); }
namespace NGameText {
struct CDataBank { CWideString GetTextBySymbol(const CCharString& sym) const { return CWideString("<" + sym.s + ">", 0); } };
}
struct CMainGameComponent {
    NGameText::CDataBank Bank;
    static CMainGameComponent* Get() { static CMainGameComponent c; return &c; }
    const NGameText::CDataBank& GetTextBank() const { return Bank; }
};
// ---- maths ----
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
};
struct C3DVector { float X, Y, Z; float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; } };
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
// ---- world ----
struct CRegion {
    CCharString Name, RegionDefName, DisplayName; bool OnWorldMap; C2DVector Offset; long Number;
    CRegion() : OnWorldMap(false), Offset(0.0f, 0.0f), Number(0) { }
    const CCharString& GetName() const { return Name; }
    const CCharString& GetRegionDefName() const { return RegionDefName; }
    const CCharString& GetDisplayName() const { return DisplayName; }
    bool IsOnWorldMap() const { return OnWorldMap; }
    const C2DVector GetWorldMapOffset() const { return Offset; }
};
struct CWorldMap {
    std::vector<CRegion> Regions; long Loaded; std::map<long, long> RegionOfMap; long RecallMap;
    const CRegion& GetRegion(long n) const { return Regions[n]; }
    long GetRegionCount() const { return (long)Regions.size(); }
    long GetLoadedRegion() const { return Loaded; }
    long GetRegionNumberFromName(const CCharString& name) const
    { for (size_t i = 0; i < Regions.size(); ++i) if (Regions[i].Name == name) return (long)i; return 0; }
    long GetRegionNumberMapIsIn(long map) const { std::map<long, long>::const_iterator f = RegionOfMap.find(map); return f == RegionOfMap.end() ? 0 : f->second; }
    long GetMapNumberAt(const C3DVector&) const { return RecallMap; }
};
struct CWorld {
    CWorldMap Map; C3DVector Recall;
    const CWorldMap& PeekWorldMap() const { return Map; }
    const C3DVector& GetGuildSealRecallPos() const { return Recall; }
};
static CWorld gWorld;
struct CTCQuestCard { CCharString Teleporter; CCharString GetTeleporterRegionName() const { return Teleporter; } };
struct CThing {
    long MapNumber; CTCQuestCard Card;
    long GetCurrentMapNumber() const { return MapNumber; }
    template<class T> bool GetTC(T** out) { *out = &Card; return true; }
};
template<class T> struct CIntelligentPointer { T* P; CIntelligentPointer(T* p) : P(p) { } T* operator->() const { return P; } };
struct CQuestManager {
    std::list<CIntelligentPointer<CThing> > Cards;
    static CQuestManager& Get() { static CQuestManager m; return m; }
    const std::list<CIntelligentPointer<CThing> >& GetActiveQuestCards() const { return Cards; }
};
// ---- UI ----
namespace NUISystem {
enum EStateChangeType { STATE_CHANGE_0, STATE_CHANGE_1, STATE_CHANGE_2, STATE_CHANGE_3 };
enum { UI_EVENT_MOUSE_MOVEMENT = 7 };
enum { UI_TYPE_COMPONENT = 0, UI_TYPE_SPRITE = 5 };
enum EActionType { UI_ACTION_TYPE_NONE, UI_ACTION_TYPE_CLOSE_TELEPORT_MENU, UI_ACTION_TYPE_CLOSE_ACTIVE_MENU, UI_ACTION_TYPE_GUILD_SEAL_RECALL, UI_ACTION_TYPE_TELEPORT };
struct CUIState {
    C2DVector Position; EStateChangeType StateChangeType; long StateChangeFlag; float UpdateTime;
    CUIState() : Position(0.0f, 0.0f), StateChangeType(STATE_CHANGE_0), StateChangeFlag(0), UpdateTime(0.0f) { }
};
struct CAction { EActionType ActionType; std::vector<long> ThingActionParams; CAction() : ActionType(UI_ACTION_TYPE_NONE) { } };
template<class T> struct CCountedPointer {
    T* P;
    CCountedPointer() : P(0) { }
    CCountedPointer(T* p) : P(p) { }
    T* Get() const { return P; }
    T* operator->() const { return P; }
    template<class U> void SetStaticCast(const CCountedPointer<U>& o) { P = static_cast<T*>(o.Get()); }
};
struct CComponent {
    Str Name; long Graphic; Str Text;
    std::vector<CComponent*> Children, PositionChildren, Fake, Scrolling, NonScrolling;
    std::vector<CAction*> BackActions, Actions; std::map<unsigned long, CUIState> States; std::vector<long> Defaults;
    long Offsets; int Observed;
    CComponent() : Graphic(-1), Offsets(-1), Observed(0) { }
    virtual ~CComponent() { }
    CComponent* FindChild(const CCharString& name);
    void AddPositionChild(CComponent* c) { PositionChildren.push_back(c); }
    void AddChild(const CCountedPointer<CComponent>& c) { Children.push_back(c.Get()); }
    void ObserveEvent(int e) { Observed = e; }
    void AddActionOnBack(const CCountedPointer<CAction>& a) { BackActions.push_back(a.Get()); }
    void AddAction(const CCountedPointer<CAction>& a) { Actions.push_back(a.Get()); }
    std::map<unsigned long, CUIState>& GetStates() { return States; }
    int GetType() const { return Name.find("SPRITE") != Str::npos ? UI_TYPE_SPRITE : UI_TYPE_COMPONENT; }
};
struct CList : CComponent {
    void AddNonScrollingChild(const CCountedPointer<CComponent>& c) { NonScrolling.push_back(c.Get()); }
    void AddScrollingChild(const CCountedPointer<CComponent>& c) { Scrolling.push_back(c.Get()); }
    void AddScrollingChildAndSetOffsets(const CCountedPointer<CComponent>& c) { Scrolling.push_back(c.Get()); }
    void InitialiseOffsets(unsigned long n) { Offsets = (long)n; }
    void SetDefaultChild(long i) { Defaults.push_back(i); }
};
struct CViewport : CList { void AddFakeChild(CComponent* c) { Fake.push_back(c); } };
struct CSprite : CViewport { void SetGraphicIndex(long g) { Graphic = g; } };
struct CText : CSprite { void SetValue(const CWideString& t, bool, bool) { Text = t.s; } };
struct CZoomingComponent : CText {
    C2DVector MaxZoom, MinZoom, Centre;
    void SetMaxZoom(const C2DVector& v) { MaxZoom = v; }
    void SetMinZoom(const C2DVector& v) { MinZoom = v; }
    void SetZoomCenter(const C2DVector& v) { Centre = v; }
};
static std::set<Str> gMissing;                 // FindChild names that return null
static std::vector<CComponent*> gCreated;
static CComponent* Make(const Str& name) { CZoomingComponent* c = new CZoomingComponent; c->Name = name; gCreated.push_back(c); return c; }
CComponent* CComponent::FindChild(const CCharString& name)
{
    if (gMissing.count(name.s)) return 0;
    for (size_t i = 0; i < Children.size(); ++i) if (Children[i]->Name == name.s) return Children[i];
    CComponent* c = Make(name.s); Children.push_back(c); return c;
}
struct CManager {
    C2DVector Scale; std::vector<CComponent*> Observers;
    static CManager* GetInstance() { static CManager m; return &m; }
    CComponent* CreateComponent(const CCharString& name, bool) { return Make(name.s); }
    CComponent* CreateMenuEntryComponent(const CWideString& text, float, float, const CCharString& templ, float, const CCharString&, float, const CCharString&)
    { CComponent* c = Make("ENTRY " + templ.s); c->Text = text.s; return c; }
    void AddObserver(CComponent* c) { Observers.push_back(c); }
    const C2DVector& GetUIScale() const { return Scale; }
};
struct CUIMiscThingsDef { C2DVector HeroDollFrameTL_PC; float HeroDollFrameEmulateListOffset; C2DVector PCRingCenter; };
struct CUIScreenUnitConverter { static C2DVector ConvertVector(C2DVector v) { return C2DVector(v.X * 2.0f, v.Y * 2.0f); } };
}
using namespace NUISystem;
static CUIMiscThingsDef gMisc;
template<class T> struct CDefPointer { T* P; CDefPointer() : P(0) { } T* operator->() const { return P; } };
struct CDefinitionManager {
    template<class T> bool GetDef(long index, CDefPointer<const T>* result) const { if (index != 77) return false; result->P = &gMisc; return true; }
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    long GetGlobalIndexFromClassIndex(const CCharString& name, long i) const { return (name == CCharString("UI_MISC_THINGS_DEF") && i == 1) ? 77 : 0; }
};
// ---- the row's class ----
struct CTCInventoryMap {
    bool Inventory; CThing Hero; std::set<long> Display; std::set<long> Teleporters; std::map<long, CThing> TeleporterThings;
    std::vector<CRegion> MarkerRegions; std::vector<Str> Calls;
    CZoomingComponent* pmap_real; CList* MapList; std::map<long, CRegion> MapRegions; bool PanMode;
    CTCInventoryMap() : Inventory(true), pmap_real(0), MapList(0), PanMode(true) { }
    bool IsInventoryScreen() const { return Inventory; }
    const CWorld& PeekWorld() const { return gWorld; }
    const CWorldMap& PeekWorldMap() const { return gWorld.Map; }
    CWorldMap& GetWorldMap() { return gWorld.Map; }
    CThing& GetParentThing() { return Hero; }
    void GetRegionsToDisplay(std::set<long>& out) { for (std::set<long>::const_iterator i = Display.begin(); i != Display.end(); ++i) out.insert(*i); }
    CThing* GetRegionTeleporter(const CRegion& r) { return Teleporters.count(r.Number) ? &TeleporterThings[r.Number] : 0; }
    long GetGuildSealRecallRegion() const;
    void CreateMapMarkers(CComponent* parent, std::vector<CRegion>& regions) { Calls.push_back("markers:" + parent->Name); MarkerRegions = regions; }
    void ConstructRegionDescription(CComponent* c, CRegion& r, bool) { c->Text = "desc " + r.Name.s; }
    long GetMiniMapGraphic(const CRegion& r) { return r.Number * 100; }
    void CreateMiniMapMarkers(CComponent* c, CRegion& r) { Calls.push_back("minimap " + r.Name.s + " " + c->Name); }
    CComponent* ConstructPCMapList(bool check_only, bool* has_regions);
};
// ==== the row's statements (unchanged from src/0060eeb0.cpp) ====
inline long CTCInventoryMap::GetGuildSealRecallRegion() const
{
    const C3DVector& recallPos = PeekWorld().GetGuildSealRecallPos();
    if (recallPos.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        const CWorldMap& worldMap = PeekWorld().PeekWorldMap();
        return worldMap.GetRegionNumberMapIsIn(worldMap.GetMapNumberAt(recallPos));
    }
    return 0;
}
CComponent* CTCInventoryMap::ConstructPCMapList(bool check_only, bool* has_regions)
{
    PanMode = false;
    std::vector<CRegion> regions;
    MapRegions.clear();
    CCharString marker_name("");
    // FITTED (review): `regions` is still empty here, so VC7.1 folds this test to no code, but the branch halves c2's static
    // frequency of the gather, which retail's inline split needs (push_back called at 0x0060F168, Compare at 0x0060F39E / 0x0060F4CE,
    // the misc-def GetDef at 0x0060F629, the std::set destructor expanded at 0x0060F075); `regions.size() == 0`, `!regions.size()`,
    // `begin() == end()` and `if (true)` give the same calls but one x87 operand order (0x0060F752) differs. No PDB / string evidence.
    if (regions.empty()) {
        if (IsInventoryScreen()) {
            std::set<long> shown_regions;
            GetRegionsToDisplay(shown_regions);
            for (std::set<long>::iterator it = shown_regions.begin(); it != shown_regions.end(); ++it) {
                if (PeekWorld().PeekWorldMap().GetRegion(*it).IsOnWorldMap())
                    regions.push_back(PeekWorld().PeekWorldMap().GetRegion(*it));
                else if (*it == GetWorldMap().GetRegionNumberMapIsIn(GetParentThing().GetCurrentMapNumber()))
                    regions.push_back(PeekWorld().PeekWorldMap().GetRegion(*it));
            }
        } else {
            for (long region_number = 1; region_number < PeekWorld().PeekWorldMap().GetRegionCount(); ++region_number) {
                const CWorldMap& world_map = PeekWorld().PeekWorldMap();
                if (GetRegionTeleporter(PeekWorld().PeekWorldMap().GetRegion(region_number))
                    && world_map.GetLoadedRegion() != region_number
                    && PeekWorld().PeekWorldMap().GetRegion(region_number).IsOnWorldMap())
                    regions.push_back(PeekWorld().PeekWorldMap().GetRegion(region_number));
            }
            std::list<CIntelligentPointer<CThing> > quest_cards(CQuestManager::Get().GetActiveQuestCards());
            for (std::list<CIntelligentPointer<CThing> >::iterator card = quest_cards.begin(); card != quest_cards.end(); ++card) {
                CTCQuestCard* quest_card;
                (*card)->GetTC(&quest_card);
                CCharString teleporter_region_name = quest_card->GetTeleporterRegionName();
                if (quest_card->GetTeleporterRegionName() != "") {
                    if (GetRegionTeleporter(PeekWorld().PeekWorldMap().GetRegion(
                            PeekWorld().PeekWorldMap().GetRegionNumberFromName(quest_card->GetTeleporterRegionName()))) != 0) {
                        const CRegion& region = PeekWorld().PeekWorldMap().GetRegion(
                            PeekWorld().PeekWorldMap().GetRegionNumberFromName(quest_card->GetTeleporterRegionName()));
                        marker_name = quest_card->GetTeleporterRegionName();
                        bool already_listed = false;
                        for (unsigned long j = 0; j < regions.size(); ++j) {
                            if (regions[j].GetRegionDefName() == region.GetRegionDefName()) {
                                already_listed = true;
                                break;
                            }
                        }
                        if (!already_listed
                            && region.GetRegionDefName() != PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetRegionDefName())
                            regions.push_back(region);
                    }
                }
            }
            if (!IsInventoryScreen()) {
                long recall_region = GetGuildSealRecallRegion();
                if (recall_region > 0) {
                    const CRegion& region = PeekWorld().PeekWorldMap().GetRegion(recall_region);
                    bool already_listed = false;
                    for (unsigned long j = 0; j < regions.size(); ++j) {
                        if (regions[j].GetRegionDefName() == region.GetRegionDefName()) {
                            already_listed = true;
                            break;
                        }
                    }
                    if (!already_listed)
                        regions.push_back(region);
                }
            }
        }
    }
    if (check_only && has_regions) {
        *has_regions = !regions.empty();
        return 0;
    }
    unsigned long num_regions = regions.size();
    if (num_regions == 0)
        return 0;

    CDefPointer<const CUIMiscThingsDef> misc_def;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &misc_def);
    CManager* manager = CManager::GetInstance();
    CCountedPointer<CComponent> world_map(manager->CreateComponent("PC_WORLD_MAP", false));

    pmap_real = static_cast<CZoomingComponent*>(world_map.Get());
    C2DVector zoom_center = CUIScreenUnitConverter::ConvertVector(misc_def->HeroDollFrameTL_PC);
    zoom_center.Y += CManager::GetInstance()->GetUIScale().Y * misc_def->HeroDollFrameEmulateListOffset;
    zoom_center.X += CManager::GetInstance()->GetUIScale().X * misc_def->PCRingCenter.X;
    zoom_center.Y += CManager::GetInstance()->GetUIScale().Y * misc_def->PCRingCenter.Y;
    C2DVector min_zoom(0.2f, 0.2f);
    C2DVector max_zoom(1.5f, 1.5f);
    pmap_real->SetMaxZoom(max_zoom);
    pmap_real->SetMinZoom(min_zoom);
    pmap_real->SetZoomCenter(zoom_center);

    CList* list;
    if (IsInventoryScreen()) {
        list = static_cast<CList*>(manager->CreateComponent("PC_MAP_LIST", false));
        CComponent* dummy = list->FindChild("PC_LIST_POSITION_DUMMY");
        if (dummy) {
            CComponent* map_frame = list->FindChild("PC_UI_MAP_FRAME");
            if (map_frame)
                dummy->AddPositionChild(map_frame);
        }
    } else {
        list = static_cast<CList*>(manager->CreateComponent("PC_TELEPORT_LIST", false));
        CComponent* dummy = list->FindChild("PC_LIST_POSITION_DUMMY");
        if (dummy) {
            CComponent* map_frame = list->FindChild("PC_UI_MAP_FRAME");
            if (map_frame)
                dummy->AddPositionChild(map_frame);
        }
        CComponent* scroll_description_up = list->FindChild("SCROLL_DESCRIPTION_UP");
        if (scroll_description_up) {
            manager->AddObserver(scroll_description_up);
            scroll_description_up->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        CComponent* scroll_description_down = list->FindChild("SCROLL_DESCRIPTION_DOWN");
        if (scroll_description_down) {
            manager->AddObserver(scroll_description_down);
            scroll_description_down->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        CComponent* back_clickable = list->FindChild("PC_BACK_CLICKABLE");
        if (back_clickable) {
            manager->AddObserver(back_clickable);
            back_clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        CComponent* close_clickable = list->FindChild("PC_CLOSE_CLICKABLE");
        if (close_clickable) {
            manager->AddObserver(close_clickable);
            close_clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        CComponent* scroll_list_up = list->FindChild("SCROLL_LIST_UP");
        if (scroll_list_up) {
            manager->AddObserver(scroll_list_up);
            scroll_list_up->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        CComponent* scroll_list_down = list->FindChild("SCROLL_LIST_DOWN");
        if (scroll_list_down) {
            manager->AddObserver(scroll_list_down);
            scroll_list_down->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
    }
    MapList = list;
    CComponent* frame = MapList->FindChild("PC_UI_MAP_FRAME");
    CComponent* title = MapList->FindChild("PC_TITLE_WORLD_MAP");
    if (title)
        frame->AddPositionChild(title);
    title = MapList->FindChild("PC_TITLE_TELEPORT_REGIONS");
    if (title)
        frame->AddPositionChild(title);
    CCountedPointer<CComponent> movie_viewport(manager->CreateComponent("PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT", false));
    list->AddNonScrollingChild(movie_viewport);
    CCountedPointer<CComponent> description_viewport(manager->CreateComponent("PC_DESCRIPTION_VIEWPORT", false));
    list->AddNonScrollingChild(description_viewport);
    frame->AddPositionChild(movie_viewport.Get());
    frame->AddPositionChild(description_viewport.Get());
    list->InitialiseOffsets(num_regions);

    std::set<long> display_regions;
    std::vector<CRegion> marker_regions;
    GetRegionsToDisplay(display_regions);
    if (!IsInventoryScreen()) {
        for (unsigned long j = 0; j < num_regions; ++j)
            display_regions.insert(PeekWorld().PeekWorldMap().GetRegionNumberFromName(regions[j].GetName()));
    }
    for (std::set<long>::iterator it = display_regions.begin(); it != display_regions.end(); ++it) {
        if (PeekWorld().PeekWorldMap().GetRegion(*it).IsOnWorldMap())
            marker_regions.push_back(PeekWorld().PeekWorldMap().GetRegion(*it));
    }
    CreateMapMarkers(world_map.Get(), marker_regions);

    unsigned long i;
    long recall = -1;
    if (!IsInventoryScreen()) {
        long guild = -1;
        for (unsigned long j = 0; j < num_regions; ++j) {
            const CRegion& region = regions[j];
            if (PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName()) == GetGuildSealRecallRegion())
                recall = j;
            if (region.GetName() == CCharString("HeroGuildComplexInside"))
                guild = j;
        }
        CRegion temp;
        long next = 0;
        if (recall != -1) {
            temp = regions[recall];
            regions[recall] = regions[0];
            regions[0] = temp;
            next = 1;
        }
        if (guild != -1 && !(recall == 0 && guild == 1) && !(recall == 1 && guild == 0)) {
            temp = regions[guild];
            regions[guild] = regions[next];
            regions[next] = temp;
        }
        recall = -1;
    }

    for (i = 0; i < num_regions; ++i) {
        CRegion region = regions[i];
        CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(region.GetDisplayName());
        bool recall_entry = false;
        if (!IsInventoryScreen()) {
            long region_number = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
            if (region_number == GetGuildSealRecallRegion()) {
                text += L" " + CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_RECALL");
                recall_entry = true;
                recall = region_number;
            }
        }
        CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(text, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                                            0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        if (!IsInventoryScreen()) {
            CCountedPointer<CAction> close_action(new CAction);
            close_action->ActionType = UI_ACTION_TYPE_CLOSE_TELEPORT_MENU;
            entry->AddActionOnBack(close_action);
        } else {
            CCountedPointer<CAction> close_action(new CAction);
            close_action->ActionType = UI_ACTION_TYPE_CLOSE_ACTIVE_MENU;
            entry->AddActionOnBack(close_action);
        }
        list->AddScrollingChildAndSetOffsets(entry);
        CCountedPointer<CComponent> subtitle(manager->CreateComponent("PC_SUBTITLE_INDEPENDANT", false));
        CText* subtitle_text = static_cast<CText*>(subtitle.Get());
        subtitle_text->SetValue(text, true, true);
        entry->AddChild(subtitle);
        frame->AddPositionChild(subtitle_text);

        long region_number = PeekWorld().PeekWorldMap().GetRegionNumberFromName(region.GetName());
        long hero_region = GetWorldMap().GetRegionNumberMapIsIn(GetParentThing().GetCurrentMapNumber());
        if (IsInventoryScreen()) {
            if (region_number == hero_region)
                list->SetDefaultChild((long)i);
        } else if (region_number == recall && recall != -1) {
            list->SetDefaultChild((long)i);
        } else if (recall == -1 && region.GetName() == CCharString("HeroGuildComplexInside")) {
            list->SetDefaultChild((long)i);
        } else if (marker_name != "" && region.GetName() == marker_name) {
            list->SetDefaultChild((long)i);
        }

        CCountedPointer<CComponent> description(manager->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT", false));
        ConstructRegionDescription(description.Get(), region, true);
        entry->AddChild(description);
        if (description.Get())
            frame->AddPositionChild(description.Get());
        if (description_viewport.Get())
            static_cast<CViewport*>(description_viewport.Get())->AddFakeChild(description.Get());

        if (IsInventoryScreen()) {
            CCountedPointer<CComponent> sub_list(manager->CreateComponent("PC_SUB_LIST", false));
            CCountedPointer<CList> sub_list_as_list;
            sub_list_as_list.SetStaticCast(sub_list);
            entry->AddChild(sub_list);
            CCountedPointer<CComponent> region_title(manager->CreateComponent("PC_TITLE_REGION_MAP", false));
            sub_list_as_list->AddNonScrollingChild(region_title);
            if (region_title.Get())
                frame->AddPositionChild(region_title.Get());
            CWideString zoom_text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_ZOOMED_REGION");
            CCountedPointer<CComponent> zoom_entry(manager->CreateMenuEntryComponent(zoom_text, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                                     0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            CCountedPointer<CComponent> mini_map(manager->CreateComponent("PC_MAP_MENU_MINI_MAP_TEMPLATE", false));
            frame->AddPositionChild(mini_map.Get());
            CViewport* viewport = static_cast<CViewport*>(movie_viewport.Get());
            CCountedPointer<CComponent> mini_map_sprite(manager->CreateComponent("PC_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE", false));
            frame->AddPositionChild(mini_map.Get());
            if (mini_map_sprite->GetType() == UI_TYPE_SPRITE)
                static_cast<CSprite*>(mini_map_sprite.Get())->SetGraphicIndex(GetMiniMapGraphic(region));
            mini_map->AddChild(mini_map_sprite);
            viewport->AddFakeChild(mini_map.Get());
            zoom_entry->AddChild(mini_map);
            sub_list_as_list->AddScrollingChild(zoom_entry);
            CreateMiniMapMarkers(mini_map.Get(), region);
        } else {
            CCountedPointer<CComponent> sub_list(manager->CreateComponent("PC_SUB_LIST", false));
            CCountedPointer<CList> sub_list_as_list;
            sub_list_as_list.SetStaticCast(sub_list);
            CCountedPointer<CComponent> teleport_title(manager->CreateComponent("PC_TITLE_TELEPORT", false));
            sub_list_as_list->AddNonScrollingChild(teleport_title);
            if (teleport_title.Get())
                frame->AddPositionChild(teleport_title.Get());
            entry->AddChild(sub_list);
            CWideString teleport_text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_TELEPORT");
            CCountedPointer<CComponent> teleport_entry(manager->CreateMenuEntryComponent(teleport_text, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                                         0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            sub_list_as_list->AddScrollingChild(teleport_entry);
            CCountedPointer<CAction> teleport_action(new CAction);
            if (recall_entry) {
                teleport_action->ActionType = UI_ACTION_TYPE_GUILD_SEAL_RECALL;
            } else {
                CThing* teleporter = GetRegionTeleporter(region);
                if (teleporter) {
                    teleport_action->ActionType = UI_ACTION_TYPE_TELEPORT;
                    const CWorldMap& teleporter_world_map = PeekWorldMap();
                    teleport_action->ThingActionParams.push_back(teleporter_world_map.GetRegionNumberMapIsIn(teleporter->GetCurrentMapNumber()));
                }
            }
            teleport_entry->AddAction(teleport_action);
            CCountedPointer<CComponent> mini_map(manager->CreateComponent("PC_MAP_MENU_MINI_MAP_TEMPLATE", false));
            frame->AddPositionChild(mini_map.Get());
            CViewport* viewport = static_cast<CViewport*>(movie_viewport.Get());
            CCountedPointer<CComponent> mini_map_sprite(manager->CreateComponent("PC_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE", false));
            if (mini_map_sprite->GetType() == UI_TYPE_SPRITE)
                static_cast<CSprite*>(mini_map_sprite.Get())->SetGraphicIndex(GetMiniMapGraphic(region));
            mini_map->AddChild(mini_map_sprite);
            viewport->AddFakeChild(mini_map.Get());
            teleport_entry->AddChild(mini_map);
            CreateMiniMapMarkers(mini_map.Get(), region);
        }

        CUIState state;
        std::pair<const long, CRegion> map_region(i, region);
        MapRegions.insert(map_region);
        C2DVector origin = CUIScreenUnitConverter::ConvertVector(misc_def->HeroDollFrameTL_PC);
        origin.Y += CManager::GetInstance()->GetUIScale().Y * misc_def->HeroDollFrameEmulateListOffset;
        state.Position = misc_def->PCRingCenter - region.GetWorldMapOffset();
        state.Position.X *= CManager::GetInstance()->GetUIScale().X;
        state.Position.Y *= CManager::GetInstance()->GetUIScale().Y;
        state.StateChangeType = (EStateChangeType)3;
        state.StateChangeFlag = 0x67;
        // FITTED: origin first (retail 0x00611334 `fld origin.X` ; 0x00611342 `fadd Position.X`); operator+= loads Position first.
        state.Position.X = origin.X + state.Position.X;
        state.Position.Y = origin.Y + state.Position.Y;
        state.UpdateTime = 0.5f;
        std::pair<const unsigned long, CUIState> map_state(i + 8, state);
        world_map->GetStates().insert(world_map->GetStates().begin(), map_state);
    }

    list->AddNonScrollingChild(world_map);
    return list;
}
// ==== end of the row's statements ====
static int gFail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++gFail; } }
static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
static void World(int nregions)
{
    gWorld = CWorld(); gWorld.Recall.X = gWorld.Recall.Y = gWorld.Recall.Z = 0.0f; gWorld.Map.Loaded = 0; gWorld.Map.RecallMap = 0;
    for (int n = 0; n < nregions; ++n) {
        CRegion r; r.Number = n; r.Name = CCharString(("R" + Num(n)).c_str()); r.RegionDefName = CCharString(("D" + Num(n)).c_str());
        r.DisplayName = CCharString(("TEXT_R" + Num(n)).c_str()); r.Offset = C2DVector(10.0f * n, 20.0f * n);
        gWorld.Map.Regions.push_back(r);
    }
    CQuestManager::Get().Cards.clear(); gMissing.clear(); gCreated.clear(); CManager::GetInstance()->Observers.clear();
    CManager::GetInstance()->Scale = C2DVector(2.0f, 3.0f);
    gMisc.HeroDollFrameTL_PC = C2DVector(1.0f, 2.0f); gMisc.HeroDollFrameEmulateListOffset = 5.0f; gMisc.PCRingCenter = C2DVector(100.0f, 200.0f);
}
static Str Names(const std::vector<CRegion>& v) { Str s; for (size_t i = 0; i < v.size(); ++i) s += v[i].Name.s + " "; return s; }
static Str MapNames(const std::map<long, CRegion>& m) { Str s; for (std::map<long, CRegion>::const_iterator i = m.begin(); i != m.end(); ++i) s += Num(i->first) + "=" + i->second.Name.s + " "; return s; }
static Str Defaults(CList* l) { Str s; for (size_t i = 0; i < l->Defaults.size(); ++i) s += Num(l->Defaults[i]) + " "; return s; }
static Str EntryTexts(CList* l) { Str s; for (size_t i = 0; i < l->Scrolling.size(); ++i) s += l->Scrolling[i]->Text + "|"; return s; }
static CComponent* Child(CComponent* c, const char* name) { for (size_t i = 0; i < c->Children.size(); ++i) if (c->Children[i]->Name == name) return c->Children[i]; return 0; }
static CAction* TeleportAction(CComponent* entry)
{
    CComponent* sub = Child(entry, "PC_SUB_LIST"); if (!sub || sub->Scrolling.empty()) return 0;
    CComponent* tele = sub->Scrolling[0]; return tele->Actions.empty() ? 0 : tele->Actions[0];
}
int main()
{
    // (A) inventory map: shown regions 2..5; 2 and 4 on the world map, the hero (map 50) stands in region 3.
    {
        World(7);
        CTCInventoryMap m; m.Inventory = true; m.Hero.MapNumber = 50; gWorld.Map.RegionOfMap[50] = 3;
        m.Display.insert(2); m.Display.insert(3); m.Display.insert(4); m.Display.insert(5);
        gWorld.Map.Regions[2].OnWorldMap = true; gWorld.Map.Regions[4].OnWorldMap = true;
        CRegion junk; m.MapRegions[9] = junk;
        CList* list = static_cast<CList*>(m.ConstructPCMapList(false, 0));
        Check(list != 0 && list->Name == "PC_MAP_LIST", "A list");
        Check(!m.PanMode, "A pan mode cleared");
        Check(m.MapList == list, "A MapList");
        Check(MapNames(m.MapRegions) == "0=R2 1=R3 2=R4 ", "A MapRegions");
        Check(EntryTexts(list) == "<TEXT_R2>|<TEXT_R3>|<TEXT_R4>|", "A entries");
        Check(list->Offsets == 3, "A offsets");
        Check(Defaults(list) == "1 ", "A default = hero region");
        Check(Names(m.MarkerRegions) == "R2 R4 ", "A markers");
        Check(m.pmap_real != 0 && m.pmap_real->Name == "PC_WORLD_MAP", "A pmap_real");
        Check(Near(m.pmap_real->MinZoom.X, 0.2f) && Near(m.pmap_real->MaxZoom.Y, 1.5f), "A zoom limits");
        // centre: ConvertVector(1,2) = (2,4); Y += 3*5; X += 2*100; Y += 3*200
        Check(Near(m.pmap_real->Centre.X, 202.0f) && Near(m.pmap_real->Centre.Y, 619.0f), "A zoom centre");
        Check(m.pmap_real->States.size() == 3 && m.pmap_real->States.count(8) && m.pmap_real->States.count(10), "A states");
        const CUIState& s = m.pmap_real->States[9];   // region 3: (100-30)*2 + 2, (200-60)*3 + 4 + 15
        Check(Near(s.Position.X, 142.0f) && Near(s.Position.Y, 439.0f) && s.StateChangeType == 3 && s.StateChangeFlag == 0x67 && Near(s.UpdateTime, 0.5f), "A state 9");
        Check(list->Scrolling[0]->BackActions.size() == 1 && list->Scrolling[0]->BackActions[0]->ActionType == UI_ACTION_TYPE_CLOSE_ACTIVE_MENU, "A close action");
        CComponent* sub = Child(list->Scrolling[1], "PC_SUB_LIST");
        Check(sub && sub->Scrolling.size() == 1 && sub->Scrolling[0]->Name == "ENTRY PC_MENU_ENTRY_TEMPLATE", "A zoom entry");
        CComponent* mini = sub ? Child(sub->Scrolling[0], "PC_MAP_MENU_MINI_MAP_TEMPLATE") : 0;
        CComponent* sprite = mini ? Child(mini, "PC_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE") : 0;
        Check(sprite && sprite->Graphic == 300, "A mini map graphic");
        Check(m.Calls.size() == 4 && m.Calls[0] == "markers:PC_WORLD_MAP" && m.Calls[2] == "minimap R3 PC_MAP_MENU_MINI_MAP_TEMPLATE", "A calls");
        Check(CManager::GetInstance()->Observers.empty(), "A no observers");
    }
    // (B) teleport screen.
    {
        World(7);
        CTCInventoryMap m; m.Inventory = false; m.Hero.MapNumber = 50; gWorld.Map.RegionOfMap[50] = 3;
        gWorld.Map.Loaded = 2; gWorld.Map.Regions[6].Name = CCharString("HeroGuildComplexInside");
        int tele[] = { 1, 2, 3, 5, 6 };
        for (int k = 0; k < 5; ++k) { m.Teleporters.insert(tele[k]); m.TeleporterThings[tele[k]].MapNumber = 10 * tele[k] + 1; gWorld.Map.RegionOfMap[10 * tele[k] + 1] = tele[k]; }
        gWorld.Map.Regions[1].OnWorldMap = gWorld.Map.Regions[2].OnWorldMap = gWorld.Map.Regions[3].OnWorldMap = true;
        gWorld.Map.Regions[6].OnWorldMap = true;
        gWorld.Recall.X = 1.0f; gWorld.Map.RecallMap = 70; gWorld.Map.RegionOfMap[70] = 4;
        const char* cards[] = { "R5", "", "R2", "R3" };
        static CThing things[4];
        for (int k = 0; k < 4; ++k) { things[k].Card.Teleporter = CCharString(cards[k]); CQuestManager::Get().Cards.push_back(CIntelligentPointer<CThing>(&things[k])); }
        m.Display.insert(2);
        gMissing.insert("PC_CLOSE_CLICKABLE");
        bool has = false;
        Check(m.ConstructPCMapList(true, &has) == 0 && has, "B check_only");
        Check(gCreated.empty(), "B check_only creates nothing");
        CList* list = static_cast<CList*>(m.ConstructPCMapList(false, &has));
        Check(list != 0 && list->Name == "PC_TELEPORT_LIST", "B list");
        Check(MapNames(m.MapRegions) == "0=R4 1=HeroGuildComplexInside 2=R3 3=R5 4=R1 ", "B order: recall, guild, rest");
        Check(EntryTexts(list) == "<TEXT_R4><TEXT_GUI_MENU_RECALL>|<TEXT_R6>|<TEXT_R3>|<TEXT_R5>|<TEXT_R1>|"
              || EntryTexts(list) == "<TEXT_R4> <TEXT_GUI_MENU_RECALL>|<TEXT_R6>|<TEXT_R3>|<TEXT_R5>|<TEXT_R1>|", "B entries");
        Check(Defaults(list) == "0 2 ", "B defaults: recall, quest-card marker region");
        Check(Names(m.MarkerRegions) == "R1 R2 R3 HeroGuildComplexInside ", "B markers");
        Check(CManager::GetInstance()->Observers.size() == 5, "B observers");
        Check(list->Scrolling[1]->BackActions.size() == 1 && list->Scrolling[1]->BackActions[0]->ActionType == UI_ACTION_TYPE_CLOSE_TELEPORT_MENU, "B close action");
        CAction* a0 = TeleportAction(list->Scrolling[0]); CAction* a3 = TeleportAction(list->Scrolling[3]);
        Check(a0 && a0->ActionType == UI_ACTION_TYPE_GUILD_SEAL_RECALL && a0->ThingActionParams.empty(), "B recall action");
        Check(a3 && a3->ActionType == UI_ACTION_TYPE_TELEPORT && a3->ThingActionParams.size() == 1 && a3->ThingActionParams[0] == 5, "B teleport action");
        Check(list->Scrolling[2]->Children.size() == 3, "B entry children");
    }
    // (C) teleport screen with no recall position: only the guild; it becomes the default.
    {
        World(7);
        CTCInventoryMap m; m.Inventory = false; gWorld.Map.Regions[6].Name = CCharString("HeroGuildComplexInside");
        m.Teleporters.insert(6); m.TeleporterThings[6].MapNumber = 61; gWorld.Map.RegionOfMap[61] = 6; gWorld.Map.Regions[6].OnWorldMap = true;
        CList* list = static_cast<CList*>(m.ConstructPCMapList(false, 0));
        Check(list && MapNames(m.MapRegions) == "0=HeroGuildComplexInside ", "C regions");
        Check(list && Defaults(list) == "0 ", "C guild default");
    }
    // (E) teleport screen, guild before the recall region: after the recall swap the guild is already second (no second swap).
    {
        World(4);
        CTCInventoryMap m; m.Inventory = false; gWorld.Map.Regions[1].Name = CCharString("HeroGuildComplexInside");
        for (int k = 1; k <= 2; ++k) { m.Teleporters.insert(k); m.TeleporterThings[k].MapNumber = 10 * k + 1; gWorld.Map.RegionOfMap[10 * k + 1] = k; gWorld.Map.Regions[k].OnWorldMap = true; }
        gWorld.Recall.Z = 2.0f; gWorld.Map.RecallMap = 80; gWorld.Map.RegionOfMap[80] = 2;
        CList* list = static_cast<CList*>(m.ConstructPCMapList(false, 0));
        Check(list && MapNames(m.MapRegions) == "0=R2 1=HeroGuildComplexInside ", "E order");
        Check(list && Defaults(list) == "0 ", "E recall default");
    }
    // (D) nothing to show.
    {
        World(4);
        CTCInventoryMap m; m.Inventory = true; m.Display.insert(1); CRegion junk; m.MapRegions[3] = junk;
        bool has = true;
        Check(m.ConstructPCMapList(true, &has) == 0 && !has, "D check_only false");
        Check(m.ConstructPCMapList(false, &has) == 0 && m.MapRegions.empty() && gCreated.empty() && !m.PanMode, "D empty");
    }
    if (gFail == 0) std::printf("0060eeb0_TEST PASS\n");
    return gFail;
}
