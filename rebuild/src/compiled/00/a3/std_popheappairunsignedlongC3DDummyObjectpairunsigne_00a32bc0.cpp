// std::__pop_heap<pair<unsigned long, C3DDummyObject>*, pair<unsigned long, C3DDummyObject>, CKeyPairCompareLess<unsigned long, C3DDummyObject>, int>(...) -- heap pop step of the dummy-object sort
// Retail: Fable.exe 0x00A32BC0, 87 bytes, __fastcall (/Gr TU: first in ecx, last in edx; result, the 0x38-byte value,
// the comparator and the distance pointer on the stack, `ret 44h`): *result = *first (rep movsd x 0xE), then
// __adjust_heap(first, 0, (last - first) / 0x38, value, comp) (0x00A32500) with the value copied into the argument
// area (key, then rep movsd x 0xD).
// Identity: STLport 4.6 `__pop_heap(__first, __last, __result, _Tp __val, _Compare __comp, _Distance*)
// { *__result = *__first; __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp); }`, kept out
// of line here and called from the heap routines 0x00A32DC9 / 0x00A33389. The 0x38-byte element is
// pair<unsigned long, C3DDummyObject> (Ego_r.pdb C3DDummyObject: Transform CMatrix3x4 +0, BoneIndex +0x30) of
// C3DMeshStats::DummyObjects (CPreAllocVectorMap<unsigned long, C3DDummyObject, CKeyPairCompareLess<...> >); the PDB
// name sets hold its _Adjust_heap<pair<unsigned long, C3DDummyObject>*, int, ..., CKeyPairCompareLess<unsigned long,
// C3DDummyObject> >.
// Leaf: ??$__pop_heap@PAU?$pair@KVC3DDummyObject@@@std@@U12@V?$CKeyPairCompareLess@KVC3DDummyObject@@@@H@std@@
class CMatrix3x4 { public: float M[12]; };

class C3DDummyObject {
public:
    CMatrix3x4 Transform;                               // +0x00
    long       BoneIndex;                               // +0x30
};

namespace std {
    template <class _T1, class _T2> struct pair {
        _T1 first;
        _T2 second;
        pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's user copy ctor: key, then the block
    };
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
    template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
    void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                                  _Compare __comp);                                                   // 0x00A32500

    template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
    inline void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                      _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
    {
        *__result = *__first;
        __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
    }

    template void __fastcall
    __pop_heap<pair<unsigned long, C3DDummyObject>*, pair<unsigned long, C3DDummyObject>,
               CKeyPairCompareLess<unsigned long, C3DDummyObject>, int>(
        pair<unsigned long, C3DDummyObject>*, pair<unsigned long, C3DDummyObject>*, pair<unsigned long, C3DDummyObject>*,
        pair<unsigned long, C3DDummyObject>, CKeyPairCompareLess<unsigned long, C3DDummyObject>, int*);
}