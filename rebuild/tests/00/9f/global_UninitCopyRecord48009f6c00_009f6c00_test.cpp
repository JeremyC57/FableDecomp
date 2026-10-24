#include <cstdio>
#include <new>

struct __false_type { };
static int g_arrayCopies = 0;
struct CArray12 {
    void* m_first; void* m_last; void* m_end;
    CArray12() : m_first(0), m_last(0), m_end(0) { }
    CArray12(const CArray12& o) : m_first(o.m_first), m_last(o.m_last), m_end(o.m_end) { ++g_arrayCopies; }
};
struct SQuad16 { long m_a, m_b, m_c, m_d; };
struct SRecord12 {
    long m_a, m_b, m_c;
    SRecord12() : m_a(0), m_b(0), m_c(0) { }
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};
struct SRecord48 {
    CArray12  m_array;
    long      m_c;
    SQuad16   m_quad;
    long      m_20;
    SRecord12 m_record;
    SRecord48() : m_c(0), m_20(0) { m_quad.m_a = m_quad.m_b = m_quad.m_c = m_quad.m_d = 0; }
    SRecord48(const SRecord48& o) : m_array(o.m_array), m_c(o.m_c), m_quad(o.m_quad), m_20(o.m_20), m_record(o.m_record) { }
};
static long g_cells[3][3];

typedef SRecord48 T_;

static T_* UninitCopy(const T_* first, const T_* last, T_* result, const __false_type&)
{
    T_* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) T_(*first);
    return cur;
}

int main()
{
    bool ok = true;
    T_* src = (T_*)new double[(3 * sizeof(T_)) / sizeof(double) + 1];
    for (int i = 0; i < 3; ++i) {
        T_* s = new (&src[i]) T_();
        s->m_c = 100 * i + 1;
        s->m_quad.m_a = 100 * i + 2;
        s->m_quad.m_b = 100 * i + 3;
        s->m_quad.m_c = 100 * i + 4;
        s->m_quad.m_d = 100 * i + 5;
        s->m_20 = 100 * i + 6;
        s->m_record.m_a = 100 * i + 7;
        s->m_record.m_b = 100 * i + 8;
        s->m_record.m_c = 100 * i + 9;
        s->m_array.m_first = &g_cells[i][0]; s->m_array.m_last = &g_cells[i][1]; s->m_array.m_end = &g_cells[i][2];
    }
    static double storage[64];
    T_* dst = (T_*)storage;
    __false_type tag;
    g_arrayCopies = 0;
    T_* r = UninitCopy(src, src + 3, dst, tag);
    if (g_arrayCopies != 3) ok = false;
    if (r != dst + 3) ok = false;
    for (int i = 0; i < 3; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->m_c != s->m_c) ok = false;
        if (d->m_quad.m_a != s->m_quad.m_a) ok = false;
        if (d->m_quad.m_b != s->m_quad.m_b) ok = false;
        if (d->m_quad.m_c != s->m_quad.m_c) ok = false;
        if (d->m_quad.m_d != s->m_quad.m_d) ok = false;
        if (d->m_20 != s->m_20) ok = false;
        if (d->m_record.m_a != s->m_record.m_a) ok = false;
        if (d->m_record.m_b != s->m_record.m_b) ok = false;
        if (d->m_record.m_c != s->m_record.m_c) ok = false;
        if (d->m_array.m_first != &g_cells[i][0] || d->m_array.m_last != &g_cells[i][1] || d->m_array.m_end != &g_cells[i][2]) ok = false;
    }
    if (UninitCopy(src, src, dst + 1, tag) != dst + 1) ok = false;
    if (!ok) { std::printf("009f6c00_TEST FAIL\n"); return 1; }
    std::printf("009f6c00_TEST PASS\n");
    return 0;
}