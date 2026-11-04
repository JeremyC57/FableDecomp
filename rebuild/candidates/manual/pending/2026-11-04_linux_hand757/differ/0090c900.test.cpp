// Behaviour test for 0x0090C900 CThingFilter_IsValidGossipee::operator(): a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct T { int faction; bool hasFaction; int village; bool hasVillage; int home; };
static bool Valid(const T* self, int fac, int vil, bool curfew, const T& t, bool inside)
{
    if (fac) { if (!t.hasFaction || fac != t.faction) return false; } else if (t.hasFaction) return false;
    if (vil) { if (!t.hasVillage || vil != t.village) return false; } else if (t.hasVillage && t.village) return false;
    if (vil && !curfew) {
        if (t.home) return &t != self && self->home == t.home && inside;
        return &t != self && !self->home;
    }
    return &t != self;
}
int main() {
    T me = { 1, true, 2, true, 5 }, o = { 1, true, 2, true, 5 }, f = { 3, true, 2, true, 5 }, n = { 0, false, 0, false, 0 };
    int ok = Valid(&me, 1, 2, false, o, true) && !Valid(&me, 1, 2, false, o, false) && !Valid(&me, 1, 2, false, f, true)
        && !Valid(&me, 1, 2, true, me, true) && Valid(&me, 0, 0, false, n, false) && !Valid(&me, 0, 0, false, o, true);
    std::printf(ok ? "0090c900_TEST PASS\n" : "0090c900_TEST FAIL\n");
    return ok ? 0 : 1;
}
