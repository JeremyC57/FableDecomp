// std::__pop_heap(_RandomAccessIterator first, _RandomAccessIterator last, _RandomAccessIterator result, _Tp val, _Compare comp, _Distance*) -- (pair<long, CCountedPointer<CRegionConnectionNode> > under CKeyPairCompareLess) move the heap top to result, then sift the saved value into the hole
// Leaf: ??$__pop_heap@PAU?$pair@JV?$CCountedPointer@VCRegionConnectionNode@@@@@std@@U12@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCRegionConnectionNode@@@@@@H@std@@YIXPAU?$pair@JV?$CCountedPointer@VCRegionConnectionNode@@@@@0@00U10@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCRegionConnectionNode@@@@@@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00828FE0, 175 bytes, /Gr `__fastcall` (first in ecx, last in edx; result, the
//   12-byte value, the empty comparator and the dead distance tag on the stack), `ret 18h`.
// Identity (proven): std::__pop_heap<pair<long, CCountedPointer<CRegionConnectionNode> >*, pair<...>, CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> >, int>; evidence: reached from the sort 0x008298F0 whose element H047 proves (CRegionConnectionGraph::GraphNodes (InitialiseGraph 0x00828710); swap 0x00827FD0) by direct calls 0x008298F0 -> 0x008297F0 -> 0x00829630 -> 0x00828FE0 (sort -> introsort_loop -> partial_sort -> this); its four callers 0x00829148, 0x008291F8, 0x00829483, 0x00829677 are that TU's heap code.
// Label: the manifest's "_Pop_heap<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>*,int,std::pair<class_CCharString,class_CCountedPointer<class_CEntityScriptBindingBase>_>,CCompareString<CCountedPointer<CEntityScriptBindingBase>_>_>" is propagated (family H048: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one element type).
// Callees: __adjust_heap 0x00828DC0 (__fastcall: first, hole 0, len, the value copied into its argument slot, comp) and
//   operator delete 0x00BFE9BC (the releases in the inline pair assignment and in the by-value parameter's destructor).
// Shape: STLport 4.6 _heap.h __pop_heap verbatim: `*result = *first; __adjust_heap(first, 0, last - first, val, comp);`
//   (len by the 0x2AAAAAAB reciprocal, 12-byte stride); the pair's implicit assignment inlines CCountedPointer::operator=
//   (landed 0x00448380 body), and the callee-destroyed `val` is released after the call.
class CCPPointerInfo { public:                                    // CCountedPointer's shared block
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
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                  _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
typedef pair<long, CCountedPointer<CRegionConnectionNode> > _Elem;
typedef CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> > _Cmp;
template void __fastcall __pop_heap<_Elem*, _Elem, _Cmp, int>(_Elem*, _Elem*, _Elem*, _Elem, _Cmp, int*);
} // namespace std