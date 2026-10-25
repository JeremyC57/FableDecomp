// Behaviour test for 0x00A80980 std::vector<CNavigationPosition>::reserve: grows only when capacity < n, keeps the size and the
// element values, frees the old block; an empty (null) vector just allocates.
#include <cstdio>
#include <cstdlib>
#include <new>

struct CNavigationPosition { float x0, y0, x1; };   // 12 bytes (x, y, layer as a float slot)
static int g_frees = 0;
struct SVec {
    CNavigationPosition* start; CNavigationPosition* finish; CNavigationPosition* end;
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(end - start); }
    CNavigationPosition* allocate(unsigned int n) { return n != 0 ? (CNavigationPosition*)std::malloc(n * sizeof(CNavigationPosition)) : 0; }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old = size();
            CNavigationPosition* tmp;
            if (start) {
                tmp = allocate(n);
                CNavigationPosition* cur = tmp;
                for (CNavigationPosition* f = start; f != finish; ++f, ++cur) if (cur) new (cur) CNavigationPosition(*f);
                if (start) { std::free(start); ++g_frees; }
            }
            else
                tmp = allocate(n);
            start = tmp; finish = tmp + old; end = tmp + n;
        }
    }
};

int main()
{
    SVec v; v.start = v.finish = v.end = 0;
    v.reserve(2);
    if (v.capacity() != 2 || v.size() != 0 || g_frees != 0) { std::printf("00a80980_TEST FAIL\n"); return 1; }
    v.start[0].x0 = 1.0f; v.start[0].y0 = 4.0f; v.start[1].x1 = 7.0f; v.finish = v.start + 2;
    CNavigationPosition* before = v.start;
    v.reserve(1);
    if (v.start != before || v.capacity() != 2) { std::printf("00a80980_TEST FAIL\n"); return 1; }
    v.reserve(5);
    if (v.capacity() != 5 || v.size() != 2 || g_frees != 1 || v.start[0].x0 != 1.0f || v.start[0].y0 != 4.0f || v.start[1].x1 != 7.0f) {
        std::printf("00a80980_TEST FAIL\n"); return 1;
    }
    std::free(v.start);
    std::printf("00a80980_TEST PASS\n");
    return 0;
}