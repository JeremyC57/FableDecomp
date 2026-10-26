// Behaviour test for 0x006F0810 (STLport vector<C3DVector> copy constructor: exact-size allocation + element copies).
#include <cstdio>
#include <cstdlib>
#include <new>
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
static int g_mallocs = 0;
struct Vec {
    C3DVector* start; C3DVector* finish; C3DVector* end_of_storage;
    Vec() : start(0), finish(0), end_of_storage(0) { }
    unsigned int size() const { return (unsigned int)(finish - start); }
    Vec(const Vec& x) : start(0), finish(0), end_of_storage(0)
    {
        unsigned int n = x.size();
        start = n != 0 ? (++g_mallocs, (C3DVector*)std::malloc(n * sizeof(C3DVector))) : 0;
        finish = start; end_of_storage = start + n;
        C3DVector* cur = start;
        for (const C3DVector* p = x.start; p != x.finish; ++p, ++cur)
            new (cur) C3DVector(*p);
        finish = cur;
    }
};
#define FAIL { std::printf("006f0810_TEST FAIL\n"); return 1; }
int main()
{
    Vec empty;
    Vec e2(empty);
    if (e2.start != 0 || e2.finish != 0 || e2.end_of_storage != 0 || g_mallocs != 0) FAIL;
    C3DVector src[3] = { C3DVector(1, 2, 3), C3DVector(4, 5, 6), C3DVector(7, 8, 9) };
    Vec v; v.start = src; v.finish = src + 3; v.end_of_storage = src + 3;
    Vec c(v);
    if (g_mallocs != 1 || c.size() != 3 || c.end_of_storage != c.start + 3 || c.start == src) FAIL;
    for (int i = 0; i < 3; ++i)
        if (c.start[i].X != src[i].X || c.start[i].Y != src[i].Y || c.start[i].Z != src[i].Z) FAIL;
    std::free(c.start);
    std::printf("006f0810_TEST PASS\n");
    return 0;
}