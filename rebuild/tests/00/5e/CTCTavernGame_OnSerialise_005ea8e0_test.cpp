// Behaviour test for 0x005ea8e0: CTCTavernGame::OnSerialise: three Transfer calls with defaults 0 / 0 / false.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Ctx { int n; const char* names[3]; float fd; long ld; bool bd;
  void Transfer(const char* k, float&, const float& d) { names[n++] = k; fd = d; }
  void Transfer(const char* k, long&, const long& d) { names[n++] = k; ld = d; }
  void Transfer(const char* k, bool&, const bool& d) { names[n++] = k; bd = d; } };
struct Game { float TargetTime; long TargetRound; bool HasPrizeBeenGiven;
  void OnSerialise(Ctx& c, bool) { float d = 0.0f; c.Transfer("TargetTime", TargetTime, d); c.Transfer("TargetRound", TargetRound, 0L); c.Transfer("HasPrizeBeenGiven", HasPrizeBeenGiven, false); } };
static int run() { Ctx c; c.n = 0; c.fd = 1; c.ld = 1; c.bd = true; Game g = {1,2,true}; g.OnSerialise(c, false);
  CHECK(c.n == 3 && c.fd == 0.0f && c.ld == 0 && !c.bd && c.names[1][6] == 'R'); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("005ea8e0_TEST FAIL\n"); return 1; }
    std::printf("005ea8e0_TEST PASS\n");
    return 0;
}