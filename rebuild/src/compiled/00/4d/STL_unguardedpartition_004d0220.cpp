// std::__unguarded_partition(pair<float, CCharString>* first, pair<float, CCharString>* last, pair<float, CCharString> pivot, CKeyPairCompareLess comp) -- STLport quicksort partition of a subtitled message's animation tags (time -> animation name) around a by-value pivot; returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D0220, 138 bytes, `ret 0Ch` (/Gr algorithm TU, explicit __fastcall here: first in
//   ecx, last in edx, the 8-byte pivot and the comparator slot on the stack; the callee destroys the pivot).
// Caller: the matching __introsort_loop 0x004D2240 (this chunk).
// Callees: CCharString copy 0x0099EC30, operator= 0x0099EFB0 (twice), ~CCharString 0x0099EAE0 (temporary and pivot).
// Identity: CSubtitledMessage::AnimationTags, a CVectorMap<float, CCharString, CKeyPairCompareLess<...> > (Ego_r.pdb;
//   the landed __linear_insert 0x004D0F80 and __adjust_heap 0x004D0590 share the 8-byte element).
// Shape: STLport 4.x `__unguarded_partition` verbatim; the 8-byte pair temporary of the inlined __iter_swap gets an
//   aligned frame (`push ebp ; mov ebp,esp ; and esp,-8`), and the by-value pivot's string is destroyed on the return
//   path (`lea ecx,[ebp+0Ch] ; call ~CCharString`).
class CCharString {
public:
    void* PStringData;                                  // the shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace std {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<float, CCharString> TTextPair;                           // 8 bytes
typedef CKeyPairCompareLess<float, CCharString> TTextLess;

namespace std {
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __value_type(__a));
}

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
template TTextPair* __fastcall __unguarded_partition<TTextPair*, TTextPair, TTextLess>(TTextPair*, TTextPair*, TTextPair, TTextLess);
}