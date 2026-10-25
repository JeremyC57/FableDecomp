// _STL::__unguarded_partition(pair<float, EConversationAttitude>* first, pair<float, EConversationAttitude>* last, pair<float, EConversationAttitude> pivot, CKeyPairCompareLess comp) -- STLport quicksort partition of a subtitled message's attitude tags (time -> attitude) around a by-value pivot; returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D0170, 84 bytes, `ret 0Ch` (/Gr algorithm TU, explicit __fastcall here: first in
//   ecx, last in edx, the 8-byte pivot and the empty comparator slot on the stack). No callees.
// Caller: the matching __introsort_loop 0x004D2160 (this chunk).
// Identity: CSubtitledMessage::AttitudeTags, CVectorMap<float, EConversationAttitude, CKeyPairCompareLess<...> > (Ego_r.pdb; the
//   landed CVectorMap<float,EConversationAttitude>::Insert 0x004D1E20 and __adjust_heap 0x004D04F0 share the element).
//   The catalog's "std::introsort_Partition_3way_Float" is descriptive.
// Shape: STLport 4.x `__unguarded_partition` verbatim: both scans are `fld ; fcomp ; fnstsw ; test ah,5 ; jp` float
//   less-thans against the pivot key in its argument slot; the inlined __iter_swap temporary keeps its first word in the
//   8-byte frame (`mov [esp+4],eax`).
enum EConversationAttitude { CONVERSATION_ATTITUDE_NONE = 0 };

namespace _STL {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty, passed by value (4-byte slot)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<float, EConversationAttitude> TAttitudePair;             // 8 bytes
typedef CKeyPairCompareLess<float, EConversationAttitude> TAttitudeLess;

namespace _STL {
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
template TAttitudePair* __fastcall __unguarded_partition<TAttitudePair*, TAttitudePair, TAttitudeLess>(TAttitudePair*, TAttitudePair*, TAttitudePair, TAttitudeLess);
}