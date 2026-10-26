// std::__median(CThing* const& a, CThing* const& b, CThing* const& c, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- STLport median-of-three pivot choice of the advance-in-cover candidates
// Leaf: ??$__median@PAVCThing@@VCThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0x209ebfc2@@@std@@YIABQAVCThing@@ABQAV1@00VCThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0x209ebfc2@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00978920, 132 bytes (+int3 pad to 144), `__fastcall` `ret 14h` (&a in ecx, &b in
//   edx; &c and the 16-byte comparator by value on the stack); returns the chosen reference in eax.
// Callee: (anonymous namespace)::CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator()(const CThing*, const CThing*) const 0x00978790 (AdvanceInCover's own copy).
// Identity (proven): _Tp = CThing*, _Compare = CThingCompare_InDirectionOfAndNearestToHeuristicBitch. Its only caller is the __introsort_loop 0x00979760 of std::sort 0x00979870 (landed), called by CAIStateGroup_CreatureAdvanceInCover::ProcessCoverThingList 0x00979260; the comparator lives in that TU's anonymous namespace (Ego_r.pdb ?A0xffab493a, two TUs define the class) and its operator() is 0x00978790; `ret 14h` is &c plus the 16-byte comparator.
//   Family H005: 7 bodies that differ only in the `ret` immediate (the comparator's size) and, for a comparator returning
//   the nearer pointer, `test eax,eax` for `test al,al`; retail has no identical-code folding, so each address is exactly
//   one instantiation. The manifest label "std_sort_Median3_HeuristicBitch" is descriptive.
// Shape: STLport 4.6 _algo.c __median verbatim (nested if/else returning a, b or c); each compare pushes the two element
//   values and calls the comparator out of line on `lea ecx,[esp+..]`, its own argument slot.
class CThing;
class C2DVector {                                   // Ego_r.pdb: X +0, Y +4
public:
    float X, Y;
};
namespace {
// Ego_r.pdb: 16 bytes, in the anonymous namespace of CAIStateGroup_CreatureAdvanceInCover.cpp (?A0xffab493a)
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
public:
    C2DVector Pos;                                  // +0x00
    C2DVector Dir;                                  // +0x08
    bool operator()(const CThing* a, const CThing* b) const;   // 0x00978790
};
}

namespace std {
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
template CThing* const& __fastcall __median<CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch >(CThing* const&, CThing* const&, CThing* const&, CThingCompare_InDirectionOfAndNearestToHeuristicBitch);
}