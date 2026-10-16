#include <cstdio>
// Behaviour model of CV_TalentlessBardScript::ProcessSongsQuestComplete: every available quest's tag is appended to
// SongItems; a terminating thread stops the walk.
struct Bard {
    int tags[12];
    bool avail[12];
    int items[12];
    int count;
    int terminateAfter;    // number of terminate checks that answer false
    int checks;
    bool IsActiveThreadTerminating() { return ++checks > terminateAfter; }
    void ProcessSongsQuestComplete()
    {
        for (long i = 0; i < 12; ++i) {
            if (IsActiveThreadTerminating()) return;
            if (avail[i]) {
                if (IsActiveThreadTerminating()) return;
                int tag = tags[i];
                items[count++] = tag;
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
};
int main()
{
    Bard b;
    for (int i = 0; i < 12; ++i) { b.tags[i] = 100 + i; b.avail[i] = (i % 3) == 0; }
    b.count = 0; b.checks = 0; b.terminateAfter = 1000;
    b.ProcessSongsQuestComplete();
    if (b.count != 4 || b.items[0] != 100 || b.items[3] != 109 || b.checks != 12 + 4 + 1) { std::printf("00ed9b40_TEST FAIL\n"); return 1; }
    b.count = 0; b.checks = 0; b.terminateAfter = 2;   // stops at the first available entry's inner check... after 2 checks
    b.ProcessSongsQuestComplete();
    if (b.count != 1) { std::printf("00ed9b40_TEST FAIL\n"); return 1; }
    std::printf("00ed9b40_TEST PASS\n");
    return 0;
}
