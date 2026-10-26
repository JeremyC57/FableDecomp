// Behaviour test for 0x004fa277 vector<CCardDef>::_M_fill_insert (STLport insert(pos, n, x)): element order after the in-place
// (elems_after > n and <= n) and the growing paths, and the element lifetime balance (x_copy and every moved
// element constructed/destroyed exactly once).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
class CCardDef { public:
    int Key;                                         // stands for the def's base-class body
    long CardName; long CardVal;
    CCardDef() : Key(0) { ++g_live; }
    CCardDef(const CCardDef& o) : Key(o.Key) { Copy(o); ++g_live; }
    CCardDef& operator=(const CCardDef& o) { Key = o.Key; Copy(o); return *this; }
    ~CCardDef() { --g_live; }
    void Copy(const CCardDef& o);
};
void CCardDef::Copy(const CCardDef& o) { CardName = o.CardName; CardVal = o.CardVal; }

static CCardDef Make(int key) { CCardDef d; d.Key = key; d.CardName = key * 2; d.CardVal = key * 3; return d; }

struct Vec {
    CCardDef* start; CCardDef* finish; CCardDef* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CCardDef* pos, const CCardDef& x, unsigned int fill_len)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CCardDef* ns = (CCardDef*)std::malloc(len * sizeof(CCardDef));
        CCardDef* nf = ns;
        for (CCardDef* p = start; p != pos; ++p, ++nf) new (nf) CCardDef(*p);
        for (unsigned int k = fill_len; k > 0; --k, ++nf) new (nf) CCardDef(x);
        for (CCardDef* p = pos; p != finish; ++p, ++nf) new (nf) CCardDef(*p);
        for (CCardDef* p = start; p != finish; ++p) p->~CCardDef();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
    void fill_insert(CCardDef* pos, unsigned int n, const CCardDef& x)
    {
        if (n != 0) {
            if ((unsigned int)(eos - finish) >= n) {
                CCardDef x_copy = x;
                const unsigned int after = (unsigned int)(finish - pos);
                CCardDef* old_finish = finish;
                if (after > n) {
                    CCardDef* o = finish;
                    for (CCardDef* p = finish - n; p != finish; ++p, ++o) new (o) CCardDef(*p);
                    finish += n;
                    CCardDef* r = old_finish; CCardDef* l = old_finish - n;
                    for (int k = (int)(l - pos); k > 0; --k) *--r = *--l;
                    for (CCardDef* p = pos; p != pos + n; ++p) *p = x_copy;
                } else {
                    CCardDef* o = finish;
                    for (unsigned int k = n - after; k > 0; --k, ++o) new (o) CCardDef(x_copy);
                    finish += n - after;
                    o = finish;
                    for (CCardDef* p = pos; p != old_finish; ++p, ++o) new (o) CCardDef(*p);
                    finish += after;
                    for (CCardDef* p = pos; p != old_finish; ++p) *p = x_copy;
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
        if (!(v.start[i].CardName == v.start[i].Key * 2 && v.start[i].CardVal == v.start[i].Key * 3)) return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    {
        Vec v; v.start = v.finish = v.eos = 0;
        CCardDef one = Make(1), two = Make(2), three = Make(3);
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
        for (CCardDef* p = v.start; p != v.finish; ++p) p->~CCardDef();
        std::free(v.start);
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("004fa277_TEST FAIL\n"); return 1; }
    std::printf("004fa277_TEST PASS\n");
    return 0;
}