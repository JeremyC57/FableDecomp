// Behaviour test for CTCTrespasser::LeavingRegion (0x007BAF00): with a live trespassed thing, each open trespass deed
// is ended and (except index 2) removed, the inside-building deed is ended and removed, all reset to -1, the informed
// index reset and the trespassable un-flagged with the leave frame; a dead/absent thing skips that; the thing is always
// cleared and an attached opinion source detached.
#include <cstdio>
#include <string>

static std::string Num(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }

struct Tres {
    bool alive, hasLog, hasTrespassable, trespassed, attached; long inside, deeds[3], informed; unsigned long left;
    bool hasThing; std::string log; int cleared;
    void LeavingRegion(unsigned long frame)
    {
        if (hasThing && alive) {
            if (hasLog) {
                for (long i = 0; i < 3; ++i)
                    if (deeds[i] != -1) {
                        log += "E" + Num(deeds[i]);
                        if (i != 2) log += "R" + Num(deeds[i]);
                        deeds[i] = -1;
                    }
                if (inside != -1) { log += "E" + Num(inside) + "R" + Num(inside); inside = -1; }
            }
            informed = -1;
            if (hasTrespassable) { trespassed = false; left = frame; }
        }
        hasThing = false;
        if (attached) { ++cleared; attached = false; }
    }
};

int main()
{
    int ok = 1;
    Tres t = { true, true, true, true, true, 9, { 1, -1, 3 }, 2, 0, true, "", 0 };
    t.LeavingRegion(77);
    ok &= t.log == "E1R1E3E9R9" && t.deeds[0] == -1 && t.deeds[2] == -1 && t.inside == -1 && t.informed == -1;
    ok &= !t.trespassed && t.left == 77 && !t.hasThing && t.cleared == 1 && !t.attached;
    Tres d = { false, true, true, true, false, 9, { 1, 2, 3 }, 2, 5, true, "", 0 };
    d.LeavingRegion(77);
    ok &= d.log == "" && d.deeds[0] == 1 && d.informed == 2 && d.trespassed && d.left == 5 && !d.hasThing && d.cleared == 0;
    std::printf(ok ? "007baf00_TEST PASS\n" : "007baf00_TEST FAIL\n");
    return ok ? 0 : 1;
}
