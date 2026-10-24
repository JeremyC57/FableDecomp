// std::fill(pair<EHeroMorphType, CTextureMorphEntry>* first, pair<EHeroMorphType, CTextureMorphEntry>* last, const pair<EHeroMorphType, CTextureMorphEntry>& val) -- assign val to every element of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x00819230, 80 bytes, `ret 4`: __fastcall (first ecx, last edx, the value reference on the
// stack). Call sites 0x0081CC73, 0x0081CCD0 (the hero-morph vector<pair<EHeroMorphType, CTextureMorphEntry> > insert path).
// Types (Ego_r.pdb): CTextureMorphEntry (20 bytes) = MorphType +0, TextureLayer +4, ReplacingTextureIndex +8,
//   BankIndex +0xC, Blend +0x10; the pair is 24 bytes, all POD.
// Shape: the pair's implicit (trivial) copy assignment copies the whole 24-byte object as one block (`mov esi,eax` /
// `mov edi,ecx`, six words through ebx); the induction add and compare are scheduled before the last store.
enum EHeroMorphType { HERO_MORPH_TYPE_NONE };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE };
struct CTextureMorphEntry {                     // 20 bytes
    EHeroMorphType      MorphType;              // +0x00
    long                TextureLayer;           // +0x04
    long                ReplacingTextureIndex;  // +0x08
    long                BankIndex;              // +0x0C
    ECompositeBlendType Blend;                  // +0x10
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
typedef pair<EHeroMorphType, CTextureMorphEntry> CMorphEntryPair;
template void __fastcall fill(CMorphEntryPair*, CMorphEntryPair*, const CMorphEntryPair&);
}