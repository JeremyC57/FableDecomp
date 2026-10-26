// _global::UninitFillN_AnimationEntryPair_005e1740(CAnimationEntryPair* first, unsigned int n, const CAnimationEntryPair& x, const __false_type&) -- STLport __uninitialized_fill_n<pair<unsigned long, CAnimationEntry>*>: copy-construct n keyed animation entries (add a reference to the component map), return first + n
// Leaf: UninitFillN_AnimationEntryPair_005e1740
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1740, length 69 bytes (+11 int3 pad), `ret 8`.
// Callers: 0x005E189A (0x005E1820's neighbour region: the landed std::fill<pair<unsigned long, CAnimationEntry>*> 0x005E1820
//   is the assignment counterpart of this element), 0x005E3296 (landed vector<Element>::_M_insert_overflow 0x005E31F0,
//   20-byte element with the counted pointer last) and 0x005E33C3 (resize 0x005E32F0).
// Identity: the catalog label `_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>*, ...>` is
//   propagated. Types from Ego_r.pdb as the landed 0x005E1820: CAnimationEntry (16 bytes) = BankIndex +0, AnimName
//   CDefString +4, GroupName CDefString +8, Components CIVCountedPointer<CAnimComponentMap> +0xC (count at +4 of the map);
//   the pair is 20 bytes (first +0, second +4).
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx -> eax as `cur`,
//   n in edx -> ecx counted down, &x on the stack -> edx loaded once). STLport pair's memberwise user copy constructor over
//   a member whose implicit copy is non-trivial keeps a second induction register at &second + 8 (`lea esi,[eax+0Ch]`,
//   GOTCHAS 00769000); the key, BankIndex and the two one-word CDefStrings, then `P(x.P) { if (P) ++P->m_count; }`;
//   placement-new null test.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

class CAnimComponentMap { public:                      // CIVCountedPointeeBase-derived: vptr, intrusive count
    void* __vftable;                            // +0x00
    long  m_count;                              // +0x04
};

struct CIVCountedPointer_CAnimComponentMap {    // CIVCountedPointer<CAnimComponentMap>
    CAnimComponentMap* Object;
    CIVCountedPointer_CAnimComponentMap(const CIVCountedPointer_CAnimComponentMap& x) : Object(x.Object) { if (Object) ++Object->m_count; }
};

class CDefString { public:                             // one word (Ego_r.pdb: TablePos)
    long TablePos;                              // +0x00
};

class CAnimationEntry { public:                        // 16 bytes
    long                                BankIndex;     // +0x00
    CDefString                          AnimName;      // +0x04
    CDefString                          GroupName;     // +0x08
    CIVCountedPointer_CAnimComponentMap Components;    // +0x0C
};

struct CAnimationEntryPair {                    // std::pair<unsigned long, CAnimationEntry>, 20 bytes
    unsigned long   first;                      // +0x00
    CAnimationEntry second;                     // +0x04
    CAnimationEntryPair(const CAnimationEntryPair& o) : first(o.first), second(o.second) { }
};

CAnimationEntryPair* __fastcall UninitFillN_AnimationEntryPair_005e1740(CAnimationEntryPair* first, unsigned int n, const CAnimationEntryPair& x, const __false_type&)
{
    CAnimationEntryPair* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CAnimationEntryPair(x);
    return cur;
}