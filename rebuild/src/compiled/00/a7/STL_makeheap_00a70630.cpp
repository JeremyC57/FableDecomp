// _STL::__make_heap(pair<CCharString,C3DHelperPoint>* first, pair<CCharString,C3DHelperPoint>* last, CCompareString<C3DHelperPoint> comp, pair<CCharString,C3DHelperPoint>*, int*) -- build a heap over the helper-point map entries (name order) by sifting down every parent from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A70630, 124 bytes (+4 int3), /Gr `__fastcall` (first in ecx, last in
//   edx; the comparator, the value-type and distance-type tag pointers on the stack, `ret 0Ch`).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (the by-value element copy), _STL::__adjust_heap
//   0x00A6FA20 (landed "SGI_AdjustHeapstring20"; __fastcall first/hole, then len, the 20-byte value, the comparator).
// Identity: the manifest's "HeapSort_MakeHeap_PairStringHelperPoint" is descriptive. 20-byte (0x66666667 + `sar 3`)
//   pair<CCharString, C3DHelperPoint> entries of CPhysicsMesh::HelperPoints ordered by CCompareString.
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- first
//   is kept in the `push ecx` frame word; the by-value copy is built in the argument area by STLport's pair copy
//   constructor (string copy call, then the C3DHelperPoint words through a pointer at element + 0xC).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};

class C3DVector {                                       // user memberwise copy ctor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DHelperPoint {                                  // 16 bytes (Ego_r.pdb)
public:
    C3DVector Position;                                 // +0x00
    long      BoneIndex;                                // +0x0C
};

namespace _STL {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class V> class CCompareString {
public:
    bool operator()(const _STL::pair<CCharString, V>& a, const _STL::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};

namespace _STL {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00A6FA20

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
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

typedef pair<CCharString, C3DHelperPoint> THelperEntry;
template void __fastcall __make_heap<THelperEntry*, CCompareString<C3DHelperPoint>, THelperEntry, int>(THelperEntry*, THelperEntry*, CCompareString<C3DHelperPoint>, THelperEntry*, int*);

}