// _STL::__make_heap(pair<CCharString,unsigned long>* first, pair<CCharString,unsigned long>* last, CKeyPairCompareLess<CCharString,unsigned long> comp, pair<CCharString,unsigned long>*, int*) -- STLport heapify: sift every parent from (len - 2) / 2 down to the root with __adjust_heap
// Flags: /Gr
// Leaf: ??$__make_heap@PAU?$pair@VCCharString@@K@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B6D20, 88 bytes (+ int3 padding), `ret 0Ch` (/Gr TU: first in ecx,
//   last in edx; the empty comparator, the unused T* and distance* on the stack).
// Identity: the CVectorMap<CCharString, unsigned long> heap (the callee 0x009B5C70 is the landed __adjust_heap over
//   pair<CCharString,unsigned long>, FableWin.pdb _Adjust_heap<pair<CCharString,unsigned long>*,int,...,
//   CKeyPairCompareLess<CCharString,unsigned long>>); called from make_heap users 0x009B7789 / 0x009B86CC.
//   Catalogued as `MakeHeap_Pair8byte_CCharString`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (the by-value element built in the argument area,
//   `second` copied after it), __adjust_heap(first, parent, len, value, comp) @ 0x009B5C70.
// Shape: STLport _heap.c `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- first is
//   spilled to the frame (`push ecx`), the signed halving is `cdq ; sub ; sar`.
class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
private:
    void* PStringData;
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const;
};

namespace _STL {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x009B5C70

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
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

template void __make_heap(pair<CCharString, unsigned long>*, pair<CCharString, unsigned long>*,
                          CKeyPairCompareLess<CCharString, unsigned long>, pair<CCharString, unsigned long>*, int*);
}