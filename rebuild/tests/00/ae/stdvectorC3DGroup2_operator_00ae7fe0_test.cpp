// Behaviour test for vector<C3DGroup2>::operator= (0x00AE7FE0): the three STLport assignment paths (grow into a fresh
// block, shrink with destruction of the excess, grow within capacity with copy-construction of the tail) on elements
// whose inner block is deep-copied, counting live inner blocks to catch leaks and double frees.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
struct Blend { unsigned long V; float W; };
struct Inner {                                     // CArray<C3DVertexBlend2>: owns a block
    Blend* s; Blend* f; Blend* e;
    Inner() : s(0), f(0), e(0) { }
    Inner(const Inner& o) : s(0), f(0), e(0) { assign(o); }
    ~Inner() { if (s) { std::free(s); --g_live; } }
    Inner& operator=(const Inner& o) { if (&o != this) { if (s) { std::free(s); --g_live; } s = f = e = 0; assign(o); } return *this; }
    void assign(const Inner& o)
    {
        unsigned int n = (unsigned int)(o.f - o.s);
        if (n) { s = (Blend*)std::malloc(n * sizeof(Blend)); ++g_live; for (unsigned int i = 0; i < n; ++i) s[i] = o.s[i]; }
        f = s + n; e = f;
    }
    void put(unsigned long v) { Inner t; t.s = (Blend*)std::malloc(sizeof(Blend)); ++g_live; t.s[0].V = v; t.s[0].W = 1.0f; t.f = t.e = t.s + 1; *this = t; }
};
struct Group { long BoneIndex; Inner Blends; };
struct Vec {
    Group* s; Group* f; Group* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    unsigned int capacity() const { return (unsigned int)(e - s); }
    void clear() { for (Group* p = s; p != f; ++p) p->~Group(); if (s) std::free(s); }
    Vec& operator=(const Vec& x)
    {
        if (&x != this) {
            unsigned int xlen = x.size();
            if (xlen > capacity()) {
                Group* tmp = xlen ? (Group*)std::malloc(xlen * sizeof(Group)) : 0;
                Group* c = tmp;
                for (const Group* p = x.s; p != x.f; ++p, ++c) new (c) Group(*p);
                clear();
                s = tmp; e = s + xlen;
            } else if (size() >= xlen) {
                Group* r = s;
                for (int n = (int)(x.f - x.s); n > 0; --n) *r++ = x.s[(int)xlen - n];
                for (Group* p = r; p != f; ++p) p->~Group();
            } else {
                unsigned int sz = size();
                for (unsigned int i = 0; i < sz; ++i) s[i] = x.s[i];
                Group* c = f;
                for (const Group* p = x.s + sz; p != x.f; ++p, ++c) new (c) Group(*p);
            }
            f = s + xlen;
        }
        return *this;
    }
};
static void make(Vec& v, unsigned int n, unsigned int cap, long base)
{
    v.s = cap ? (Group*)std::malloc(cap * sizeof(Group)) : 0; v.f = v.s; v.e = v.s + cap;
    for (unsigned int i = 0; i < n; ++i, ++v.f) { new (v.f) Group(); v.f->BoneIndex = base + (long)i; v.f->Blends.put((unsigned long)(base * 10 + (long)i)); }
}
static bool same(const Vec& a, const Vec& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i) {
        if (a.s[i].BoneIndex != b.s[i].BoneIndex) return false;
        if (a.s[i].Blends.s == b.s[i].Blends.s) return false;           // deep copy
        if (a.s[i].Blends.s[0].V != b.s[i].Blends.s[0].V) return false;
    }
    return true;
}
int main()
{
    bool ok = true;
    Vec a, b, c, d;
    make(a, 2, 2, 1);
    make(b, 4, 4, 5);
    a = b;                                          // grow: fresh block
    ok = ok && same(a, b) && a.capacity() == 4 && g_live == 8;
    make(c, 1, 1, 9);
    a = c;                                          // shrink within capacity: destroy the excess
    ok = ok && same(a, c) && a.capacity() == 4 && g_live == 2 + 4;
    make(d, 3, 3, 20);
    a = d;                                          // grow within capacity: assign then copy-construct the tail
    ok = ok && same(a, d) && a.capacity() == 4 && g_live == 3 + 4 + 1 + 3;
    a = a;                                          // self-assignment is a no-op
    ok = ok && a.size() == 3 && a.s[2].BoneIndex == 22;
    a.clear(); b.clear(); c.clear(); d.clear();
    ok = ok && g_live == 0;
    std::printf(ok ? "00ae7fe0_TEST PASS\n" : "00ae7fe0_TEST FAIL\n");
    return ok ? 0 : 1;
}