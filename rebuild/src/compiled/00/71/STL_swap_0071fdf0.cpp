// _STL::swap(pair<EHeroMorphType,CTextureMorphEntry>& a, pair<EHeroMorphType,CTextureMorphEntry>& b) -- exchange two hero texture-morph table entries (morph type key + texture layer + blend description) through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071FDF0, 118 bytes (+ int3 padding), /Gr `__fastcall` (a in ecx -> esi, b in edx
//   -> edi), plain `ret`; 0x18-byte frame temporary.
// Identity: the hero texture-morph table sort TU (landed __adjust_heap 0x0071FF00 and __pop_heap 0x00720610 over the
//   same 24-byte pair); callers are the out-of-line iter_swap 0x007205C0 (a bare `jmp`) and the partition step
//   0x00720C3A. Retail element (as in those landed rows): pair { EHeroMorphType first; { long TextureLayer;
//   CCompositeBlendDesc Blend (16 bytes) } } -- the layout Ego_r.pdb gives CTCHeroMorph::CTextureLayer (TextureID,
//   Blend), hence the catalog's "swap<CTCHeroMorph::CTextureLayer>".
// Callee: CCompositeBlendDesc copy ctor 0x005BA48A (out of line).
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` over STLport's pair (user copy constructor): the
//   temporary takes the key and the layer as words and the blend through its copy constructor; both assignments are
//   the implicit six-dword copies (no destructor: every member is trivially destructible).
enum EHeroMorphType { HERO_MORPH_NONE = 0 };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };

struct CRGBColour {                                     // D3DCOLOR byte order
    unsigned char B, G, R, A;
};

struct CCompositeBlendDesc {                            // 16 bytes
    unsigned long       TextureIndex;                   // +0x00
    ECompositeBlendType BlendType;                      // +0x04
    CRGBColour          DiffuseColour;                  // +0x08
    CRGBColour          SpecularColour;                 // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};

struct CTextureMorphEntry {                             // retail, 20 bytes
    long                TextureLayer;                   // +0x00
    CCompositeBlendDesc Blend;                          // +0x04
};

namespace _STL {
template <class _T1, class _T2>
struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap<pair<EHeroMorphType, CTextureMorphEntry> >(pair<EHeroMorphType, CTextureMorphEntry>&, pair<EHeroMorphType, CTextureMorphEntry>&);
}