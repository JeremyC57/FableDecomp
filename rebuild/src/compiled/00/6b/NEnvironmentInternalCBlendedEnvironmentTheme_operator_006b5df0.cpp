// NEnvironmentInternal::CBlendedEnvironmentTheme::operator=(const CBlendedEnvironmentTheme&) -- compiler-generated memberwise assignment: time of day, 19 colours, 30 scalers, then the 3 texture sets (two CArray assignments each)
// Leaf: ??4CBlendedEnvironmentTheme
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B5DF0, 224 bytes incl. padding (ret 4).
// Callees: CArray<long>::operator= @ 0x00454886, CArray<float>::operator= @ 0x0045EF2D (incremental-link thunks;
//          manifest "CActiveFile::operator=" is propagated).
// Layout (Ego_r.pdb CBlendedEnvironmentTheme with the retail 12-byte CArray, so 0x1F4 not 0x20C): +0 TimeOfDay,
//          +4 Colours CRGBFloatColour[19], +0x134 Scalers float[30], +0x1AC TextureSets CIDrawEnvironmentTextureSet[3]
//          (+0 Textures CArray<long>, +0xC Blends CArray<float>).
struct CRGBFloatColour {
    float R, G, B, A;
};

template <class T> struct CArray {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CArray& operator=(const CArray& other);
};

struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;                     // +0x00
    CArray<float> Blends;                       // +0x0C
};

namespace NEnvironmentInternal {
struct CBlendedEnvironmentTheme {
    float                       TimeOfDay;      // +0x000
    CRGBFloatColour             Colours[19];    // +0x004
    float                       Scalers[30];    // +0x134
    CIDrawEnvironmentTextureSet TextureSets[3]; // +0x1AC
};
}

void AssignTheme(NEnvironmentInternal::CBlendedEnvironmentTheme& a, const NEnvironmentInternal::CBlendedEnvironmentTheme& b)
{
    a = b;
}