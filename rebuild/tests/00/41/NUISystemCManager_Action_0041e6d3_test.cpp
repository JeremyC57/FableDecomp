// Behaviour test for NUISystem::CManager::Action(CCountedPointer<CAction>&) (Fable.exe 0x0041E6D3, 35,651 bytes): the head and
// the cases whose source shape changed in the session that matched the row, over a portable model of the UI manager, the game
// component, the hero's components and the action parameters. Each case body is a copy of the row's logic (std::list walks
// written as index walks over a parameter array, CWideString as a char text). Checks:
//  - an invalid action does nothing; with no game component the action goes to the front end first, then to the switch;
//  - CLOSE_TELEPORT_MENU (falls into default:), CLOSE_MENU: AddCloseInventoryEvent on the player's map / inventory component
//    only when the player, the component and its pointer exist; an unknown action type does nothing;
//  - CHEAT_MORALITY / CHEAT_RENOWN: SetMorality((long)(GetMoralityMax() * f)) / ForceRenownLevelTo((long)(RenownMaxLevel * f))
//    (truncating), then the hero's inventory closes when the hero has one (MORALITY ends with `return;`, RENOWN with `break;`);
//  - POP_MAP_STATE: the removed quest's state (position + 8) is dropped, the later states move down one, and the map changes to
//    that state when something moved, else to the one before it; nothing happens when the state is not in the map;
//  - PLAY_MOVIE, SCROLL_ACTIVE_LIST_UP / DOWN (delta 0,-25 / 0,+25), MOVE_COMPONENT (last three parameters, needs three),
//    CHANGE_ALPHA (each SLONG sets the alpha used by the components after it, default 0xFF), the ITEM_ASSIGN_* slots 2/3/1/0,
//    TELEPORT (needs a thing and a valid teleport; counts the teleport in the hero's stats), OPEN_PC_SKILLS_MENU (event 0x46 for the main player),
//    CONSTRUCT_STYLE_CARDS_LIST (only when the screen has no STYLE_CARDS_LIST child), ASSIGN_SPELL_TO_ITEM_SLOT_PC (plain v
//    swapping assignment of the spell's ABXY priority, then the spell to assign is cleared).
// Plain C++98 (VC7.1 and g++).
#include <cstdio>
#include <cstring>

// ---- call log ---------------------------------------------------------------------------------------------------------------
static char Calls[64][96];
static int NumCalls = 0;
static void Note(const char* text) { if (NumCalls < 64) std::strcpy(Calls[NumCalls++], text); }
static void Note1(const char* name, long a) { char b[96]; std::sprintf(b, "%s %ld", name, a); Note(b); }
static void Note2(const char* name, long a, long b2) { char b[96]; std::sprintf(b, "%s %ld %ld", name, a, b2); Note(b); }
static void NoteF(const char* name, float x, float y, float t) { char b[96]; std::sprintf(b, "%s %.2f %.2f %.2f", name, x, y, t); Note(b); }
static void Reset() { NumCalls = 0; }
static int Failures = 0;
static void Expect(const char* what, const char* const* want, int n)
{
    bool ok = NumCalls == n;
    for (int i = 0; ok && i < n; ++i)
        ok = std::strcmp(Calls[i], want[i]) == 0;
    if (!ok) {
        ++Failures;
        std::printf("mismatch in %s:\n", what);
        for (int j = 0; j < NumCalls; ++j) std::printf("  got  %s\n", Calls[j]);
        for (int k = 0; k < n; ++k) std::printf("  want %s\n", want[k]);
    }
}
#define EXPECT(what, list) Expect(what, list, (int)(sizeof(list) / sizeof(list[0])))
static const char* const NOTHING[1] = { "" };
static void ExpectNothing(const char* what) { Expect(what, NOTHING, 0); }

// ---- enums (rebuild/include/engine values) ----------------------------------------------------------------------------------
enum EActionType {
    UI_ACTION_TYPE_TELEPORT = 5, UI_ACTION_TYPE_ITEM_ASSIGN_LEFT = 26, UI_ACTION_TYPE_ITEM_ASSIGN_RIGHT = 27,
    UI_ACTION_TYPE_ITEM_ASSIGN_DOWN = 28, UI_ACTION_TYPE_ITEM_ASSIGN_UP = 29, UI_ACTION_TYPE_CLOSE_MENU = 60,
    UI_ACTION_TYPE_PLAY_MOVIE = 65, UI_ACTION_TYPE_CLOSE_TELEPORT_MENU = 80, UI_ACTION_TYPE_CHANGE_ALPHA = 212,
    UI_ACTION_TYPE_POP_MAP_STATE = 216, UI_ACTION_TYPE_MOVE_COMPONENT = 232, UI_ACTION_TYPE_OPEN_PC_SKILLS_MENU = 234,
    UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UP = 236, UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_DOWN = 237,
    UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC = 285, UI_ACTION_TYPE_CONSTRUCT_STYLE_CARDS_LIST = 286,
    UI_ACTION_TYPE_CHEAT_MORALITY = 3000, UI_ACTION_TYPE_CHEAT_RENOWN = 3001
};
enum EActionParamType { UI_ACTION_PARAM_TYPE_COMPONENT = 1, UI_ACTION_PARAM_TYPE_SLONG = 3, UI_ACTION_PARAM_TYPE_STRING = 4,
                        UI_ACTION_PARAM_TYPE_ULONG = 5, UI_ACTION_PARAM_TYPE_FLOAT = 6, UI_ACTION_PARAM_TYPE_2DVECTOR = 10 };
