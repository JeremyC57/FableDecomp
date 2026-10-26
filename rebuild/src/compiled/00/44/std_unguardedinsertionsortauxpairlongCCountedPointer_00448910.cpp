// std::__unguarded_insertion_sort_aux<pair<long, CCountedPointer<CRegionConnectionNode> >*, pair<long, CCountedPointer<CRegionConnectionNode> >, CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> > >(...) -- unguarded linear insert of each element of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux
// Retail: Fable.exe @ 0x00448910, 62 bytes, __fastcall (first ecx, last edx; the unused T* and the 4-byte comparator on the
// stack, `ret 8`); callers 0x00448B27, 0x004493E5 (the final-insertion-sort code). For each i: copy *i by value (key, then
// the counted pointer Data/Info with ++Info->RefCount) and call __unguarded_linear_insert(i, copy, comp) (0x00448520,
// which compares only the long key: `val.first < (i-1)->first`).
// Catalog label std::_Copy_impl<CNavigationPosition*, CNavigationPosition*> is wrong. The element is a 12-byte
// pair<long, CCountedPointer<X> >; X = CRegionConnectionNode follows the neighbouring
// _Destroy_range<pair<long, CCountedPointer<CRegionConnectionNode> > > instance at 0x004488B0 (same element shape); the
// key-only comparator is modelled as the engine's CKeyPairCompareLess.
// Shape: /O2 (16-aligned entry, `nop` loop-head pad).
class CCPPointerInfo { public:
    long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer();
};
class CRegionConnectionNode;
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class RandomIter, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomIter last, T val, Compare comp);   // 0x00448520 for this instance
template <class RandomIter, class T, class Compare>
void __fastcall __unguarded_insertion_sort_aux(RandomIter first, RandomIter last, T*, Compare comp)
{
    for (RandomIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
typedef pair<long, CCountedPointer<CRegionConnectionNode> > _NodeEntry;
template void __fastcall __unguarded_insertion_sort_aux<_NodeEntry*, _NodeEntry, CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> > >(_NodeEntry*, _NodeEntry*, _NodeEntry*, CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> >);
}