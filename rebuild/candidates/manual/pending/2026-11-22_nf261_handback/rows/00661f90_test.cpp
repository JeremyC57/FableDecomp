// NF261 (q12742): links the REAL row object (the HEAD test carried its own copy of the body with a made-up g_scale = 2.0f; retail
// multiplies by the pooled 0.5f). The local layout structs are the HEAD test's (in place).
#include <cstdio>

struct Inner { char pad[0x134]; float f134; };
struct Other { char pad[0x34]; float f34; };
class CThingCreatureBase { public:
    char pad0[0x60];
    Other* q;
    char pad1[0xe0 - 0x64];
    Inner* p;
};
float g_thresh = 5.0f;   // the row's extern (0x01230A5C)
float __fastcall CThingCreatureBase_GetCombatRadius(const CThingCreatureBase* creature);   // the row

int main()
{
    Inner inr;
    inr.f134 = 12.0f;
    Other oth;
    oth.f34 = 99.0f;
    CThingCreatureBase c;
    c.p = &inr;
    c.q = &oth;
    float r1 = CThingCreatureBase_GetCombatRadius(&c);
    if (r1 != 6.0f) { std::printf("FAIL r1=%f\n", r1); return 1; }

    inr.f134 = 10.0f;
    float r2 = CThingCreatureBase_GetCombatRadius(&c);
    if (r2 != 99.0f) { std::printf("FAIL r2=%f\n", r2); return 1; }

    std::printf("CThingCreatureBase_00661f90_TEST PASS\n");
    return 0;
}