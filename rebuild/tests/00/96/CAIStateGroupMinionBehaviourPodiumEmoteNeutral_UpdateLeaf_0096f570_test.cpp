#include <cstdio>
// Behaviour copy of CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::UpdateLeaf (0x0096F570).
struct Sim { float speech, pause, last; unsigned long seed; int emotes; float talkDuration; };
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Random(Sim& s, unsigned long range) { s.seed = s.seed * 0x24A1U + 0x24DFU; s.seed = Ror13(s.seed); return s.seed % range; }
static void UpdateLeaf(Sim& s, long frame, long fps) {
    float now = (float)frame / (float)fps;
    if (now - s.last > s.speech + s.pause) {
        s.speech = s.talkDuration;
        s.last = now;
        s.pause = (float)Random(s, 100) * 0.03f + 2.0f;
        if ((float)Random(s, 100) * 0.01f < 0.1f) ++s.emotes;
    }
}
int main() {
    Sim s = { 1.0f, 2.0f, 0.0f, 12345, 0, 1.5f };
    UpdateLeaf(s, 30, 15);    // now 2 s: not past 3 s
    if (s.last != 0.0f) { std::printf("0096f570_TEST FAIL\n"); return 1; }
    UpdateLeaf(s, 60, 15);    // now 4 s
    if (s.last != 4.0f || s.speech != 1.5f || s.pause < 2.0f || s.pause > 5.0f) { std::printf("0096f570_TEST FAIL\n"); return 1; }
    int calls = 0;
    for (long f = 60; f < 60000; f += 15) { float before = s.last; UpdateLeaf(s, f, 15); if (s.last != before) ++calls; }
    if (calls < 100 || s.emotes == 0 || s.emotes > calls / 2) { std::printf("0096f570_TEST FAIL\n"); return 1; }
    std::printf("0096f570_TEST PASS\n"); return 0;
}