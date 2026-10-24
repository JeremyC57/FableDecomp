// std::__uninitialized_fill_n(pair<EHeroMorphType, CTextureMorphEntry>* first, unsigned int n, const pair<..>& x, const __false_type&) -- copy-construct n copies of a hero-morph texture entry into raw storage at first and return first + n
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe @ 0x0081A200, 70 bytes, `ret 8`: __fastcall (first ecx -> eax, n edx; x and the empty tag by
// reference on the stack), returning the end pointer in eax. Call sites 0x0081A4DA, 0x0081C939, 0x0081CC91 (the
// vector<pair<EHeroMorphType, CTextureMorphEntry> > insert/fill paths; the fill twin is 0x00819230).
// Types (Ego_r.pdb): CTextureMorphEntry (20 bytes) = MorphType +0, TextureLayer +4, ReplacingTextureIndex +8,
//   BankIndex +0xC, Blend +0x10; the pair is 24 bytes.
// Shape: STLport's pair declares a copy constructor ("VC++ needs this": `pair(const pair& o) : first(o.first),
// second(o.second)`), so the placement copy is memberwise: the key word directly, then the trivially copyable
// CTextureMorphEntry as a 20-byte block through `lea esi,[ecx+4]` / `lea edi,[eax+4]`. The trivially copied twin
// 0x007183E0 lacks that constructor. Null check of the placement new kept; n counts down in edx.
#include <new>
enum EHeroMorphType { HERO_MORPH_TYPE_NONE };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE };
struct CTextureMorphEntry {                     // 20 bytes
    EHeroMorphType      MorphType;              // +0x00
    long                TextureLayer;           // +0x04
    long                ReplacingTextureIndex;  // +0x08
    long                BankIndex;              // +0x0C
    ECompositeBlendType Blend;                  // +0x10
};
struct __false_type { __false_type() {} };
namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
typedef pair<EHeroMorphType, CTextureMorphEntry> CMorphEntryPair;
template CMorphEntryPair* __fastcall __uninitialized_fill_n(CMorphEntryPair*, unsigned int, const CMorphEntryPair&, const __false_type&);
}