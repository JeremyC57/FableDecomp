#include <cstdio>
#include <new>
// Behaviour copy of CEnvironmentThemeDefVector::operator= (0x00472eda): STLport vector assignment with a non-trivial element.
static int g_live = 0, g_assigns = 0, g_copies = 0;
struct Def {
    int v;
    Def(int x) : v(x) { ++g_live; }
    Def(const Def& o) : v(o.v) { ++g_live; ++g_copies; }
    Def& operator=(const Def& o) { v = o.v; ++g_assigns; return *this; }
    ~Def() { --g_live; }
};
struct Vec {
    Def* s; Def* f; Def* e;
    Vec() : s(0), f(0), e(0) {}
    ~Vec() { clear(); ::operator delete(s); }
    void clear() { for (Def* p = s; p != f; ++p) p->~Def(); f = s; }
    void push(const Def& d) {
        if (f == e) { unsigned n = (unsigned)(e - s); unsigned c = n ? n * 2 : 1; Def* ns = (Def*)::operator new(c * sizeof(Def)); Def* nf = ns;
            for (Def* p = s; p != f; ++p, ++nf) new (nf) Def(*p); clear(); ::operator delete(s); s = ns; f = nf; e = ns + c; }
        new (f) Def(d); ++f;
    }
    unsigned size() const { return (unsigned)(f - s); }
    unsigned capacity() const { return (unsigned)(e - s); }
    Vec& operator=(const Vec& x) {
        if (&x != this) {
            unsigned xlen = x.size();
            if (xlen > capacity()) {
                Def* ns = (Def*)::operator new(xlen * sizeof(Def)); Def* nf = ns;
                for (const Def* p = x.s; p != x.f; ++p, ++nf) new (nf) Def(*p);
                clear(); ::operator delete(s); s = ns; e = s + xlen;
            } else if (size() >= xlen) {
                Def* i = s; for (const Def* p = x.s; p != x.f; ++p, ++i) *i = *p;
                for (Def* q = i; q != f; ++q) q->~Def();
            } else {
                Def* i = s; const Def* p = x.s; for (unsigned n = size(); n; --n, ++p, ++i) *i = *p;
                Def* c = f; for (; p != x.f; ++p, ++c) new (c) Def(*p);
            }
            f = s + xlen;
        }
        return *this;
    }
};
static bool eq(const Vec& a, const Vec& b) { if (a.size() != b.size()) return false; for (unsigned i = 0; i < a.size(); ++i) if (a.s[i].v != b.s[i].v) return false; return true; }
int main()
{
    bool ok = true;
    {
        Vec a, b; b.push(Def(1)); b.push(Def(2)); b.push(Def(3));
        a = b; ok = ok && eq(a, b) && a.size() == 3;                       // grow path
        Vec c; c.push(Def(9)); c.push(Def(8)); c.push(Def(5)); (--c.f)->~Def();   // size 2, capacity 4
        g_assigns = 0; c = a; ok = ok && eq(c, a) && g_assigns == 2 && c.capacity() == 4; // partial assign + tail copy
        g_assigns = 0; Vec d; d.push(Def(7)); a = d; ok = ok && eq(a, d) && a.size() == 1 && a.capacity() == 3 && g_assigns == 1; // shrink path
        a = a; ok = ok && a.size() == 1 && a.s[0].v == 7;                // self assign
    }
    ok = ok && g_live == 0;
    std::printf(ok ? "00472eda_TEST PASS\n" : "00472eda_TEST FAIL\n");
    return ok ? 0 : 1;
}