// Behaviour test for 0x0049B5A0 vector<CTexture>::_M_fill_insert (STLport insert(pos, n, x)).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct CTexture {
    void* PD3DTexture;
    unsigned int ByteLength : 28;
    unsigned int AllocationSource : 4;
    CTexture(int v) : PD3DTexture(0), ByteLength(v), AllocationSource(2) { ++g_live; }
    CTexture(const CTexture& o) : PD3DTexture(o.PD3DTexture), ByteLength(o.ByteLength), AllocationSource(o.AllocationSource) { ++g_live; }
    CTexture& operator=(const CTexture& o) { PD3DTexture = o.PD3DTexture; ByteLength = o.ByteLength; AllocationSource = o.AllocationSource; return *this; }
    ~CTexture() { --g_live; }
};

struct Vec {
    CTexture* start; CTexture* finish; CTexture* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CTexture* pos, const CTexture& x, unsigned int fill_len)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CTexture* ns = len ? (CTexture*)std::malloc(len * sizeof(CTexture)) : 0;
        CTexture* nf = ns;
        for (CTexture* p = start; p != pos; ++p, ++nf) new (nf) CTexture(*p);
        for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CTexture(x);
        for (CTexture* p = pos; p != finish; ++p, ++nf) new (nf) CTexture(*p);
        for (CTexture* p = start; p != finish; ++p) p->~CTexture();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
    void fill_insert(CTexture* pos, unsigned int n, const CTexture& x)
    {
        if (n == 0) return;
        if ((unsigned int)(eos - finish) >= n) {
            CTexture x_copy = x;
            unsigned int after = (unsigned int)(finish - pos);
            CTexture* old_finish = finish;
            if (after > n) {
                CTexture* o = finish;
                for (CTexture* p = finish - n; p != finish; ++p, ++o) new (o) CTexture(*p);
                finish += n;
                CTexture* r = old_finish; CTexture* l = old_finish - n;
                for (int k = (int)(l - pos); k > 0; --k) *--r = *--l;
                for (CTexture* p = pos; p != pos + n; ++p) *p = x_copy;
            } else {
                CTexture* o = finish;
                for (unsigned int k = n - after; k > 0; --k, ++o) new (o) CTexture(x_copy);
                finish += n - after;
                o = finish;
                for (CTexture* p = pos; p != old_finish; ++p, ++o) new (o) CTexture(*p);
                finish += after;
                for (CTexture* p = pos; p != old_finish; ++p) *p = x_copy;
            }
        } else
            insert_overflow(pos, x, n);
    }
};

static int check(const Vec& v, const unsigned int* want, unsigned int n)
{
    if (v.size() != n) return 0;
    for (unsigned int i = 0; i < n; ++i) if (v.start[i].ByteLength != want[i]) return 0;
    return 1;
}

int main()
{
    int ok = 1;
    Vec v; v.start = v.finish = v.eos = 0;
    CTexture one(1), two(2), three(3);
    v.fill_insert(v.finish, 0, one);                        // n == 0: no-op
    if (v.start != 0) ok = 0;
    v.fill_insert(v.finish, 4, one);                        // overflow: [1 1 1 1], cap 4
    { unsigned int w[] = { 1, 1, 1, 1 }; if (!check(v, w, 4) || v.eos - v.start != 4) ok = 0; }
    v.fill_insert(v.start + 2, 2, two);                     // overflow: cap 4+4 = 8
    { unsigned int w[] = { 1, 1, 2, 2, 1, 1 }; if (!check(v, w, 6) || v.eos - v.start != 8) ok = 0; }
    v.start[5].ByteLength = 9;
    v.fill_insert(v.start + 3, 1, three);                   // in place, elems_after (3) > n (1)
    { unsigned int w[] = { 1, 1, 2, 3, 2, 1, 9 }; if (!check(v, w, 7)) ok = 0; }
    v.fill_insert(v.start + 6, 1, two);                     // in place, elems_after (1) == n (1)
    { unsigned int w[] = { 1, 1, 2, 3, 2, 1, 2, 9 }; if (!check(v, w, 8) || v.eos - v.start != 8) ok = 0; }
    if (g_live != 3 + 8) ok = 0;
    for (CTexture* p = v.start; p != v.finish; ++p) p->~CTexture();
    std::free(v.start);
    if (g_live != 3) ok = 0;
    if (!ok) { std::printf("0049b5a0_TEST FAIL\n"); return 1; }
    std::printf("0049b5a0_TEST PASS\n");
    return 0;
}