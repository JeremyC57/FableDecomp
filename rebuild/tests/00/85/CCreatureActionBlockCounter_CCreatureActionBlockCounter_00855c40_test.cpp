#include <cstdio>
#include <cstring>
// Behaviour model: the constructor records the action code and the animation name.
struct Action { long code; char anim[40]; long s0, s1; };
static void construct(Action& a, long code, const char* name) { a.s0 = 0; a.s1 = 0; a.code = code; std::strcpy(a.anim, name); }
int main()
{
    Action a; construct(a, 150, "BLOCK_COUNTER_ATTACK");
    if (a.code != 150 || std::strcmp(a.anim, "BLOCK_COUNTER_ATTACK") != 0 || a.s0 || a.s1) { std::printf("00855c40_TEST FAIL\n"); return 1; }
    std::printf("00855c40_TEST PASS\n");
    return 0;
}