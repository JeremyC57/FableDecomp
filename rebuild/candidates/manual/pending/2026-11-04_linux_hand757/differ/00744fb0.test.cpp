// Behaviour test for 0x00744FB0 CTCPerceiveThing::UpdateHearing: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct Noise { bool dead; float x; };
struct P { bool heard, direct; float timer, lastX, hearTime; int aware, lastInfo; bool perceived; };
static void UpdateHearing(P& p, bool quiet, Noise* best)
{
    p.heard = false;
    if (!quiet && best && !best->dead) {
        p.heard = true;
        if (p.timer != 0.0f && p.direct) { if (p.perceived) { p.direct = true; p.lastInfo++; } }
        else { p.lastX = best->x; p.timer = p.hearTime; }
    }
    p.aware++;
}
int main() {
    Noise n = { false, 4.0f }; P p = { false, false, 0.0f, 0.0f, 2.5f, 0, 0, true };
    UpdateHearing(p, false, &n); int ok = p.heard && p.lastX == 4.0f && p.timer == 2.5f && p.aware == 1;
    p.direct = true; UpdateHearing(p, false, &n); ok &= p.lastInfo == 1;
    UpdateHearing(p, true, &n); ok &= !p.heard && p.aware == 3;
    std::printf(ok ? "00744fb0_TEST PASS\n" : "00744fb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
