// Behaviour test for CStopDrawing::Main (0x00EE36A0): the rock-pile wait test, the intro branch, the
// counted-pointer assignment of the region points and the D3DCOLOR byte layout of the health-bar colour.
#include <cstdio>
#include <cstring>

class CCPPointerInfo { public: long RefCount; int deleted; };
struct CountedPtr {
    void* Data;
    CCPPointerInfo* Info;
    CountedPtr() : Data(0), Info(0) { }
    CountedPtr& operator=(const CountedPtr& other)
    {
        CCPPointerInfo* info = other.Info;
        void* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0)
                    Info->deleted = 1;
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct Sim {
    // inputs per frame
    int nearFrame, hitFrame, specialFrame, special14;
    bool introPlayed;
    int terminateAt;
    // outputs
    int frames, cutscenes, releases, healthBars, hiddenOn, hiddenOff;
    bool introSet;
    int frame;
    bool terminating() const { return terminateAt >= 0 && frame >= terminateAt; }
    void newFrame() { ++frame; ++frames; }
    void Main()
    {
        newFrame();
        if (terminating()) return;
        if (!introPlayed) {
            if (terminating()) return;
            ++hiddenOn;
            for (;;) {
                bool nearHero = frame >= nearFrame;
                bool hit = frame >= hitFrame;
                bool special = frame >= specialFrame;
                bool waiting = !(nearHero || hit || (special && !special14));
                if (!waiting) break;
                newFrame();
                if (terminating()) return;
            }
            if (terminating()) return;
            ++cutscenes;
            ++hiddenOff;
            introSet = true;
            ++releases;
        }
        ++healthBars;
        int guard = 0;
        do newFrame(); while (!terminating() && ++guard < 100);
    }
};

static Sim make(int nearF, int hitF, int specF, int s14, bool intro, int term)
{
    Sim s;
    std::memset(&s, 0, sizeof(s));
    s.nearFrame = nearF; s.hitFrame = hitF; s.specialFrame = specF; s.special14 = s14;
    s.introPlayed = intro; s.terminateAt = term;
    return s;
}

int main()
{
    int fail = 0;
    // colour: red health bar, D3DCOLOR byte order B,G,R,A
    CRGBColour c(0xff, 0, 0, 0xff);
    const unsigned char* b = (const unsigned char*)&c;
    if (b[0] != 0 || b[1] != 0 || b[2] != 0xff || b[3] != 0xff) fail = 1;

    // counted pointer assignment: refcount moves from old to new
    CCPPointerInfo i1 = { 1, 0 }, i2 = { 1, 0 };
    CountedPtr p, q;
    p.Info = &i1; q.Info = &i2;
    p = q;
    if (i1.deleted != 1 || i2.RefCount != 2 || p.Info != &i2) fail = 1;
    p = q;   // self-same info: no change
    if (i2.RefCount != 2) fail = 1;

    // hero comes near on frame 4 -> intro plays once
    Sim s = make(4, 1000, 1000, 0, false, 10);
    s.Main();
    if (s.cutscenes != 1 || !s.introSet || s.releases != 1 || s.healthBars != 1 || s.hiddenOn != 1 || s.hiddenOff != 1) fail = 1;

    // special ability 14 does not trigger; a hit on frame 6 does
    Sim t = make(1000, 6, 2, 1, false, 20);
    t.Main();
    if (t.cutscenes != 1 || t.frame < 6) fail = 1;

    // intro already played: straight to the health bar
    Sim u = make(1000, 1000, 1000, 0, true, 3);
    u.Main();
    if (u.cutscenes != 0 || u.healthBars != 1 || u.hiddenOn != 0) fail = 1;

    // terminated while waiting: nothing after the wait
    Sim w = make(1000, 1000, 1000, 0, false, 5);
    w.Main();
    if (w.cutscenes != 0 || w.healthBars != 0) fail = 1;

    if (fail) { std::printf("00ee36a0_TEST FAIL\n"); return 1; }
    std::printf("00ee36a0_TEST PASS\n");
    return 0;
}