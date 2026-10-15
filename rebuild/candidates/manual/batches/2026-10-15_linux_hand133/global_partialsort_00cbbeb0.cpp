// _global::__partial_sort(SPersistPair* first, SPersistPair* middle, SPersistPair* last, SPersistPair*, CKeyPairCompareLess comp) -- SGI partial_sort core for (UID, persist info) pairs: heapify [first, middle), pop every smaller later element into the heap, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBBEB0, length 123 bytes (`ret 0Ch`: last, the value-type tag pointer and the comparator on the
// stack; first/middle in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CEditPrimitiveStats::CopySort" is propagated and wrong: the callees are the same instantiation's
// __make_heap @ 0x00CBAAB0 (first, middle, comp, (T*)0, (Distance*)0), __pop_heap @ 0x00CBA750 and sort_heap @ 0x00CBB9D0, over
// pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (0x18 bytes).
// Callees: also CCharString::CCharString(const CCharString&) @ 0x0099EC30 (the by-value T(*i) copy built in the argument area).
// Shape: the comparator kept in ebp and pushed by value each time; the loop `for (i = middle; i < last; ++i)` re-reads `last` from its
// argument slot; the inline CKeyPairCompareLess is the unsigned 64-bit `(*i).first < (*first).first` (`ja` skip, `jb` take, then the
// low words with `jae`), whose loaded words are reused for the T(*i) copy.
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

void __fastcall __make_heap(SPersistPair* first, SPersistPair* last, CKeyPairCompareLess comp, SPersistPair*, long*);   // 0x00CBAAB0
void __fastcall __pop_heap(SPersistPair* first, SPersistPair* last, SPersistPair* result, SPersistPair value,
                           CKeyPairCompareLess comp, long*);   // 0x00CBA750
void __fastcall sort_heap(SPersistPair* first, SPersistPair* last, CKeyPairCompareLess comp);   // 0x00CBB9D0

inline void make_heap(SPersistPair* first, SPersistPair* last, CKeyPairCompareLess comp)
{
    __make_heap(first, last, comp, (SPersistPair*)0, (long*)0);
}

void __fastcall __partial_sort(SPersistPair* first, SPersistPair* middle, SPersistPair* last, SPersistPair*, CKeyPairCompareLess comp)
{
    make_heap(first, middle, comp);
    for (SPersistPair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SPersistPair(*i), comp, (long*)0);
    sort_heap(first, middle, comp);
}
