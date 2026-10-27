// std::__pop_heap(pair<long,CRandomAppearanceMorph::CSkeletalMorph>* first, last, result, value, CKeyPairCompareLess<..>, int*) -- move the heap top to result, then sift value down from the root
// Leaf: ??$__pop_heap@PAU?$pair@JVCSkeletalMorph@CRandomAppearanceMorph@@@std@@U12@V?$CKeyPairCompareLess@JVCSkeletalMorph@CRandomAppearanceMorph@@@@H@std@@YIXPAU?$pair@JVCSkeletalMorph@CRandomAppearanceMorph@@@0@00U10@V?$CKeyPairCompareLess@JVCSkeletalMorph@CRandomAppearanceMorph@@@@PAH@Z
// Flags: /Gr
// Retail: Fable.exe @ 0x0086B4D0, 87 bytes, __fastcall (first ecx, last edx; result, the 16-byte value, the comparator and
// the distance-type pointer on the stack), `ret 1Ch`.
// Identity (proven by shape and callee): STLport 4.6 __pop_heap is `*result = *first; __adjust_heap(first, 0, last - first,
// value, comp);` -- exactly this body (16-byte copy, `(last - first) >> 4`, hole 0 in edx); the callee 0x0086B070 is bound
// as ??$__adjust_heap@PAU?$pair@JVCSkeletalMorph@CRandomAppearanceMorph@@@std@@HU12@V?$CKeyPairCompareLess@JVCSkeletalMorph@CRandomAppearanceMorph@@@@@std@@...
// (landed: element pair<long, CRandomAppearanceMorph::CSkeletalMorph>, 16 bytes), so this is its pop-heap instantiation.
// Its callers 0x0086B6BD / 0x0086C70D are the sort-heap / partial-sort loops of the same CRandomAppearanceMorph sort.
// Label correction: catalog "_gapscan::sub_0086b4d0".
// Callees: 0x0086B070 std::__adjust_heap<pair<long, CRandomAppearanceMorph::CSkeletalMorph>*, int, pair<..>,
// CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph> > (__fastcall).
// Layout: Ego_r.pdb CRandomAppearanceMorph::CSkeletalMorph = { long GroupID, CDefString Name, bool AllowVariation } (12 bytes),
// as landed 0x0086B070 models it; its inline copy constructor is written as an assignment there, so the by-value argument
// is built key first, then `add eax,4` and the morph word by word.
// Shape: the implicit pair assignment copies four dwords through edi; the value argument is rebuilt in the callee's slot.
class CDefString {
public:
    long TablePos;
};

class CRandomAppearanceMorph {
public:
    class CSkeletalMorph {
    public:
        long GroupID;                                       // +0x0
        CDefString Name;                                    // +0x4
        bool AllowVariation;                                // +0x8
        CSkeletalMorph(const CSkeletalMorph& other) { *this = other; }
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<long, CRandomAppearanceMorph::CSkeletalMorph> TMorphEntry;
typedef CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph> TMorphLess;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);

template <> void __fastcall __adjust_heap(TMorphEntry* __first, int __holeIndex, int __len, TMorphEntry __val, TMorphLess __comp);   // 0x0086B070

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                  _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template void __fastcall __pop_heap(TMorphEntry*, TMorphEntry*, TMorphEntry*, TMorphEntry, TMorphLess, int*);
}