// std::__partial_sort(const CThingObject** first, const CThingObject** middle, const CThingObject** last, const CThingObject**, CThingCompare_Nearest comp) -- STLport partial sort for the nearest-first sort: heapify [first, middle), sift every nearer element of [middle, last) in, then sort_heap
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- port of the landed byte-identical twin 0x0095DE60 (relocations aside); see Twin below
// Leaf: ??$__partial_sort@PAPBVCThingObject@@
// Element (sweep3, FINDINGS "Family pass 2" G015): the sorted range is const CThingObject* -- the caller
//   CAIStateGroup_TestTestTest::Update 0x0091F140 fills it with GetAllThingsInRoughArea 0x00920200 (type mask 0x807 =
//   CThingObject) before sort 0x009206A0, and FableWin.pdb has sort<vector<const CThingObject*>, CThingCompare_Nearest> /
//   sort_heap<const CThingObject**, CThingCompare_Nearest>; the comparator 0x0091F740 is therefore
//   operator()<const CThingObject> (??$?R$$CBVCThingObject@@@CThingCompare_Nearest@@...), not operator()<CThing> (0x004A8D80).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009204F0, 172 bytes, __fastcall (first in ecx, middle in edx; last, the value-type
//   tag and the 12-byte comparator on the stack), `ret 14h`.
// Identity: STLport 4.6 __partial_sort<const CThingObject**, const CThingObject*, CThingCompare_Nearest>; callers: __introsort_loop 0x009205D0
//   (depth-limit fallback, 0x009205C1) and partial_sort 0x00920692. The catalog label "Sort_IntroSort" is an autoname.
// Callees: __make_heap<const CThingObject**, CThingCompare_Nearest, const CThingObject*, int> 0x0091FDB0 (two null type tags), CThingCompare_Nearest::
//   operator() 0x0091F740, __adjust_heap 0x0091FB30, sort_heap 0x00920160.
// Family: the STLport sort<const CThingObject**, CThingCompare_Nearest> instantiation of this TU (CAIStateGroup_TestTestTest::Update
//   0x0091F140 sorts its things by distance to the creature, call at 0x0091F2EF); /Gr algorithm TU: every free template is
//   __fastcall (first in ecx, second in edx when it is a pointer/int, the rest -- the 12-byte comparator last -- on the
//   stack). Same family as the landed CThingCompare_NearestToPos sort (0x007A4260 / 0x007A3B60). The comparator's
//   operator()<T> returns the nearer pointer (its catalog label says <CThingMarker>: the template body is identical for
//   every T), tested as a bool (`test eax,eax`).
// Layout: CThingCompare_Nearest (Ego_r.pdb, 12 bytes) = Pos (C3DVector, byte-packed, memberwise user copy ctor: the
//   comparator words are re-read from its argument slot for every by-value copy).
// Shape: the STLport body verbatim: make_heap's inline front end pushes the (T*)0 / (int*)0 tags, the loop compares
//   `comp(*i, *first)` and runs the inline __pop_heap (`*i = *first`, then __adjust_heap(first, 0, middle - first, val));
//   the comparator is copied into the argument area for each out-of-line call.
// Twin: the landed 0x0095DE60 has the same bytes up to relocations; its source (with this instance's callee addresses) matches here where the parked model did not.
class CThing;

// C3DVector with the engine's constructors (Ego_r.pdb: X, Y, Z): the user copy constructor makes the 12-byte
// comparator copy memberwise into the argument area; operator- / GetLengthSquared serve the stand-in below.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

class CThing {
public:
    const C3DVector& GetPos() const;                 // 0x004C73D0
};
class CThingObject : public CThing { };              // mask 0x807

// Ego_r.pdb CThingCompare_Nearest: 12 bytes, Pos (C3DVector) +0x00. Its operator() returns the NEARER of the two
// things (T*, not bool); this TU hands the functor to the STL directly (no CAdaptThingCompareToSTLComparitor),
// so every comparison is `test eax,eax` on the returned pointer.
class CThingCompare_Nearest {
public:
    C3DVector Pos;                                        // +0x00
    template <class T> __declspec(noinline) T* operator()(T* a, T* b) const;   // 0x0091F740
};

// Same-TU stand-in for the template operator() (retail 0x0091F740, operator()<const CThingObject>; outside
// these bytes): with the body in the TU VC7.1 knows the call keeps the comparator intact and pipelines its copies.
#pragma auto_inline(off)
template <class T> T* CThingCompare_Nearest::operator()(T* a, T* b) const
{
    if (!a)
        return b;
    if (!b)
        return a;
    C3DVector deltaA = a->GetPos() - Pos;
    C3DVector deltaB = b->GetPos() - Pos;
    return deltaA.GetLengthSquared() < deltaB.GetLengthSquared() ? a : b;
}
#pragma auto_inline(on)
typedef CThingCompare_Nearest TComp;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x0091FB30
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);   // 0x0091FDB0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00920160

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (const CThingObject**)0, (int*)0);
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}
template void __fastcall __partial_sort<const CThingObject**, const CThingObject*, TComp>(const CThingObject**, const CThingObject**, const CThingObject**, const CThingObject**, TComp);
} // namespace std