// std::__median(CThing* const& a, CThing* const& b, CThing* const& c, CThingCompare_Nearest comp) -- STLport median-of-three pivot choice of the preferred fire points (nearest first)
// Leaf: ??$__median@PAVCThing@@VCThingCompare_Nearest@@@std@@YIABQAVCThing@@ABQAV1@00VCThingCompare_Nearest@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095CEC0, 132 bytes (+int3 pad to 144), `__fastcall` `ret 10h` (&a in ecx, &b in
//   edx; &c and the 12-byte comparator by value on the stack); returns the chosen reference in eax.
// Callee: CThingCompare_Nearest::operator()<CThing>(CThing*, CThing*) const 0x004A8D80 (landed; returns the nearer pointer, tested as a bool with `test eax,eax`).
// Identity (proven): _Tp = CThing*, _Compare = CThingCompare_Nearest. Its only caller is the __introsort_loop 0x0095DF40 of the landed std::sort<CThing**, CThingCompare_Nearest> 0x0095E010 (CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList 0x0095D4C0 / IsRunnable 0x0095D590); its callee 0x004A8D80 is operator()<CThing>, the one the GetPBestThing<..., CThingCompare_Nearest, CThing> searches (0x004A9DA0, 0x009107B0, 0x0093F930) call; `ret 10h` is &c plus the 12-byte comparator.
//   Family H005: 7 bodies that differ only in the `ret` immediate (the comparator's size) and, for a comparator returning
//   the nearer pointer, `test eax,eax` for `test al,al`; retail has no identical-code folding, so each address is exactly
//   one instantiation. The manifest label "Sort_Partition_MedianOfThree_CThingComparator" is descriptive.
// Shape: STLport 4.6 _algo.c __median verbatim (nested if/else returning a, b or c); each compare pushes the two element
//   values and calls the comparator out of line on `lea ecx,[esp+..]`, its own argument slot.
class CThing;                                      // retail key: class (PDB VCThing)
#pragma pack(push, 1)
class C3DVector {                                   // retail: byte-packed
public:
    float X, Y, Z;
};
#pragma pack(pop)
class CThingCompare_Nearest {                       // Ego_r.pdb: 12 bytes
public:
    C3DVector Pos;                                  // +0x00
    template <class T> T* operator()(T* a, T* b) const;   // 0x004A8D80 = operator()<CThing> (the nearer thing)
};

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
template CThing* const& __fastcall __median<CThing*, CThingCompare_Nearest >(CThing* const&, CThing* const&, CThing* const&, CThingCompare_Nearest);
}