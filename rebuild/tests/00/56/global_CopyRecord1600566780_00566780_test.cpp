#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

struct random_access_iterator_tag { };
static int g_assigns = 0;
struct CCharString {
    const char* m_pData;
    CCharString() : m_pData("") { }
    CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; ++g_assigns; return *this; }
};
static const char* g_text[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
struct SRecord16 {
    CCharString m_0, m_4, m_8;
    bool        m_flagC;
    bool        m_flagD;
};

typedef SRecord16 T_;
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
        s->m_0.m_pData = g_text[i]; s->m_4.m_pData = g_text[i + 2]; s->m_8.m_pData = g_text[i + 4]; s->m_flagC = (i == 1); s->m_flagD = (i != 1); d->m_flagC = true; d->m_flagD = true;
    }
    random_access_iterator_tag tag;
    T_* r = Copy(src, src + N, dst, tag, (int*)0);
    if (r != dst + N) ok = false;
    for (int i = 0; i < N; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->m_0.m_pData != g_text[i] || d->m_4.m_pData != g_text[i + 2] || d->m_8.m_pData != g_text[i + 4] || d->m_flagC != (i == 1) || d->m_flagD != (i != 1)) ok = false;
    }
    if (g_assigns != 3 * N) ok = false;
    if (Copy(src, src, dst + 1, tag, (int*)0) != dst + 1) ok = false;
    if (!ok) { std::printf("00566780_TEST FAIL\n"); return 1; }
    std::printf("00566780_TEST PASS\n");
    return 0;
}