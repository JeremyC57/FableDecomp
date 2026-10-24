// _STL::__median(CThing* const& a, CThing* const& b, CThing* const& c, CThingCompare_Nearest comp) -- STLport median of three for the nearest-first sort: the middle element by the comparator, returned by reference
// Leaf: ??$__median@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091FA00, 132 bytes, __fastcall (&a in ecx, &b in edx; &c and the
//   comparator on the stack), `ret 10h`.
// Identity: STLport 4.6 __median<CThing*, CThingCompare_Nearest>; single caller __introsort_loop 0x009205D0 (call at
//   0x0092063C). The catalog label "Sort_Partition_MedianOfThree" is an autoname.
// Callees: CThingCompare_Nearest::operator() 0x0091F740 (on the comparator's own argument slot).
// Family: the STLport sort<CThing**, CThingCompare_Nearest> instantiation of this TU (CAIStateGroup_TestTestTest::Update
//   0x0091F140 sorts its things by distance to the creature, call at 0x0091F2EF); /Gr algorithm TU: every free template is
//   __fastcall (first in ecx, second in edx when it is a pointer/int, the rest -- the 12-byte comparator last -- on the
//   stack). Same family as the landed CThingCompare_NearestToPos sort (0x007A4260 / 0x007A3B60). The comparator's
//   operator()<T> returns the nearer pointer (its catalog label says <CThingMarker>: the template body is identical for
//   every T), tested as a bool (`test eax,eax`).
// Layout: CThingCompare_Nearest (Ego_r.pdb, 12 bytes) = Pos (C3DVector, byte-packed, memberwise user copy ctor: the
//   comparator words are re-read from its argument slot for every by-value copy).
// Shape: the STLport nested if/else verbatim; each compare pushes the two element values (`mov eax,[b] ; mov ecx,[a] ;
//   push eax ; push ecx`) and calls on `lea ecx,[esp+..]` (the by-value comparator's slot); a/b/c live in edi/esi/ebx.
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

namespace _STL {

template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template CThing* const& __fastcall __median<CThing*, CThingCompare_Nearest>(CThing* const&, CThing* const&, CThing* const&,
    CThingCompare_Nearest);

}