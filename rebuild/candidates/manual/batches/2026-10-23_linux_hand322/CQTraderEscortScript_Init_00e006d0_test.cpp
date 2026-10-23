#include <cstdio>
#include <cstring>
// behaviour test: CQ_TraderEscortScript::Init -- 10 regions, 5 generators off, state reset, def times, master flags, 3 boasts
static int g_regions = 0, g_generatorsOff = 0, g_boasts = 0, g_boastSum = 0;
struct Game {
    void AddQuestRegion(const char* q, const char*) const { if (std::strcmp(q, "Q_TraderEscort") == 0) ++g_regions; }
    void SetCreatureGeneratorsEnabled(const char*, bool on) const { if (!on) ++g_generatorsOff; }
    void AddBoast(const char*, long id, long cost, long reward, bool, const char*, long) const { ++g_boasts; g_boastSum += id * 100 + cost * 10 + reward; }
};
struct Def { long NC, NR, DC, DR, Low, Med, High, AC, AR; };
struct Master { bool AllAlive; long Pickpocketed; };
struct Script {
    Game* G; Master* M; long Balverines, Alive; bool Flags[7]; long Low, Med, High, Pick; bool Flags2[6]; long Banter; const char* Talk;
    void Init(const Def& d)
    {
        const char* r[10] = { "Darkwood1", "Darkwood2", "DemonDoor_DarkwoodSporeSwamp", "Darkwood3", "Darkwood4", "DarkwoodChapel", "Darkwood5", "Darkwood6", "BarrowFields", "DemonDoor_Barrowfields" };
        for (int i = 0; i < 10; ++i) G->AddQuestRegion("Q_TraderEscort", r[i]);
        const char* g[5] = { "Darkwood1", "Darkwood2", "Darkwood3", "Darkwood5", "Darkwood6" };
        for (int i = 0; i < 5; ++i) G->SetCreatureGeneratorsEnabled(g[i], false);
        Pick = 0; Balverines = 0; Alive = 0; for (int i = 0; i < 7; ++i) Flags[i] = false; for (int i = 0; i < 6; ++i) Flags2[i] = false; Banter = 0;
        Low = d.Low; Med = d.Med; High = d.High; Talk = "";
        M->AllAlive = true; M->Pickpocketed = 1;
        G->AddBoast("NAKED", 1, d.NC, d.NR, false, "", 0);
        G->AddBoast("NODAMAGE", 3, d.DC, d.DR, false, "", 0);
        G->AddBoast("ALLTRADERSALIVE", 9, d.AC, d.AR, false, "", 0);
    }
};
int main()
{
    Game g; Master m = { false, 0 }; Script s; std::memset(&s, 1, sizeof(s)); s.G = &g; s.M = &m;
    Def d = { 1, 2, 3, 4, 50, 60, 70, 5, 6 };
    s.Init(d);
    if (g_regions != 10 || g_generatorsOff != 5 || g_boasts != 3 || g_boastSum != (100 + 12) + (300 + 34) + (900 + 56)
        || s.Low != 50 || s.Med != 60 || s.High != 70 || s.Pick != 0 || s.Banter != 0 || s.Flags[6] || s.Flags2[5] || !m.AllAlive || m.Pickpocketed != 1) {
        std::printf("00e006d0_TEST FAIL\n"); return 1;
    }
    std::printf("00e006d0_TEST PASS\n");
    return 0;
}
