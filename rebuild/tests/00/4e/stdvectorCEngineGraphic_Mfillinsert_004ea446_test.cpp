// Behaviour test for 0x004EA446 vector<CEngineGraphic>::_M_fill_insert (STLport insert(pos, n, x)).
#include <cstdio>
#include <cstdlib>
#include <new>

#pragma pack(push, 1)
class CEngineGraphic { public:
    unsigned int  ByteLength;      // BankIndex
    float         AnimStep;
    float         RenderSizeX;
    bool          AdditiveAlpha;
    unsigned char AllocationSource; // Type
};
#pragma pack(pop)
static int g_live = 0;
static CEngineGraphic MakeG(int v) { CEngineGraphic g; g.ByteLength = v; g.AnimStep = 0.5f; g.RenderSizeX = 1.0f; g.AdditiveAlpha = true; g.AllocationSource = 1; return g; }

struct Vec {
    CEngineGraphic* start; CEngineGraphic* finish; CEngineGraphic* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CEngineGraphic* pos, const CEngineGraphic& x, unsigned int fill_len)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CEngineGraphic* ns = len ? (CEngineGraphic*)std::malloc(len * sizeof(CEngineGraphic)) : 0;
        CEngineGraphic* nf = ns;
        for (CEngineGraphic* p = start; p != pos; ++p, ++nf) new (nf) CEngineGraphic(*p);
        for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CEngineGraphic(x);
        for (CEngineGraphic* p = pos; p != finish; ++p, ++nf) new (nf) CEngineGraphic(*p);
        
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
    void fill_insert(CEngineGraphic* pos, unsigned int n, const CEngineGraphic& x)
    {
        if (n == 0) return;
        if ((unsigned int)(eos - finish) >= n) {
            CEngineGraphic x_copy = x;
            unsigned int after = (unsigned int)(finish - pos);
            CEngineGraphic* old_finish = finish;
            if (after > n) {
                CEngineGraphic* o = finish;
                for (CEngineGraphic* p = finish - n; p != finish; ++p, ++o) new (o) CEngineGraphic(*p);
                finish += n;
                CEngineGraphic* r = old_finish; CEngineGraphic* l = old_finish - n;
                for (int k = (int)(l - pos); k > 0; --k) *--r = *--l;
                for (CEngineGraphic* p = pos; p != pos + n; ++p) *p = x_copy;
            } else {
                CEngineGraphic* o = finish;
                for (unsigned int k = n - after; k > 0; --k, ++o) new (o) CEngineGraphic(x_copy);
                finish += n - after;
                o = finish;
                for (CEngineGraphic* p = pos; p != old_finish; ++p, ++o) new (o) CEngineGraphic(*p);
                finish += after;
                for (CEngineGraphic* p = pos; p != old_finish; ++p) *p = x_copy;
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
    if (sizeof(CEngineGraphic) != 14) ok = 0;
    Vec v; v.start = v.finish = v.eos = 0;
    CEngineGraphic one = MakeG(1), two = MakeG(2), three = MakeG(3);
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
    
    
    std::free(v.start);
    
    if (!ok) { std::printf("004ea446_TEST FAIL\n"); return 1; }
    std::printf("004ea446_TEST PASS\n");
    return 0;
}