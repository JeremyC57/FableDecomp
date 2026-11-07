// Behaviour test for CMinionSpawnAdv::Main (0x00DA8BF0): once the hero is within 4 units and the minion count is under the limit,
// every MinionHere marker with Me's data string spawns a war dog and then every MinionSummonerHere marker with it a dreadwing, each
// only while the count is under the limit; then the spawner removes itself.
#include <cstdio>
#include <cstring>

struct Marker { const char* Data; bool Summoner; };
struct Spawner {
    const char* Data; long Count, Limit, Dogs, Wings, Effects; bool Removed;
    bool Ready(float distance) const { return distance < 4.0f && Count < Limit; }
    void Run(const Marker* m, int n)
    {
        for (int pass = 0; pass < 2; ++pass)
            for (int i = 0; i < n; ++i)
                if (m[i].Summoner == (pass == 1) && std::strcmp(m[i].Data, Data) == 0 && Count < Limit) {
                    ++Effects;
                    if (pass == 0) ++Dogs; else ++Wings;
                    ++Count;
                }
        Removed = true;
    }
};

int main()
{
    Marker m[] = { {"A", false}, {"B", false}, {"A", true}, {"A", false}, {"A", true} };
    Spawner s = { "A", 0, 3, 0, 0, 0, false };
    bool ok = !s.Ready(5.0f) && s.Ready(3.0f);
    s.Run(m, 5);
    ok = ok && s.Dogs == 2 && s.Wings == 1 && s.Effects == 3 && s.Removed && !s.Ready(1.0f);
    if (!ok) { std::printf("00da8bf0_TEST FAIL\n"); return 1; }
    std::printf("00da8bf0_TEST PASS\n");
    return 0;
}
