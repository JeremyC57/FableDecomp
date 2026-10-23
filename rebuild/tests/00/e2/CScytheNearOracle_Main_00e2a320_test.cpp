// Behaviour test for CScytheNearOracle::Main (0x00E2A320): a frame-driven simulation of the main loop -- the 20-unit
// face-the-hero check gated by the 2-tick timer, the 12-unit oracle cutscene (flags the parent and removes Me), and the
// hit reaction (special ability 14 alone does not count) with its pause/unpause pair.
#include <cstdio>
#include <cstring>

struct Sim {
    int dist[64];              // hero distance per frame
    int hitAt, specialAt, special14At, terminateAt;
    int frame, timer, faces, cutscenes, removes, hitLines, allies, pauses, unpauses;
    bool oracleAwakened;
    bool term() const { return frame >= terminateAt; }
    void newFrame() { ++frame; if (timer > 0) --timer; }
    void Main()
    {
        for (;;) {
            if (term()) return;
            int d = dist[frame < 63 ? frame : 63];
            if (d < 20 && timer == 0) {
                if (term()) return;
                ++faces;
                timer = 2;
            }
            if (d < 12) {
                if (term()) return;
                ++pauses;
                ++cutscenes;
                oracleAwakened = true;
                ++removes;
                ++unpauses;
            }
            bool special = frame == specialAt && frame != special14At;
            if (frame == hitAt || special) {
                if (term()) return;
                ++pauses;
                ++hitLines;
                allies += 2;
                ++unpauses;
            }
            newFrame();
        }
    }
};

int main()
{
    int fail = 0;
    Sim s;
    std::memset(&s, 0, sizeof(s));
    for (int i = 0; i < 64; ++i) s.dist[i] = i < 5 ? 30 : i < 15 ? 15 : 5;
    s.hitAt = 7; s.specialAt = 9; s.special14At = 9; s.terminateAt = 17;
    s.Main();
    // frames 5..14 within 20 units: faces at 5, 7, 9, 11, 13 (timer 2 ticks); within 12 from frame 15: 2 cutscene frames
    if (s.faces != 6 || s.cutscenes != 2 || !s.oracleAwakened || s.hitLines != 1 || s.allies != 2) fail = 1;
    if (s.pauses != s.unpauses || s.removes != 2) fail = 1;

    if (fail) { std::printf("00e2a320_TEST FAIL\n"); return 1; }
    std::printf("00e2a320_TEST PASS\n");
    return 0;
}