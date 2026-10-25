#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <new>

static int g_blocks = 0;                                    // live string blocks (leak / double-free check)

template <class T> class CBasicString {
public:
    T* m_pData;
    bool operator<(const CBasicString& other) const
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
};

// Test CCharString: one heap block per non-empty string (null block = empty), copies duplicate the block.
class CCharString {
public:
    CBasicString<char>* PStringData;
    CCharString() : PStringData(0) { }
    explicit CCharString(const char* s) : PStringData(0) { Attach(s); }
    CCharString(const CCharString& other) : PStringData(0) { if (other.PStringData) Attach(other.PStringData->m_pData); }
    ~CCharString() { Detach(); }
    CCharString& operator=(const CCharString& other)
    {
        if (this != &other) {
            char* s = other.PStringData ? other.PStringData->m_pData : 0;
            Detach();
            if (s)
                Attach(s);
        }
        return *this;
    }
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
    const char* c_str() const { return PStringData ? PStringData->m_pData : ""; }
private:
    void Attach(const char* s)
    {
        PStringData = new CBasicString<char>;
        PStringData->m_pData = (char*)s;
        ++g_blocks;
    }
    void Detach()
    {
        if (PStringData) {
            delete PStringData;
            PStringData = 0;
            --g_blocks;
        }
    }
};

class CSoundBankLH {
public:
    int Id;
};

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const K& a, const std::pair<K, V>& b) const { return a < b.first; }
    bool operator()(const std::pair<K, V>& a, const K& b) const { return a.first < b; }
};

typedef std::pair<CCharString, CCharString>      SNamePair;
typedef std::pair<CCharString, CSoundBankLH*>     SNameEntry;
typedef std::pair<SNamePair, CSoundBankLH*>       SNamePairEntry;
typedef CKeyPairCompareLess<CCharString, CSoundBankLH*> CNameLess;
typedef CKeyPairCompareLess<SNamePair, CSoundBankLH*>   CNamePairLess;

static CSoundBankLH g_banks[16];

// raw arrays of entries (the pairs have no default constructor)
static SNameEntry* MakeNames(const char* const* names, int n, int capacity)
{
    SNameEntry* p = (SNameEntry*)malloc(capacity * sizeof(SNameEntry));
    for (int i = 0; i < n; ++i)
        new (p + i) SNameEntry(CCharString(names[i]), &g_banks[i]);
    return p;
}
static void FreeNames(SNameEntry* p, int n)
{
    for (int i = 0; i < n; ++i)
        p[i].~SNameEntry();
    free(p);
}
static SNamePairEntry* MakePairs(const char* const* names, int n, int capacity)   // names: n (first, second) pairs
{
    SNamePairEntry* p = (SNamePairEntry*)malloc(capacity * sizeof(SNamePairEntry));
    for (int i = 0; i < n; ++i)
        new (p + i) SNamePairEntry(SNamePair(CCharString(names[2 * i]), CCharString(names[2 * i + 1])), &g_banks[i]);
    return p;
}
static void FreePairs(SNamePairEntry* p, int n)
{
    for (int i = 0; i < n; ++i)
        p[i].~SNamePairEntry();
    free(p);
}
static bool NameIs(const SNameEntry& e, const char* s) { return strcmp(e.first.c_str(), s) == 0; }
static bool PairIs(const SNamePairEntry& e, const char* a, const char* b)
{
    return strcmp(e.first.first.c_str(), a) == 0 && strcmp(e.first.second.c_str(), b) == 0;
}
template <class T, class C> static bool Sorted(const T* p, int n, C comp)
{
    for (int i = 1; i < n; ++i)
        if (comp(p[i], p[i - 1]))
            return false;
    return true;
}
template <class T, class C> static bool IsHeap(const T* p, int n, C comp)
{
    for (int i = 1; i < n; ++i)
        if (comp(p[(i - 1) / 2], p[i]))
            return false;
    return true;
}

namespace std {
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = copy(__last, _M_finish, __first);
        _Destroy(__i, _M_finish);
        _M_finish = __i;
        return __first;
    }
};
}

int main()
{
    const char* names[10] = { "a", "1", "b", "2", "c", "3", "d", "4", "e", "5" };
    std::vector<SNamePairEntry> v;
    v._M_start = MakePairs(names, 5, 5);
    v._M_finish = v._M_end_of_storage = v._M_start + 5;
    SNamePairEntry* r = v.erase(v._M_start + 1, v._M_start + 3);
    if (r != v._M_start + 1 || v._M_finish != v._M_start + 3 || !PairIs(v._M_start[0], "a", "1") || !PairIs(v._M_start[1], "d", "4")
        || !PairIs(v._M_start[2], "e", "5") || v._M_start[2].second != &g_banks[4] || g_blocks != 6)
        { std::printf("00a40c10_TEST FAIL\n"); return 1; }
    r = v.erase(v._M_start + 2, v._M_start + 3);            // erase the tail element
    if (r != v._M_start + 2 || v._M_finish != v._M_start + 2 || g_blocks != 4)
        { std::printf("00a40c10_TEST FAIL\n"); return 1; }
    FreePairs(v._M_start, 2);
    if (g_blocks != 0)
        { std::printf("00a40c10_TEST FAIL\n"); return 1; }
    std::printf("00a40c10_TEST PASS\n");
    return 0;
}