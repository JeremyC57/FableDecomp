// Behaviour test for 0x00e0b020 (CQ_UndeadRisingScript::Init): the reconstructed logic run over a host model.
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
struct Undead { GSI* Game; unsigned long tokens, kills; bool a, b, c, d; bool Lights[8]; long status;
    void Init() { Game->AddQuestRegion("Q_UndeadRising", "Graveyard"); tokens = 0; kills = 0; status = 0; a = b = c = d = false; for (int i = 0; i < 8; ++i) Lights[i] = false; } };

int main()
{
    GSI gsi; Undead u; u.Game = &gsi; u.tokens = 3; u.kills = 2; u.status = 5; u.a = u.b = u.c = u.d = true; for (int i = 0; i < 8; ++i) u.Lights[i] = true;
    u.Init();
    CHECK(u.tokens == 0 && u.kills == 0 && u.status == 0 && !u.a && !u.b && !u.c && !u.d); for (int i = 0; i < 8; ++i) CHECK(!u.Lights[i]);
    CHECK(std::strcmp(gsi.quest, "Q_UndeadRising") == 0 && std::strcmp(gsi.region, "Graveyard") == 0);
    if (g_fail) { std::printf("00e0b020_TEST FAIL\n"); return 1; }
    std::printf("00e0b020_TEST PASS\n");
    return 0;
}