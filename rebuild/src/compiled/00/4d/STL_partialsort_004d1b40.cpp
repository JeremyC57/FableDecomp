// _STL::__partial_sort(pair<float, EConversationAttitude>* first, pair<float, EConversationAttitude>* middle, pair<float, EConversationAttitude>* last, pair<float, EConversationAttitude>*, CKeyPairCompareLess comp) -- STLport partial sort of a subtitled message's attitude tags by time: heapify the prefix, sift every earlier tail tag into it, then heap-sort the prefix
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D1B40, 114 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU, explicit
//   __fastcall here: first in ecx, middle in edx, then last, the value-type tag pointer and the comparator slot).
// Callers: 0x004D1F6C (partial_sort front end) and the depth-limit fallback of __introsort_loop 0x004D2160 (this chunk).
// Callees: __make_heap 0x004D0BF0 (this chunk), __adjust_heap 0x004D04F0 (landed), sort_heap 0x004D15A0 (landed).
// Identity: CSubtitledMessage::AttitudeTags, CVectorMap<float, EConversationAttitude> entries (8 bytes); the catalog's
//   "std::introsort_Heapsort_Top_Element" is descriptive.
// Shape: STLport 4.x `__partial_sort` with make_heap and __pop_heap inlined. The by-value T(*i) copies its float key
//   through the FPU (`fld [esi]` ... `fstp [eax]` straight into the argument area: the pair's copy constructor
//   initialises a float member) while the assignment `*i = *first` moves dwords.
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
template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*);   // 0x004D0BF0
template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x004D04F0 (landed)
template <class _RandomAccessIter, class _Compare>
void __fastcall sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x004D15A0 (landed)

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIter, class _Compare>
inline void make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
template void __fastcall __partial_sort<TAttitudePair*, TAttitudePair, TAttitudeLess>(TAttitudePair*, TAttitudePair*, TAttitudePair*, TAttitudePair*, TAttitudeLess);
}