// CVectorMap<long, long, CKeyPairCompareLess<long, long> >::Sort() -- sort the map's (key, value) pairs with its Compare and clear the Dirty flag
// Leaf: ?Sort@?$CVectorMap@JJV?$CKeyPairCompareLess@JJ@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005fa496, 25 bytes, thiscall, no arguments.
// Renamed from CGuiVarTransferBool::TransferToGui (a label propagated across byte-identical bodies; FINDINGS chunk 380).
// Name: FableWin.pdb ?Sort@?$CVectorMap@JJV?$CKeyPairCompareLess@JJ@@@@QAEXXZ. Instance: the only direct caller
//       is SetExpressionToButton 0x5F66D9 (call 0x5F6782), which works on CTCInventoryAbilities::ExpressionButtons (struct_layouts_egor: CVectorMap<long,long,
//       CKeyPairCompareLess<long,long> > at PDB +0x208 -> retail +0x1C8, per the landed SetExpressionToButton); retail has no identical-code folding, so the
//       byte-twin Sort for the other CTCInventoryAbilities map is a separate COMDAT.
// Layout (struct_layouts_egor CVectorMap<long,EHeroAbility,...>: 20 bytes, Compare +16, Dirty +17 with the 16-byte PDB vector):
//       retail STLport vector {First, Last, EndOfStorage} at +0, the empty Compare at +0xC, bool Dirty at +0xD.
// Callee: std::sort<pair<long,long>*, CKeyPairCompareLess<long,long> > @ 0x005fa41f (STLport, __fastcall: first in ecx,
//       last in edx, the empty comparator by value on the stack, `ret 4`; body = __lg loop, __introsort_loop, __final_insertion_sort).
// Shape: `xor eax,eax ; mov al,[esi+0Ch] ; push eax` is the by-value copy of the one-byte empty comparator.
// Update 0x5FA4AF/0x5FA4BB (`if (Dirty) Sort();`, tail jmp) follow in the same order (FableWin.pdb ?Update@ for both maps).
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x005fa41f
}


template <class K, class V> class CKeyPairCompareLess {           // empty (FableWin.pdb mangles it as a class)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                             // +0x00
    value_type* Last;                                              // +0x04
    value_type* EndOfStorage;                                      // +0x08
    C           Compare;                                           // +0x0C
    bool        Dirty;                                             // +0x0D
    void Sort();
};

template <class K, class V, class C>
void CVectorMap<K, V, C>::Sort()
{
    std::sort(First, Last, Compare);
    Dirty = false;
}

template class CVectorMap<long, long, CKeyPairCompareLess<long, long> >;
