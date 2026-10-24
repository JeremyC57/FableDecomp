#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

struct random_access_iterator_tag { };
static int g_vectorAssigns = 0;
struct SVectorA { void* m_first; void* m_last; void* m_end;
    SVectorA& operator=(const SVectorA& o) { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; ++g_vectorAssigns; return *this; } };
struct SVectorB { void* m_first; void* m_last; void* m_end;
    SVectorB& operator=(const SVectorB& o) { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; g_vectorAssigns += 10; return *this; } };
struct SVectorC { void* m_first; void* m_last; void* m_end;
    SVectorC& operator=(const SVectorC& o) { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; g_vectorAssigns += 100; return *this; } };
struct SEmptyMember { };
struct SRecord40 {
    SVectorA     m_a;
    SVectorB     m_b;
    SVectorC     m_c;
    SEmptyMember m_empty;
    bool         m_flag;
};
static long g_cells[16];

typedef SRecord40 T_;
enum { N = 3 };

static T_* Copy(const T_* first, const T_* last, T_* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    bool ok = true;
    static T_ src[N];
    static T_ dst[N];
    for (int i = 0; i < N; ++i) {
        T_* s = &src[i];
        T_* d = &dst[i];
        s->m_a.m_first = &g_cells[i]; s->m_a.m_last = &g_cells[i + 1]; s->m_a.m_end = &g_cells[i + 2];
        s->m_b.m_first = &g_cells[i + 3]; s->m_b.m_last = &g_cells[i + 4]; s->m_b.m_end = &g_cells[i + 5];
        s->m_c.m_first = &g_cells[i + 6]; s->m_c.m_last = &g_cells[i + 7]; s->m_c.m_end = &g_cells[i + 8];
        s->m_flag = (i == 2); d->m_flag = (i != 2);
    }
    random_access_iterator_tag tag;
    T_* r = Copy(src, src + N, dst, tag, (int*)0);
    if (r != dst + N) ok = false;
    for (int i = 0; i < N; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->m_a.m_first != &g_cells[i] || d->m_a.m_last != &g_cells[i + 1] || d->m_a.m_end != &g_cells[i + 2]) ok = false;
        if (d->m_b.m_first != &g_cells[i + 3] || d->m_b.m_end != &g_cells[i + 5] || d->m_c.m_first != &g_cells[i + 6] || d->m_c.m_end != &g_cells[i + 8]) ok = false;
        if (d->m_flag != (i == 2)) ok = false;
    }
    if (g_vectorAssigns != 111 * N) ok = false;
    if (Copy(src, src, dst + 1, tag, (int*)0) != dst + 1) ok = false;
    if (!ok) { std::printf("00a85870_TEST FAIL\n"); return 1; }
    std::printf("00a85870_TEST PASS\n");
    return 0;
}