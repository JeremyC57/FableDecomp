// behaviour test for 0x00C40C10 CLibpkObject78::SetFloat60: an equal value leaves the field and the flags alone; a different value is stored and raises 0x20000 (other flag bits kept)
#include <cstdio>
struct Obj { float F; unsigned int Flags; };
static void Set(Obj& o, float v) { if (o.F != v) { o.F = v; o.Flags |= 0x20000u; } }
int main()
{
    int fails = 0;
    Obj o; o.F = 1.5f; o.Flags = 0x1u;
    Set(o, 1.5f);
    if (o.F != 1.5f || o.Flags != 0x1u) fails |= 1;
    Set(o, 2.25f);
    if (o.F != 2.25f || o.Flags != (0x1u | 0x20000u)) fails |= 2;
    o.Flags = 0;
    Set(o, -0.0f);                      // -0.0 != 2.25: stored
    if (o.Flags != 0x20000u) fails |= 4;
    o.Flags = 0;
    Set(o, 0.0f);                       // 0.0 == -0.0: unchanged
    if (o.Flags != 0) fails |= 8;
    if (fails) { std::printf("00c40c10_TEST FAIL %d\n", fails); return 1; }
    std::printf("00c40c10_TEST PASS\n");
    return 0;
}