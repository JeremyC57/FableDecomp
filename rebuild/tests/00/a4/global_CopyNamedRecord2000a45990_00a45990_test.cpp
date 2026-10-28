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
struct SRecord12 { long m_a, m_b, m_c; };
struct SNamedRecord20 {
    CCharString m_name;
    long        m_index;
    SRecord12   m_record;
};

typedef SNamedRecord20 T_;
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
        s->m_name.m_pData = g_text[i + 2]; s->m_index = 3 * i; s->m_record.m_a = i; s->m_record.m_b = 2 * i; s->m_record.m_c = -i;
    }
    random_access_iterator_tag tag;
    T_* r = Copy(src, src + N, dst, tag, (int*)0);
    if (r != dst + N) ok = false;
    for (int i = 0; i < N; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->m_name.m_pData != g_text[i + 2] || d->m_index != 3 * i || d->m_record.m_a != i || d->m_record.m_b != 2 * i || d->m_record.m_c != -i) ok = false;
    }
    if (g_assigns != N) ok = false;
    if (Copy(src, src, dst + 1, tag, (int*)0) != dst + 1) ok = false;
    if (!ok) { std::printf("00a45990_TEST FAIL\n"); return 1; }
    std::printf("00a45990_TEST PASS\n");
    return 0;
}