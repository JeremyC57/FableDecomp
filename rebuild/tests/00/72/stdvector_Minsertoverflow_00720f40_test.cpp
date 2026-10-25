// Behaviour test for 0x00720F40 std::vector<pair<CSkeletalMorphs::CEntry, float> >::_M_insert_overflow: growth (size + max(size, n)), single and multi-copy insertion in the middle, copy/destroy balance
#include <cstdio>
#include <cstdlib>
#include <new>

namespace model {

// pair<CSkeletalMorphs::CEntry, float>: def-string position, priority, flag and the blend weight (trivially destructible).
struct Elem {
    long  MorphName;
    short Priority;
    bool  SupressesLowerMorphs;
    float Weight;
};
Elem make(long k) { Elem e; e.MorphName = k; e.Priority = (short)(k * 2); e.SupressesLowerMorphs = (k & 1) != 0; e.Weight = (float)k / 4.0f; return e; }
long key(const Elem& x) { return x.MorphName; }
bool check(const Elem* p, unsigned int n, const Elem&, const Elem&, const Elem&)
{
    for (unsigned int i = 0; i < n; ++i) {
        float w = p[i].Weight - (float)p[i].MorphName / 4.0f;
        if (w < -0.0001f || w > 0.0001f) return false;
        if (p[i].Priority != (short)(p[i].MorphName * 2) || p[i].SupressesLowerMorphs != ((p[i].MorphName & 1) != 0)) return false;
    }
    return true;
}
bool final_check() { return true; }

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
    if (bad) { std::printf("00720f40_TEST FAIL\n"); return 1; }
    std::printf("00720f40_TEST PASS\n");
    return 0;
}