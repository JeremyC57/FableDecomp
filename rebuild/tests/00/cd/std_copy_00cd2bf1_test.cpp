// behaviour test for 0x00CD2BF1 std::__copy over CScriptGameResourceObjectScriptedThingBase: last - first assignments front to back (overlapping copy-down safe), the
// advanced result returned, an empty range untouched
#include <cstdio>
static int g_assign = 0;
struct Elem {                                               // stands in for the 0x10-byte CScriptGameResourceObjectScriptedThingBase
    int v[4];                                   // 4-byte words (int: 32-bit under both compilers)
    Elem& operator=(const Elem& o) { ++g_assign; for (int i = 0; i < 4; ++i) v[i] = o.v[i]; return *this; }
};
static Elem* Copy(Elem* first, Elem* last, Elem* result)       // the function under test
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    int fail = 0;
    if (sizeof(Elem) != 0x10u) fail |= 1;
    Elem a[6];
    for (int k = 0; k < 6; ++k) for (int i = 0; i < 4; ++i) a[k].v[i] = k * 10 + i;
    Elem* r = Copy(a + 2, a + 6, a);                         // erase-style copy down
    if (r != a + 4 || g_assign != 4) fail |= 2;
    for (int k = 0; k < 4; ++k) if (a[k].v[0] != (k + 2) * 10) fail |= 4;
    if (Copy(a + 1, a + 1, a + 3) != a + 3 || g_assign != 4) fail |= 8;
    if (fail) { std::printf("00cd2bf1_TEST FAIL %d\n", fail); return 1; }
    std::printf("00cd2bf1_TEST PASS\n");
    return 0;
}