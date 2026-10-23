// Behaviour test for CEndScythe::Main (0x00d79d50): wait for the parent's HeardJackMessage, take scripted control, then
// each frame speak the on-talk line when talked to, and the on-hit line plus mutual alliance when hit plainly or by a
// special ability other than ability 14.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int jackFrame; int talkFrame; int hitFrame; int specialFrame; long specialAbility;
    int talkLines; int hitLines; int allies; bool scripted; int pauses; int unpauses;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; }
static bool HitPlain(const Sim& s) { return s.frame == s.hitFrame; }
static bool HitSpecial(const Sim& s) { return s.frame == s.specialFrame; }
static bool HitSpecialFrom(const Sim& s, long a) { return s.frame == s.specialFrame && s.specialAbility == a; }
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    while (s.frame < s.jackFrame) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    s.scripted = true;
    while (!Term(s)) {
        if (s.frame == s.talkFrame) { ++s.pauses; ++s.talkLines; ++s.unpauses; }
        if (HitPlain(s) || (HitSpecial(s) && !HitSpecialFrom(s, 14))) { ++s.pauses; ++s.hitLines; s.allies += 2; ++s.unpauses; }
        Frame(s);
    }
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 40; a.jackFrame = 5; a.talkFrame = 8; a.hitFrame = 10; a.specialFrame = 12; a.specialAbility = 3;
    Main(a);
    Sim b = a; std::memset(&b, 0, sizeof(b));
    b.terminateAt = 40; b.jackFrame = 5; b.talkFrame = 2; b.hitFrame = 3; b.specialFrame = 12; b.specialAbility = 14;
    Main(b);
    bool ok = a.scripted && a.talkLines == 1 && a.hitLines == 2 && a.allies == 4 && a.pauses == a.unpauses
           && b.scripted && b.talkLines == 0 && b.hitLines == 0 && b.allies == 0;
    std::printf(ok ? "00d79d50_TEST PASS\n" : "00d79d50_TEST FAIL\n");
    return ok ? 0 : 1;
}
