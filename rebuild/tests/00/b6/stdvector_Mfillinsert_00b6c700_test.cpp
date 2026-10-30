// Behaviour test for 0x00b6c700 std::vector<CLandscapeTheme>::_M_fill_insert (56-byte elements) (STLport 4.6 vector::_M_fill_insert: in-capacity slide + fill, else _M_insert_overflow).
#include <cstdio>
#include <cstdlib>

struct E { int w[14]; };                                 // element stand-in, copied memberwise
static E make(int v) { E e; for (int i = 0; i < 14; ++i) e.w[i] = v * 100 + i; return e; }
static bool same(const E& a, const E& b) { for (int i = 0; i < 14; ++i) if (a.w[i] != b.w[i]) return false; return true; }

struct Vec {
    E* start; E* finish; E* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(E* pos, const E& x, unsigned int n)
    {
        unsigned int old_size = size(), len = old_size + (old_size < n ? n : old_size);
        E* ns = (E*)malloc(len * sizeof(E)); E* nf = ns;
        for (E* p = start; p != pos; ++p) *nf++ = *p;
        for (unsigned int i = 0; i < n; ++i) *nf++ = x;
        for (E* p = pos; p != finish; ++p) *nf++ = *p;
        free(start); start = ns; finish = nf; eos = ns + len;
    }
    void fill_insert(E* pos, unsigned int n, const E& x)
    {
        if (n == 0) return;
        if ((unsigned int)(eos - finish) >= n) {
            E x_copy = x;
            unsigned int elems_after = (unsigned int)(finish - pos);
            E* old_finish = finish;
            if (elems_after > n) {
                for (E *s = finish - n, *d = finish; s != old_finish; ++s, ++d) *d = *s;
                finish += n;
                for (E *s = old_finish - n, *d = old_finish; s != pos; ) *--d = *--s;
                for (E* p = pos; p != pos + n; ++p) *p = x_copy;
            } else {
                for (unsigned int i = 0; i < n - elems_after; ++i) finish[i] = x_copy;
                finish += n - elems_after;
                for (E *s = pos, *d = finish; s != old_finish; ++s, ++d) *d = *s;
                finish += elems_after;
                for (E* p = pos; p != old_finish; ++p) *p = x_copy;
            }
        } else
            insert_overflow(pos, x, n);
    }
};

static int fail() { std::printf("00b6c700_TEST FAIL\n"); return 1; }

int main()
{
    Vec v; v.start = (E*)malloc(8 * sizeof(E)); v.finish = v.start; v.eos = v.start + 8;
    for (int i = 0; i < 4; ++i) *v.finish++ = make(i);         // 0 1 2 3, capacity 8
    v.fill_insert(v.start + 1, 2, make(9));                    // elems_after 3 > 2: 0 9 9 1 2 3
    int want1[] = { 0, 9, 9, 1, 2, 3 };
    if (v.size() != 6) return fail();
    for (int i = 0; i < 6; ++i) if (!same(v.start[i], make(want1[i]))) return fail();
    v.fill_insert(v.start + 5, 2, make(7));                    // elems_after 1 <= 2: 0 9 9 1 2 7 7 3
    int want2[] = { 0, 9, 9, 1, 2, 7, 7, 3 };
    if (v.size() != 8 || v.eos - v.start != 8) return fail();
    for (int i = 0; i < 8; ++i) if (!same(v.start[i], make(want2[i]))) return fail();
    v.fill_insert(v.start, 0, make(5));                        // n == 0: no change
    if (v.size() != 8) return fail();
    v.fill_insert(v.start + 2, 3, make(4));                    // overflow: capacity 8 + 8
    int want3[] = { 0, 9, 4, 4, 4, 9, 1, 2, 7, 7, 3 };
    if (v.size() != 11 || v.eos - v.start != 16) return fail();
    for (int i = 0; i < 11; ++i) if (!same(v.start[i], make(want3[i]))) return fail();
    free(v.start);
    std::printf("00b6c700_TEST PASS\n");
    return 0;
}