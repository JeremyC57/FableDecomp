// Behaviour test for 0x00759190 std::vector<pair<EBodyLocation, CWoundMorphs::CEntry> >::_M_insert_overflow: growth (size + max(size, n)), single and multi-copy insertion in the middle, copy/destroy balance
#include <cstdio>
#include <cstdlib>
#include <new>

namespace model {

// pair<EBodyLocation, CWoundMorphs::CEntry>: the key location + the 40-byte wound entry (44 bytes of plain data).
struct CEntry {                                  // int: 32-bit fields under both g++ (LP64) and VC7.1
    int   Location;
    int   ReplacingTextureIndex, ReplacingBumpMapIndex, WoundTextureIndex, WoundBumpMapIndex;
    int   ScarTextureIndex, ScarBumpMapIndex, FadeToScarDuration, TotalDuration;
    float PermanentScarStrength;
};
struct Elem { int first; CEntry second; };
Elem make(long lk)
{
    int k = (int)lk;
    Elem e;
    e.first = k;
    e.second.Location = k;
    e.second.ReplacingTextureIndex = k + 1; e.second.ReplacingBumpMapIndex = k + 2;
    e.second.WoundTextureIndex = k + 3; e.second.WoundBumpMapIndex = k + 4;
    e.second.ScarTextureIndex = k + 5; e.second.ScarBumpMapIndex = k + 6;
    e.second.FadeToScarDuration = k * 100; e.second.TotalDuration = k * 200;
    e.second.PermanentScarStrength = (float)k * 0.125f;
    return e;
}
long key(const Elem& x) { return x.first; }
bool check(const Elem* p, unsigned int n, const Elem&, const Elem&, const Elem&)
{
    if (sizeof(Elem) != 44u) return false;
    for (unsigned int i = 0; i < n; ++i) {
        const CEntry& c = p[i].second;
        if (c.Location != p[i].first || c.ScarBumpMapIndex != p[i].first + 6 || c.TotalDuration != p[i].first * 200) return false;
        float d = c.PermanentScarStrength - (float)p[i].first * 0.125f;
        if (d < -0.0001f || d > 0.0001f) return false;
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
    if (bad) { std::printf("00759190_TEST FAIL\n"); return 1; }
    std::printf("00759190_TEST PASS\n");
    return 0;
}