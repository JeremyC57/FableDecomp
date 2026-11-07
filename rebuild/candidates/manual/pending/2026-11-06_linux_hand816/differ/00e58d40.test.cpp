// Behaviour test for NScript::CV_BeggarAndChildScript::CLookoutPointBeggar::Main (0x00E58D40): the loop runs until the beggar or the
// bully is hit or leaves; the belch reaction counts up and the fourth belch makes the beggar leave (and marks the bully belched at);
// the fart (battle cry) reaction counts its own calls with the third and later ones using the THIRD line. A model of the counters.
#include <cstdio>
#include <cstring>
struct BeggarState { int belchedAtBeggar; bool beggarLeft; bool belchedAtBully; int battleCryCount; };
static int Belch(BeggarState& s)
{
    int n = ++s.belchedAtBeggar;
    switch (n) {
    case 4: s.beggarLeft = s.belchedAtBully = true; break;
    default: break;
    }
    return n;
}
static const char* Fart(BeggarState& s)
{
    switch (++s.battleCryCount) {
    case 1: return "FIRST";
    case 2: return "SECOND";
    default: return "THIRD";
    }
}
static bool LoopRuns(bool beggarHit, bool bullyHit, bool beggarLeft, bool bullyLeft) { return !beggarHit && !bullyHit && !beggarLeft && !bullyLeft; }
int main()
{
    BeggarState s; std::memset(&s, 0, sizeof s);
    bool ok = Belch(s) == 1 && !s.beggarLeft && Belch(s) == 2 && Belch(s) == 3 && !s.beggarLeft && Belch(s) == 4 && s.beggarLeft && s.belchedAtBully;
    ok = ok && !std::strcmp(Fart(s), "FIRST") && !std::strcmp(Fart(s), "SECOND") && !std::strcmp(Fart(s), "THIRD") && !std::strcmp(Fart(s), "THIRD");
    ok = ok && LoopRuns(false, false, false, false) && !LoopRuns(true, false, false, false) && !LoopRuns(false, false, false, true);
    std::printf(ok ? "00e58d40_TEST PASS\n" : "00e58d40_TEST FAIL\n");
    return ok ? 0 : 1;
}
