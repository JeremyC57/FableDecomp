// Behaviour test for CQ_DragonBossFightScript::SpawnEnemies (0x00D25C20): minions always enabled; every N-th fly-by
// enables summoners and restarts the count, unless the thread is terminating.
#include <cstdio>
struct CScript {
    long NumFlyBysBetweenSummonerSpawns; long NumFlyBysSinceLastSummonerSpawn; bool MinionSpawningEnabled; bool SummonerSpawningEnabled; bool terminating;
    bool IsActiveThreadTerminating() { return terminating; }
    void SpawnEnemies()
    {
        MinionSpawningEnabled = true;
        if (++NumFlyBysSinceLastSummonerSpawn >= NumFlyBysBetweenSummonerSpawns) {
            if (!IsActiveThreadTerminating()) {
                SummonerSpawningEnabled = true;
                NumFlyBysSinceLastSummonerSpawn = 0;
            }
        }
    }
};
int main()
{
    CScript s; s.NumFlyBysBetweenSummonerSpawns = 3; s.NumFlyBysSinceLastSummonerSpawn = 0; s.MinionSpawningEnabled = false; s.SummonerSpawningEnabled = false; s.terminating = false;
    s.SpawnEnemies();
    bool ok = s.MinionSpawningEnabled && !s.SummonerSpawningEnabled && s.NumFlyBysSinceLastSummonerSpawn == 1;
    s.SpawnEnemies(); s.SpawnEnemies();
    ok = ok && s.SummonerSpawningEnabled && s.NumFlyBysSinceLastSummonerSpawn == 0;
    s.SummonerSpawningEnabled = false; s.NumFlyBysSinceLastSummonerSpawn = 2; s.terminating = true;
    s.SpawnEnemies();
    ok = ok && !s.SummonerSpawningEnabled && s.NumFlyBysSinceLastSummonerSpawn == 3;
    if (!ok) { std::printf("00d25c20_TEST FAIL\n"); return 1; }
    std::printf("00d25c20_TEST PASS\n");
    return 0;
}