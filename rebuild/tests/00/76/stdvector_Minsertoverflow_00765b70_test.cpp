// Behaviour test for 0x00765B70 std::vector<vector<C3DVector> >::_M_insert_overflow (STLport growth path).
#include <cstdio>
#include <cstdlib>
#include <new>

struct Pt { float X, Y, Z; };

struct Inner {                                  // vector<C3DVector>
    Pt* s; Pt* f; Pt* e;
    Inner() : s(0), f(0), e(0) { }
    Inner(const Inner& o)                       // 0x006F0810: allocate exactly size(), copy
    {
        unsigned int n = (unsigned int)(o.f - o.s);
        s = n ? (Pt*)std::malloc(n * sizeof(Pt)) : 0;
        f = s; e = s + n;
        for (Pt* p = o.s; p != o.f; ++p, ++f) *f = *p;
    }
};

struct __false_type { __false_type() { } };

struct Outer {
    Inner* m_start; Inner* m_finish; Inner* m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    void clear_()                               // 0x00765AD0
    {
        for (Inner* p = m_start; p != m_finish; ++p) if (p->s) std::free(p->s);
        if (m_start) std::free(m_start);
    }
    void insert_overflow(Inner* pos, const Inner& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Inner* ns = len != 0 ? (Inner*)std::malloc(len * sizeof(Inner)) : 0;
        Inner* nf = ns;
        for (Inner* p = m_start; p != pos; ++p, ++nf) new (nf) Inner(*p);
        if (fill_len == 1) { new (nf) Inner(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) Inner(x);
        if (!atend) for (Inner* p = pos; p != m_finish; ++p, ++nf) new (nf) Inner(*p);
        clear_();
        m_start = ns; m_finish = nf; m_end = ns + len;
    }
};

static Inner mk(int n, float base)
{
    Inner v;
    v.s = (Pt*)std::malloc(n * sizeof(Pt)); v.f = v.s + n; v.e = v.f;
    for (int i = 0; i < n; ++i) { v.s[i].X = base + i; v.s[i].Y = 0; v.s[i].Z = 0; }
    return v;
}

int main()
{
    int ok = 1;
    Outer o; o.m_start = o.m_finish = o.m_end = 0;
    Inner a = mk(2, 1.0f), b = mk(3, 10.0f), c = mk(1, 100.0f);
    o.insert_overflow(o.m_finish, a, __false_type(), 1, true);
    o.insert_overflow(o.m_finish, b, __false_type(), 1, true);
    if (o.size() != 2 || o.m_end - o.m_start != 2) ok = 0;
    o.insert_overflow(o.m_start + 1, c, __false_type(), 2, false);   // len = 2 + 2 = 4
    if (o.size() != 4 || o.m_end - o.m_start != 4) ok = 0;
    if (o.m_start[0].f - o.m_start[0].s != 2 || o.m_start[0].s[1].X != 2.0f) ok = 0;
    if (o.m_start[1].s[0].X != 100.0f || o.m_start[2].s[0].X != 100.0f) ok = 0;
    if (o.m_start[3].f - o.m_start[3].s != 3 || o.m_start[3].s[2].X != 12.0f) ok = 0;
    if (o.m_start[1].s == c.s) ok = 0;          // deep copies
    o.clear_();
    std::free(a.s); std::free(b.s); std::free(c.s);
    if (!ok) { std::printf("00765b70_TEST FAIL\n"); return 1; }
    std::printf("00765b70_TEST PASS\n");
    return 0;
}