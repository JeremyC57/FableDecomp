#include <cstdio>
#include <cstring>
// Behaviour model: the constructor records the action code and the animation name.
struct Action { long code; char anim[40]; long s0, s1; };
static void construct(Action& a, long code, const char* name) { a.s0 = 0; a.s1 = 0; a.code = code; std::strcpy(a.anim, name); }
int main()
{
    Action a; construct(a, 150, "WILL_CAST_FORCE_SPELL_DELIVER");
    if (a.code != 150 || std::strcmp(a.anim, "WILL_CAST_FORCE_SPELL_DELIVER") != 0 || a.s0 || a.s1) { std::printf("008bc0c0_TEST FAIL\n"); return 1; }
    std::printf("008bc0c0_TEST PASS\n");
    return 0;
}