enum ETCInterface { TCI_HERO_STATS = 4, TCI_INVENTORY = 17, TCI_INVENTORY_QUESTS = 25, TCI_INVENTORY_MAP = 27, TCI_HERO_ABILITIES = 111 };
enum EUIEvent { UI_EVENT_LEFT = 2 };

// ---- action parameters ------------------------------------------------------------------------------------------------------
struct C2DVector { C2DVector(float x, float y) : X(x), Y(y) { } float X, Y; };
struct CRGBColour { unsigned char R, G, B, A; };
class CActionParam {
public:
    CActionParam(EActionParamType type) : ActionParamType(type) { }
    virtual ~CActionParam() { }
    EActionParamType ActionParamType;
};
class CActionParamSLONG : public CActionParam { public: CActionParamSLONG(long v) : CActionParam(UI_ACTION_PARAM_TYPE_SLONG), Value(v) { } long Value; };
class CActionParamULONG : public CActionParam { public: CActionParamULONG(unsigned long v) : CActionParam(UI_ACTION_PARAM_TYPE_ULONG), Value(v) { } unsigned long Value; };
class CActionParamFloat : public CActionParam { public: CActionParamFloat(float v) : CActionParam(UI_ACTION_PARAM_TYPE_FLOAT), Value(v) { } float Value; };
class CActionParam2DVector : public CActionParam { public: CActionParam2DVector(float x, float y) : CActionParam(UI_ACTION_PARAM_TYPE_2DVECTOR), Value(x, y) { } C2DVector Value; };
class CActionParamString : public CActionParam {
public:
    // CWideString model: ToCharString() gives the narrow text.
    struct CText { const char* Text; const char* ToCharString() const { return Text; } };
    CActionParamString(const char* text) : CActionParam(UI_ACTION_PARAM_TYPE_STRING) { Value.Text = text; }
    CText Value;
};

// CComponent: a UI component (itself an action parameter); its state map is a small sorted array.
struct CUIState { long Tag; };
class CComponent : public CActionParam {
public:
    CComponent(const char* name) : CActionParam(UI_ACTION_PARAM_TYPE_COMPONENT), Name(name), NumStates(0), HasStyleCardsList(false)
        { Colour.R = Colour.G = Colour.B = 10; Colour.A = 0; States[0].second.Tag = 0; }
    CRGBColour GetColour() { return Colour; }
    void ChangeColour(const CRGBColour& c, float time, bool) { Colour = c; char b[96]; std::sprintf(b, "%s.ChangeColour A=%d %.1f", Name, c.A, time); Note(b); }
    void ChangePositionDelta(const C2DVector& d, float time, bool) { char b[96]; std::sprintf(b, "%s.ChangePositionDelta", Name); NoteF(b, d.X, d.Y, time); }
    void ChangeTargetPositionDelta(const C2DVector& d, float time, bool) { char b[96]; std::sprintf(b, "%s.ChangeTargetPositionDelta", Name); NoteF(b, d.X, d.Y, time); }
    void ChangeState(unsigned long state) { char b[96]; std::sprintf(b, "%s.ChangeState", Name); Note1(b, (long)state); }
    bool FindChild(const char* name) { return HasStyleCardsList && std::strcmp(name, "STYLE_CARDS_LIST") == 0; }
    struct Entry { unsigned long first; CUIState second; };
    int FindState(unsigned long key) { for (int i = 0; i < NumStates; ++i) if (States[i].first == key) return i; return -1; }
    void AddState(unsigned long key, long tag) { States[NumStates].first = key; States[NumStates].second.Tag = tag; ++NumStates; }
    const char* Name;
    CRGBColour Colour;
    Entry States[16];
    int NumStates;
    bool HasStyleCardsList;
};

