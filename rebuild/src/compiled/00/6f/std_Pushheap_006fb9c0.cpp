// std::_Push_heap(std::pair<unsigned long,CSmallVector<NAnimationEvents::CEvent,32> >* first, int hole, int top, std::pair<unsigned long,CSmallVector<NAnimationEvents::CEvent,32> > val, CKeyPairCompareLess<unsigned long,CSmallVector<NAnimationEvents::CEvent,32> > pred) -- percolate a keyed animation-event small vector up a max-heap, then free the by-value vector
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x006FB9C0, 134 bytes (/O2). Called from the landed _Adjust_heap @ 0x006FBD60
// (same instantiation, same model). Callees: CSmallVector<CEvent,32>::Copy 0x006FB5C0 (landed),
// operator delete[] 0x00BFEB1C (MSVCR71 import thunk).
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 12-byte element:
// unsigned long key +0, CSmallVector<NAnimationEvents::CEvent,32> +4 (Ego_r.pdb: CEvent* Start, int
// Size). The implicit pair operator= is the key move plus CSmallVector::operator= (`&other != this`
// self-test around the out-of-line Copy); the key test is UNSIGNED (`jae`); the by-value vector's
// destructor (`delete[] Start`, `pop ecx`) runs in the callee. /Gr: first=ecx, hole=edx; `ret 14h` =
// top + 12-byte pair + the empty comparator.
// Register shape: retail keeps the value's key in ebp across the Copy calls (compare `cmp ecx,ebp`, final
// `mov [eax],ebp`) and leaves `top` in its slot. The verbatim STLport body makes the lane keep `top` in
// ebp and reload the key (140 bytes); reading the key into a local once, comparing through
// CKeyPairCompareLess's (entry, key) overload and storing key + vector memberwise at the end gives the
// retail allocation (same fix as the landed __unguarded_linear_insert 0x006FB960 of this TU).
void __cdecl operator delete[](void* block);              // 0x00BFEB1C (the lane <new> lacks it)
namespace N006fb9c0 {
namespace NAnimationEvents { struct CEvent { long m_a, m_b; }; }
template<class T, int N> struct CSmallVector {
    T* Start;                                             // +0x00
    int Size;                                             // +0x04
    void Copy(const CSmallVector& other);                 // 0x006FB5C0
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    ~CSmallVector() { delete[] Start; }
};
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const pair<K, V>& entry, const K& key) const { return entry.first < key; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    const unsigned long _Key = _Val.first;
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Key); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    (_First + _Hole)->first = _Key;
    (_First + _Hole)->second = _Val.second;
}
typedef pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > Elem;
typedef CKeyPairCompareLess<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}