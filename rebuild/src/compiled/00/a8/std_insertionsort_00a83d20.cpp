// std::__insertion_sort(_RandomAccessIter first, _RandomAccessIter last, _Compare comp) -- (pair<long, CCountedPointer<CNavConnectorNode> > under CKeyPairCompareLess) insert each entry after the first into the sorted run before it
// Leaf: ??$__insertion_sort@PAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@std@@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCNavConnectorNode@@@@@@@std@@YIXPAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@0@0V?$CKeyPairCompareLess@JV?$CCountedPointer@VCNavConnectorNode@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A83D20, 74 bytes, /Gr `__fastcall` (first in ecx, last in edx, the empty
//   comparator on the stack), `ret 4`.
// Identity (proven): std::__insertion_sort<pair<long, CCountedPointer<CNavConnectorNode> >*, CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> > >; evidence: reached from the sort 0x00A84CF0 whose element H047 proves (CNavQuadTree::ConnectorNodes (LinkToNeighbouringMaps); swap 0x00A7FE70) by direct calls 0x00A84CF0 -> 0x00A84380 -> 0x00A83D20 (sort -> __final_insertion_sort -> this); its callers 0x00A843A9, 0x00A843C7 are the __final_insertion_sort (and inlined sorts) over the same element.
// Label: the manifest's "Vector_MakeHeap_InsertHeap" is propagated (family H050: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one element type).
// Callee: __linear_insert 0x00A834B0 (out of line here: __fastcall first ecx, i edx, then the 12-byte value
//   copy-constructed in its argument slot (`inc [Info]`) and the comparator).
// Shape: STLport 4.6 _algo.c __insertion_sort (landed 0x00ADBE70 model) with __linear_insert left out of line:
//   `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);`.
struct CCPPointerInfo {                                    // CCountedPointer's shared block
    long  RefCount;                                        // +0x0
    void  (__fastcall* DeleteFunc)(void* data);            // +0x4
    void* Data;                                            // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0x0
    CCPPointerInfo* Info;                                  // +0x4
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        T* const              data = o.Data;
        CCPPointerInfo* const info = o.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CNavConnectorNode;

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x0
    _T2 second;                                            // +0x4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
}
template <class K, class V> class CKeyPairCompareLess {    // Ego_r.pdb: orders vector-map entries by key
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val,
                                _Compare __comp);          // 0x00A834B0
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
typedef pair<long, CCountedPointer<CNavConnectorNode> > _Elem;
typedef CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> > _Cmp;
template void __fastcall __insertion_sort<_Elem*, _Cmp>(_Elem*, _Elem*, _Cmp);
} // namespace std