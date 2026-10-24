// std::_Push_heap(std::pair<unsigned long,CAnimationEntry>* first, int hole, int top, std::pair<unsigned long,CAnimationEntry> val, CKeyPairCompareLess<unsigned long,CAnimationEntry> pred) -- percolate a CRC-keyed animation entry up a max-heap, then release the by-value entry's component map
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x005E1310, 237 bytes (/O2). No direct calls: the component map's virtual slot 1
// (destroy on the last reference) is the only callee.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 20-byte element
// (`lea eax,[ebx+ebx*4]` * 4): unsigned long key +0, CAnimationEntry +4 (Ego_r.pdb: BankIndex,
// CDefString AnimName, GroupName, CIVCountedPointer<CAnimComponentMap> Components), as labelled. The
// implicit pair/entry operator= moves four dwords and then runs the intrusive CIVCountedPointer
// assignment (release the old object: `dec [obj+4]`, slot 1 at zero, store 0; adopt and add a
// reference) -- both sites expanded inline, as in the landed __copy_backward 0x005E0E70 of this TU.
// The key test is UNSIGNED (`jae`); the by-value entry's pointer is released at the end without a
// zero store (its destructor). /Gr: first=ecx, hole=edx; `ret 1Ch` = top + 20-byte pair + comparator.
namespace N005e1310 {
struct CAnimComponentMap {                                // intrusively counted: count at +4
    virtual void v00();
    virtual void Destroy();                               // slot 1 (+0x04)
    long m_refCount;                                      // +0x04
};
template <class T> struct CIVCountedPointer {             // one word: the counted object
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object) { if (Object) ++Object->m_refCount; }
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
    ~CIVCountedPointer()
    {
        T* object = Object;
        if (object && --object->m_refCount == 0)
            object->Destroy();
    }
};
struct CDefString {
    long TablePos;                                        // +0x00
};
struct CAnimationEntry {
    long BankIndex;                                       // +0x00
    CDefString AnimName;                                  // +0x04
    CDefString GroupName;                                 // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;      // +0x0C
};
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<unsigned long, CAnimationEntry> Elem;
typedef CKeyPairCompareLess<unsigned long, CAnimationEntry> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}