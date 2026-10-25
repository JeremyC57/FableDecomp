// Behaviour test for 0x00709D60 std::vector<pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet> >::_M_insert_overflow: growth (size + max(size, n)), single and multi-copy insertion in the middle, copy/destroy balance
#include <cstdio>
#include <cstdlib>
#include <new>

namespace model {

// pair<CMeshInfo, CEngineTextureMorphSet>: the morph set owns an array that every copy duplicates (24 bytes).
int g_live = 0;
struct Elem {
    long  Index;
    float SizeX;
    unsigned char Primary;
    long* Morphs;
    long  MorphCount;
    explicit Elem(long k) : Index(k), SizeX((float)k), Primary(1), Morphs(new long[2]), MorphCount(2) { Morphs[0] = k; Morphs[1] = k * 10; ++g_live; }
    Elem(const Elem& o) : Index(o.Index), SizeX(o.SizeX), Primary(o.Primary), Morphs(new long[o.MorphCount]), MorphCount(o.MorphCount)
    { for (long i = 0; i < MorphCount; ++i) Morphs[i] = o.Morphs[i]; ++g_live; }
    ~Elem() { delete[] Morphs; --g_live; }
};
Elem make(long k) { return Elem(k); }
long key(const Elem& x) { return x.Index; }
bool check(const Elem* p, unsigned int n, const Elem&, const Elem&, const Elem& c)
{
    for (unsigned int i = 0; i < n; ++i) {
        if (p[i].Morphs[1] != p[i].Index * 10) return false;
        if (p[i].Morphs == c.Morphs) return false;       // deep copies
    }
    return g_live == 8;                                // 5 in the vector + a, b, c
}
bool final_check() { return g_live == 0; }

struct Vec {
    Elem* s;
    Elem* f;
    Elem* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    unsigned int capacity() const { return (unsigned int)(e - s); }
    // STLport _M_insert_overflow: len = size + max(size, n); copy head, construct x (once or n times), copy tail
    // unless atend, destroy + free the old block, adopt the new one.
    void insert_overflow(Elem* pos, const Elem& x, unsigned int n, bool atend)
    {
        unsigned int old = size();
        unsigned int len = old + (old < n ? n : old);
        Elem* ns = len != 0 ? (Elem*)std::malloc(len * sizeof(Elem)) : 0;
        Elem* nf = ns;
        for (Elem* p = s; p != pos; ++p, ++nf)
            new (nf) Elem(*p);
        if (n == 1) {
            new (nf) Elem(x);
            ++nf;
        }
        else {
            for (unsigned int i = 0; i < n; ++i, ++nf)
                new (nf) Elem(x);
        }
        if (!atend)
            for (Elem* p = pos; p != f; ++p, ++nf)
                new (nf) Elem(*p);
        for (Elem* p = s; p != f; ++p)
            p->~Elem();
        if (s)
            std::free(s);
        s = ns;
        f = nf;
        e = ns + len;
    }
    void destroy()
    {
        for (Elem* p = s; p != f; ++p)
            p->~Elem();
        if (s)
            std::free(s);
        s = f = e = 0;
    }
};

}

int main()
{
    using namespace model;
    int bad = 0;
    Vec v;
    v.s = v.f = v.e = 0;
    {
        Elem a = make(1), b = make(2), c = make(3);
        v.insert_overflow(v.f, a, 1, true);                 // empty -> capacity 1
        if (v.size() != 1 || v.capacity() != 1 || key(v.s[0]) != 1) bad = 1;
        v.insert_overflow(v.f, b, 1, true);                 // capacity 2
        if (v.size() != 2 || v.capacity() != 2 || key(v.s[1]) != 2) bad = 2;
        v.insert_overflow(v.s + 1, c, 3, false);            // 3 copies in the middle: capacity 2 + max(2, 3)
        if (v.size() != 5 || v.capacity() != 5) bad = 3;
        long want[5] = { 1, 3, 3, 3, 2 };
        for (int i = 0; i < 5; ++i)
            if (key(v.s[i]) != want[i]) bad = 4;
        if (!check(v.s, v.size(), a, b, c)) bad = 5;
        v.destroy();
    }
    if (!final_check()) bad = 6;
    if (bad) { std::printf("00709d60_TEST FAIL\n"); return 1; }
    std::printf("00709d60_TEST PASS\n");
    return 0;
}