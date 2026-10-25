// std::__make_heap(pair<float, EConversationAttitude>* first, pair<float, EConversationAttitude>* last, CKeyPairCompareLess comp, pair<float, EConversationAttitude>*, int*) -- STLport heapify of a subtitled message's attitude tags by time: sift every parent down, from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D0BF0, 74 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU, explicit
//   __fastcall here: first in ecx, last in edx, then the comparator slot and the two tag pointers).
// Callers: 0x004D1089 (make_heap front end) and the __partial_sort 0x004D1B40 (this chunk).
// Callee: __adjust_heap(first, parent, len, T(*(first + parent)), comp) 0x004D04F0 (landed as the Dinkumware-named
//   `_Adjust_heap` of the (float key, EConversationAttitude) pair).
// Identity: CSubtitledMessage::AttitudeTags, CVectorMap<float, EConversationAttitude> entries (8 bytes); the catalog's
//   "std::make_heap_Pair" is descriptive.
// Shape: STLport 4.x `__make_heap` verbatim -- len is `sar 3`, the element is re-indexed every pass (`[edi+esi*8]`, no
//   strength reduction for an 8-byte stride) and copied into the argument area by the pair's copy constructor.
enum EConversationAttitude { CONVERSATION_ATTITUDE_NONE = 0 };

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

typedef std::pair<float, EConversationAttitude> TAttitudePair;             // 8 bytes
typedef CKeyPairCompareLess<float, EConversationAttitude> TAttitudeLess;

namespace std {
template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x004D04F0 (landed)

template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}
template void __fastcall __make_heap<TAttitudePair*, TAttitudeLess, TAttitudePair, int>(TAttitudePair*, TAttitudePair*, TAttitudeLess, TAttitudePair*, int*);
}