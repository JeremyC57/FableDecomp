// std::__make_heap(pair<CCharString,C3DDummyObject>* first, pair<CCharString,C3DDummyObject>* last, CCompareString<C3DDummyObject> comp, pair<CCharString,C3DDummyObject>*, int*) -- build a heap over the dummy-object map entries (name order) by sifting down every parent from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A706B0, 131 bytes, /Gr `__fastcall` (first in ecx, last in edx; the
//   comparator, the value-type and distance-type tag pointers on the stack, `ret 0Ch`).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (the by-value element copy), std::__adjust_heap
//   0x00A6FB40 (landed "global_Adjustheap"; __fastcall first/hole, then len, the 56-byte value, the comparator).
// Identity: the manifest's "HeapSort_MakeHeap_PairStringDummyObject" is descriptive. 56-byte (0x92492493 + `sar 5`)
//   pair<CCharString, C3DDummyObject> entries of CPhysicsMesh::DummyObjects ordered by CCompareString.
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- first
//   and len in the two frame words; the by-value copy is built in the argument area by STLport's pair copy
//   constructor (string copy call, then C3DDummyObject's implicit 13-dword `rep movsd`).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};

class CMatrix3x4 {                                      // 48 bytes
public:
    float M[12];
};

class C3DDummyObject {                                  // 0x34 bytes (Ego_r.pdb)
public:
    CMatrix3x4 Transform;                               // +0x00
    long       BoneIndex;                               // +0x30
};

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};

namespace std {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00A6FB40

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

typedef pair<CCharString, C3DDummyObject> TDummyEntry;
template void __fastcall __make_heap<TDummyEntry*, CCompareString<C3DDummyObject>, TDummyEntry, int>(TDummyEntry*, TDummyEntry*, CCompareString<C3DDummyObject>, TDummyEntry*, int*);

}