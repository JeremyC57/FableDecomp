// Behaviour test for 0x00dfb100: CTraderToRescue::OnPredicateFail: the quest is failed with the traders-died text only when the trader was killed
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

static int g_failed = 0; static const char* g_msg = 0;
struct Thing { bool killed; bool MsgIsKilledBy(const char* by) const { return killed && by[0] == 0; } };
struct Trader { Thing Me; void OnPredicateFail() { bool killed; { const char* anyone = ""; killed = Me.MsgIsKilledBy(anyone); } if (killed) { ++g_failed; g_msg = "TEXT_QST_B11_QUEST_FAILED_TRADERS_DIED"; } } };
static void run() { Trader t; t.Me.killed = false; t.OnPredicateFail(); CHECK(g_failed == 0); t.Me.killed = true; t.OnPredicateFail(); CHECK(g_failed == 1 && !std::strcmp(g_msg, "TEXT_QST_B11_QUEST_FAILED_TRADERS_DIED")); }

int main()
{
    run();
    if (g_fail) { std::printf("00dfb100_TEST FAIL\n"); return 1; }
    std::printf("00dfb100_TEST PASS\n");
    return 0;
}
