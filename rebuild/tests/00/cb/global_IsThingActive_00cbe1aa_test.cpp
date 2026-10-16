#include <cstdio>
// Behaviour model of IsThingActive (0x00CBE1AA): alive, not killed, not knocked out, not unconscious.
struct Thing { bool alive, killed, ko, unconscious; };
static bool IsThingActive(const Thing& t) { char r = t.alive && !t.killed && !t.ko && !t.unconscious; return r; }
int main()
{
    Thing a = { true, false, false, false }, b = { true, false, true, false }, c = { false, false, false, false };
    if (!IsThingActive(a) || IsThingActive(b) || IsThingActive(c)) { std::printf("00cbe1aa_TEST FAIL\n"); return 1; }
    std::printf("00cbe1aa_TEST PASS\n"); return 0;
}