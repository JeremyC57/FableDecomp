// std::__median(CThing* const& a, CThing* const& b, CThing* const& c, CThingCompare_NearestToPos comp) -- STLport median-of-three pivot choice of the lightning-target sort (nearest first)
// Leaf: ??$__median@PAVCThing@@VCThingCompare_NearestToPos@@@std@@YIABQAVCThing@@ABQAV1@00VCThingCompare_NearestToPos@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A3380, 132 bytes (+int3 pad to 144), `__fastcall` `ret 10h` (&a in ecx, &b in
//   edx; &c and the 12-byte comparator by value on the stack); returns the chosen reference in eax.
// Callee: CThingCompare_NearestToPos::operator()<CThing>(CThing*, CThing*) const 0x007A2F90 (landed; bool in al).
// Identity (proven): _Tp = CThing*, _Compare = CThingCompare_NearestToPos. Its only caller is the __introsort_loop 0x007A4140 of std::sort<CThing**, CThingCompare_NearestToPos> 0x007A4260 (CTCWillLightning::GetTargets 0x007A0D90, CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom 0x008A9C40); the landed __partial_sort 0x007A3FA0 is the same instantiation; the callee 0x007A2F90 is the bool NearestToPos::operator()<CThing> (Ego_r.pdb holds only the <CThing> instantiation) and `ret 10h` is &c plus the 12-byte comparator.
//   Family H005: 7 bodies that differ only in the `ret` immediate (the comparator's size) and, for a comparator returning
//   the nearer pointer, `test eax,eax` for `test al,al`; retail has no identical-code folding, so each address is exactly
//   one instantiation. The manifest label "MedianOf3_CThingAICreature" is descriptive.
// Shape: STLport 4.6 _algo.c __median verbatim (nested if/else returning a, b or c); each compare pushes the two element
//   values and calls the comparator out of line on `lea ecx,[esp+..]`, its own argument slot.
class CThing;
#pragma pack(push, 1)
class C3DVector {                                   // retail: byte-packed
public:
    float X, Y, Z;
};
#pragma pack(pop)
class CThingCompare_NearestToPos {                  // Ego_r.pdb: 12 bytes
public:
    C3DVector Pos;                                  // +0x00
    template <class T> bool operator()(T* a, T* b) const;   // 0x007A2F90 = operator()<CThing>
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
template CThing* const& __fastcall __median<CThing*, CThingCompare_NearestToPos >(CThing* const&, CThing* const&, CThing* const&, CThingCompare_NearestToPos);
}