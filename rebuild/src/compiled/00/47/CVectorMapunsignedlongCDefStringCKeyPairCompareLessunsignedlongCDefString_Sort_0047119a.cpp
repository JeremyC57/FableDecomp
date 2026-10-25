// CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<unsigned long, CDefString> >::Sort() -- sort the sound map's (key, value) pairs with its Compare and clear the Dirty flag
// Leaf: ?Sort@?$CVectorMap@KVCDefString@@V?$CKeyPairCompareLess@KVCDefString@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047119A, 25 bytes, thiscall, no arguments.
// Identity (sweep1, 2026-10-25): FINDINGS chunk 457 / hand457 README ("0x0047119A is the CDefString CVectorMap::Sort (landed placeholder
//   CVectorMap_T_0047119a)"); its callee 0x004711B3 is the CriteriaMap std::sort (the __lg loop, then __introsort_loop
//   0x00471338 (landed) and __final_insertion_sort 0x004711F5 over 8-byte entries); its only caller is 0x004706EA in the
//   CSoundMap code. Element = CSoundMap::CriteriaMap's pair<unsigned long, CDefString>.
// Layout: retail STLport vector {First, Last, EndOfStorage} at +0, the empty Compare at +0xC, bool Dirty at +0xD (as the
//   landed CVectorMap<long,long,CKeyPairCompareLess<long,long> >::Sort 0x005FA496, whose FableWin.pdb name and model this
//   row follows).
// Callee: std::sort<pair<unsigned long,CDefString>*, CKeyPairCompareLess<unsigned long,CDefString> > @ 0x004711B3 (STLport,
//   __fastcall: first in ecx, last in edx, the empty comparator by value on the stack, `ret 4`).
// Shape: `xor eax,eax ; mov al,[esi+0Ch] ; push eax` is the by-value copy of the one-byte empty comparator.
class CDefString {                                          // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    long TablePos;                                          // +0x0 def-string table index
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x004711B3
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

template class CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<unsigned long, CDefString> >;
