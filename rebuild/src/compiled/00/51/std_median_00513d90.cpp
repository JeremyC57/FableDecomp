// std::__median(CThingBuilding* const& a, CThingBuilding* const& b, CThingBuilding* const& c, CIsHouseLargerThan comp) -- STLport median-of-three pivot choice of the region population's houses (largest first)
// Leaf: ??$__median@PAVCThingBuilding@@VCIsHouseLargerThan@@@std@@YIABQAVCThingBuilding@@ABQAV1@00VCIsHouseLargerThan@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00513D90, 132 bytes (+int3 pad to 144), `__fastcall` `ret 8` (&a in ecx, &b in
//   edx; &c and the empty comparator by value on the stack); returns the chosen reference in eax.
// Callee: CIsHouseLargerThan::operator()(const CThingBuilding*, const CThingBuilding*) const 0x00512410 (landed; PDB ??RCIsHouseLargerThan@@QBE_NPBVCThingBuilding@@0@Z).
// Identity (proven): _Tp = CThingBuilding*, _Compare = CIsHouseLargerThan. Its only caller is the __introsort_loop 0x0051BAF0 of std::sort<CThingBuilding**, CIsHouseLargerThan> 0x0051C400 (CWorldMap::PopulateRegionWithVillagers 0x00502E90), whose landed relatives 0x005154C0 / 0x00515570 / 0x005140A0 are the same instantiation; every compare calls 0x00512410 and `ret 8` is &c plus the empty comparator slot.
//   Family H005: 7 bodies that differ only in the `ret` immediate (the comparator's size) and, for a comparator returning
//   the nearer pointer, `test eax,eax` for `test al,al`; retail has no identical-code folding, so each address is exactly
//   one instantiation. The manifest label "IntrosortShellsort_MedianOfThree" is descriptive.
// Shape: STLport 4.6 _algo.c __median verbatim (nested if/else returning a, b or c); each compare pushes the two element
//   values and calls the comparator out of line on `lea ecx,[esp+..]`, its own argument slot.
class CThingBuilding;
class CIsHouseLargerThan {                          // Ego_r.pdb: empty functor
public:
    bool operator()(const CThingBuilding* a, const CThingBuilding* b) const;   // 0x00512410
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
template CThingBuilding* const& __fastcall __median<CThingBuilding*, CIsHouseLargerThan >(CThingBuilding* const&, CThingBuilding* const&, CThingBuilding* const&, CIsHouseLargerThan);
}