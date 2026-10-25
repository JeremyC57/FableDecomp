// std::__push_heap<pair<unsigned __int64, list<C2DLineF> >*, int, pair<unsigned __int64, list<C2DLineF> >,
// CKeyPairCompareLess<unsigned __int64, list<C2DLineF> > >(first, holeIndex, topIndex, val, comp) -- percolate a
// CNavigationLayer::SwitchableLineLists entry up a max-heap, then destroy the by-value list
// Flags: /Gr
// Retail: Fable.exe @ 0x00A820E0, 152 bytes (/O2). Called from __adjust_heap @ 0x00A82BE0. Callees: list<C2DLineF>::operator=
// 0x00517720 (catalog "std::list::operator=") and the list destructor 0x005127A0 (walks and frees the nodes, then the
// sentinel).
// Shape: STLport 4.6 `__push_heap` (`holeIndex > topIndex` test, rotated loop) over a 16-byte element (`shl eax,4`): the
// UNSIGNED 64-bit key +0 (high word compared first: `ja done ; jb move`, then the low word `jae done`) and the one-word
// STLport list<C2DLineF> +8 (+0xC padding). The implicit pair operator= is the two key words then the list's out-of-line
// operator=; the by-value pair is destroyed at the end (its list's out-of-line destructor).
// /Gr: first=ecx, holeIndex=edx; `ret 18h` = topIndex + the 16-byte pair + the empty comparator.
// Name: Ego_r.pdb 0x00BC46D0 `??$_Push_heap@PAU?$pair@_KV?$list@VC2DLineF@@...@std@@HU12@V?$CKeyPairCompareLess@_KV?$list@
//   VC2DLineF@@...@@@std@@YIX...` is the same instantiation under Dinkumware (element pair<unsigned __int64,
//   list<C2DLineF> >, the CNavigationLayer::SwitchableLineLists value_type); retail's STLport spelling is __push_heap.
//   Was catalogued `std::_Push_heap` over a descriptive 8-byte SList8_00517720 (retail STLport lists are one word).
// Leaf: ??$__push_heap@PAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@std@@HU12@V?$CKeyPairCompareLess@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@@@std@@YIXPAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@0@HHU10@V?$CKeyPairCompareLess@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@@@Z
class C2DVector {
public:
    float X, Y;
};
class C2DLineF {                                   // 16 bytes (Ego_r.pdb)
public:
    C2DVector Point1;                              // +0x00
    C2DVector Point2;                              // +0x08
};

namespace std {
template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                       // STLport 4.6: one word, the sentinel node pointer
public:
    void* _M_node;
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);   // 0x00517720
    ~list();                                       // 0x005127A0
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

typedef pair<unsigned __int64, list<C2DLineF> > _CSwitchableLineEntry;
typedef CKeyPairCompareLess<unsigned __int64, list<C2DLineF> > _CSwitchableLineLess;
template void __push_heap(_CSwitchableLineEntry*, int, int, _CSwitchableLineEntry, _CSwitchableLineLess);
}
