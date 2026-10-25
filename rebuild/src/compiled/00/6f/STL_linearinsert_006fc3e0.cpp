// _STL::__linear_insert(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents val, CKeyPairCompareLess comp) -- STLport insertion step over the keyed animation-event vector: shift [first, last) up one and put val first when it sorts before *first, else the unguarded insert from last
// Leaf: ??$__linear_insert@
// Grade: RELOCATION_MATCH (agent902, 4th grade of this pass) -- the real STLport 4.6 template (namespace _STL, `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else __unguarded_linear_insert(last, val, comp);`, pair with its user ctors, copy_backward -> __copy_backward_ptrs forceinline) plus a same-TU noinline CSmallVector::Copy body (chunk-422 recipe: Copy receives &val.second, and a visible body that never keeps it leaves val uncaptured, so val.first stays in ebx across the __copy_backward call).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FC3E0, 137 bytes, `ret 0x10` (/Gr: first, last in ecx/edx; val by value; comp).
// Label correction: catalog "Vector_Erase_WithMove" -> _STL::__linear_insert of the keyed event vector (called from
//   __insertion_sort 0x006FCB30).
// Callees: __copy_backward 0x006FB830 (same-TU noinline body so the empty tag shares the dead comp slot), CSmallVector::Copy
//   0x006FB5C0 (same-TU noinline body, the landed source's shape), __unguarded_linear_insert 0x006FB960, operator
//   delete[] 0x00BFEB1C.
// Layout: element pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > (12 bytes: key, Start, Size).
void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C

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