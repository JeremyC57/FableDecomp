#include <cstdio>
#include <cstring>
// Behaviour model: the constructor records the action code, the member and the animation name.
struct Action { long code; long member; char anim[40]; };
static void construct(Action& a, long code, long v, const char* n) { a.code = code; a.member = v; std::strcpy(a.anim, n); }
int main()
{
    Action a; construct(a, 150, 3, "ST_CAST_SUMMON_SPELL");
    if (a.code != 150 || a.member != 3 || std::strcmp(a.anim, "ST_CAST_SUMMON_SPELL") != 0) { std::printf("008bc660_TEST FAIL\n"); return 1; }
    std::printf("008bc660_TEST PASS\n");
    return 0;
}