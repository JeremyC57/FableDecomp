// Behaviour test for 0x00476F9B vector<CEnvironmentThemeDef>::erase(first, last): the tail is assigned down over [first, last), the surplus elements are destroyed, finish moves, first is returned.
#include <cstdio>
#include <new>
static int g_assign = 0, g_destroyed = 0;
struct Def {
    int id; unsigned char body[0xC4 - sizeof(int)];
    Def& operator=(const Def& o) { id = o.id; ++g_assign; return *this; }
    ~Def() { ++g_destroyed; }
};
struct Vec {
    Def* start; Def* finish;
    Def* erase(Def* first, Def* last)
    {
        Def* result = first;
        for (int n = (int)(finish - last); n > 0; --n) { *result = *last; ++last; ++result; }
        for (Def* p = result; p != finish; ++p) p->~Def();
        finish = result;
        return first;
    }
};
static int fail() { std::printf("00476f9b_TEST FAIL\n"); return 1; }
int main()
{
    if (sizeof(Def) != 0xC4) return fail();
    static unsigned char raw[6 * sizeof(Def)];
    Def* d = (Def*)raw;
    for (int i = 0; i < 6; ++i) { new (&d[i]) Def(); d[i].id = i; }
    Vec v; v.start = d; v.finish = d + 6;
    Def* r = v.erase(d + 1, d + 3);                    // drop ids 1, 2: ids 3..5 move down
    if (r != d + 1 || v.finish != d + 4 || g_assign != 3 || g_destroyed != 2 ||
        d[0].id != 0 || d[1].id != 3 || d[2].id != 4 || d[3].id != 5) return fail();
    r = v.erase(d + 2, d + 4);                         // tail erase: no assignment
    if (r != d + 2 || v.finish != d + 2 || g_assign != 3 || g_destroyed != 4) return fail();
    r = v.erase(d + 1, d + 1);                         // empty range: the tail is assigned onto itself
    if (r != d + 1 || v.finish != d + 2 || g_assign != 4 || g_destroyed != 4) return fail();
    std::printf("00476f9b_TEST PASS\n");
    return 0;
}