class CAction {
public:
    CAction(long type) : Valid(true), ActionType(type), NumUIActionParams(0), NumThingActionParams(0) { }
    bool IsValid() { return Valid; }
    void Add(CActionParam* p) { UIActionParams[NumUIActionParams++] = p; }
    void AddThing(long t) { ThingActionParams[NumThingActionParams++] = t; }
    bool Valid;
    long ActionType;
    CActionParam* UIActionParams[8];
    int NumUIActionParams;
    long ThingActionParams[4];
    int NumThingActionParams;
};

// ---- components of the hero / player ----------------------------------------------------------------------------------------
class CTCBase { public: virtual ~CTCBase() { } };
class CTCInventoryBase : public CTCBase {
public:
    CTCInventoryBase(const char* name) : Name(name) { }
    virtual void AddCloseInventoryEvent() { char b[96]; std::sprintf(b, "%s.AddCloseInventoryEvent", Name); Note(b); }
    const char* Name;
};
class CTCInventory : public CTCInventoryBase {
public:
    CTCInventory() : CTCInventoryBase("inventory"), ThingToAssign(0), ThingSlotToSwap(7), PComponentSlotToSwap(0) { }
    void SetThingAsQuickAccessItem(long thing, long slot, bool) { Note2("SetThingAsQuickAccessItem", thing, slot); }
    void ConstructStyleCardsScreen(long category, long first, long last, CComponent* screen)
        { char b[96]; std::sprintf(b, "ConstructStyleCardsScreen %ld %ld %ld %s", category, first, last, screen->Name); Note(b); }
    void AssignThingPC(long thing, unsigned long slot, CComponent*) { Note2("AssignThingPC", thing, (long)slot); }
    void AssignThingSwappingPC(long thing, unsigned long slot, long swapSlot, CComponent*, CComponent*)
        { char b[96]; std::sprintf(b, "AssignThingSwappingPC %ld %lu %ld", thing, slot, swapSlot); Note(b); }
    long GetThingSlotToSwap() { return ThingSlotToSwap; }
    CComponent* GetComponentSlotToSwap() { return PComponentSlotToSwap; }
    long ThingToAssign;
    long ThingSlotToSwap;
    CComponent* PComponentSlotToSwap;
};
class CTCHeroStats : public CTCBase {
public:
    CTCHeroStats() : MoralityMax(1000), RenownMaxLevel(9), NumTeleports(0) { }
    long GetMoralityMax() { return MoralityMax; }
    void SetMorality(long v) { Note1("SetMorality", v); }
    void ForceRenownLevelTo(long v) { Note1("ForceRenownLevelTo", v); }
    long MoralityMax;
    long RenownMaxLevel;
    long NumTeleports;
};
class CTCInventoryQuests : public CTCBase {
public:
    CTCInventoryQuests() : Position(0) { }
    long GetQuestPositionAndRemoveIt(long thing) { Note1("GetQuestPositionAndRemoveIt", thing); return Position; }
    long Position;
};
struct CAbilityDef { long ABXYPriority; };
class CTCInventoryAbilities : public CTCBase {
public:
    CTCInventoryAbilities() : SpellToAssign(0) { Def.ABXYPriority = 40; }
    const CAbilityDef* GetAbilityPDef(long ability) { Note1("GetAbilityPDef", ability); return &Def; }
    long SpellToAssign;
    CAbilityDef Def;
};

// CThing::GetTC model: false when the thing has no component of that interface, else the stored pointer (which may be null).
class CThing {
public:
    CThing() : NumTCs(0) { }
    void Set(long type, CTCBase* tc) { Types[NumTCs] = type; TCs[NumTCs] = tc; ++NumTCs; }
    template <class T> bool GetTC(long type, T** out)
    {
        for (int i = 0; i < NumTCs; ++i)
            if (Types[i] == type) { *out = static_cast<T*>(TCs[i]); return true; }
        return false;
    }
    long Types[8];
    CTCBase* TCs[8];
    int NumTCs;
};

// ---- game component, front end, events --------------------------------------------------------------------------------------
class CGameEvent { public: CGameEvent(long type) : Type(type), Player(-1) { } long Type; signed char Player; };
class CPlayer { public: CThing* GetPControlledCreature() { return Hero; } CThing* Hero; };
class CPlayerManager {
public:
    CPlayer& GetMainPlayer() { return MainPlayer; }
    long GetMainPlayerNumber() { return MainPlayerNumber; }
    CPlayer MainPlayer;
    long MainPlayerNumber;
};
class CMainGameComponent {
public:
    static CMainGameComponent* Get() { return PMainGameComponent; }
    CPlayerManager& GetPlayerManager() { return PlayerManager; }
    CPlayerManager& PeekPlayerManager() { return PlayerManager; }
    void AddGameEvent(const CGameEvent& e) { Note2("AddGameEvent", e.Type, e.Player); }
    CPlayerManager PlayerManager;
    static CMainGameComponent* PMainGameComponent;
};
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;
static CMainGameComponent* GameAfterFrontEnd = 0;   // what the front end's Action leaves as the game component
class CFrontEndManager {
public:
    static CFrontEndManager* GetInstance() { static CFrontEndManager instance; return &instance; }
    void Action(CAction* action) { Note1("FrontEnd.Action", action->ActionType); CMainGameComponent::PMainGameComponent = GameAfterFrontEnd; }
};
static void GFPlayFullScreenXBoxMovie(const char* name, unsigned long flags) { char b[96]; std::sprintf(b, "PlayMovie %s %lu", name, flags); Note(b); }

