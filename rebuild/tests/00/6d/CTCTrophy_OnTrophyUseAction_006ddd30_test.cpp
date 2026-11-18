// Standalone C++03 fixture for the recovered trophy use action.
#include <cstdio>
#include <functional>
#include <list>
#include <string>

typedef std::string CCharString;
typedef std::wstring CWideString;

enum EPlayerMode {
    PLAYER_MODE_CONTROL_CREATURE = 1, PLAYER_MODE_Z_TARGET = 2,
    PLAYER_MODE_DEAD = 3, PLAYER_MODE_QUICK_ACCESS_MENU = 36, PLAYER_MODE_LIVE_GUI = 43
};
enum ETCInterfaceType { TCI_INVENTORY_TRADE = 23, TCI_VILLAGE_MEMBER = 35 };

struct CTCInventoryTrade {
    long TrophyUseNeeded;
    CTCInventoryTrade() : TrophyUseNeeded(0) {}
};

struct CThing {
    unsigned long Definition;
    bool Dead;
    CTCInventoryTrade* Trade;
    CThing() : Definition(0), Dead(false), Trade(0) {}
    unsigned long GetDefGlobalIndex() const { return Definition; }
    bool IsDead() const { return Dead; }
    bool GetTC(ETCInterfaceType type, CTCInventoryTrade** out) {
        *out = Trade;
        return type == TCI_INVENTORY_TRADE && Trade != 0;
    }
};

typedef CThing CThingPlayerCreature;
typedef CThing CThingAICreature;

struct CCreatureActionBase {
    const CThing* Trophy;
    CCharString Name;
    long Flag;
    virtual ~CCreatureActionBase() {}
};

template<class T> struct CCountedPointer {
    T* Object;
    CCountedPointer() : Object(0) {}
    ~CCountedPointer() { delete Object; }
    void Reset(T* object) { delete Object; Object = object; }
};

struct CThingCreatureBase : CThing {
    int ActionCalls;
    const CThing* Shown;
    CCharString Animation;

    CThingCreatureBase() : ActionCalls(0), Shown(0) {}
    bool SetCurrentAction(const CCountedPointer<CCreatureActionBase>& action) {
        ++ActionCalls;
        Shown = action.Object->Trophy;
        Animation = action.Object->Name;
        return action.Object->Flag == 0;
    }
};

struct CCreatureAction_ShowTrophy : CCreatureActionBase {
    CCreatureAction_ShowTrophy(CThingCreatureBase&, CThing& trophy, const CCharString& name, long flag) {
        Trophy = &trophy;
        Name = name;
        Flag = flag;
    }
};

struct CPlayer {
    EPlayerMode Mode;
    CThingPlayerCreature* Controlled;
    CPlayer() : Mode(PLAYER_MODE_CONTROL_CREATURE), Controlled(0) {}
    EPlayerMode GetCurrentMode() const { return Mode; }
    CThingPlayerCreature* GetPControlledCreature() const { return Controlled; }
};

struct CPlayerManager {
    CPlayer Main;
    CPlayer& GetMainPlayer() { return Main; }
};

struct CDrawQuestInfo {
    int Minigame;
    CDrawQuestInfo() : Minigame(0) {}
    int GetMiniGameType() const { return Minigame; }
};

namespace NHeroInformationScreens {
struct CManager {
    bool Open;
    CManager() : Open(false) {}
    bool IsOpen() const { return Open; }
};
}

struct CPlayerGui {
    CDrawQuestInfo Quest;
    NHeroInformationScreens::CManager Manager;
    static CPlayerGui& Get();
    CDrawQuestInfo& GetQuestInfo() { return Quest; }
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() { return &Manager; }
};
CPlayerGui* g_pPlayerGui = 0;
CPlayerGui& CPlayerGui::Get() { return *g_pPlayerGui; }

namespace NGameText {
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& key) const {
        return key == "TEXT_GUI_TROPHY_PLAY_GAME" ? L"Play" : L"No people";
    }
};
static long WitnessCount, ExistingBest;
bool ConstructString(CWideString& text, long witnesses, long best) {
    WitnessCount = witnesses;
    ExistingBest = best;
    text += L" again";
    return true;
}
}

