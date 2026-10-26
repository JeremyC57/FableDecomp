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
    CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; ++g_assigns; return *this; }
};
static const char* g_text[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
struct CPersistInfo {
    CCharString QuestName;
    CCharString PersistScript;
};
struct SPairCharStringPersistInfo {
    CCharString  first;
    CPersistInfo second;
};

typedef SPairCharStringPersistInfo T_;
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
        s->first.m_pData = g_text[i]; s->second.QuestName.m_pData = g_text[i + 3]; s->second.PersistScript.m_pData = g_text[i + 5];
    }
    random_access_iterator_tag tag;
    T_* r = Copy(src, src + N, dst, tag, (int*)0);
    if (r != dst + N) ok = false;
    for (int i = 0; i < N; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->first.m_pData != g_text[i] || d->second.QuestName.m_pData != g_text[i + 3] || d->second.PersistScript.m_pData != g_text[i + 5]) ok = false;
    }
    if (g_assigns != 3 * N) ok = false;
    if (Copy(src, src, dst + 1, tag, (int*)0) != dst + 1) ok = false;
    if (!ok) { std::printf("004b84f0_TEST FAIL\n"); return 1; }
    std::printf("004b84f0_TEST PASS\n");
    return 0;
}