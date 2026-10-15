// Behaviour test for 0x00449cb0: CPlayerManager::SwapToHero: main player swap with fallback and old hero killed.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Thing { int kills; bool flag; void Kill(bool f) { ++kills; flag = f; } };
struct Player { long Number; bool swapOk; Thing* hero; bool Switch(Player*) { return swapOk; } Thing* GetHero() { return hero; } };
struct PM { Player* players[3]; long MainPlayer;
  bool IsPlayer(long n) { return n >= 0 && n < 3; } Player* Get(long n) { return players[n]; }
  void Swap(long n) { if (IsPlayer(n)) { Player* np = Get(n); Player* op = Get(MainPlayer); MainPlayer = np->Number; if (!np->Switch(op)) MainPlayer = op->Number; if (op->GetHero()) op->GetHero()->Kill(true); } } };
static int run() { Thing h = {0, false}; Player a = {0, true, &h}, b = {1, true, 0}, c = {2, false, 0}; PM pm = {{&a, &b, &c}, 0};
  pm.Swap(1); CHECK(pm.MainPlayer == 1 && h.kills == 1 && h.flag);
  pm.Swap(2); CHECK(pm.MainPlayer == 1); pm.Swap(5); CHECK(pm.MainPlayer == 1); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00449cb0_TEST FAIL\n"); return 1; }
    std::printf("00449cb0_TEST PASS\n");
    return 0;
}