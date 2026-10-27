#include <cstdio>
// Behaviour model: the base handler always runs; only a live creature user is given the new action.
static int g_base, g_set;
struct Thing { bool dead; };
static void OnActionUse(Thing* user) { ++g_base; if (user && !user->dead) ++g_set; }
int main()
{
    Thing alive = { false }, dead = { true };
    OnActionUse(&alive); OnActionUse(&dead);
    if (g_base != 2 || g_set != 1) { std::printf("007eb570_TEST FAIL\n"); return 1; }
    std::printf("007eb570_TEST PASS\n");
    return 0;
}