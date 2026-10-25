// CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<unsigned long, CSoundPair> >::Sort() -- sort the sound map's (key, value) pairs with its Compare and clear the Dirty flag
// Leaf: ?Sort@?$CVectorMap@KVCSoundPair@@V?$CKeyPairCompareLess@KVCSoundPair@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00470CD8, 25 bytes, thiscall, no arguments.
// Identity (sweep1, 2026-10-25): FINDINGS chunk 457 / hand457 README ("0x00470CD8 is CVectorMap<unsigned long, CSoundPair,
//   CKeyPairCompareLess<..> >::Sort. It was landed as global_TransferToGui_00470cd8, with a CGuiVarTransferBool model");
//   its callee is the landed sound-map std::sort 0x00470CF1 (hand457: CSoundMap::SoundMap, pair<unsigned long,
//   CSoundPair>, 12 bytes); its only caller is 0x004705E9 in the CSoundMap code. The CGuiVarTransferBool::TransferToGui
//   label is propagated across byte-identical CVectorMap::Sort bodies (FINDINGS chunk 380, landed 0x005FA496).
// Layout: retail STLport vector {First, Last, EndOfStorage} at +0, the empty Compare at +0xC, bool Dirty at +0xD (as the
//   landed CVectorMap<long,long,CKeyPairCompareLess<long,long> >::Sort 0x005FA496, whose FableWin.pdb name and model this
//   row follows).
// Callee: std::sort<pair<unsigned long,CSoundPair>*, CKeyPairCompareLess<unsigned long,CSoundPair> > @ 0x00470CF1 (STLport,
//   __fastcall: first in ecx, last in edx, the empty comparator by value on the stack, `ret 4`).
// Shape: `xor eax,eax ; mov al,[esi+0Ch] ; push eax` is the by-value copy of the one-byte empty comparator.
class CSoundPair {                                          // Ego_r.pdb, 8 bytes (a class: mangled V)
public:
    long FirstSound;                                        // +0x0
    long LastSound;                                         // +0x4
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00470CF1
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

template class CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<unsigned long, CSoundPair> >;
