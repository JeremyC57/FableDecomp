// Behaviour test for CAT_ConvoyMember::Init (0x00F08C30): a non-trader becomes mutually hostile with the hero and every living bandit; the trader gets a green mini-map marker; both register WatchForEnemyDeath; a done script returns early.
#include <cstdio>
#include <cstring>
struct CWorld {
    int enemyPairs[16][2]; int enemyCount; char marker[32]; int markerThing; int adds; bool done;
    void Enemy(int a, int b) { enemyPairs[enemyCount][0] = a; enemyPairs[enemyCount][1] = b; ++enemyCount; }
};
struct CConvoyMember {
    CWorld* W; int Thing; const char* DefName; int Bandits[4]; bool BanditAlive[4]; int BanditCount;
    enum { HERO = 100 };
    bool IsDone() const { return W->done; }
    void Init()
    {
        if (std::strcmp(DefName, "CREATURE_TRADER_01") != 0) {
            if (IsDone()) return;
            W->Enemy(Thing, HERO);
            W->Enemy(HERO, Thing);
            for (int i = 0; i < BanditCount; ++i) {
                if (BanditAlive[i]) {
                    if (IsDone()) return;
                    W->Enemy(Thing, Bandits[i]);
                    W->Enemy(Bandits[i], Thing);
                }
            }
        } else {
            if (IsDone()) return;
            std::strcpy(W->marker, "HUD_ORB_GREEN_SMALL");
            W->markerThing = Thing;
        }
        ++W->adds;
    }
};
int main()
{
    CWorld w; std::memset(&w, 0, sizeof(w));
    CConvoyMember m; m.W = &w; m.Thing = 7; m.DefName = "CREATURE_GUARD"; m.BanditCount = 3;
    m.Bandits[0] = 1; m.Bandits[1] = 2; m.Bandits[2] = 3; m.BanditAlive[0] = true; m.BanditAlive[1] = false; m.BanditAlive[2] = true;
    m.Init();
    bool ok = w.enemyCount == 6 && w.enemyPairs[0][0] == 7 && w.enemyPairs[1][0] == 100 && w.enemyPairs[2][1] == 1
        && w.enemyPairs[4][1] == 3 && w.adds == 1 && w.markerThing == 0;
    CWorld w2; std::memset(&w2, 0, sizeof(w2));
    m.W = &w2; m.DefName = "CREATURE_TRADER_01";
    m.Init();
    ok = ok && w2.enemyCount == 0 && w2.markerThing == 7 && std::strcmp(w2.marker, "HUD_ORB_GREEN_SMALL") == 0 && w2.adds == 1;
    CWorld w3; std::memset(&w3, 0, sizeof(w3)); w3.done = true;
    m.W = &w3; m.DefName = "CREATURE_GUARD";
    m.Init();
    ok = ok && w3.enemyCount == 0 && w3.adds == 0;
    if (!ok) { std::printf("00f08c30_TEST FAIL\n"); return 1; }
    std::printf("00f08c30_TEST PASS\n");
    return 0;
}