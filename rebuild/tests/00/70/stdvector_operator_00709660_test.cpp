// Behaviour test for 0x00709660 std::vector<vector<CModifierEntry> >::operator=: the outer STLport operator= over
// owning inner vectors (grow = allocate-and-copy + destroy old; shrink = assign + destroy the surplus; middle =
// assign the live part + copy-construct the rest), deep copies, no leaks, self-assignment untouched
#include <cstdio>
#include <cstdlib>
#include <new>

namespace model {

int g_live = 0;

struct CModifierEntry { int DefIndex; int Flags; };

struct Inner {                                       // vector<CModifierEntry>
    CModifierEntry* s;
    unsigned int n;
    Inner() : s(0), n(0) { ++g_live; }
    Inner(const Inner& o) : s(0), n(0) { ++g_live; copy(o); }
    void copy(const Inner& o)
    {
        CModifierEntry* t = o.n ? (CModifierEntry*)std::malloc(o.n * sizeof(CModifierEntry)) : 0;
        for (unsigned int i = 0; i < o.n; ++i) t[i] = o.s[i];
        if (s) std::free(s);
        s = t;
        n = o.n;
    }
    Inner& operator=(const Inner& o) { if (this != &o) copy(o); return *this; }
    ~Inner() { if (s) std::free(s); --g_live; }
};

struct Outer {
    Inner* s;
    Inner* f;
    Inner* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    unsigned int capacity() const { return (unsigned int)(e - s); }
    void clear_all() { for (Inner* p = s; p != f; ++p) p->~Inner(); if (s) std::free(s); s = f = e = 0; }
    Outer& assign(const Outer& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                Inner* tmp = (Inner*)std::malloc(xlen * sizeof(Inner));
                for (unsigned int i = 0; i < xlen; ++i) new (tmp + i) Inner(x.s[i]);
                clear_all();
                s = tmp;
                e = s + xlen;
            }
            else if (size() >= xlen) {
                Inner* i = s;
                for (unsigned int k = 0; k < xlen; ++k, ++i) *i = x.s[k];
                for (Inner* p = i; p != f; ++p) p->~Inner();
            }
            else {
                unsigned int old = size();
                for (unsigned int k = 0; k < old; ++k) s[k] = x.s[k];
                for (unsigned int k = old; k < xlen; ++k) new (s + k) Inner(x.s[k]);
            }
            f = s + xlen;
        }
        return *this;
    }
};

Outer make(unsigned int n, int base)
{
    Outer o;
    o.s = (Inner*)std::malloc(n * sizeof(Inner));
    o.f = o.s + n;
    o.e = o.s + n;
    for (unsigned int i = 0; i < n; ++i) {
        new (o.s + i) Inner();
        o.s[i].n = i + 1;
        o.s[i].s = (CModifierEntry*)std::malloc((i + 1) * sizeof(CModifierEntry));
        for (unsigned int j = 0; j <= i; ++j) { o.s[i].s[j].DefIndex = base + (int)(i * 10 + j); o.s[i].s[j].Flags = 1 << j; }
    }
    return o;
}
bool same(const Outer& a, const Outer& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i) {
        if (a.s[i].n != b.s[i].n || (a.s[i].n && a.s[i].s == b.s[i].s)) return false;
        for (unsigned int j = 0; j < a.s[i].n; ++j)
            if (a.s[i].s[j].DefIndex != b.s[i].s[j].DefIndex || a.s[i].s[j].Flags != b.s[i].s[j].Flags) return false;
    }
    return true;
}

}

int main()
{
    using namespace model;
    int bad = 0;
    {
        Outer four = make(4, 100), two = make(2, 200), three = make(3, 300);
        Outer d = make(1, 0);
        d.assign(four);                              // grow
        if (!same(d, four) || d.capacity() != 4 || g_live != 4 + 2 + 3 + 4) bad = 1;
        d.assign(two);                               // shrink: surplus destroyed, capacity kept
        if (!same(d, two) || d.capacity() != 4 || g_live != 4 + 2 + 3 + 2) bad = 2;
        d.assign(three);                             // middle
        if (!same(d, three) || d.capacity() != 4 || g_live != 4 + 2 + 3 + 3) bad = 3;
        d.assign(d);
        if (d.size() != 3 || d.s[2].s[2].DefIndex != 322) bad = 4;
        four.clear_all(); two.clear_all(); three.clear_all(); d.clear_all();
    }
    if (g_live != 0) bad = 5;
    if (bad) { std::printf("00709660_TEST FAIL\n"); return 1; }
    std::printf("00709660_TEST PASS\n");
    return 0;
}