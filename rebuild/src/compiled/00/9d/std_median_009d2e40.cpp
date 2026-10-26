// std::__median(unsigned long const& a, unsigned long const& b, unsigned long const& c, CompareDataOffset comp) -- STLport median-of-three pivot choice of a bank's entry indices by data offset
// Leaf: ??$__median@KUCompareDataOffset@@@std@@YIABKABK00UCompareDataOffset@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D2E40, 132 bytes (+int3 pad to 144), `__fastcall` `ret 8` (&a in ecx, &b in
//   edx; &c and the 4-byte comparator by value on the stack); returns the chosen reference in eax.
// Callee: CBankFile::CompareDataOffset::operator()(unsigned long, unsigned long) 0x009CC640 (landed; PDB ??RCompareDataOffset@CBankFile@@QAE_NKK@Z, non-const).
// Identity (proven): _Tp = unsigned long, _Compare = CompareDataOffset. Its only caller is the landed __introsort_loop 0x009D48C0, which declares __median 0x009D2E40 over unsigned long with this comparator (catalog binding ??$__median@KUCompareDataOffset@@@std@@..., which this source defines); every compare calls 0x009CC640; `ret 8` is &c plus the one-pointer comparator.
//   Family H005: 7 bodies that differ only in the `ret` immediate (the comparator's size) and, for a comparator returning
//   the nearer pointer, `test eax,eax` for `test al,al`; retail has no identical-code folding, so each address is exactly
//   one instantiation. The manifest label "CBankFile_CompareDataOffset_Median" is descriptive.
// Shape: STLport 4.6 _algo.c __median verbatim (nested if/else returning a, b or c); each compare pushes the two element
//   values and calls the comparator out of line on `lea ecx,[esp+..]`, its own argument slot.
class CBankFile;
// CBankFile::CompareDataOffset (Ego_r.pdb, nested, 4 bytes); modelled at namespace scope as the landed caller 0x009D48C0 and
// callee 0x009CC640 do (engine/CBankFile.h cannot take a nested class), so this defines the catalog's binding name.
struct CompareDataOffset {
    CBankFile* BankFile;                            // +0x00
    bool operator()(unsigned long a, unsigned long b);   // 0x009CC640
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
template unsigned long const& __fastcall __median<unsigned long, CompareDataOffset >(unsigned long const&, unsigned long const&, unsigned long const&, CompareDataOffset);
}