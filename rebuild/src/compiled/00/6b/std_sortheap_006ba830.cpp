// std::sort_heap(SCountedPointerPair* first, SCountedPointerPair* last, CKeyPairCompareLess comp) -- SGI sort_heap over a (dword key, CCountedPointer<T>) vector map: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BA830, length 122 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,C3DHelperPoint>*,...>` is propagated over the
// byte-identical skeleton (twin of 0x00A83600). The callee 0x006BA180 is the SGI `__pop_heap` of a 12-byte pair
// (`*result = *first` assigns a dword key and a CCountedPointer inline: release through CCPPointerInfo's
// DeleteFunc + operator delete, then `inc [info]`); its manifest label `_Pop_heap<pair<CCharString,
// CCountedPointer<CEntityScriptBindingBase>>*,...>` is itself propagated -- the key is a plain dword, not a
// CCharString (no string copy anywhere in either body).
// Callees: __pop_heap @ 0x006BA180 (out of line, __fastcall, `ret 18h`).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline; the value copy is the pair's implicit copy constructor in the argument area: the key,
// then CCountedPointer's inline copy (Data, Info, `test ; je ; inc [Info->RefCount]`). CCountedPointer /
// CCPPointerInfo members from Ego_r.pdb (struct_layouts_egor.tsv).
class CCPPointerInfo { public:                     // 12 bytes
    unsigned long RefCount;                 // +0x00
    void (__fastcall* DeleteFunc)(void*);   // +0x04
    void*         Data;                     // +0x08
};
struct CCountedPointer {                    // CCountedPointer<T>, 8 bytes
    void*           Data;                   // +0x00
    CCPPointerInfo* Info;                   // +0x04
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
};
struct SCountedPointerPair {                // std::pair<unsigned long, CCountedPointer<T>> (12 bytes)
    unsigned long   first;                  // +0x00
    CCountedPointer second;                 // +0x04
};
struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SCountedPointerPair& a, const SCountedPointerPair& b) const { return a.first < b.first; }
};

void __fastcall __pop_heap(SCountedPointerPair* first, SCountedPointerPair* last, SCountedPointerPair* result,
                           SCountedPointerPair value, CKeyPairCompareLess comp, int*);   // 0x006BA180

inline void __pop_heap_aux(SCountedPointerPair* first, SCountedPointerPair* last, SCountedPointerPair*,
                           CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SCountedPointerPair(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(SCountedPointerPair* first, SCountedPointerPair* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SCountedPointerPair*)0, comp);
}

void __fastcall sort_heap(SCountedPointerPair* first, SCountedPointerPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}