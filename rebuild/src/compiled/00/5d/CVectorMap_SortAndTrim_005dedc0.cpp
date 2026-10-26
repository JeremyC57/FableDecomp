// CVectorMap::SortAndTrim() -- CVectorMap<unsigned long, CAnimationEntry>: re-sort the pairs when Dirty, clear Dirty, then give back the unused capacity
// Leaf: ?SortAndTrim@?$CVectorMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DEDC0, 38 bytes. Callers: CAppearanceDef::Transfer (0x00473588, on
// the CAnimationSet `Animation` member at +0x34) and CAnimatingObjectDef::Transfer (0x004ED469) -- the post-load fixup
// of the CAnimationSet::Anims map.
// Callees: SGI std::sort over the pair array @ 0x005E3CD0 (`__fastcall`: first in ecx, last in edx, the comparator by
//          value on the stack; its `imul 66666667h ; sar edx,3` is a divide by 20 = sizeof(pair<ulong, CAnimationEntry>)),
//          CArray<pair<...>>::DeallocateUnusedMemory @ 0x005E3920 (tail jump; same /20 stride).
//
// LABEL CORRECTION: "CTCInventoryQuests::ProcessButtonBReleased" is propagated; the callers, the 20-byte element and the
// Compare/Dirty bytes at +0x0C/+0x0D (PDB CVectorMap = vector + Compare + Dirty; retail vector 12 bytes) make this the
// CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess> member. The method's real name is unknown.
//
// Shape: `cmp byte [esi+0Dh],0 ; je ; mov edx,[esi+4] ; mov ecx,[esi] ; xor eax,eax ; mov al,[esi+0Ch] ; push eax ;
// call ; mov byte [esi+0Dh],0 ; mov ecx,esi ; jmp` -- the empty comparator passed by value is a zero-extended byte
// push; the algorithm TU is /Gr, so the free template is declared __fastcall (GOTCHAS wave 13/14).
class CAnimationEntry { public:
    long m_words[4];
};

template <class K, class V> struct SPair {
    K first;
    V second;
};

template <class K, class V> class CKeyPairCompareLess { public:
};

template <class T> class CArray { public:                  // retail 12 bytes
    T* _First;
    T* _Last;
    T* _End;
    T* begin() { return _First; }
    T* end() { return _Last; }
    void DeallocateUnusedMemory();                  // 0x005E3920
};

template <class T, class C> void __fastcall sort(T* first, T* last, C comp);   // 0x005E3CD0

template <class K, class V, class C> class CVectorMap { public:
    CArray< SPair<K, V> > Data;                     // +0x00
    C Compare;                                      // +0x0C
    bool Dirty;                                     // +0x0D
    void SortAndTrim();
};

template <class K, class V, class C> void CVectorMap<K, V, C>::SortAndTrim()
{
    if (Dirty) {
        sort(Data.begin(), Data.end(), Compare);
        Dirty = false;
    }
    Data.DeallocateUnusedMemory();
}

template void CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> >::SortAndTrim();