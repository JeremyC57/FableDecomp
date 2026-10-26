// Behaviour test for 0x004f9df8 vector<CCardPositionsDef>::_M_fill_insert (STLport insert(pos, n, x)): element order after the in-place
// (elems_after > n and <= n) and the growing paths, and the element lifetime balance (x_copy and every moved
// element constructed/destroyed exactly once).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
class CCardPositionsDef { public:
    int Key;                                         // stands for the def's base-class body
    int OffsetCount; float OffsetX;
    CCardPositionsDef() : Key(0) { ++g_live; }
    CCardPositionsDef(const CCardPositionsDef& o) : Key(o.Key) { Copy(o); ++g_live; }
    CCardPositionsDef& operator=(const CCardPositionsDef& o) { Key = o.Key; Copy(o); return *this; }
    ~CCardPositionsDef() { --g_live; }
    void Copy(const CCardPositionsDef& o);
};
void CCardPositionsDef::Copy(const CCardPositionsDef& o) { OffsetCount = o.OffsetCount; OffsetX = o.OffsetX; }

static CCardPositionsDef Make(int key) { CCardPositionsDef d; d.Key = key; d.OffsetCount = key * 2; d.OffsetX = key * 3; return d; }

struct Vec {
    CCardPositionsDef* start; CCardPositionsDef* finish; CCardPositionsDef* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CCardPositionsDef* pos, const CCardPositionsDef& x, unsigned int fill_len)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CCardPositionsDef* ns = (CCardPositionsDef*)std::malloc(len * sizeof(CCardPositionsDef));
        CCardPositionsDef* nf = ns;
        for (CCardPositionsDef* p = start; p != pos; ++p, ++nf) new (nf) CCardPositionsDef(*p);
        for (unsigned int k = fill_len; k > 0; --k, ++nf) new (nf) CCardPositionsDef(x);
        for (CCardPositionsDef* p = pos; p != finish; ++p, ++nf) new (nf) CCardPositionsDef(*p);
        for (CCardPositionsDef* p = start; p != finish; ++p) p->~CCardPositionsDef();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
    void fill_insert(CCardPositionsDef* pos, unsigned int n, const CCardPositionsDef& x)
    {
        if (n != 0) {
            if ((unsigned int)(eos - finish) >= n) {
                CCardPositionsDef x_copy = x;
                const unsigned int after = (unsigned int)(finish - pos);
                CCardPositionsDef* old_finish = finish;
                if (after > n) {
                    CCardPositionsDef* o = finish;
                    for (CCardPositionsDef* p = finish - n; p != finish; ++p, ++o) new (o) CCardPositionsDef(*p);
                    finish += n;
                    CCardPositionsDef* r = old_finish; CCardPositionsDef* l = old_finish - n;
                    for (int k = (int)(l - pos); k > 0; --k) *--r = *--l;
                    for (CCardPositionsDef* p = pos; p != pos + n; ++p) *p = x_copy;
                } else {
                    CCardPositionsDef* o = finish;
                    for (unsigned int k = n - after; k > 0; --k, ++o) new (o) CCardPositionsDef(x_copy);
                    finish += n - after;
                    o = finish;
                    for (CCardPositionsDef* p = pos; p != old_finish; ++p, ++o) new (o) CCardPositionsDef(*p);
                    finish += after;
                    for (CCardPositionsDef* p = pos; p != old_finish; ++p) *p = x_copy;
                }
            } else
                insert_overflow(pos, x, n);
        }
    }
};

static int check(const Vec& v, const int* want, unsigned int n)
{
    if (v.size() != n) return 0;
    for (unsigned int i = 0; i < n; ++i) {
        if (v.start[i].Key != want[i]) return 0;
        if (!(v.start[i].OffsetCount == v.start[i].Key * 2 && v.start[i].OffsetX == v.start[i].Key * 3)) return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    {
        Vec v; v.start = v.finish = v.eos = 0;
        CCardPositionsDef one = Make(1), two = Make(2), three = Make(3);
        v.fill_insert(v.finish, 0, one);                    // n == 0: no-op
        if (v.start != 0) ok = 0;
        v.fill_insert(v.finish, 4, one);                    // grows: [1 1 1 1], capacity 4
        { int w[] = { 1, 1, 1, 1 }; if (!check(v, w, 4) || v.eos - v.start != 4) ok = 0; }
        v.fill_insert(v.start + 2, 2, two);                 // grows: capacity 4 + 4
        { int w[] = { 1, 1, 2, 2, 1, 1 }; if (!check(v, w, 6) || v.eos - v.start != 8) ok = 0; }
        v.start[5] = Make(9);
        v.fill_insert(v.start + 3, 1, three);               // in place, elems_after (3) > n (1)
        { int w[] = { 1, 1, 2, 3, 2, 1, 9 }; if (!check(v, w, 7)) ok = 0; }
        v.fill_insert(v.start + 6, 1, two);                 // in place, elems_after (1) == n (1)
        { int w[] = { 1, 1, 2, 3, 2, 1, 2, 9 }; if (!check(v, w, 8) || v.eos - v.start != 8) ok = 0; }
        if (g_live != 8 + 3) ok = 0;                        // the 8 elements and the 3 named values
        for (CCardPositionsDef* p = v.start; p != v.finish; ++p) p->~CCardPositionsDef();
        std::free(v.start);
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("004f9df8_TEST FAIL\n"); return 1; }
    std::printf("004f9df8_TEST PASS\n");
    return 0;
}