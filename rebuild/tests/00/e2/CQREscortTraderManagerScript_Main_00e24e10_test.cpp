#include <cstdio>
#include <cstring>
// behaviour test: CQR_EscortTrader_ManagerScript::Main -- first-run reset, then the phase machine: activate an escort,
// wait for it to end and for the gating quest (Q_TraderEscort / Q_BanditCamp / Q_Arena + barricade / graveyard passage),
// and in the repeat phase re-activate whenever the hero is away from LookoutPoint on a quest day after the start time
static int g_frames, g_endAt, g_activates, g_barricade, g_escortEndsAfter, g_lastActivate;
static int g_lookoutUntil;
class CScriptDef { public: long EscortTraderQuestDayInterval; long EscortTraderQuestStartTime; };
static CScriptDef g_def;
static CScriptDef* g_pScriptDef = &g_def;
static __inline long DayModInterval(long day, const CScriptDef* def) { return day % def->EscortTraderQuestDayInterval; }
static __inline bool NotPastStartTime(long time, const CScriptDef* def) { return time <= def->EscortTraderQuestStartTime; }
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    bool IsRegionLoaded(const char* n) const { return !std::strcmp(n, "LookoutPoint") && g_frames < g_lookoutUntil; }
    bool IsQuestActive(const char* q) const { return !std::strcmp(q, "QR_EscortTrader") && g_frames < g_lastActivate + g_escortEndsAfter; }
    bool IsQuestCompleted(const char* q) const
    {
        return !std::strcmp(q, "Q_TraderEscort") || !std::strcmp(q, "Q_BanditCamp") || !std::strcmp(q, "Q_Arena")
            || !std::strcmp(q, "Q_OpeningGraveyardSecretPassage");
    }
    void ActivateQuest(const char* q) const { if (!std::strcmp(q, "Q_OrchardFarm_Barricade")) ++g_barricade; }
    long GetDayOfWeek() const { return g_frames / 4; }
    long GetTimeOfDay() const { return g_frames % 4; }
};
enum EscortTraderManagerPhase {
    PHASE_ACTIVATE_FIRST, PHASE_WAIT_TRADER_ESCORT, PHASE_ACTIVATE_SECOND, PHASE_WAIT_BANDIT_CAMP, PHASE_ACTIVATE_THIRD,
    PHASE_WAIT_ARENA, PHASE_ACTIVATE_FOURTH, PHASE_WAIT_GRAVEYARD, PHASE_REPEAT
};
struct CQR_EscortTrader_ManagerScript {
    CGameScriptInterface* Game; long OrchardFarmTraderEscortCounter; bool Activated; EscortTraderManagerPhase OrchardFarmTraderEscortPhase;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void Activate() { ++g_activates; g_lastActivate = g_frames; }
    bool WaitEscort(const char* gate)
    {
        if (IsActiveThreadTerminating()) return false;
        while (Game->IsQuestActive("QR_EscortTrader")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return false; }
        if (IsActiveThreadTerminating()) return false;
        while (!Game->IsQuestCompleted(gate)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return false; }
        return !IsActiveThreadTerminating();
    }
    void Main();
};
void CQR_EscortTrader_ManagerScript::Main()
{
    if (!Activated) {
        if (IsActiveThreadTerminating()) return;
        OrchardFarmTraderEscortCounter = 0; OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_FIRST; Activated = true;
    }
    while (!IsActiveThreadTerminating()) {
        EscortTraderManagerPhase p = OrchardFarmTraderEscortPhase;
        if (p == PHASE_ACTIVATE_FIRST || p == PHASE_ACTIVATE_SECOND || p == PHASE_ACTIVATE_THIRD || p == PHASE_ACTIVATE_FOURTH) {
            if (IsActiveThreadTerminating()) return;
            Activate();
            OrchardFarmTraderEscortPhase = (EscortTraderManagerPhase)(p + 1);
        } else if (p == PHASE_WAIT_TRADER_ESCORT) { if (!WaitEscort("Q_TraderEscort")) return; OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_SECOND; }
        else if (p == PHASE_WAIT_BANDIT_CAMP) { if (!WaitEscort("Q_BanditCamp")) return; OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_THIRD; }
        else if (p == PHASE_WAIT_ARENA) { if (!WaitEscort("Q_Arena")) return; Game->ActivateQuest("Q_OrchardFarm_Barricade"); OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_FOURTH; }
        else if (p == PHASE_WAIT_GRAVEYARD) { if (!WaitEscort("Q_OpeningGraveyardSecretPassage")) return; OrchardFarmTraderEscortPhase = PHASE_REPEAT; }
        else if (p == PHASE_REPEAT) {
            if (IsActiveThreadTerminating()) return;
            Activate();
            if (IsActiveThreadTerminating()) return;
            for (;;) {
                while (Game->IsQuestActive("QR_EscortTrader")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                while (Game->IsRegionLoaded("LookoutPoint") || DayModInterval(Game->GetDayOfWeek(), g_pScriptDef) != 0
                       || NotPastStartTime(Game->GetTimeOfDay(), g_pScriptDef)) {
                    Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                Activate();
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
        }
        Game->NewScriptFrame();
    }
}
int main()
{
    bool ok = true;
    g_def.EscortTraderQuestDayInterval = 3; g_def.EscortTraderQuestStartTime = 1;
    CGameScriptInterface game;
    CQR_EscortTrader_ManagerScript s; s.Game = &game; s.Activated = false; s.OrchardFarmTraderEscortCounter = 7; s.OrchardFarmTraderEscortPhase = PHASE_WAIT_ARENA;
    // run until the repeat phase has activated once more: four escorts + the first repeat one + one more
    g_frames = 0; g_endAt = 30; g_escortEndsAfter = 2; g_lastActivate = -100; g_lookoutUntil = 0;
    s.Main();
    ok = ok && s.Activated && s.OrchardFarmTraderEscortCounter == 0 && s.OrchardFarmTraderEscortPhase == PHASE_REPEAT && g_barricade == 1;
    // repeat activations land only on a quest day (day % 3 == 0) after time 1
    ok = ok && g_activates >= 6 && (g_lastActivate / 4) % 3 == 0 && g_lastActivate % 4 > 1;
    int before = g_activates;
    // resumed run (already activated): stays in the repeat phase; the hero is at LookoutPoint for a while
    g_endAt = g_frames + 14; g_lookoutUntil = g_frames + 14;
    s.Main();
    ok = ok && g_activates == before + 1 && g_barricade == 1 && s.OrchardFarmTraderEscortPhase == PHASE_REPEAT;
    // the thread is already ending: nothing happens
    CQR_EscortTrader_ManagerScript t; t.Game = &game; t.Activated = false; t.OrchardFarmTraderEscortPhase = PHASE_REPEAT;
    t.Main();
    ok = ok && !t.Activated && g_activates == before + 1;
    if (!ok) { std::printf("00e24e10_TEST FAIL\n"); return 1; }
    std::printf("00e24e10_TEST PASS\n");
    return 0;
}