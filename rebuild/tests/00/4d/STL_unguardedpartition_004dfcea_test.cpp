// Behaviour test for 0x004DFCEA (source body copied from src/004dfcea.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
#pragma optimize("s", on)
template <class T> class CBasicString {
public:
    T* m_pData;
    bool operator<(const CBasicString& other) const;
};
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CBasicString<T>::operator<(const CBasicString& other) const
{
    const T* b = other.m_pData;
    const T* a = m_pData;
    for (;;) {
        if (*a == 0 && *b == 0)
            return false;
        if (*a < *b)
            return true;
        if (*a > *b)
            return false;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

class CCharString {
public:
    CBasicString<char>* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    __forceinline bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (!PStringData)
            return true;
        if (!other.PStringData)
            return false;
        return *PStringData < *other.PStringData;
    }
};

class CThing;
class CTCBase {
public:
    class CRegisteredTCInfo {
    public:
        CTCBase* (__cdecl* AllocFunc)(CThing*);
        unsigned long DriverType;
        CCharString   Name;
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};
template <class _ForwardIter1, class _ForwardIter2> void __fastcall iter_swap(_ForwardIter1 __a, _ForwardIter2 __b);
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    __forceinline bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<CCharString, CTCBase::CRegisteredTCInfo> TEntry;
template TEntry* __fastcall std::__unguarded_partition(TEntry*, TEntry*, TEntry, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo>);

// ---- test harness ----
static int g_dtors = 0, g_swaps = 0;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { }
CCharString::~CCharString() { ++g_dtors; }
CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
namespace std {
template <> void __fastcall iter_swap<TEntry*, TEntry*>(TEntry* a, TEntry* b)
{
    ++g_swaps;
    CBasicString<char>* k = a->first.PStringData; a->first.PStringData = b->first.PStringData; b->first.PStringData = k;
    unsigned long t = a->second.DriverType; a->second.DriverType = b->second.DriverType; b->second.DriverType = t;
}
}
static CBasicString<char> g_str[6];
static char g_text[6][4] = { "d", "a", "f", "b", "e", "c" };
int main()
{
    static double raw[(6 * sizeof(TEntry)) / sizeof(double) + 1];
    memset(raw, 0, sizeof raw);
    TEntry* e = (TEntry*)raw;
    for (int i = 0; i < 6; ++i) {
        g_str[i].m_pData = g_text[i];
        e[i].first.PStringData = &g_str[i];
        e[i].second.DriverType = (unsigned long)(100 + i);
    }
    static CBasicString<char> pivotStr; static char pivotText[4] = "c";
    pivotStr.m_pData = pivotText;
    static double praw[sizeof(TEntry) / sizeof(double) + 1];
    memset(praw, 0, sizeof praw);
    TEntry* pv = (TEntry*)praw;
    pv->first.PStringData = &pivotStr;
    // e[0] "d" >= "c" stops the left scan; a guard element on the right ("a" at e[1]) stops the right scan
    TEntry* cut = std::__unguarded_partition(e, e + 6, *pv, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo>());
    int ok = 1;
    int k = (int)(cut - e);
    for (int i = 0; i < 6; ++i) {
        char c = e[i].first.PStringData->m_pData[0];
        if (i < k && c > 'c') ok = 0;
        if (i >= k && c < 'c') ok = 0;
        if (e[i].second.DriverType != (unsigned long)(100 + (e[i].first.PStringData - g_str))) ok = 0;
    }
    if (k != 3 || g_swaps != 2 || g_dtors != 2) ok = 0;
    printf("004dfcea_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}