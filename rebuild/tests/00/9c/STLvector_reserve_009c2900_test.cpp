// Behaviour test for 0x009C2900 vector<C2DLineF>::reserve: below the requested capacity, a non-empty vector copies its
// lines into a fresh n-element block and frees the old one, an empty one just allocates; size is kept, capacity becomes n;
// a request at or below the capacity changes nothing.
#include <cstdio>
#include <cstdlib>
#include <new>

struct C2DVector { float X, Y; };
struct C2DLineF {
    C2DVector Point1, Point2;
    C2DLineF(const C2DVector& a, const C2DVector& b) : Point1(a), Point2(b) { }
};

static int g_mallocs = 0;
static int g_frees = 0;

struct LineVec {
    C2DLineF* start;
    C2DLineF* finish;
    C2DLineF* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    static C2DLineF* allocate(unsigned int n) { if (!n) return 0; ++g_mallocs; return (C2DLineF*)std::malloc(n * sizeof(C2DLineF)); }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old_size = size();
            C2DLineF* tmp;
            if (start) {
                tmp = allocate(n);
                C2DLineF* cur = tmp;
                for (const C2DLineF* p = start; p != finish; ++p, ++cur)
                    new (cur) C2DLineF(*p);
                ++g_frees;
                std::free(start);
            } else {
                tmp = allocate(n);
            }
            start = tmp; finish = tmp + old_size; eos = tmp + n;
        }
    }
};

static int Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    LineVec v; v.start = v.finish = v.eos = 0;
    int ok = 1;
    v.reserve(2);                                             // empty: plain allocation
    if (g_mallocs != 1 || g_frees != 0 || v.size() != 0 || v.capacity() != 2) ok = 0;
    C2DVector a = { 1.0f, 2.0f }, b = { 3.0f, 4.0f }, c = { 5.0f, 6.0f };
    new (v.finish) C2DLineF(a, b); ++v.finish;
    new (v.finish) C2DLineF(b, c); ++v.finish;
    v.reserve(2);                                             // already enough: untouched
    if (g_mallocs != 1 || v.capacity() != 2) ok = 0;
    v.reserve(5);                                             // grow: copy then free the old block
    if (g_mallocs != 2 || g_frees != 1 || v.size() != 2 || v.capacity() != 5) ok = 0;
    if (!Near(v.start[0].Point1.X, 1.0f) || !Near(v.start[0].Point2.Y, 4.0f)) ok = 0;
    if (!Near(v.start[1].Point1.X, 3.0f) || !Near(v.start[1].Point2.Y, 6.0f)) ok = 0;
    std::free(v.start);
    std::printf(ok ? "009c2900_TEST PASS\n" : "009c2900_TEST FAIL\n");
    return ok ? 0 : 1;
}