class CMapInventory : public CTCInventoryBase {
public:
    CMapInventory() : CTCInventoryBase("mapInventory"), TeleportValid(true) { }
    bool IsTeleportValid() { return TeleportValid; }
    void TeleportHeroToRegionTeleporter(long t) { Note1("TeleportHeroToRegionTeleporter", t); }
    bool TeleportValid;
};
static bool GTeleportOpenedFromMap = true;
static CComponent* GPreviousSelectedFront = 0;   // CList::GPreviousSelected.front()

// ---- CManager model: the row's head and the modelled cases ------------------------------------------------------------------
#define CLOSE_INVENTORY_EVENT(type) { \
        CTCInventoryBase* inventory; \
        if (PPlayer->GetTC(type, &inventory) && inventory) \
            inventory->AddCloseInventoryEvent(); \
    }

class CManager {
public:
    CManager() : PPlayer(0), PMapInventory(0), PInventory(0), NumThingActionParams(0), AssignmentItemsExpressionsSwapping(false), AssignmentSpellsSwapping(false) { }
    void SetEvent(long e) { Note1("SetEvent", e); }
    CComponent* GetCorrectComponentInterface(CComponent* c) { char b[96]; std::sprintf(b, "GetCorrectComponentInterface %s", c->Name); Note(b); return c; }
    void Action(CAction*& action);
    CThing* PPlayer;
    CMapInventory* PMapInventory;
    CTCInventory* PInventory;
    long ThingActionParams[4];
    int NumThingActionParams;
    bool AssignmentItemsExpressionsSwapping;
    bool AssignmentSpellsSwapping;
};

