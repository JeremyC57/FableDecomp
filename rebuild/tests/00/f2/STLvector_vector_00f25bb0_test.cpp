#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour test for 00f25bb0: the vector<CArenaWaveDef> copy constructor allocates x.size() elements and copy-constructs each one in
// order, leaving finish == start + size (an empty source gives an empty vector).
static int g_copies = 0;
struct Elem { int id; Elem(int i) : id(i) {} Elem(const Elem& o) : id(o.id) { ++g_copies; } };
struct VectorBase {
    Elem* start; Elem* finish; Elem* end;
    VectorBase(unsigned int n) : start(0), finish(0), end(0) { start = n ? (Elem*)std::malloc(n * sizeof(Elem)) : 0; finish = start; end = start + n; }
};
struct Vector : VectorBase {
    unsigned int size() const { return (unsigned int)(finish - start); }
    Vector(unsigned int n) : VectorBase(n) {}
    Vector(const Vector& x) : VectorBase(x.size())
    {
        Elem* cur = start;
        for (const Elem* f = x.start; f != x.finish; ++f, ++cur)
            if (cur) new (cur) Elem(*f);
        finish = cur;
    }
};
int main()
{
    Vector a(3);
    for (int i = 0; i < 3; ++i) new (a.start + i) Elem(10 + i);
    a.finish = a.start + 3;
    Vector b(a);
    Vector e(0);
    Vector f(e);
    int ok = b.size() == 3 && b.start != a.start && b.start[0].id == 10 && b.start[2].id == 12 && g_copies == 3
          && b.end == b.start + 3 && f.size() == 0 && f.start == 0;
    std::free(a.start); std::free(b.start);
    if (!ok) { std::printf("00f25bb0_TEST FAIL\n"); return 1; }
    std::printf("00f25bb0_TEST PASS\n");
    return 0;
}