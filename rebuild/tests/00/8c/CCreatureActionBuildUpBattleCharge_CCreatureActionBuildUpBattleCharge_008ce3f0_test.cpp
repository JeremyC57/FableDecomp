#include <cstdio>
#include <cstring>
// Behaviour model: the constructor records the action code and the animation name.
struct Action { long code; char anim[40]; long s0, s1; };
static void construct(Action& a, long code, const char* name) { a.s0 = 0; a.s1 = 0; a.code = code; std::strcpy(a.anim, name); }
int main()
{
    Action a; construct(a, 140, "ST_BATTLE_CHARGE_BUILD_UP");
    if (a.code != 140 || std::strcmp(a.anim, "ST_BATTLE_CHARGE_BUILD_UP") != 0 || a.s0 || a.s1) { std::printf("008ce3f0_TEST FAIL\n"); return 1; }
    std::printf("008ce3f0_TEST PASS\n");
    return 0;
}