// Behaviour test for 0x006F20A0 (STLport vector<CProng> copy constructor: exact-size allocation, then __uninitialized_copy).
#include <cstdio>
#include <cstdlib>
#include <new>
struct CProng { bool Blocked; float X, Y, Z; long Timer; void* Occupier; };
static int g_copyCalls = 0;
static CProng* UninitCopy(const CProng* first, const CProng* last, CProng* result)
{
    ++g_copyCalls;
    for (; first != last; ++first, ++result)
        new (result) CProng(*first);
    return result;
}
struct Vec {
    CProng* start; CProng* finish; CProng* end_of_storage;
    Vec() : start(0), finish(0), end_of_storage(0) { }
    unsigned int size() const { return (unsigned int)(finish - start); }
    Vec(const Vec& x) : start(0), finish(0), end_of_storage(0)
    {
        unsigned int n = x.size();
        start = n != 0 ? (CProng*)std::malloc(n * sizeof(CProng)) : 0;
        finish = start; end_of_storage = start + n;
        finish = UninitCopy(x.start, x.finish, start);
    }
};
#define FAIL { std::printf("006f20a0_TEST FAIL\n"); return 1; }
int main()
{
    Vec empty;
    Vec e2(empty);
    if (e2.start != 0 || e2.finish != 0 || e2.end_of_storage != 0 || g_copyCalls != 1) FAIL;   // copy still called (empty range)
    CProng src[2] = { { true, 1, 2, 3, 10, 0 }, { false, 4, 5, 6, 20, src } };
    Vec v; v.start = src; v.finish = src + 2; v.end_of_storage = src + 2;
    Vec c(v);
    if (c.size() != 2 || c.end_of_storage != c.finish || g_copyCalls != 2) FAIL;
    if (!c.start[0].Blocked || c.start[0].Timer != 10 || c.start[1].Blocked || c.start[1].Z != 6 || c.start[1].Occupier != src) FAIL;
    std::free(c.start);
    std::printf("006f20a0_TEST PASS\n");
    return 0;
}