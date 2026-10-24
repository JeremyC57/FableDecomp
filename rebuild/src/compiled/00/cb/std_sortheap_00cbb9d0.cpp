// std::sort_heap(SPersistPair* first, SPersistPair* last, CKeyPairCompareLess comp) -- SGI sort_heap over the scripted-entity persist table (unsigned __int64 UID, CPersistInfo): pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBB9D0, length 135 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,C3DHelperPoint>*,...>` is propagated over the
// byte-identical skeleton. The callee is the landed SGI `__pop_heap` @ 0x00CBA750 over pair<unsigned __int64,
// NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (0x18 bytes: the 0x2AAAAAAB / sar 2 stride here); element
// model reused from that source (Ego_r.pdb CPersistInfo: UID +0, PersistScript +8).
// Callees: __pop_heap @ 0x00CBA750 (out of line, __fastcall, `ret 24h`), CCharString copy constructor @ 0x0099EC30.
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline. The value copy in the argument area is the implicit pair copy: four dword moves (the two
// 64-bit keys), then the PersistScript string through its out-of-line copy constructor.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CPersistInfo {                        // NScript::CRuntimeScriptedEntityPersist::CPersistInfo (0x10 bytes)
    unsigned __int64 UID;                    // +0x00
    CCharString      PersistScript;          // +0x08
};

struct SPersistPair {                        // std::pair<unsigned __int64, CPersistInfo> (0x18 bytes, 8-aligned)
    unsigned __int64 first;                  // +0x00
    CPersistInfo     second;                 // +0x08
};

struct CKeyPairCompareLess {                 // CKeyPairCompareLess<unsigned __int64, CPersistInfo>: empty, passed by value
    bool operator()(const SPersistPair& a, const SPersistPair& b) const { return a.first < b.first; }
};

void __fastcall __pop_heap(SPersistPair* first, SPersistPair* last, SPersistPair* result, SPersistPair value,
                           CKeyPairCompareLess comp, long*);   // 0x00CBA750

inline void __pop_heap_aux(SPersistPair* first, SPersistPair* last, SPersistPair*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SPersistPair(*(last - 1)), comp, (long*)0);
}

inline void pop_heap(SPersistPair* first, SPersistPair* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SPersistPair*)0, comp);
}

void __fastcall sort_heap(SPersistPair* first, SPersistPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}