struct CMainGameComponent {
    NGameText::CDataBank Bank;
    static CMainGameComponent* Get() {
        static CMainGameComponent component;
        return &component;
    }
    const NGameText::CDataBank& GetTextBank() const { return Bank; }
};

struct CDefString {
    CCharString GetString() const { return "show_trophy"; }
    operator CCharString() const { return GetString(); }
};
struct CTrophyDef {
    CCharString SoundStartGame;
    CDefString ShowTrophyAnimName;
    CTrophyDef() : SoundStartGame("start_game") {}
};

struct CThingSearchTools {
    int Witnesses, Searches;
    CThingSearchTools() : Witnesses(0), Searches(0) {}
    template<class T, class Filter, class Container> void GetAllThings(const Filter&, Container& list) {
        ++Searches;
        for (int i = 0; i < Witnesses; ++i)
            list.push_back(i);
    }
};
struct CWorld {
    CThingSearchTools Search;
    CThingSearchTools& GetThingSearchTools() { return Search; }
};

struct CThingFilter_HasTC { explicit CThingFilter_HasTC(ETCInterfaceType) {} };
struct CThingFilter_AICreatureHasProperty { explicit CThingFilter_AICreatureHasProperty(unsigned long) {} };

struct CTCTrophy {
    CThing* ParentThing;
    CTrophyDef* PDef;
    bool MinigameInProgress;
    long BestWitnessesAheadToDate;
    std::list<int> AvailableWitnesses;
    CWorld World;
    CPlayerManager PlayerManager;
    int DialogCount, SoundCount;
    CWideString LastText, LastHelp;
    unsigned long LastButtons;
    CCharString LastSound;

    CTCTrophy(CThing& thing, CTrophyDef& def) : ParentThing(&thing), PDef(&def),
        MinigameInProgress(false), BestWitnessesAheadToDate(4),
        DialogCount(0), SoundCount(0), LastButtons(0) {}

    CWorld& GetWorld() { return World; }
    CPlayerManager& GetPlayerManager() { return PlayerManager; }
    unsigned long PlaySoundW(const CCharString& name) {
        ++SoundCount;
        LastSound = name;
        return SoundCount;
    }
    void StartDialogBox(CWideString text, unsigned long buttons, CWideString, CWideString, CWideString help) {
        ++DialogCount;
        LastText = text;
        LastButtons = buttons;
        LastHelp = help;
    }
    void OnTrophyUseAction(CThingCreatureBase&);
};

class CThingFilter_HasScriptSupervision : public std::unary_function<CThing, bool> {
    bool CheckForActive;
public:
    explicit CThingFilter_HasScriptSupervision(bool active) : CheckForActive(active) {}
};
class CThingFilter_IsLyingDown : public std::unary_function<CThing, bool> {};

template<class F> class CFilter_Not {
    const F& Filter;
public:
    explicit CFilter_Not(const F& filter) : Filter(filter) {}
};

template<class A, class B, class C, class D, class E> class CFilter_Bind5 {
    const A& First;
    const B& Second;
    const C& Third;
    const D& Fourth;
    const E& Fifth;
public:
    CFilter_Bind5(const A& a, const B& b, const C& c, const D& d, const E& e)
        : First(a), Second(b), Third(c), Fourth(d), Fifth(e) {}
};

