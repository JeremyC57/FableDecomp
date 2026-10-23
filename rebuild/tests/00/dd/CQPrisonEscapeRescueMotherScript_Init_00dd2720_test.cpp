#include <cstdio>
#include <cstring>
// behaviour test: CQ_PrisonEscapeRescueMotherScript::Init -- 11 quest regions, 3 boasts from the def, state reset, guard/multiplier calls
static char g_log[4096];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_regions = 0, g_boastSum = 0;
struct Game {
    void AddQuestRegion(const char* q, const char*) const { if (std::strcmp(q, "Q_PrisonEscapeRescueMother") == 0) ++g_regions; }
    void AddBoast(const char* d, long id, long cost, long reward, bool has, const char* extra, long ev) const { L(d); g_boastSum += id * 1000 + cost * 10 + reward; if (has || extra[0] || ev) L("bad"); }
    void SetGuardsIgnoreCrimes(bool on) const { L(on ? "ignore" : "notice"); }
    void ResetPlayerCreatureCombatMultiplier() const { L("reset"); }
};
struct Def { long NC, NR, DC, DR, WC, WR; };
struct Master { bool Kraken, Tortured, GameDone; };
struct Script {
    Game* G; Master* M; long CurrentTask; bool Flags[8]; long Years, PrisonerConv, Conv; bool Escaped, Alarm, Saved;
    void Init(const Def& d)
    {
        const char* r[11] = { "GraveyardSecretPassage_1", "KrakenChamber", "PrisonSecretPassage", "PrisonSecretPassageExterior", "PrisonTortureChamber", "PrisonCourtyard", "PrisonCells_1", "PrisonCells_2", "PrisonCells_3", "PrisonBarracks", "PrisonOffice" };
        for (int i = 0; i < 11; ++i) G->AddQuestRegion("Q_PrisonEscapeRescueMother", r[i]);
        G->AddBoast("NAKED", 1, d.NC, d.NR, false, "", 0);
        G->AddBoast("NODAMAGE", 3, d.DC, d.DR, false, "", 0);
        G->AddBoast("NOWEAPONS", 6, d.WC, d.WR, false, "", 0);
        for (int i = 0; i < 8; ++i) Flags[i] = false;
        Years = 0; CurrentTask = 0; PrisonerConv = -1; Conv = -1; Escaped = Alarm = Saved = false;
        M->Kraken = M->Tortured = M->GameDone = false;
        G->SetGuardsIgnoreCrimes(true); G->ResetPlayerCreatureCombatMultiplier();
    }
};
int main()
{
    Game g; Master m = { true, true, true }; Script s; std::memset(&s, 1, sizeof(s)); s.G = &g; s.M = &m;
    Def d = { 1, 2, 3, 4, 5, 6 };
    s.Init(d);
    if (g_regions != 11 || std::strcmp(g_log, "NAKED;NODAMAGE;NOWEAPONS;ignore;reset;") != 0 || g_boastSum != (1000 + 12) + (3000 + 34) + (6000 + 56)
        || s.PrisonerConv != -1 || s.Conv != -1 || s.CurrentTask != 0 || s.Years != 0 || s.Flags[7] || s.Saved || m.Kraken || m.GameDone) {
        std::printf("00dd2720_TEST FAIL\n"); return 1;
    }
    std::printf("00dd2720_TEST PASS\n");
    return 0;
}