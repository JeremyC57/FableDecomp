// Behaviour test for 0x00777FE0 std::vector<CTCShop::GoodsPlace>::_M_insert_overflow (STLport growth path).
#include <cstdio>
#include <cstdlib>
#include <new>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CRightHandedSet { public: C3DVector Forward; C3DVector Up; };
struct GoodsPlace { C3DVector pos; CRightHandedSet rhset; };

struct __false_type { __false_type() { } };

static GoodsPlace* uninit_copy(GoodsPlace* f, GoodsPlace* l, GoodsPlace* r, const __false_type&)
{
    for (; f != l; ++f, ++r) new (r) GoodsPlace(*f);
    return r;
}
static GoodsPlace* uninit_fill_n(GoodsPlace* f, unsigned int n, const GoodsPlace& x, const __false_type&)
{
    for (; n > 0; --n, ++f) new (f) GoodsPlace(x);
    return f;
}

struct Vec {
    GoodsPlace* m_start;
    GoodsPlace* m_finish;
    GoodsPlace* m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    void insert_overflow(GoodsPlace* pos, const GoodsPlace& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        GoodsPlace* ns = len != 0 ? (GoodsPlace*)std::malloc(len * sizeof(GoodsPlace)) : 0;
        GoodsPlace* nf = uninit_copy(m_start, pos, ns, __false_type());
        if (fill_len == 1) { new (nf) GoodsPlace(x); ++nf; }
        else nf = uninit_fill_n(nf, fill_len, x, __false_type());
        if (!atend) nf = uninit_copy(pos, m_finish, nf, __false_type());
        if (m_start) std::free(m_start);
        m_start = ns; m_finish = nf; m_end = ns + len;
    }
};

static GoodsPlace mk(float v)
{
    GoodsPlace g;
    g.pos.X = v; g.pos.Y = v + 1; g.pos.Z = v + 2;
    g.rhset.Forward.X = v + 3; g.rhset.Up.Z = v + 4;
    return g;
}

int main()
{
    int ok = 1;
    if (sizeof(GoodsPlace) != 36) ok = 0;
    Vec v; v.m_start = v.m_finish = v.m_end = 0;
    GoodsPlace a = mk(10.0f);
    v.insert_overflow(v.m_finish, a, __false_type(), 1, true);          // empty: len = 0 + max(0,1) = 1
    if (v.size() != 1 || v.m_end - v.m_start != 1 || v.m_start[0].pos.X != 10.0f) ok = 0;
    GoodsPlace b = mk(20.0f);
    v.insert_overflow(v.m_finish, b, __false_type(), 1, true);          // len = 1 + 1 = 2
    if (v.size() != 2 || v.m_end - v.m_start != 2 || v.m_start[1].rhset.Up.Z != 24.0f) ok = 0;
    GoodsPlace c = mk(30.0f);
    v.insert_overflow(v.m_start + 1, c, __false_type(), 3, false);      // middle insert of 3: len = 2 + 3 = 5
    if (v.size() != 5 || v.m_end - v.m_start != 5) ok = 0;
    if (v.m_start[0].pos.X != 10.0f || v.m_start[1].pos.Y != 31.0f || v.m_start[3].rhset.Forward.X != 33.0f ||
        v.m_start[4].pos.X != 20.0f) ok = 0;
    std::free(v.m_start);
    if (!ok) { std::printf("00777fe0_TEST FAIL\n"); return 1; }
    std::printf("00777fe0_TEST PASS\n");
    return 0;
}