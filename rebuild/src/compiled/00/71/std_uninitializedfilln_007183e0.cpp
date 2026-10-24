// std::__uninitialized_fill_n(pair<EHeroMorphType, CTextureMorphEntry>* first, unsigned int n, const pair<..>& x, const __false_type&) -- copy-construct n copies of a 24-byte morph entry into raw storage at first and return first + n
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe @ 0x007183E0, 70 bytes, `ret 8`: __fastcall (first ecx -> eax, n edx; x and the empty tag by reference
// on the stack), returning the end pointer in eax. Only call site 0x0x00719775 (the vector reallocation path at 0x007196D0, catalogued Vector_ReallocateAndCopyMorphs).
// Types (Ego_r.pdb): CTextureMorphEntry (20 bytes) = MorphType +0, TextureLayer +4, ReplacingTextureIndex +8,
//   BankIndex +0xC, Blend +0x10; with the EHeroMorphType key the element is 24 bytes, all POD.
// Shape: the element is copied as ONE 24-byte block (`mov esi,ecx` / `mov edi,eax`, six words through ebx), so its copy
// constructor is the implicit trivial one -- unlike the 0x0081A200 instantiation, whose STLport pair declares the
// memberwise "VC++ needs this" copy constructor (first, then second through `lea`). Same catalog name, so either this
// module's element is a plain record of the same layout or its pair lacks that constructor; the record form is kept.
// The placement new keeps the `test eax,eax` null check; n counts down in edx; `lea ecx,[ecx]` aligns the loop head.
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