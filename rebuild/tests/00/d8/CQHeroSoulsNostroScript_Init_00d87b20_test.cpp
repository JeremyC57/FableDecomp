// Behaviour test for 0x00d87b20 (CQ_HeroSoulsNostroScript::Init): the reconstructed logic run over a host model.
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
struct Nostro { GSI* Game; long SummonerCount; bool PlayedNostroCS, KickedOff; long UndeadAlive; Timer NostroVulnerableTimer;
    void Init() { SummonerCount = 0; PlayedNostroCS = false; KickedOff = false; UndeadAlive = 0; NostroVulnerableTimer.Set(0); Game->AddQuestRegion("Q_HeroSoulsNostro", "GraveyardCircle"); } };

int main()
{
    GSI gsi; g_gsi = &gsi; Nostro n; n.Game = &gsi; n.SummonerCount = 5; n.PlayedNostroCS = true; n.KickedOff = true; n.UndeadAlive = 3; n.NostroVulnerableTimer.Index = 2; gsi.timerTime[2] = 9;
    n.Init();
    CHECK(n.SummonerCount == 0 && !n.PlayedNostroCS && !n.KickedOff && n.UndeadAlive == 0); CHECK(gsi.timerTime[2] == 0);
    CHECK(std::strcmp(gsi.quest, "Q_HeroSoulsNostro") == 0 && std::strcmp(gsi.region, "GraveyardCircle") == 0 && gsi.regions == 1);
    if (g_fail) { std::printf("00d87b20_TEST FAIL\n"); return 1; }
    std::printf("00d87b20_TEST PASS\n");
    return 0;
}