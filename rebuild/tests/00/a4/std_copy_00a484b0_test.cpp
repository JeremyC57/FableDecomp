// Behaviour test for 0x00A484B0: std::__copy<CLipSyncPhoneme*> (counted assignment copy)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

static int g_liveStrings = 0;
class CCharString {                                   // test model: a deep-copying string with a live count
public:
    char* m_pData;
    CCharString() : m_pData(0) { ++g_liveStrings; }
    CCharString(const char* s) : m_pData(Dup(s)) { ++g_liveStrings; }
    CCharString(const CCharString& other) : m_pData(Dup(other.m_pData)) { ++g_liveStrings; }
    ~CCharString() { free(m_pData); --g_liveStrings; }
    const CCharString& operator=(const CCharString& other)
    {
        if (this != &other) {
            char* p = Dup(other.m_pData);
            free(m_pData);
            m_pData = p;
        }
        return *this;
    }
    const char* c_str() const { return m_pData ? m_pData : ""; }
    bool operator<(const CCharString& o) const { return strcmp(c_str(), o.c_str()) < 0; }
    static char* Dup(const char* s)
    {
        if (s == 0) return 0;
        char* p = (char*)malloc(strlen(s) + 1);
        strcpy(p, s);
        return p;
    }
};
static bool Eq(const CCharString& s, const char* t) { return strcmp(s.c_str(), t) == 0; }

class CLipSyncPhoneme {
public:
    CCharString   Symbol;
    unsigned long ID;
    float         Weight;
    CLipSyncPhoneme() : ID(0), Weight(0.0f) { }
    CLipSyncPhoneme(const char* s, unsigned long id, float w) : Symbol(s), ID(id), Weight(w) { }
};
static bool Same(const CLipSyncPhoneme& p, const char* s, unsigned long id, float w)
{
    return Eq(p.Symbol, s) && p.ID == id && Near(p.Weight, w);
}

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}

int main()
{
    {
        CLipSyncPhoneme src[3];
        src[0] = CLipSyncPhoneme("AA", 1, 0.25f);
        src[1] = CLipSyncPhoneme("EE", 2, 0.5f);
        src[2] = CLipSyncPhoneme("OO", 3, 0.75f);
        CLipSyncPhoneme dst[4];
        dst[3] = CLipSyncPhoneme("ZZ", 9, 1.0f);
        int live = g_liveStrings;
        CLipSyncPhoneme* end = std::__copy(src, src + 3, dst, std::random_access_iterator_tag(), (int*)0);
        Check(g_liveStrings == live);
        Check(end == dst + 3);
        Check(Same(dst[0], "AA", 1, 0.25f) && Same(dst[1], "EE", 2, 0.5f) && Same(dst[2], "OO", 3, 0.75f));
        Check(Same(dst[3], "ZZ", 9, 1.0f) && Same(src[1], "EE", 2, 0.5f));
        Check(std::__copy(src, src, dst, std::random_access_iterator_tag(), (int*)0) == dst);
    }
    Check(g_liveStrings == 0);
    if (g_fail) {
        printf("00a484b0_TEST FAIL\n");
        return 1;
    }
    printf("00a484b0_TEST PASS\n");
    return 0;
}