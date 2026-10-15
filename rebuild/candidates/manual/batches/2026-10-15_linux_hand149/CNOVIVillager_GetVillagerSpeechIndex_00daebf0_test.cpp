#include <cstdio>
static int g_seq[8]; static int g_pos = 0;
static int fake_rand() { return g_seq[g_pos++]; }
struct Game { int updates; void Update() { ++updates; } };
struct Quest { long lastVillagerSpeechIdx; };
struct CNOVI_Villager {
    Game* G; Quest* ParentClass; int doneAfter; int doneCalls;
    bool IsDone() { return ++doneCalls > doneAfter; }
    long GetVillagerSpeechIndex(long count)
    {
        while (!IsDone()) {
            long index = fake_rand() % count;
            if (index != ParentClass->lastVillagerSpeechIdx) {
                if (IsDone()) return 0;
                ParentClass->lastVillagerSpeechIdx = index;
                return IsDone() ? 0 : index;
            }
            G->Update();
        }
        return 0;
    }
};
int main()
{
    Game g; g.updates = 0; Quest q; q.lastVillagerSpeechIdx = 2;
    g_seq[0] = 7; g_seq[1] = 12; g_seq[2] = 9;      // 7%5=2 (repeat), 12%5=2 (repeat), 9%5=4
    CNOVI_Villager v; v.G = &g; v.ParentClass = &q; v.doneAfter = 100; v.doneCalls = 0;
    bool fail = false;
    if (v.GetVillagerSpeechIndex(5) != 4 || q.lastVillagerSpeechIdx != 4 || g.updates != 2) fail = true;
    v.doneAfter = 0; v.doneCalls = 0; g_pos = 0;
    if (v.GetVillagerSpeechIndex(5) != 0 || q.lastVillagerSpeechIdx != 4) fail = true;
    v.doneAfter = 2; v.doneCalls = 0; g_pos = 2; q.lastVillagerSpeechIdx = 0;   // done at the final test
    if (v.GetVillagerSpeechIndex(5) != 0 || q.lastVillagerSpeechIdx != 4) fail = true;
    if (fail) { std::printf("00daebf0_TEST FAIL\n"); return 1; }
    std::printf("00daebf0_TEST PASS\n");
    return 0;
}
