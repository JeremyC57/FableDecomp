// Behaviour test for CPlayer::GetPNearestTCVillage const (0x004886F0): a model of the row.
#include <stdio.h>

struct Thing { bool dead; bool village; float x; int tc; };
static int Nearest(const Thing* player, const Thing* things, int n)
{
    if (!player || player->dead) return 0;
    const Thing* best = 0; float bestD = 1e30f;
    for (int i = 0; i < n; ++i) {
        if (!things[i].village) continue;
        float d = things[i].x - player->x; if (d < 0) d = -d;
        if (d < bestD) { bestD = d; best = &things[i]; }
    }
    return best ? best->tc : 0;
}
static int run()
{
    Thing player = { false, false, 10.0f, 0 }, dead = { true, false, 0.0f, 0 };
    Thing things[3] = { { false, true, 0.0f, 1 }, { false, false, 9.0f, 9 }, { false, true, 12.0f, 2 } };
    return Nearest(&player, things, 3) == 2 && Nearest(&dead, things, 3) == 0 && Nearest(0, things, 3) == 0
        && Nearest(&player, things + 1, 1) == 0;
}
int main()
{
    int ok = run();
    printf(ok ? "004886f0_TEST PASS\n" : "004886f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
