// std::__uninitialized_copy(const pair<long,CEntry>* first, const pair<long,CEntry>* last, pair<long,CEntry>* result, const __false_type&) -- copy-construct a range of 0x2C-byte (long key, 40-byte POD entry) pairs into raw storage and return the end
// Flags: /Gr
// Leaf: __uninitialized_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00673d30, 57 bytes (`ret 8`).
// Identity: the catalog label `_Cons_val<allocator<CAppearanceModifierGraphics::CEntry>,..>` names
// only the inlined construct step; the body is STLport's
// `__uninitialized_copy(first, last, result, __false_type)` (first/last in ecx/edx, result and the
// empty tag on the stack, returns the advanced result in eax).  Callers 0x006740B9.. are the vector
// insert/reallocation paths.
// Element: 0x2C bytes = long key +0 and a 40-byte POD +4 copied as one `rep movsd` of 10 dwords.
// Ego_r.pdb has exactly one 40-byte CEntry (EBodyLocation Location, then texture/bump indices,
// durations and float PermanentScarStrength): a POD of ten dwords, modelled here as that record.
// Shape: `cmp ebx,edx ; je` guards a bottom-tested loop; `lea ebp,[ebx+4]` strength-reduces
// &first->second; the placement new null-tests the destination; STLport's explicit pair copy
// constructor copies the key by register and the POD second by `rep movsd`.
inline void* operator new(unsigned int, void* where) { return where; }

struct __false_type { __false_type() {} };

namespace FableCopy_00673d30 {

enum EBodyLocation { BODY_LOCATION_NONE };

struct CEntry {                                          // 0x28 bytes, POD
    EBodyLocation Location;                              // +0x00
    long  ReplacingTextureIndex;                         // +0x04
    long  ReplacingBumpMapIndex;                         // +0x08
    long  WoundTextureIndex;                             // +0x0C
    long  WoundBumpMapIndex;                             // +0x10
    long  ScarTextureIndex;                              // +0x14
    long  ScarBumpMapIndex;                              // +0x18
    long  FadeToScarDuration;                            // +0x1C
    long  TotalDuration;                                 // +0x20
    float PermanentScarStrength;                         // +0x24
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }   // STLport's explicit pair copy constructor
};

typedef pair<long, CEntry> SKeyedEntry;

}

namespace std {

FableCopy_00673d30::SKeyedEntry* __fastcall __uninitialized_copy(const FableCopy_00673d30::SKeyedEntry* first,
    const FableCopy_00673d30::SKeyedEntry* last, FableCopy_00673d30::SKeyedEntry* result, const __false_type&)
{
    FableCopy_00673d30::SKeyedEntry* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_00673d30::SKeyedEntry(*first);
    return cur;
}

}