// std::__make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp, _Tp*, _Distance*) -- (pair<long, CCountedPointer<CRegionConnectionNode> > under CKeyPairCompareLess) heapify: sift every parent down from the last one to the root
// Leaf: ??$__make_heap@PAU?$pair@JV?$CCountedPointer@VCRegionConnectionNode@@@@@std@@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCRegionConnectionNode@@@@@@U12@H@std@@YIXPAU?$pair@JV?$CCountedPointer@VCRegionConnectionNode@@@@@0@0V?$CKeyPairCompareLess@JV?$CCountedPointer@VCRegionConnectionNode@@@@@@0PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00829090, 107 bytes, /Gr `__fastcall` (first in ecx, last in edx; the empty
//   comparator and the dead value/distance tags on the stack), `ret 0Ch`.
// Identity (proven): std::__make_heap<pair<long, CCountedPointer<CRegionConnectionNode> >*, CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> >, pair<...>, int>; evidence: reached from the sort 0x008298F0 whose element H047 proves (CRegionConnectionGraph::GraphNodes (InitialiseGraph 0x00828710); swap 0x00827FD0) by direct calls 0x008298F0 -> 0x008297F0 -> 0x00829630 -> 0x00829090 (sort -> introsort_loop -> partial_sort -> this); its callers 0x00829199, 0x00829641 are that TU's partial_sort / heap code.
// Label: the manifest's "std_make_partial_heap_NavConnectorNode_v2" is propagated (family H049: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one element type).
// Callee: __adjust_heap 0x00828DC0 (__fastcall: first ecx, parent edx, then len, the 12-byte value copy-constructed in its
//   argument slot (`inc [Info]`) and the comparator, `ret 14h`).
// Shape: STLport 4.6 _heap.c __make_heap verbatim (landed 0x00ADB420 model): `len = last - first` by the 0x2AAAAAAB
//   reciprocal, the parent index `(len - 2) / 2` by the signed cdq/sub/sar idiom.
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
class CRegionConnectionNode;

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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);            // 0x00828DC0
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}
typedef pair<long, CCountedPointer<CRegionConnectionNode> > _Elem;
typedef CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> > _Cmp;
template void __fastcall __make_heap<_Elem*, _Cmp, _Elem, int>(_Elem*, _Elem*, _Cmp, _Elem*, int*);
} // namespace std