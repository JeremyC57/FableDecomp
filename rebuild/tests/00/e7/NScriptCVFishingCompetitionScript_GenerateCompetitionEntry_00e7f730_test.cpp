// Behaviour test for 0x00e7f730: CV_FishingCompetitionScript::GenerateCompetitionEntry: an entry under the threshold becomes the best catch and names a fisher
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

static int g_rand[3]; static int g_ri = 0; static int r() { return g_rand[g_ri++]; }
struct Fish { float best; float threshold; int winner;
  void Gen() { if (r() % 4 <= 0) { float w = (r() % 9 + 1) * 0.1f + best; if (threshold > w) { best = w; winner = r() % 3; } } } };
static void run() { Fish f; f.best = 1.0f; f.threshold = 2.0f; f.winner = -1; g_rand[0] = 8; g_rand[1] = 4; g_rand[2] = 5; f.Gen(); CHECK(f.best > 1.49f && f.best < 1.51f && f.winner == 2);
  g_ri = 0; g_rand[0] = 1; f.Gen(); CHECK(f.winner == 2 && f.best < 1.51f); g_ri = 0; g_rand[0] = 0; g_rand[1] = 8; f.threshold = 1.6f; f.Gen(); CHECK(f.best < 1.51f); }

int main()
{
    run();
    if (g_fail) { std::printf("00e7f730_TEST FAIL\n"); return 1; }
    std::printf("00e7f730_TEST PASS\n");
    return 0;
}