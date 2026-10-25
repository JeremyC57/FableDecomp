// Behaviour test for 0x006FC3E0 (source body copied from src/006fc3e0.cpp; out-of-line callees stubbed below).
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
// (operator new[] / delete[]: the CRT's)

namespace NAnimationEvents { class CEvent { public: long m_a, m_b; }; }   // 8 bytes (Ego_r.pdb: Index bits + Time)

template <class T, int N> class CSmallVector {
public:
    T*  Start;                                  // +0x00
    int Size;                                   // +0x04
    void Copy(const CSmallVector& other);       // 0x006FB5C0
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    void Clear()
    {
        delete[] Start;
        Start = 0;
        Size = 0;
    }
    ~CSmallVector() { Clear(); }
};
#pragma auto_inline(off)
// 0x006FB5C0 (landed separately): same-TU noinline body -- it never keeps `other`'s address, so the by-value val the
// caller passes by reference stays uncaptured (chunk-422 recipe).
template <class T, int N> __declspec(noinline) void CSmallVector<T, N>::Copy(const CSmallVector& other)
{
    Clear();
    Start = new T[other.Size];
    Size = other.Size;
    for (unsigned long i = 0; i < (unsigned long)other.Size; ++i)
        Start[i] = other.Start[i];
}
#pragma auto_inline(on)

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

#pragma auto_inline(off)
// 0x006FB830 (landed separately; same-TU body so the empty tag can share a dead slot)
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
#pragma auto_inline(on)
template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                                  _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 __fastcall copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                           _BidirectionalIter2 __result)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x006FB960

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > SKeyedEvents;
typedef CKeyPairCompareLess<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > SKeyedLess;

template void __fastcall _STL::__linear_insert(SKeyedEvents*, SKeyedEvents*, SKeyedEvents, SKeyedLess);


// ---- test harness ----
typedef NAnimationEvents::CEvent TEv;
static int g_unguarded = 0;
static unsigned long g_unguardedKey = 0;
static int g_unguardedSize = 0;
namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare)
{
    ++g_unguarded; g_unguardedKey = __val.first; g_unguardedSize = __val.second.Size; (void)__last;
}
}
static void Set(SKeyedEvents& e, unsigned long key, int n, long base)
{
    e.first = key;
    e.second.Start = n ? new TEv[n] : 0;
    e.second.Size = n;
    for (int i = 0; i < n; ++i) { e.second.Start[i].m_a = base + i; e.second.Start[i].m_b = -(base + i); }
}
int main()
{
    static double raw[(4 * sizeof(SKeyedEvents)) / sizeof(double) + 1];
    memset(raw, 0, sizeof raw);
    SKeyedEvents* v = (SKeyedEvents*)raw;
    Set(v[0], 20, 1, 100); Set(v[1], 30, 2, 200); Set(v[2], 40, 0, 0);
    static double vr[sizeof(SKeyedEvents) / sizeof(double) + 1];
    memset(vr, 0, sizeof vr);
    SKeyedEvents* val = (SKeyedEvents*)vr;
    Set(*val, 10, 3, 300);                       // sorts before v[0]: shift [0,2) up, put val first
    SKeyedLess less;
    int ok = 1;
    _STL::__linear_insert(v, v + 2, *val, less);
    if (v[0].first != 10 || v[1].first != 20 || v[2].first != 30) ok = 0;
    if (v[0].second.Size != 3 || v[0].second.Start[2].m_a != 302 || v[1].second.Size != 1 || v[1].second.Start[0].m_a != 100) ok = 0;
    if (v[2].second.Size != 2 || v[2].second.Start[1].m_b != -201 || g_unguarded != 0) ok = 0;
    Set(*val, 25, 2, 500);                       // not before v[0]: the unguarded insert gets a copy
    _STL::__linear_insert(v, v + 2, *val, less);
    if (g_unguarded != 1 || g_unguardedKey != 25 || g_unguardedSize != 2) ok = 0;
    printf("006fc3e0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}