void CManager::Action(CAction*& action)
{
    if (!action->IsValid())
        return;
    CMainGameComponent* game = CMainGameComponent::Get();
    if (!game) {
        CFrontEndManager::GetInstance()->Action(action);
        game = CMainGameComponent::Get();
    }

    switch (action->ActionType) {
    case UI_ACTION_TYPE_TELEPORT:
        if (action->NumThingActionParams > 0 && PMapInventory->IsTeleportValid()) {
            long teleporter = action->ThingActionParams[action->NumThingActionParams - 1];
            CTCHeroStats* stats;
            CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_HERO_STATS, &stats);
            ++stats->NumTeleports;
            PMapInventory->TeleportHeroToRegionTeleporter(teleporter);
            GTeleportOpenedFromMap = false;
            PMapInventory->AddCloseInventoryEvent();
        }
        break;
    case UI_ACTION_TYPE_ITEM_ASSIGN_LEFT:
        if (NumThingActionParams > 0) {
            PInventory->SetThingAsQuickAccessItem(ThingActionParams[NumThingActionParams - 1], 2, true);
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_ITEM_ASSIGN_RIGHT:
        if (NumThingActionParams > 0) {
            PInventory->SetThingAsQuickAccessItem(ThingActionParams[NumThingActionParams - 1], 3, true);
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_ITEM_ASSIGN_DOWN:
        if (NumThingActionParams > 0) {
            PInventory->SetThingAsQuickAccessItem(ThingActionParams[NumThingActionParams - 1], 1, true);
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_ITEM_ASSIGN_UP:
        if (NumThingActionParams > 0) {
            PInventory->SetThingAsQuickAccessItem(ThingActionParams[NumThingActionParams - 1], 0, true);
            SetEvent(UI_EVENT_LEFT);
        }
        break;
    case UI_ACTION_TYPE_CLOSE_MENU:
        CLOSE_INVENTORY_EVENT(TCI_INVENTORY);
        break;
    case UI_ACTION_TYPE_CLOSE_TELEPORT_MENU:
        if (PPlayer)
            CLOSE_INVENTORY_EVENT(TCI_INVENTORY_MAP);
    default:
        break;
    case UI_ACTION_TYPE_PLAY_MOVIE: {
        CActionParamString* movie = static_cast<CActionParamString*>(action->UIActionParams[action->NumUIActionParams - 1]);
        GFPlayFullScreenXBoxMovie(movie->Value.ToCharString(), 0x20);
        return;
    }
    case UI_ACTION_TYPE_CHANGE_ALPHA: {
        long alpha = 0xff;
        for (int it = 0; it != action->NumUIActionParams; ++it) {
            if (action->UIActionParams[it]->ActionParamType == UI_ACTION_PARAM_TYPE_SLONG)
                alpha = static_cast<CActionParamSLONG*>(action->UIActionParams[it])->Value;
            if (action->UIActionParams[it]->ActionParamType == UI_ACTION_PARAM_TYPE_COMPONENT) {
                CComponent* component = static_cast<CComponent*>(action->UIActionParams[it]);
                CRGBColour colour = component->GetColour();
                colour.A = (unsigned char)alpha;
                component->ChangeColour(colour, 0.2f, false);
                component->States[0].second.Tag = (unsigned char)alpha;   // GetStates()[0].Colour.A
            }
        }
        break;
    }
    case UI_ACTION_TYPE_POP_MAP_STATE: {
        CComponent* map = static_cast<CComponent*>(action->UIActionParams[0]);
        CTCInventoryQuests* quests;
        CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY_QUESTS, &quests);
        unsigned long state = quests->GetQuestPositionAndRemoveIt(action->ThingActionParams[0]) + 8;
        if (map->FindState(state) >= 0) {
            CComponent::Entry states[16];
            int numStates = 0;
            bool moved = false;
            for (int it = 0; it != map->NumStates; ++it) {
                if (map->States[it].first > state) {
                    states[numStates].first = map->States[it].first - 1;
                    states[numStates].second = map->States[it].second;
                    ++numStates;
                    moved = true;
                }
                if (map->States[it].first < state)
                    states[numStates++] = map->States[it];
            }
            map->NumStates = 0;
            for (int k = 0; k < numStates; ++k)
                map->States[map->NumStates++] = states[k];
            if (moved)
                map->ChangeState(state);
            else
                map->ChangeState(state - 1);
        }
        break;
    }
    case UI_ACTION_TYPE_MOVE_COMPONENT:
        if (action->NumUIActionParams > 2) {
            int it = action->NumUIActionParams;
            CActionParamFloat* time = static_cast<CActionParamFloat*>(action->UIActionParams[--it]);
            CActionParam2DVector* delta = static_cast<CActionParam2DVector*>(action->UIActionParams[--it]);
            static_cast<CComponent*>(action->UIActionParams[--it])->ChangePositionDelta(delta->Value, time->Value, false);
        }
        break;
    case UI_ACTION_TYPE_OPEN_PC_SKILLS_MENU: {
        CGameEvent event(0x46);
        event.Player = (signed char)game->GetPlayerManager().GetMainPlayerNumber();
        CMainGameComponent::Get()->AddGameEvent(event);
        break;
    }
    case UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UP: {
        C2DVector delta(0.0f, -25.0f);
        GPreviousSelectedFront->ChangeTargetPositionDelta(delta, 0.0f, false);
        break;
    }
    case UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_DOWN: {
        C2DVector delta(0.0f, 25.0f);
        GPreviousSelectedFront->ChangeTargetPositionDelta(delta, 0.0f, false);
        return;
    }
    case UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC: {
        int it = action->NumUIActionParams - 1;
        CActionParamULONG* slot = static_cast<CActionParamULONG*>(action->UIActionParams[it]);
        CComponent* entry = static_cast<CComponent*>(action->UIActionParams[--it]);
        CTCInventory* inventory = 0;
        CTCInventoryAbilities* abilities = 0;
        CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_INVENTORY, &inventory);
        CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_HERO_ABILITIES, &abilities);
        if (inventory && abilities) {
            if (abilities->SpellToAssign) {
                if (!AssignmentItemsExpressionsSwapping)
                    inventory->AssignThingPC(abilities->GetAbilityPDef(abilities->SpellToAssign)->ABXYPriority, slot->Value, entry);
                else
                    inventory->AssignThingSwappingPC(abilities->GetAbilityPDef(abilities->SpellToAssign)->ABXYPriority, slot->Value,
                                                     inventory->GetThingSlotToSwap(), entry, inventory->GetComponentSlotToSwap());
                abilities->SpellToAssign = 0;
            } else if (inventory->ThingToAssign) {
                if (!AssignmentItemsExpressionsSwapping)
                    inventory->AssignThingPC(inventory->ThingToAssign, slot->Value, entry);
                else
                    inventory->AssignThingSwappingPC(inventory->ThingToAssign, slot->Value, inventory->ThingSlotToSwap, entry, inventory->PComponentSlotToSwap);
                inventory->ThingToAssign = 0;
            }
            AssignmentItemsExpressionsSwapping = false;
            AssignmentSpellsSwapping = false;
        }
        break;
    }
    case UI_ACTION_TYPE_CONSTRUCT_STYLE_CARDS_LIST: {
        CThing* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory) && inventory) {
            int it = 0;
            CComponent* screen = static_cast<CComponent*>(action->UIActionParams[it]);
            if (!screen->FindChild("STYLE_CARDS_LIST")) {
                GetCorrectComponentInterface(static_cast<CComponent*>(action->UIActionParams[++it]));
                CActionParamSLONG* category = static_cast<CActionParamSLONG*>(action->UIActionParams[++it]);
                CActionParamSLONG* first = static_cast<CActionParamSLONG*>(action->UIActionParams[++it]);
                CActionParamSLONG* last = static_cast<CActionParamSLONG*>(action->UIActionParams[++it]);
                inventory->ConstructStyleCardsScreen(category->Value, first->Value, last->Value, screen);
            }
        }
        break;
    }
    case UI_ACTION_TYPE_CHEAT_MORALITY: {
        CActionParamFloat* fraction = static_cast<CActionParamFloat*>(action->UIActionParams[action->NumUIActionParams - 1]);
        CThing* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCHeroStats* stats;
        hero->GetTC(TCI_HERO_STATS, &stats);
        stats->SetMorality((long)(stats->GetMoralityMax() * fraction->Value));
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory))
            inventory->AddCloseInventoryEvent();
        return;
    }
    case UI_ACTION_TYPE_CHEAT_RENOWN: {
        CActionParamFloat* fraction = static_cast<CActionParamFloat*>(action->UIActionParams[action->NumUIActionParams - 1]);
        CThing* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCHeroStats* stats;
        hero->GetTC(TCI_HERO_STATS, &stats);
        stats->ForceRenownLevelTo((long)(stats->RenownMaxLevel * fraction->Value));
        CTCInventory* inventory;
        if (hero->GetTC(TCI_INVENTORY, &inventory))
            inventory->AddCloseInventoryEvent();
        break;
    }
    }
}

