// behaviour test for 0x00825CF0 std::vector<CMap::CScriptedMapBrushUpdateArea >::erase(first, last): the tail moves down onto first, exactly the stale
// elements at the end are destroyed, the size shrinks by last - first, and first is returned
#include <cstdio>
static int g_dtors = 0;
struct Str { int v; ~Str() { ++g_dtors; } };
struct Elem { int a; Str s; };
struct Vec { Elem* start; Elem* finish; };
static Elem* erase(Vec& v, Elem* first, Elem* last)
{
    Elem* r = first;
    for (Elem* p = last; p != v.finish; ++p, ++r) { r->a = p->a; r->s.v = p->s.v; }
    for (Elem* p = r; p != v.finish; ++p) p->s.~Str();
    v.finish = r;
    return first;
}
int main()
{
    int fail = 0;
    static Elem store[6];
    for (int i = 0; i < 6; ++i) { store[i].a = i; store[i].s.v = 100 + i; }
    Vec v; v.start = store; v.finish = store + 6;
    g_dtors = 0;
    if (erase(v, store + 1, store + 3) != store + 1) fail |= 1;
    if (v.finish != store + 4 || g_dtors != 2) fail |= 2;
    if (store[0].a != 0 || store[1].a != 3 || store[2].s.v != 104 || store[3].a != 5) fail |= 4;
    if (erase(v, v.finish, v.finish) != store + 4 || v.finish != store + 4 || g_dtors != 2) fail |= 8;   // empty range
    if (erase(v, v.start, v.finish) != store || v.finish != store || g_dtors != 6) fail |= 16;          // clear
    if (fail) { std::printf("00825cf0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00825cf0_TEST PASS\n");
    return 0;
}