// Behaviour test for CExpression_DigScript::Main @ 0x00EEA7C0 (restated with a fake script interface):
// the found / not-found digging paths, the weapon restore choice and the terminate exits.
#include <cstdio>
#include <cstring>

struct Fake {
    bool alive, inRange, melee, ranged;
    int terminateAt;          // IsActiveThreadTerminating call index that returns true (-1 none)
    int termCalls;
    char log[512];
    void Add(const char* s) { std::strcat(log, s); std::strcat(log, ";"); }
    bool Term() { bool r = termCalls == terminateAt; ++termCalls; return r; }
};

static void Main(Fake& f)
{
    f.Add("frame");
    if (f.Term()) return;
    f.Add("persist");
    if (f.Term()) return;
    f.Add("script");
    if (f.Term()) return;
    f.Add("fadeout");
    f.Add("spade");
    f.Add("idle");
    bool found = f.alive && f.inRange;
    if (found) {
        if (f.Term()) return;
        f.Add("face");
    }
    f.Add("dig");
    if (found) {
        if (f.Term()) return;
        f.Add("success");
    } else {
        if (f.Term()) return;
        f.Add("failure");
    }
    f.Add("stop");
    if (f.melee) {
        if (f.Term()) return;
        f.Add("melee");
    } else if (f.ranged) {
        if (f.Term()) return;
        f.Add("ranged");
    }
    f.Add("camera");
    if (found) {
        if (f.Term()) return;
        f.Add("give");
    }
    f.Add("deactivate");
}

static int Check(bool alive, bool inRange, bool melee, bool ranged, int termAt, const char* want)
{
    Fake f;
    std::memset(&f, 0, sizeof f);
    f.alive = alive; f.inRange = inRange; f.melee = melee; f.ranged = ranged; f.terminateAt = termAt;
    Main(f);
    if (std::strcmp(f.log, want) != 0) {
        std::printf("FAIL: got %s want %s\n", f.log, want);
        return 1;
    }
    return 0;
}

int main()
{
    int fails = 0;
    fails += Check(true, true, true, true, -1,
        "frame;persist;script;fadeout;spade;idle;face;dig;success;stop;melee;camera;give;deactivate;");
    fails += Check(true, false, false, true, -1,
        "frame;persist;script;fadeout;spade;idle;dig;failure;stop;ranged;camera;deactivate;");
    fails += Check(false, true, false, false, -1,
        "frame;persist;script;fadeout;spade;idle;dig;failure;stop;camera;deactivate;");
    fails += Check(true, true, false, false, 3,
        "frame;persist;script;fadeout;spade;idle;");
    fails += Check(true, true, false, false, 0, "frame;");
    if (fails == 0)
        std::printf("00eea7c0_TEST PASS\n");
    return fails;
}
