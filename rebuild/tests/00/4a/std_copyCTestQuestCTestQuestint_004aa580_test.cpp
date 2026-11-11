#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

struct random_access_iterator_tag { };
static int g_assigns = 0;
class CCharString { public:
    const char* m_pData;
    CCharString() : m_pData("") { }
    const CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; ++g_assigns; return *this; }
};
static const char* g_text[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
struct SRecord28 {
    CCharString m_0, m_4;
    long        m_8;
    CCharString m_c, m_10, m_14, m_18;
};

typedef SRecord28 T_;
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
        s->m_0.m_pData = g_text[i]; s->m_4.m_pData = g_text[i + 1]; s->m_8 = 80 + i; s->m_c.m_pData = g_text[i + 2];
        s->m_10.m_pData = g_text[i + 3]; s->m_14.m_pData = g_text[i + 4]; s->m_18.m_pData = g_text[i + 5];
    }
    random_access_iterator_tag tag;
    T_* r = Copy(src, src + N, dst, tag, (int*)0);
    if (r != dst + N) ok = false;
    for (int i = 0; i < N; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->m_0.m_pData != g_text[i] || d->m_4.m_pData != g_text[i + 1] || d->m_8 != 80 + i || d->m_c.m_pData != g_text[i + 2]) ok = false;
        if (d->m_10.m_pData != g_text[i + 3] || d->m_14.m_pData != g_text[i + 4] || d->m_18.m_pData != g_text[i + 5]) ok = false;
    }
    if (g_assigns != 6 * N) ok = false;
    if (Copy(src, src, dst + 1, tag, (int*)0) != dst + 1) ok = false;
    if (!ok) { std::printf("004aa580_TEST FAIL\n"); return 1; }
    std::printf("004aa580_TEST PASS\n");
    return 0;
}