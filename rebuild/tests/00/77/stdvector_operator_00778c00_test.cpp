// Behaviour test for 0x00778C00 std::vector<const CThing*>::operator= (STLport POD vector assignment through memmove).
#include <cstdio>
#include <cstdlib>
#include <cstring>

class CThing { public: int id; };

struct Vec {
    const CThing** m_start;
    const CThing** m_finish;
    const CThing** m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    unsigned int capacity() const { return (unsigned int)(m_end - m_start); }
    static const CThing** copy_trivial(const CThing* const* f, const CThing* const* l, const CThing** r)
    {
        if (l == f) return r;
        std::memmove(r, f, (const char*)l - (const char*)f);
        return r + (l - f);
    }
    Vec& assign(const Vec& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                const CThing** tmp = xlen != 0 ? (const CThing**)std::malloc(xlen * sizeof(const CThing*)) : 0;
                copy_trivial(x.m_start, x.m_finish, tmp);
                if (m_start) std::free(m_start);
                m_start = tmp;
                m_end = m_start + xlen;
            } else if (size() >= xlen) {
                copy_trivial(x.m_start, x.m_finish, m_start);
            } else {
                copy_trivial(x.m_start, x.m_start + size(), m_start);
                copy_trivial(x.m_start + size(), x.m_finish, m_finish);
            }
            m_finish = m_start + xlen;
        }
        return *this;
    }
};

static void make(Vec& v, unsigned int cap, unsigned int n, CThing* things)
{
    v.m_start = cap ? (const CThing**)std::malloc(cap * sizeof(const CThing*)) : 0;
    v.m_finish = v.m_start;
    v.m_end = v.m_start + cap;
    for (unsigned int i = 0; i < n; ++i) *v.m_finish++ = &things[i];
}

int main()
{
    int ok = 1;
    CThing t[6];
    for (int i = 0; i < 6; ++i) t[i].id = i;
    Vec a, b, c, d, e;
    make(a, 4, 4, t);
    make(b, 0, 0, t);
    if (&b.assign(a) != &b) ok = 0;                         // grow from empty: exact capacity 4
    if (b.size() != 4 || b.capacity() != 4 || b.m_start[3] != &t[3]) ok = 0;
    make(c, 6, 6, t + 0);
    c.m_start[0] = &t[5];
    c.assign(a);                                            // shrink in place
    if (c.size() != 4 || c.capacity() != 6 || c.m_start[0] != &t[0] || c.m_start[3]->id != 3) ok = 0;
    make(d, 8, 2, t + 4);
    d.assign(a);                                            // partial: 2 overwritten, 2 appended
    if (d.size() != 4 || d.capacity() != 8 || d.m_start[0] != &t[0] || d.m_start[1] != &t[1] || d.m_start[3] != &t[3])
        ok = 0;
    make(e, 2, 0, t);
    e.assign(b);                                            // grow from an empty vector with capacity 2
    e.assign(e);                                            // self-assignment
    if (e.size() != 4 || e.m_start[2]->id != 2) ok = 0;
    Vec z;
    make(z, 0, 0, t);
    d.assign(z);                                            // assigning an empty vector keeps the block
    if (d.size() != 0 || d.capacity() != 8) ok = 0;
    std::free(a.m_start); std::free(b.m_start); std::free(c.m_start); std::free(d.m_start); std::free(e.m_start);
    if (!ok) { std::printf("00778c00_TEST FAIL\n"); return 1; }
    std::printf("00778c00_TEST PASS\n");
    return 0;
}