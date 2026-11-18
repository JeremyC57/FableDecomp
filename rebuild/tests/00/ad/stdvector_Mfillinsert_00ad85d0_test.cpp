// Behaviour test for vector<COptimisedVertex>::_M_fill_insert 0x00AD85D0 (NF68: restated after the port onto stlport/fable_containers.h):
// the STLport 4.6 fill-insert logic over a 12-slot buffer of an owning element type (a neutral stand-in, not the engine class), checked
// for the tail-longer case, the tail-shorter case, inserting a copy of an element of the same vector, n == 0, and the overflow path.
#include <cstdio>
#include <cstdlib>
#include <new>

static int live = 0, overflowCalls = 0;
struct Vertex {
    int Index; int* Owned;
    Vertex() : Index(0), Owned(new int(0)) { ++live; }
    Vertex(const Vertex& o) : Index(o.Index), Owned(new int(*o.Owned)) { ++live; }
    Vertex& operator=(const Vertex& o) { Index = o.Index; *Owned = *o.Owned; return *this; }
    ~Vertex() { delete Owned; --live; }
};
struct Vec {
    Vertex* _M_start; Vertex* _M_finish; Vertex* _M_end_of_storage;
    void _M_insert_overflow(Vertex*, const Vertex&, unsigned int) { ++overflowCalls; }
    void _M_fill_insert(Vertex* pos, unsigned int n, const Vertex& x)
    {
        if (n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                Vertex x_copy = x;
                const unsigned int after = _M_finish - pos;
                Vertex* old_finish = _M_finish;
                if (after > n) {
                    for (Vertex *s = _M_finish - n, *d = _M_finish; s != _M_finish; ++s, ++d) new (d) Vertex(*s);
                    _M_finish += n;
                    for (Vertex *s = old_finish - n, *d = old_finish; s != pos; ) *--d = *--s;
                    for (Vertex* p = pos; p != pos + n; ++p) *p = x_copy;
                } else {
                    for (unsigned int k = 0; k < n - after; ++k) new (_M_finish + k) Vertex(x_copy);
                    _M_finish += n - after;
                    for (Vertex *s = pos, *d = _M_finish; s != old_finish; ++s, ++d) new (d) Vertex(*s);
                    _M_finish += after;
                    for (Vertex* p = pos; p != old_finish; ++p) *p = x_copy;
                }
            } else _M_insert_overflow(pos, x, n);
        }
    }
};
int main()
{
    {
        Vertex* a = (Vertex*)std::malloc(12 * sizeof(Vertex));
        for (int i = 0; i < 5; ++i) { new (a + i) Vertex(); a[i].Index = i + 1; }
        Vertex x; x.Index = 99;
        Vec v = { a, a + 5, a + 12 };
        v._M_fill_insert(a + 1, 2, x);
        int want[] = { 1, 99, 99, 2, 3, 4, 5 };
        for (int j = 0; j < 7; ++j) if (a[j].Index != want[j]) { std::printf("00ad85d0_TEST FAIL\n"); return 1; }
        for (int k = 0; k < 7; ++k) a[k].~Vertex();
        for (int q = 0; q < 3; ++q) { new (a + q) Vertex(); a[q].Index = q + 1; }
        v._M_finish = a + 3;
        v._M_fill_insert(a + 2, 4, x);
        if (v._M_finish - a != 7 || a[6].Index != 3 || a[2].Index != 99) { std::printf("00ad85d0_TEST FAIL\n"); return 1; }
        v._M_fill_insert(a + 2, 1, a[0]);
        if (v._M_finish - a != 8 || a[2].Index != 1) { std::printf("00ad85d0_TEST FAIL\n"); return 1; }
        v._M_fill_insert(a, 0, x);
        v._M_end_of_storage = v._M_finish;
        v._M_fill_insert(a, 1, x);
        if (overflowCalls != 1) { std::printf("00ad85d0_TEST FAIL\n"); return 1; }
        for (int z = 0; z < 8; ++z) a[z].~Vertex();
        std::free(a);
    }
    if (live != 0) { std::printf("00ad85d0_TEST FAIL\n"); return 1; }
    std::printf("00ad85d0_TEST PASS\n");
    return 0;
}
