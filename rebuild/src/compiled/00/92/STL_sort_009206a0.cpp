// std::sort(CThing** first, CThing** last, CThingCompare_Nearest comp) -- STLport sort of a CThing* range nearest-first: introsort to depth 2*lg(n), then the final insertion sort
// Leaf: ??$sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009206A0, 111 bytes (112 with padding), __fastcall (first in ecx, last in edx; the
//   12-byte comparator on the stack), `ret 0Ch`.
// Identity: STLport 4.6 sort<CThing**, CThingCompare_Nearest>; single caller CAIStateGroup_TestTestTest::Update (0x0091F2EF,
//   comparator built from the creature's CThing::GetPosition). The catalog label "Sort_Init" is an autoname.
// Callees: __introsort_loop<CThing**, CThing*, int, CThingCompare_Nearest> 0x009205D0, __final_insertion_sort<CThing**,
//   CThingCompare_Nearest> 0x00920480 (its catalog label "make_heap<CThing**,CThingCompare_BestForTargeting2>" is propagated).
// Family: the STLport sort<CThing**, CThingCompare_Nearest> instantiation of this TU (CAIStateGroup_TestTestTest::Update
//   0x0091F140 sorts its things by distance to the creature, call at 0x0091F2EF); /Gr algorithm TU: every free template is
//   __fastcall (first in ecx, second in edx when it is a pointer/int, the rest -- the 12-byte comparator last -- on the
//   stack). Same family as the landed CThingCompare_NearestToPos sort (0x007A4260 / 0x007A3B60). The comparator's
//   operator()<T> returns the nearer pointer (its catalog label says <CThingMarker>: the template body is identical for
//   every T), tested as a bool (`test eax,eax`).
// Layout: CThingCompare_Nearest (Ego_r.pdb, 12 bytes) = Pos (C3DVector, byte-packed, memberwise user copy ctor: the
//   comparator words are re-read from its argument slot for every by-value copy).
// Shape: the landed 0x007A4260 body: `if (first != last) { __introsort_loop(first, last, (T*)0, __lg(last - first) * 2,
//   comp); __final_insertion_sort(first, last, comp); }`, __lg as the shift-count loop.
#pragma pack(push, 1)
class C3DVector {                               // retail: byte-packed, memberwise user copy ctor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

class CThing;

class CThingCompare_Nearest {                   // Ego_r.pdb, 12 bytes, passed by value
public:
    C3DVector Pos;                              // +0x00
    template <class T> T* operator()(T* a, T* b) const;   // 0x0091F740 (returns the nearer thing; null loses)
};

namespace std {

template <class _Size>
inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                 _Size __depth_limit, _Compare __comp);   // 0x009205D0

template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last,
                                       _Compare __comp);                  // 0x00920480

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (CThing**)0, __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}

template void __fastcall sort<CThing**, CThingCompare_Nearest>(CThing**, CThing**, CThingCompare_Nearest);

}