// Behaviour test for 0x00753380 CTCLook::FrameUpdateHeadInterested(): a model over stub targeting; checks the camera when the targeting focuses on it, else the target head / focal point / position, CentreHead with neither, and the medium-speed flag added only when no speed bit is set.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct Target { bool hasLook, hasFocal; float head, focal, pos; };
struct Look { unsigned long LookFlags; int centred; float looked; };
static void Update(Look& l, bool hasTargeting, bool camera, const Target* t, float cam)
{
    const Target* target = 0;
    if (hasTargeting) target = t;
    float pos;
    if (target) {
        if (camera) pos = cam;
        else if (target->hasLook) pos = target->head;
        else if (target->hasFocal) pos = target->focal;
        else pos = target->pos;
    } else if (hasTargeting && camera) pos = cam;
    else { ++l.centred; return; }
    if (!(l.LookFlags & 0x38)) l.LookFlags |= 0x10;
    l.looked = pos;
}
int main()
{
    int ok = 1; Look l = { 0, 0, 0 };
    Target a = { true, true, 1, 2, 3 }, b = { false, true, 1, 2, 3 }, c = { false, false, 1, 2, 3 };
    Update(l, true, true, &a, 9); ok &= l.looked == 9 && l.LookFlags == 0x10;
    Update(l, true, false, &a, 9); ok &= l.looked == 1;
    Update(l, true, false, &b, 9); ok &= l.looked == 2;
    l.LookFlags = 0x20; Update(l, true, false, &c, 9); ok &= l.looked == 3 && l.LookFlags == 0x20;
    Update(l, true, false, 0, 9); ok &= l.centred == 1;
    Update(l, true, true, 0, 7); ok &= l.looked == 7;
    std::printf(ok ? "00753380_TEST PASS\n" : "00753380_TEST FAIL\n"); return ok ? 0 : 1;
}
