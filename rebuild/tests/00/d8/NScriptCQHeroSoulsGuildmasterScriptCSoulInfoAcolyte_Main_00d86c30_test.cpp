// Behaviour test for CSoulInfoAcolyte::Main (0x00D86C30): a frame-driven simulation -- wait for the ready flag, then
// each frame a talk plays the acolyte line (only while Me is alive) inside a pause/unpause pair, and a qualifying hit
// (plain, or any special ability that is not ability 14) plays the hit line and makes Me and the hero allies.
#include <cstdio>
#include <cstring>

struct Sim {
    int readyAt, terminateAt, talkAt, hitAt, specialAt, special14At, speakFrames;
    bool alive;
    int frame, pauses, unpauses, talkLines, hitLines, allies;
    bool term() const { return frame >= terminateAt; }
    void newFrame() { ++frame; }
    bool hitTest() const
    {
        bool hit = frame == hitAt;
        bool special = frame == specialAt;
        bool special14 = frame == special14At;
        return hit || (special && !special14);
    }
    bool speak(int& counter)
    {
        ++pauses;
        if (alive) {
            ++counter;
            for (int i = 0; i < speakFrames; ++i) {
                newFrame();
                if (term()) { ++unpauses; return false; }
            }
            if (term()) { ++unpauses; return false; }
        }
        return true;
    }
    void Main()
    {
        newFrame();
        if (term()) return;
        while (frame < readyAt) { newFrame(); if (term()) return; }
        if (term()) return;
        while (!term()) {
            if (frame == talkAt) {
                if (term()) return;
                if (!speak(talkLines)) return;
                ++unpauses;
            }
            if (hitTest()) {
                if (term()) return;
                if (!speak(hitLines)) return;
                allies += 2;
                ++unpauses;
            }
            newFrame();
        }
    }
};

static Sim make()
{
    Sim s;
    std::memset(&s, 0, sizeof(s));
    s.readyAt = 3; s.terminateAt = 40; s.talkAt = 5; s.hitAt = -1; s.specialAt = -1; s.special14At = -1;
    s.speakFrames = 2; s.alive = true;
    return s;
}

int main()
{
    int fail = 0;
    Sim a = make();
    a.hitAt = 12;
    a.Main();
    if (a.talkLines != 1 || a.hitLines != 1 || a.allies != 2 || a.pauses != 2 || a.unpauses != 2) fail = 1;

    Sim b = make();
    b.specialAt = 10; b.special14At = 10;   // ability 14 does not count
    b.alive = false;                        // dead: paused/unpaused but silent
    b.Main();
    if (b.talkLines != 0 || b.hitLines != 0 || b.allies != 0 || b.pauses != 1 || b.unpauses != 1) fail = 1;

    Sim c = make();
    c.terminateAt = 6;                      // terminated while speaking: unpause once and stop
    c.Main();
    if (c.talkLines != 1 || c.pauses != 1 || c.unpauses != 1) fail = 1;

    if (fail) { std::printf("00d86c30_TEST FAIL\n"); return 1; }
    std::printf("00d86c30_TEST PASS\n");
    return 0;
}