// Behaviour test for 0x006D4E60 CTCFollowed::IsCreatureValidFollower: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct C { bool supervised, allowed, hasOpinion, aiFollower, willingContinue, willingStart; };
static bool Valid(const C& c, bool heroHasTC, long followers, long maxCrowd, bool* temporary)
{
    bool dummy; bool* t = temporary ? temporary : &dummy;
    *t = !c.supervised && heroHasTC && c.hasOpinion;
    if (!c.allowed) return false;
    if (!*t) return true;
    if (!c.aiFollower && followers >= maxCrowd) return false;
    return c.aiFollower ? c.willingContinue : c.willingStart;
}
int main() {
    C c = { false, true, true, false, false, true }; bool tmp = false;
    int ok = Valid(c, true, 1, 3, &tmp) && tmp;
    ok &= !Valid(c, true, 3, 3, 0);
    c.aiFollower = true; ok &= !Valid(c, true, 3, 3, 0);
    c.supervised = true; ok &= Valid(c, true, 9, 3, &tmp) && !tmp;
    c.allowed = false; ok &= !Valid(c, true, 0, 3, 0);
    std::printf(ok ? "006d4e60_TEST PASS\n" : "006d4e60_TEST FAIL\n");
    return ok ? 0 : 1;
}
