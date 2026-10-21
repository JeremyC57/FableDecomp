// Behaviour test for 0x006C17B0 CTCBuyableHouse::SetOwnedByPlayer: with a living hero the ownership flag, frame stamp,
// persisted state and hero-stats house positions change (sale may evict); the sign post and GUI refresh always run.
#include <cstdio>

static long g_frame = 77; static int g_refresh = 0, g_evict = 0, g_add = 0, g_remove = 0, g_kill = 0, g_create = 0;
struct CThing { bool Dead; bool persisted; void Kill(bool) { ++g_kill; } };
struct CTCHeroStats { void AddHousePosition(long) { ++g_add; } void RemoveHousePosition(long) { ++g_remove; } };
struct CBuyableHouseDef { bool EvictOnSale; };
struct CTCBuyableHouse {
    CThing* Thing; CBuyableHouseDef* PDef; bool OwnedByPlayer; CThing* MainSignPost; long Frame; bool needsPost;
    void Evict() { ++g_evict; }
    void SetOwnedByPlayer(CThing* hero, CTCHeroStats* stats, bool owned)
    {
        if (hero && !hero->Dead) {
            if (owned) {
                OwnedByPlayer = true; Frame = g_frame; Thing->persisted = true; stats->AddHousePosition(1);
            } else {
                OwnedByPlayer = false; Frame = g_frame; stats->RemoveHousePosition(1);
                if (PDef->EvictOnSale) Evict();
            }
        }
        if (MainSignPost && !MainSignPost->Dead) MainSignPost->Kill(true);
        if (needsPost) ++g_create;
        ++g_refresh;
    }
};
int main()
{
    CThing hero = { false, false }, thing = { false, false }, post = { false, false };
    CTCHeroStats stats; CBuyableHouseDef def = { true };
    CTCBuyableHouse h = { &thing, &def, false, &post, 0, true };
    h.SetOwnedByPlayer(&hero, &stats, true);
    bool ok = h.OwnedByPlayer && h.Frame == 77 && thing.persisted && g_add == 1 && g_kill == 1 && g_create == 1 && g_refresh == 1;
    h.SetOwnedByPlayer(&hero, &stats, false);
    ok = ok && !h.OwnedByPlayer && g_remove == 1 && g_evict == 1 && g_refresh == 2;
    hero.Dead = true; h.MainSignPost = 0; h.OwnedByPlayer = true;
    h.SetOwnedByPlayer(&hero, &stats, false);
    ok = ok && h.OwnedByPlayer && g_remove == 1 && g_kill == 2 && g_refresh == 3;
    if (!ok) { std::printf("006c17b0_TEST FAIL\n"); return 1; }
    std::printf("006c17b0_TEST PASS\n");
    return 0;
}