// Recovered method body, with game dependencies supplied by the fixture.
void CTCTrophy::OnTrophyUseAction(CThingCreatureBase& creature_performing_action)
{
    if (!MinigameInProgress) {
        if (CPlayerGui::Get().GetQuestInfo().GetMiniGameType())
            return;
        EPlayerMode player_mode = GetPlayerManager().GetMainPlayer().GetCurrentMode();
        if (player_mode != PLAYER_MODE_CONTROL_CREATURE && player_mode != PLAYER_MODE_Z_TARGET
            && player_mode != PLAYER_MODE_QUICK_ACCESS_MENU && player_mode != PLAYER_MODE_LIVE_GUI)
            return;
        if (CPlayerGui::Get().GetPHeroInfoScreensManager()->IsOpen()) {
            CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCInventoryTrade* tc_inventory;
            if (player && !player->IsDead() && player->GetTC(TCI_INVENTORY_TRADE, &tc_inventory))
                tc_inventory->TrophyUseNeeded = ParentThing->GetDefGlobalIndex();
            return;
        }

        AvailableWitnesses.clear();
        typedef CFilter_Bind5<CThingFilter_HasTC, CThingFilter_HasTC, CThingFilter_HasScriptSupervision,
            CFilter_Not<CThingFilter_AICreatureHasProperty>, CFilter_Not<CThingFilter_IsLyingDown> > WitnessFilter;
        GetWorld().GetThingSearchTools().GetAllThings<CThingAICreature>(
            WitnessFilter(CThingFilter_HasTC((ETCInterfaceType)0x4f), CThingFilter_HasTC(TCI_VILLAGE_MEMBER),
                CThingFilter_HasScriptSupervision(false),
                CFilter_Not<CThingFilter_AICreatureHasProperty>(CThingFilter_AICreatureHasProperty(3)),
                CFilter_Not<CThingFilter_IsLyingDown>(CThingFilter_IsLyingDown())), AvailableWitnesses);
        if (!AvailableWitnesses.empty()) {
            PlaySoundW(PDef->SoundStartGame);
            CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_TROPHY_PLAY_GAME");
            NGameText::ConstructString(text, AvailableWitnesses.size(), BestWitnessesAheadToDate);
            CWideString help(L"TEXT_GUI_HELP");
            StartDialogBox(text, 4, CWideString(L""), CWideString(L""), help);
        } else {
            CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_TROPHY_NO_PEOPLE");
            StartDialogBox(text, 10, CWideString(L""), CWideString(L""), CWideString(L""));
        }
    } else {
        CCountedPointer<CCreatureActionBase> paction;
        paction.Reset(new CCreatureAction_ShowTrophy(creature_performing_action, *ParentThing,
            PDef->ShowTrophyAnimName, 0));
        creature_performing_action.SetCurrentAction(paction);
    }
}

int main()
{
    CThing item, player;
    CTCInventoryTrade trade;
    CTrophyDef def;
    CThingCreatureBase actor;
    CPlayerGui gui;
    g_pPlayerGui = &gui;
    item.Definition = 19;
    CTCTrophy owner(item, def);
    owner.PlayerManager.Main.Controlled = &player;
    player.Trade = &trade;

    gui.Quest.Minigame = 1;
    owner.OnTrophyUseAction(actor);
    bool ok = owner.DialogCount == 0 && owner.World.Search.Searches == 0;
    gui.Quest.Minigame = 0;
    owner.PlayerManager.Main.Mode = PLAYER_MODE_DEAD;
    owner.OnTrophyUseAction(actor);
    ok = ok && owner.World.Search.Searches == 0;
    owner.PlayerManager.Main.Mode = PLAYER_MODE_CONTROL_CREATURE;
    gui.Manager.Open = true;
    player.Dead = true;
    owner.OnTrophyUseAction(actor);
    ok = ok && trade.TrophyUseNeeded == 0 && owner.World.Search.Searches == 0;
    player.Dead = false;
    owner.OnTrophyUseAction(actor);
    ok = ok && trade.TrophyUseNeeded == 19 && owner.DialogCount == 0;
    gui.Manager.Open = false;
    owner.AvailableWitnesses.push_back(8);
    owner.OnTrophyUseAction(actor);
    ok = ok && owner.AvailableWitnesses.empty() && owner.DialogCount == 1
        && owner.LastText == L"No people" && owner.LastButtons == 10 && owner.SoundCount == 0;
    owner.World.Search.Witnesses = 2;
    owner.OnTrophyUseAction(actor);
    ok = ok && owner.DialogCount == 2 && owner.LastText == L"Play again"
        && owner.LastButtons == 4 && owner.LastHelp == L"TEXT_GUI_HELP"
        && owner.SoundCount == 1 && owner.LastSound == "start_game"
        && NGameText::WitnessCount == 2 && NGameText::ExistingBest == 4;
    owner.MinigameInProgress = true;
    owner.OnTrophyUseAction(actor);
    ok = ok && actor.ActionCalls == 1 && actor.Shown == &item && actor.Animation == "show_trophy"
        && owner.World.Search.Searches == 2 && owner.DialogCount == 2;

    std::puts(ok ? "006ddd30_TEST PASS" : "006ddd30_TEST FAIL");
    return ok ? 0 : 1;
}