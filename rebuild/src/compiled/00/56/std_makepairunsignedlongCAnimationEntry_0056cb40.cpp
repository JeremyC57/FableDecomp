// std::make_pair<unsigned long,CAnimationEntry>(const unsigned long& x, const CAnimationEntry& y) -- return a (key, animation entry) pair by value: copy the key, copy-construct the entry into the hidden result
// Leaf: make_pair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056CB40, length 30 bytes (`ret`: __cdecl, the hidden result pointer is the
//   first stack argument and comes back in eax).
// Caller: 0x0056BA3A (`push entry ; lea eax,[key] ; push eax ; lea ecx,[result] ; push ecx ; call ; add esp,0Ch`), whose
//   result goes straight into 0x0056CB60, the landed copy constructor of pair<unsigned long, CAnimationEntry> (the map's
//   value_type built from the make_pair temporary).
// Label correction: the manifest's `_Pair_base<NUISystem::CComponent* const, std::pair<CWideString,long> >` is propagated;
//   the second member's copy constructor is 0x004B73A0 (three dwords then the counted pointer at +0xc), the 16-byte
//   Ego_r.pdb CAnimationEntry {BankIndex, AnimName, GroupName, Components}, and the body is not a constructor.
// Callee: CAnimationEntry::CAnimationEntry(const CAnimationEntry&) @ 0x004B73A0.
// Shape: STLport `make_pair(const T1& x, const T2& y) { return pair<T1, T2>(x, y); }` with the pair constructor inlined
//   straight into the hidden result; the key is loaded and the entry reference fetched before `push esi`.
struct CAnimationEntry {                                // 0x10 bytes (Ego_r.pdb)
    long  BankIndex;                                    // +0x00
    void* AnimName;                                     // +0x04 CDefString
    void* GroupName;                                    // +0x08 CDefString
    void* Components;                                   // +0x0c CIVCountedPointer<CAnimComponentMap>
    CAnimationEntry(const CAnimationEntry& other);      // 0x004B73A0
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T1, class T2>
inline pair<T1, T2> make_pair(const T1& x, const T2& y)
{
    return pair<T1, T2>(x, y);
}
}

template std::pair<unsigned long, CAnimationEntry> std::make_pair<unsigned long, CAnimationEntry>(const unsigned long&, const CAnimationEntry&);