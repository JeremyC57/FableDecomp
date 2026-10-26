// Behaviour test for 0x00478751 std::_Destroy<CAbilityTrackLevelDef*>(first, last): every 0x48-byte element's virtual
// destructor runs once, in order, and an empty range runs none.
#include <stdio.h>
#include <new>
static int g_calls = 0;
static const void* g_seen[8];
class Def {
public:
    Def() { }
    virtual ~Def() { g_seen[g_calls++] = this; }
    unsigned char body[0x44];
};
static void destroy_range(Def* first, Def* last)
{
    for ( ; first != last; ++first)
        first->~Def();
}
int main()
{
    int ok = sizeof(Def) == 0x48;
    static unsigned char raw[5 * 0x48];
    Def* d = (Def*)raw;
    for (int i = 0; i < 5; ++i) new (&d[i]) Def();
    destroy_range(d, d + 5);
    ok &= g_calls == 5;
    for (int i = 0; i < 5 && ok; ++i) ok &= g_seen[i] == &d[i];
    g_calls = 0;
    destroy_range(d + 2, d + 2);
    ok &= g_calls == 0;
    printf(ok ? "00478751_TEST PASS\n" : "00478751_TEST FAIL\n");
    return ok ? 0 : 1;
}
