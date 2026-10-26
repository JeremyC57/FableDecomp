// _global::UninitCopy_CMorphEntryPair_0081a1b0(const CMorphEntryPair* first, const CMorphEntryPair* last, CMorphEntryPair* result, const __false_type&) -- STLport __uninitialized_copy<pair<EHeroMorphType, CTextureMorphEntry>*>: placement copy-construct hero-morph texture entries, return the advanced result
// Leaf: UninitCopy_CMorphEntryPair_0081a1b0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081A1B0, length 77 bytes (+3 int3 pad), `ret 8`.
// Callers: 0x0081C8EA / 0x0081C955 (vector allocate-and-fill 0x0081C890) and 0x0081CC49 / 0x0081CCB4 (resize/insert
//   0x0081CBB0) -- the vector<pair<EHeroMorphType, CTextureMorphEntry> > helpers whose fill counterpart is the landed
//   `__uninitialized_fill_n` 0x0081A200 (the next function in this TU).
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. Types from
//   Ego_r.pdb, as the landed 0x0081A200: CTextureMorphEntry (20 bytes) = MorphType +0, TextureLayer +4,
//   ReplacingTextureIndex +8, BankIndex +0xC, Blend +0x10; the pair is 24 bytes.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack). STLport's pair copy constructor is memberwise, so the key word is
//   copied directly and the trivially copyable 20-byte CTextureMorphEntry as an inline block through
//   `mov ebx,esi` / `lea edi,[eax+4]` (esi = the strength-reduced source + 4); placement-new null test on the
//   destination; the advanced result returned in eax. The named `cur` copy of result is what keeps the destination
//   block address a per-iteration `lea edi,[eax+4]` (a loop on `result` itself strength-reduces it into a fourth
//   induction register and spills `last`).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

enum EHeroMorphType { HERO_MORPH_TYPE_NONE };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE };

class CTextureMorphEntry { public:                     // 20 bytes (Ego_r.pdb), trivially copyable
    EHeroMorphType      MorphType;              // +0x00
    long                TextureLayer;           // +0x04
    long                ReplacingTextureIndex;  // +0x08
    long                BankIndex;              // +0x0C
    ECompositeBlendType Blend;                  // +0x10
};

struct CMorphEntryPair {                        // std::pair<EHeroMorphType, CTextureMorphEntry>, 24 bytes
    EHeroMorphType     first;                   // +0x00
    CTextureMorphEntry second;                  // +0x04
    CMorphEntryPair(const CMorphEntryPair& o) : first(o.first), second(o.second) { }
};

CMorphEntryPair* __fastcall UninitCopy_CMorphEntryPair_0081a1b0(const CMorphEntryPair* first, const CMorphEntryPair* last, CMorphEntryPair* result, const __false_type&)
{
    CMorphEntryPair* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CMorphEntryPair(*first);
    return cur;
}