// ---- scenarios ----------------------------------------------------------------------------------------------------------------
int main()
{
    CMainGameComponent game;
    CThing hero, player;
    CTCHeroStats stats;
    CTCInventory inventory;
    CTCInventoryBase heroMap("heroMap"), playerInventory("playerInventory"), playerMap("playerMap");
    CTCInventoryQuests quests;
    CTCInventoryAbilities abilities;
    hero.Set(TCI_HERO_STATS, &stats);
    hero.Set(TCI_INVENTORY, &inventory);
    hero.Set(TCI_INVENTORY_QUESTS, &quests);
    hero.Set(TCI_HERO_ABILITIES, &abilities);
    player.Set(TCI_INVENTORY, &playerInventory);
    player.Set(TCI_INVENTORY_MAP, &playerMap);
    game.PlayerManager.MainPlayer.Hero = &hero;
    game.PlayerManager.MainPlayerNumber = 3;
    CMainGameComponent::PMainGameComponent = &game;
    CMapInventory mapInventory;
    CManager manager;
    manager.PPlayer = &player;
    manager.PMapInventory = &mapInventory;
    manager.PInventory = &inventory;

    // Head: an invalid action does nothing; with no game the front end sees the action first.
    { CAction a(UI_ACTION_TYPE_CLOSE_MENU); a.Valid = false; CAction* p = &a; Reset(); manager.Action(p); ExpectNothing("invalid action"); }
    {
        CMainGameComponent::PMainGameComponent = 0; GameAfterFrontEnd = &game;
        CAction a(UI_ACTION_TYPE_OPEN_PC_SKILLS_MENU); CAction* p = &a; Reset(); manager.Action(p);
        static const char* const want[] = { "FrontEnd.Action 234", "AddGameEvent 70 3" };
        EXPECT("front end first", want);
        CMainGameComponent::PMainGameComponent = &game;
    }
    // CLOSE_TELEPORT_MENU / CLOSE_MENU / default.
    { CAction a(UI_ACTION_TYPE_CLOSE_TELEPORT_MENU); CAction* p = &a; Reset(); manager.Action(p);
      static const char* const want[] = { "playerMap.AddCloseInventoryEvent" }; EXPECT("close teleport menu", want); }
    { CAction a(UI_ACTION_TYPE_CLOSE_MENU); CAction* p = &a; Reset(); manager.Action(p);
      static const char* const want[] = { "playerInventory.AddCloseInventoryEvent" }; EXPECT("close menu", want); }
    { manager.PPlayer = 0; CAction a(UI_ACTION_TYPE_CLOSE_TELEPORT_MENU); CAction* p = &a; Reset(); manager.Action(p); ExpectNothing("no player"); manager.PPlayer = &player; }
    { player.TCs[1] = 0; CAction a(UI_ACTION_TYPE_CLOSE_TELEPORT_MENU); CAction* p = &a; Reset(); manager.Action(p); ExpectNothing("null map component"); player.TCs[1] = &playerMap; }
    { player.NumTCs = 1; CAction a(UI_ACTION_TYPE_CLOSE_TELEPORT_MENU); CAction* p = &a; Reset(); manager.Action(p); ExpectNothing("no map component"); player.NumTCs = 2; }
    { CAction a(9999); CAction* p = &a; Reset(); manager.Action(p); ExpectNothing("unknown action"); }
    // Cheats.
    { CActionParamFloat f(0.5f); CAction a(UI_ACTION_TYPE_CHEAT_MORALITY); a.Add(&f); CAction* p = &a; Reset(); manager.Action(p);
      static const char* const want[] = { "SetMorality 500", "inventory.AddCloseInventoryEvent" }; EXPECT("cheat morality", want); }
    { CActionParamFloat f(0.25f); CAction a(UI_ACTION_TYPE_CHEAT_RENOWN); a.Add(&f); CAction* p = &a; Reset(); manager.Action(p);
      static const char* const want[] = { "ForceRenownLevelTo 2", "inventory.AddCloseInventoryEvent" }; EXPECT("cheat renown", want); }
    { hero.Types[1] = 999; CActionParamFloat f(-0.5f); CAction a(UI_ACTION_TYPE_CHEAT_MORALITY); a.Add(&f); CAction* p = &a; Reset(); manager.Action(p);
      static const char* const want[] = { "SetMorality -500" }; EXPECT("cheat morality, no inventory", want); hero.Types[1] = TCI_INVENTORY; }
    // POP_MAP_STATE: quest at position 2 -> state 10 removed, 11 and 12 move down to 10 and 11.
    {
        CComponent map("map"); map.AddState(8, 1); map.AddState(9, 2); map.AddState(10, 3); map.AddState(11, 4); map.AddState(12, 5);
        quests.Position = 2;
        CAction a(UI_ACTION_TYPE_POP_MAP_STATE); a.Add(&map); a.AddThing(77); CAction* p = &a; Reset(); manager.Action(p);
        static const char* const want[] = { "GetQuestPositionAndRemoveIt 77", "map.ChangeState 10" }; EXPECT("pop map state", want);
        bool ok = map.NumStates == 4 && map.States[0].first == 8 && map.States[1].first == 9 && map.States[2].first == 10 &&
                  map.States[2].second.Tag == 4 && map.States[3].first == 11 && map.States[3].second.Tag == 5;
        if (!ok) { ++Failures; std::printf("pop map state: wrong states\n"); }
        // The last state removed: nothing moved, the map goes back one state.
        quests.Position = 3; Reset(); manager.Action(p);
        static const char* const want2[] = { "GetQuestPositionAndRemoveIt 77", "map.ChangeState 10" }; EXPECT("pop last map state", want2);
        if (map.NumStates != 3 || map.States[2].first != 10) { ++Failures; std::printf("pop last map state: wrong states\n"); }
        // A state the map does not have: nothing else happens.
        quests.Position = 40; Reset(); manager.Action(p);
        static const char* const want3[] = { "GetQuestPositionAndRemoveIt 77" }; EXPECT("pop missing map state", want3);
    }
    // PLAY_MOVIE, SCROLL_ACTIVE_LIST_UP / DOWN, MOVE_COMPONENT.
    { CActionParamString s("attract"); CAction a(UI_ACTION_TYPE_PLAY_MOVIE); a.Add(&s); CAction* p = &a; Reset(); manager.Action(p);
      static const char* const want[] = { "PlayMovie attract 32" }; EXPECT("play movie", want); }
    {
        CComponent list("list"); GPreviousSelectedFront = &list;
        CAction up(UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UP), down(UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_DOWN); CAction* p = &up; Reset(); manager.Action(p);
        p = &down; manager.Action(p);
        static const char* const want[] = { "list.ChangeTargetPositionDelta 0.00 -25.00 0.00", "list.ChangeTargetPositionDelta 0.00 25.00 0.00" };
        EXPECT("scroll active list", want);
    }
    {
        CComponent c("panel"); CActionParam2DVector d(3.0f, -4.0f); CActionParamFloat t(0.5f);
        CAction a(UI_ACTION_TYPE_MOVE_COMPONENT); a.Add(&c); a.Add(&d); a.Add(&t); CAction* p = &a; Reset(); manager.Action(p);
        static const char* const want[] = { "panel.ChangePositionDelta 3.00 -4.00 0.50" }; EXPECT("move component", want);
        CAction b(UI_ACTION_TYPE_MOVE_COMPONENT); b.Add(&d); b.Add(&t); p = &b; Reset(); manager.Action(p); ExpectNothing("move component, two params");
    }
    // CHANGE_ALPHA: the first component gets the default 0xFF, the one after the SLONG gets 0x40.
    {
        CComponent c1("c1"), c2("c2"); CActionParamSLONG alpha(0x40);
        CAction a(UI_ACTION_TYPE_CHANGE_ALPHA); a.Add(&c1); a.Add(&alpha); a.Add(&c2); CAction* p = &a; Reset(); manager.Action(p);
        static const char* const want[] = { "c1.ChangeColour A=255 0.2", "c2.ChangeColour A=64 0.2" }; EXPECT("change alpha", want);
        if (c2.States[0].second.Tag != 0x40 || c2.Colour.R != 10) { ++Failures; std::printf("change alpha: state colour\n"); }
    }
    // ITEM_ASSIGN_* (the manager's own thing parameters) and TELEPORT.
    {
        long types[4] = { UI_ACTION_TYPE_ITEM_ASSIGN_LEFT, UI_ACTION_TYPE_ITEM_ASSIGN_RIGHT, UI_ACTION_TYPE_ITEM_ASSIGN_DOWN, UI_ACTION_TYPE_ITEM_ASSIGN_UP };
        manager.ThingActionParams[0] = 55; manager.NumThingActionParams = 1; Reset();
        for (int i = 0; i < 4; ++i) { CAction a(types[i]); CAction* p = &a; manager.Action(p); }
        static const char* const want[] = { "SetThingAsQuickAccessItem 55 2", "SetEvent 2", "SetThingAsQuickAccessItem 55 3", "SetEvent 2",
                                            "SetThingAsQuickAccessItem 55 1", "SetEvent 2", "SetThingAsQuickAccessItem 55 0", "SetEvent 2" };
        EXPECT("item assign", want);
        manager.NumThingActionParams = 0; CAction a(UI_ACTION_TYPE_ITEM_ASSIGN_LEFT); CAction* p = &a; Reset(); manager.Action(p); ExpectNothing("item assign, no thing");
    }
    {
        CAction a(UI_ACTION_TYPE_TELEPORT); a.AddThing(12); CAction* p = &a; Reset(); GTeleportOpenedFromMap = true; manager.Action(p);
        static const char* const want[] = { "TeleportHeroToRegionTeleporter 12", "mapInventory.AddCloseInventoryEvent" }; EXPECT("teleport", want);
        if (GTeleportOpenedFromMap || stats.NumTeleports != 1) { ++Failures; std::printf("teleport: flag / count\n"); }
        mapInventory.TeleportValid = false; Reset(); manager.Action(p); ExpectNothing("teleport, invalid"); mapInventory.TeleportValid = true;
    }
    // CONSTRUCT_STYLE_CARDS_LIST.
    {
        CComponent screen("screen"), owner("owner"); CActionParamSLONG cat(2), first(5), last(9);
        CAction a(UI_ACTION_TYPE_CONSTRUCT_STYLE_CARDS_LIST); a.Add(&screen); a.Add(&owner); a.Add(&cat); a.Add(&first); a.Add(&last);
        CAction* p = &a; Reset(); manager.Action(p);
        static const char* const want[] = { "GetCorrectComponentInterface owner", "ConstructStyleCardsScreen 2 5 9 screen" }; EXPECT("style cards", want);
        screen.HasStyleCardsList = true; Reset(); manager.Action(p); ExpectNothing("style cards, list present");
    }
    // ASSIGN_SPELL_TO_ITEM_SLOT_PC: plain, then swapping (slot to swap 7), then nothing to assign.
    {
        CComponent entry("entry"); CActionParamULONG slot(1);
        CAction a(UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC); a.Add(&entry); a.Add(&slot); CAction* p = &a;
        abilities.SpellToAssign = 21; Reset(); manager.Action(p);
        static const char* const want[] = { "GetAbilityPDef 21", "AssignThingPC 40 1" }; EXPECT("assign spell", want);
        abilities.SpellToAssign = 22; manager.AssignmentItemsExpressionsSwapping = true; Reset(); manager.Action(p);
        static const char* const want2[] = { "GetAbilityPDef 22", "AssignThingSwappingPC 40 1 7" }; EXPECT("assign spell swapping", want2);
        if (abilities.SpellToAssign != 0 || manager.AssignmentItemsExpressionsSwapping) { ++Failures; std::printf("assign spell: reset\n"); }
        Reset(); manager.Action(p); ExpectNothing("assign spell, nothing to assign");
    }

    if (Failures) { std::printf("0041e6d3_TEST FAIL\n"); return 1; }
    std::printf("0041e6d3_TEST PASS\n");
    return 0;
}