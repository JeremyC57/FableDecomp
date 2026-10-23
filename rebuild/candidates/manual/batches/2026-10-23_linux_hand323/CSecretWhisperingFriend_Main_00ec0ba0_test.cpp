// Behaviour test for CSecretWhisperingFriend::Main (0x00EC0BA0): a frame-driven simulation of the stage machine --
// stage 5 gives Me the hero as a target and waits; stage 1 walks Me home (re-issuing the move only when no task runs),
// then faces the look-at marker and waits while the stage stays 1.
#include <cstdio>
#include <cstring>

struct Sim {
    int stageAt[64];          // quest stage per frame
    int terminateAt;
    bool questActive;
    int distFrames;           // Me is away from home until this frame
    // outputs
    int frame, removes, targets, moves, faces, clears;
    int stage() const { return stageAt[frame < 63 ? frame : 63]; }
    bool term() const { return frame >= terminateAt; }
    void newFrame() { ++frame; }
    bool away() const { return frame < distFrames; }
    bool performing() const { return (frame & 1) != 0; }
    void Main()
    {
        newFrame();
        if (term()) return;
        if (questActive) { if (term()) return; ++removes; }
        if (term()) return;
        do {
            if (stage() == 5) {
                if (term()) return;
                ++targets;
                while (stage() == 5) { newFrame(); if (term()) return; }
                if (term()) return;
            }
            if (term()) return;
            while (stage() == 1) {
                newFrame();
                if (term()) return;
                while (away() && stage() == 1) {
                    newFrame();
                    if (term()) return;
                    if (!performing()) { if (term()) return; ++moves; }
                }
                if (term()) return;
                if (stage() != 5) {
                    if (term()) return;
                    ++clears; ++faces;
                    while (stage() == 1) { newFrame(); if (term()) return; }
                    if (term()) return;
                }
            }
            if (term()) return;
            newFrame();
        } while (!term());
    }
};

int main()
{
    int fail = 0;
    Sim s;
    std::memset(&s, 0, sizeof(s));
    for (int i = 0; i < 64; ++i) s.stageAt[i] = (i >= 3 && i < 6) ? 5 : (i >= 8 && i < 20) ? 1 : 0;
    s.terminateAt = 30; s.distFrames = 14; s.questActive = true;
    s.Main();
    if (s.removes != 1 || s.targets != 1 || s.faces != 1 || s.clears != 1 || s.moves == 0 || s.frame != 30) fail = 1;

    Sim t;
    std::memset(&t, 0, sizeof(t));
    t.terminateAt = 1;
    t.Main();
    if (t.removes != 0 || t.targets != 0 || t.frame != 1) fail = 1;

    if (fail) { std::printf("00ec0ba0_TEST FAIL\n"); return 1; }
    std::printf("00ec0ba0_TEST PASS\n");
    return 0;
}
