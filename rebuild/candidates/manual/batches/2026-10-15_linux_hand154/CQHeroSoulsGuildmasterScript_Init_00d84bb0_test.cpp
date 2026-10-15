// Behaviour test for 0x00d84bb0 (CQ_HeroSoulsGuildmasterScript::Init): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Str { char s[64]; };
struct GSI { int timers[8]; int timerTime[8]; char quest[64]; char region[64]; int regions;
    GSI() : regions(0) { for (int i = 0; i < 8; ++i) { timers[i] = -1; timerTime[i] = -1; } quest[0] = region[0] = 0; }
    void SetTimer(int timer, int time) { timerTime[timer] = time; }
    void AddQuestRegion(const char* q, const char* r) { std::strcpy(quest, q); std::strcpy(region, r); ++regions; } };
static GSI* g_gsi = 0;
struct Timer { int Index; void Set(int time) { g_gsi->SetTimer(Index, time); } };
struct Guild { GSI* Game; bool A, B, C, D, Warn; long Guards, Dist;
    void Init() { B = false; A = false; C = false; D = false; Warn = true; Guards = 0; Dist = 0; Game->AddQuestRegion("Q_HeroSoulsGuildmaster", "GuildWoods"); } };

int main()
{
    GSI gsi; Guild g; g.Game = &gsi; g.A = g.B = g.C = g.D = true; g.Warn = false; g.Guards = 4; g.Dist = 9;
    g.Init();
    CHECK(!g.A && !g.B && !g.C && !g.D && g.Warn && g.Guards == 0 && g.Dist == 0);
    CHECK(std::strcmp(gsi.quest, "Q_HeroSoulsGuildmaster") == 0 && std::strcmp(gsi.region, "GuildWoods") == 0);
    if (g_fail) { std::printf("00d84bb0_TEST FAIL\n"); return 1; }
    std::printf("00d84bb0_TEST PASS\n");
    return 0;
}
