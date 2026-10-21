#include <cstdio>
// Behaviour copy of CAIStateGroupBehaviourBase::UpdateEmote (0x00957A20).
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
struct Group {
    float TriggerTime, RandomWaitTime, ThisSpeechDuration; unsigned long seed; float nowSeconds; float speech; float roll; int emotes;
    unsigned long Random(unsigned long range) { unsigned long next = seed * 0x24A1U + 0x24DFU; seed = next; seed = ror13(next); return seed % range; }
    bool UpdateEmote() {
        float now = nowSeconds;
        if (now - TriggerTime > RandomWaitTime + ThisSpeechDuration) {
            ThisSpeechDuration = speech; TriggerTime = now;
            RandomWaitTime = (float)Random(100) * 0.03f + 2.0f;
            if (roll < 0.25f) ++emotes;
        }
        return true;
    }
};
int main() {
    Group g = { 0.0f, 1.0f, 1.0f, 777, 3.0f, 0.5f, 0.1f, 0 };
    g.UpdateEmote();   // 3 - 0 > 2: fire
    if (g.TriggerTime != 3.0f || g.ThisSpeechDuration != 0.5f || g.emotes != 1 || g.RandomWaitTime < 2.0f || g.RandomWaitTime > 4.98f) { std::printf("00957a20_TEST FAIL\n"); return 1; }
    float wait = g.RandomWaitTime; g.nowSeconds = 3.1f; g.UpdateEmote();   // too soon: unchanged
    if (g.RandomWaitTime != wait || g.TriggerTime != 3.0f || g.emotes != 1) { std::printf("00957a20_TEST FAIL\n"); return 1; }
    g.nowSeconds = 100.0f; g.roll = 0.9f; g.UpdateEmote();
    if (g.TriggerTime != 100.0f || g.emotes != 1) { std::printf("00957a20_TEST FAIL\n"); return 1; }
    std::printf("00957a20_TEST PASS\n"); return 0;
}