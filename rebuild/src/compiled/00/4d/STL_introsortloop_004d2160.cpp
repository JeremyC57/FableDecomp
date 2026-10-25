// _STL::__introsort_loop(pair<float, EConversationAttitude>* first, pair<float, EConversationAttitude>* last, pair<float, EConversationAttitude>*, int depth_limit, CKeyPairCompareLess comp) -- STLport introsort core over a subtitled message's attitude tags by time: quicksort around a median-of-three pivot until runs are 16 or shorter, heap sort at the depth limit
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2160, 224 bytes, `ret 0Ch` (/Gr algorithm TU, explicit __fastcall here: first in
//   ecx, last in edx, then the value-type tag pointer, the depth limit and the comparator slot).
// Callers: itself (0x004D220D) and the sort front end 0x004D24A8 (landed 0x004D2470).
// Callees: __unguarded_partition 0x004D0170 and __partial_sort 0x004D1B40 (both this chunk).
// Identity: CSubtitledMessage::AttitudeTags, CVectorMap<float, EConversationAttitude> entries (8 bytes); the catalog's
//   "std::introsort_Heapsort_3way_Median" is descriptive.
// Shape: STLport 4.x `__introsort_loop` verbatim: byte length test (`and -8 ; cmp 80h`), the depth limit decremented in
//   its argument slot (re-read every pass), __median as five float compares returning an address (`mov eax,ecx` /
//   `mov eax,edi` / `mov eax,edx`), the pivot copy built in the argument area.
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
template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x004D0170
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x004D1B40

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {                     // __stl_threshold
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
template void __fastcall __introsort_loop<TAttitudePair*, TAttitudePair, int, TAttitudeLess>(TAttitudePair*, TAttitudePair*, TAttitudePair*, int, TAttitudeLess);
}