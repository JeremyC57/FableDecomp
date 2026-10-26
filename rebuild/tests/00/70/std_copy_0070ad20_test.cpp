// behaviour test for 0x0070AD20 std::__copy<CEngineTextureDecal*, CEngineTextureDecal*, int>: assigns last - first elements in order (forward, so an
// overlapping move down works), each through the element's member-wise operator=, and returns result + (last - first)
#include <cstdio>
static int g_assigns = 0;
struct Tail { int v; Tail& operator=(const Tail& o) { v = o.v; ++g_assigns; return *this; } };
struct Elem { int a; int b; Tail t; int pad[2]; };
static Elem* copy_fwd(const Elem* first, const Elem* last, Elem* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    int fail = 0;
    if (sizeof(Elem) != 20u) fail |= 1;
    Elem e[6];
    for (int i = 0; i < 6; ++i) { e[i].a = i; e[i].b = 10 * i; e[i].t.v = 100 * i; }
    Elem* r = copy_fwd(e + 2, e + 6, e);                   // erase-style move down by two
    if (r != e + 4) fail |= 2;
    if (e[0].a != 2 || e[1].b != 30 || e[3].t.v != 500 || e[4].a != 4 || e[5].a != 5) fail |= 4;
    if (g_assigns != 4) fail |= 8;
    if (copy_fwd(e + 3, e + 3, e) != e || g_assigns != 4) fail |= 16;   // empty range
    if (fail) { std::printf("0070ad20_TEST FAIL %d\n", fail); return 1; }
    std::printf("0070ad20_TEST PASS\n");
    return 0;
}