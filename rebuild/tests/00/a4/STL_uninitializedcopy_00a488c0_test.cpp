// Behaviour test for 0x00A488C0: std::__uninitialized_copy<const CLipSyncPhoneme*, CLipSyncPhoneme*> (placement copy-construct a range)
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
struct __false_type { __false_type() { } };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

int main()
{
    {
        CLipSyncPhoneme src[3];
        src[0] = CLipSyncPhoneme("AA", 1, 0.25f);
        src[1] = CLipSyncPhoneme("EE", 2, 0.5f);
        src[2] = CLipSyncPhoneme("OO", 3, 0.75f);
        CLipSyncPhoneme* raw = (CLipSyncPhoneme*)malloc(3 * sizeof(CLipSyncPhoneme));
        int live = g_liveStrings;
        const CLipSyncPhoneme* first = src;
        CLipSyncPhoneme* end = std::__uninitialized_copy(first, first + 3, raw, std::__false_type());
        Check(end == raw + 3 && g_liveStrings == live + 3);
        Check(Same(raw[0], "AA", 1, 0.25f) && Same(raw[1], "EE", 2, 0.5f) && Same(raw[2], "OO", 3, 0.75f));
        Check(raw[0].Symbol.m_pData != src[0].Symbol.m_pData);
        for (int i = 0; i < 3; ++i)
            raw[i].~CLipSyncPhoneme();
        free(raw);
        Check(g_liveStrings == live);
    }
    Check(g_liveStrings == 0);
    if (g_fail) {
        printf("00a488c0_TEST FAIL\n");
        return 1;
    }
    printf("00a488c0_TEST PASS\n");
    return 0;
}