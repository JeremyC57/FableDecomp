// Behaviour test for 0x0081B220 CVectorMap<unsigned long,CSkeletalMorphs::CEntry>::Insert (model copy: push_back + Dirty + sort).
#include <cstdio>
#include <cstdlib>
#include <new>
struct CEntry { int MorphName; short Priority; bool SupressesLowerMorphs; };   // CSkeletalMorphs::CEntry, 8 bytes
struct E {
    unsigned int first; CEntry second;
    E(unsigned int k, const CEntry& v) : first(k), second(v) { }
    E(const E& o) : first(o.first), second(o.second) { }
};
struct Vec {
    E* s; E* f; E* e;
    void overflow(const E& x)
    {
        unsigned int old = (unsigned int)(f - s);
        unsigned int len = old + (old > 1u ? old : 1u);
        E* ns = (E*)std::malloc(len * sizeof(E));
        E* nf = ns;
        for (E* p = s; p != f; ++p, ++nf) new (nf) E(*p);
        new (nf) E(x); ++nf;
        if (s) std::free(s);
        s = ns; f = nf; e = ns + len;
    }
    void push_back(const E& x)
    {
        if (f != e) { new (f) E(x); ++f; }
        else overflow(x);
    }
};
static void sort_model(E* first, E* last)
{
    for (E* i = first + 1; i < last; ++i) {
        E v = *i;
        E* j = i;
        while (j != first && v.first < (j - 1)->first) { *j = *(j - 1); --j; }
        *j = v;
    }
}
struct Map {
    Vec Data; unsigned char Compare; bool Dirty;
    void Insert(unsigned int key, const CEntry& value)
    {
        Data.push_back(E(key, value));
        Dirty = true;
        sort_model(Data.s, Data.f);
        Dirty = false;
    }
};
int main()
{
    Map m;
    m.Data.s = 0; m.Data.f = 0; m.Data.e = 0; m.Compare = 0; m.Dirty = true;
    unsigned int keys[5] = { 5u, 2u, 9u, 1u, 7u };
    int i;
    for (i = 0; i < 5; ++i) {
        CEntry v; v.MorphName = (int)keys[i] * 10; v.Priority = (short)-(int)keys[i]; v.SupressesLowerMorphs = (keys[i] & 1u) != 0;
        m.Insert(keys[i], v);
    }
    int ok = (m.Data.f - m.Data.s == 5) && !m.Dirty && (m.Data.e - m.Data.s >= 5);
    unsigned int want[5] = { 1u, 2u, 5u, 7u, 9u };
    for (i = 0; i < 5 && ok; ++i) {
        const E& r = m.Data.s[i];
        if (r.first != want[i] || r.second.MorphName != (int)want[i] * 10 || r.second.Priority != (short)-(int)want[i] || r.second.SupressesLowerMorphs != ((want[i] & 1u) != 0)) ok = 0;
    }
    std::free(m.Data.s);
    if (!ok) { std::printf("0081b220_TEST FAIL\n"); return 1; }
    std::printf("0081b220_TEST PASS\n");
    return 0;
}