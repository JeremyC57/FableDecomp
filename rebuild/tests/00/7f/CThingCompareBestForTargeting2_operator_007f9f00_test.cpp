// Behaviour test for 007f9f00: src/007f9f00.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CThingCompare_BestForTargeting2::operator()(CThing* a, CThing* b) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingCompare_BestForTargeting2::operator()(CThing* a, CThing* b) const -- order two targeting candidates by the vector comparator's value at their positions
// Leaf: ??RCThingCompare_BestForTargeting2
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F9F00, 94 bytes (+ int3 padding).
// Callees: CThingCompare_BestForTargeting2::GetThingCompareMultiplier(CThing*) @ 0x007F9E70 (x2, results discarded:
//          `fstp st(0)`), CThing::GetPosition @ 0x004C73D0 (x2: b first, then a), CVectorCompare_BestForTargeting::
//          GetCompareValueForPoint(const C3DVector&) @ 0x007F9660 (x2, on the comparator member at +4).
// Identity: PDB CThingCompare_BestForTargeting2 (Hero +0, VectorCompare +4, EnemyPreferenceFactor +0x30); the six
//   callers (0x007F9F71..0x007FA243) are the sort helpers of the targeting candidate list.
// Shape: the two multipliers are computed into unused float locals (the multiplication they fed is gone in this build);
//   the positions are references taken b first; both compare values are NAMED float locals (posA's parked in the dead
//   argument slot `[esp+10h]`), then `if (valueA > valueB) return true;` -- `fcomp ; pop edi ; pop esi ; fnstsw ax ;
//   pop ebx` (the inline call-expression compare schedules all three pops before fnstsw); the if-form gives the byte
//   `mov al,1` / `xor al,al` exits.
struct C3DVector;

struct CThing {
    const C3DVector& GetPosition();     // 0x004C73D0
};

struct CVectorCompare_BestForTargeting {
    unsigned char _body[0x2c];
    float GetCompareValueForPoint(const C3DVector& point);     // 0x007F9660
};

struct CThingCompare_BestForTargeting2 {
    CThing* Hero;                                       // +0x00
    CVectorCompare_BestForTargeting VectorCompare;      // +0x04
    float EnemyPreferenceFactor;                        // +0x30
    float GetThingCompareMultiplier(CThing* thing);     // 0x007F9E70
    bool operator()(CThing* a, CThing* b);
};

bool CThingCompare_BestForTargeting2::operator()(CThing* a, CThing* b)
{
    float multiplierA = GetThingCompareMultiplier(a);
    float multiplierB = GetThingCompareMultiplier(b);
    const C3DVector& posB = b->GetPosition();
    const C3DVector& posA = a->GetPosition();
    float valueA = VectorCompare.GetCompareValueForPoint(posA);
    float valueB = VectorCompare.GetCompareValueForPoint(posB);
    if (valueA > valueB)
        return true;
    return false;
}

// ---- stubs and driver ----
struct C3DVector { float X, Y, Z; };
static int g_log[16]; static int g_n = 0;
static C3DVector g_pos[2];
static CThing g_things[2];
static float g_value[2];
float CThingCompare_BestForTargeting2::GetThingCompareMultiplier(CThing* thing) { g_log[g_n++] = 10 + (int)(thing - g_things); return 2.0f; }
const C3DVector& CThing::GetPosition() { g_log[g_n++] = 20 + (int)(this - g_things); return g_pos[this - g_things]; }
float CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector& point)
{
    g_log[g_n++] = 30 + (int)(&point - g_pos);
    return g_value[&point - g_pos];
}
int main()
{
    CThingCompare_BestForTargeting2 cmp;
    g_value[0] = 5.0f; g_value[1] = 3.0f; g_n = 0;
    CHECK(cmp(&g_things[0], &g_things[1]) == true);
    CHECK(g_n == 6); CHECK(g_log[0] == 10); CHECK(g_log[1] == 11); CHECK(g_log[2] == 21); CHECK(g_log[3] == 20);
    CHECK(g_log[4] == 30); CHECK(g_log[5] == 31);
    g_n = 0; CHECK(cmp(&g_things[1], &g_things[0]) == false);
    CHECK(g_log[0] == 11); CHECK(g_log[2] == 20); CHECK(g_log[4] == 31);
    g_value[1] = 5.0f; g_n = 0; CHECK(cmp(&g_things[0], &g_things[1]) == false);   // equal: not better
    if (fails == 0) printf("007f9f00_TEST PASS\n"); else printf("007f9f00_TEST FAIL\n");
    return fails;
}