// Behaviour test for 0x004edb3e vector<CPrizeScoreDef>::_M_fill_insert (STLport insert(pos, n, x)): element order after the in-place
// (elems_after > n and <= n) and the growing paths, and the element lifetime balance (x_copy and every moved
// element constructed/destroyed exactly once).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct CPrizeScoreDef {
    int Key;                                         // stands for the def's base-class body
    float Score; long Mult;
    CPrizeScoreDef() : Key(0) { ++g_live; }
    CPrizeScoreDef(const CPrizeScoreDef& o) : Key(o.Key) { Copy(o); ++g_live; }
    CPrizeScoreDef& operator=(const CPrizeScoreDef& o) { Key = o.Key; Copy(o); return *this; }
    ~CPrizeScoreDef() { --g_live; }
    void Copy(const CPrizeScoreDef& o);
};
void CPrizeScoreDef::Copy(const CPrizeScoreDef& o) { Score = o.Score; Mult = o.Mult; }

static CPrizeScoreDef Make(int key) { CPrizeScoreDef d; d.Key = key; d.Score = key * 2; d.Mult = key * 3; return d; }

struct Vec {
    CPrizeScoreDef* start; CPrizeScoreDef* finish; CPrizeScoreDef* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CPrizeScoreDef* pos, const CPrizeScoreDef& x, unsigned int fill_len)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CPrizeScoreDef* ns = (CPrizeScoreDef*)std::malloc(len * sizeof(CPrizeScoreDef));
        CPrizeScoreDef* nf = ns;
        for (CPrizeScoreDef* p = start; p != pos; ++p, ++nf) new (nf) CPrizeScoreDef(*p);
        for (unsigned int k = fill_len; k > 0; --k, ++nf) new (nf) CPrizeScoreDef(x);
        for (CPrizeScoreDef* p = pos; p != finish; ++p, ++nf) new (nf) CPrizeScoreDef(*p);
        for (CPrizeScoreDef* p = start; p != finish; ++p) p->~CPrizeScoreDef();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
    void fill_insert(CPrizeScoreDef* pos, unsigned int n, const CPrizeScoreDef& x)
    {
        if (n != 0) {
            if ((unsigned int)(eos - finish) >= n) {
                CPrizeScoreDef x_copy = x;
                const unsigned int after = (unsigned int)(finish - pos);
                CPrizeScoreDef* old_finish = finish;
                if (after > n) {
                    CPrizeScoreDef* o = finish;
                    for (CPrizeScoreDef* p = finish - n; p != finish; ++p, ++o) new (o) CPrizeScoreDef(*p);
                    finish += n;
                    CPrizeScoreDef* r = old_finish; CPrizeScoreDef* l = old_finish - n;
                    for (int k = (int)(l - pos); k > 0; --k) *--r = *--l;
                    for (CPrizeScoreDef* p = pos; p != pos + n; ++p) *p = x_copy;
                } else {
                    CPrizeScoreDef* o = finish;
                    for (unsigned int k = n - after; k > 0; --k, ++o) new (o) CPrizeScoreDef(x_copy);
                    finish += n - after;
                    o = finish;
                    for (CPrizeScoreDef* p = pos; p != old_finish; ++p, ++o) new (o) CPrizeScoreDef(*p);
                    finish += after;
                    for (CPrizeScoreDef* p = pos; p != old_finish; ++p) *p = x_copy;
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
        if (!(v.start[i].Score == v.start[i].Key * 2 && v.start[i].Mult == v.start[i].Key * 3)) return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    {
        Vec v; v.start = v.finish = v.eos = 0;
        CPrizeScoreDef one = Make(1), two = Make(2), three = Make(3);
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
        for (CPrizeScoreDef* p = v.start; p != v.finish; ++p) p->~CPrizeScoreDef();
        std::free(v.start);
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("004edb3e_TEST FAIL\n"); return 1; }
    std::printf("004edb3e_TEST PASS\n");
    return 0;
}