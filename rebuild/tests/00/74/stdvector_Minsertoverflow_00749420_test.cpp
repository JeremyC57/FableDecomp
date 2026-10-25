// Behaviour test for 0x00749420 std::vector<pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> >::_M_insert_overflow: growth (size + max(size, n)), single and multi-copy insertion in the middle, copy/destroy balance
#include <cstdio>
#include <cstdlib>
#include <new>

namespace model {

// pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>: the registered thing pointer + {DangerStatus, Timer}.
struct CThing { long Id; long Pointers; };
struct Elem {
    const CThing* Item;
    long  DangerStatus;
    long  Timer;
    Elem(const CThing* t, long status, long timer) : Item(t), DangerStatus(status), Timer(timer) { if (t) ++((CThing*)t)->Pointers; }
    Elem(const Elem& o) : Item(o.Item), DangerStatus(o.DangerStatus), Timer(o.Timer) { if (Item) ++((CThing*)Item)->Pointers; }
    ~Elem() { if (Item) --((CThing*)Item)->Pointers; }
};
CThing g_things[4];
Elem make(long k) { g_things[k].Id = k; g_things[k].Pointers = 0; return Elem(&g_things[k], k + 10, k * 3); }
long key(const Elem& x) { return x.Item->Id; }
bool check(const Elem* p, unsigned int n, const Elem&, const Elem&, const Elem&)
{
    for (unsigned int i = 0; i < n; ++i)
        if (p[i].DangerStatus != p[i].Item->Id + 10 || p[i].Timer != p[i].Item->Id * 3) return false;
    return g_things[3].Pointers == 4 && g_things[1].Pointers == 2;
}
bool final_check() { return g_things[1].Pointers == 0 && g_things[3].Pointers == 0; }

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
    if (bad) { std::printf("00749420_TEST FAIL\n"); return 1; }
    std::printf("00749420_TEST PASS\n");
    return 0;
}