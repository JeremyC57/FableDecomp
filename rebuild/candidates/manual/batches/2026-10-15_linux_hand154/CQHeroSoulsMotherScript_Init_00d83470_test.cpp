// Behaviour test for 0x00d83470 (CQ_HeroSoulsMotherScript::Init): the reconstructed logic run over a host model.
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
struct Mother { GSI* Game; bool PlayedMotherCS; long ScreamersAlive; float BarToFill; long BarID; bool QuestStartScreened, GotSuperScreamer;
    void Init() { PlayedMotherCS = false; BarToFill = 0; BarID = 0; ScreamersAlive = 0; QuestStartScreened = false; GotSuperScreamer = false; Game->AddQuestRegion("Q_HeroSoulsMother", "OakvaleMemorialGarden"); } };

int main()
{
    GSI gsi; Mother m; m.Game = &gsi; m.PlayedMotherCS = true; m.ScreamersAlive = 4; m.BarToFill = 2.5f; m.BarID = 3; m.QuestStartScreened = true; m.GotSuperScreamer = true;
    m.Init();
    CHECK(!m.PlayedMotherCS && m.ScreamersAlive == 0 && m.BarToFill == 0.0f && m.BarID == 0 && !m.QuestStartScreened && !m.GotSuperScreamer);
    CHECK(std::strcmp(gsi.quest, "Q_HeroSoulsMother") == 0 && std::strcmp(gsi.region, "OakvaleMemorialGarden") == 0);
    if (g_fail) { std::printf("00d83470_TEST FAIL\n"); return 1; }
    std::printf("00d83470_TEST PASS\n");
    return 0;
}
