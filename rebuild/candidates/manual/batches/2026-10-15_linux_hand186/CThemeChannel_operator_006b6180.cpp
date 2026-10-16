// CThemeChannel::operator=(const CThemeChannel&) -- compiler-generated memberwise assignment of an environment theme channel: 19 colours, 30 scalers, then the 3 texture sets (two CArray assignments each)
// Leaf: ??4CThemeChannel
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B6180, 208 bytes incl. padding (ret 4).
// Callees: CArray<long>::operator= @ 0x00454886, CArray<float>::operator= @ 0x0045EF2D (incremental-link thunks;
//          manifest "CActiveFile::operator=" is propagated).
// The manifest label "CBlendedEnvironmentTheme::operator=" is propagated: this is the sibling CThemeChannel (no TimeOfDay).
// Layout (Ego_r.pdb CThemeChannel with the retail 12-byte CArray, so 0x1F0 not 0x208):
//          +0 Colours CRGBFloatColour[19], +0x130 Scalers float[30], +0x1A8 TextureSets CIDrawEnvironmentTextureSet[3]
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

struct CThemeChannel {
    CRGBFloatColour             Colours[19];    // +0x000
    float                       Scalers[30];    // +0x130
    CIDrawEnvironmentTextureSet TextureSets[3]; // +0x1A8
};

void AssignChannel(CThemeChannel& a, const CThemeChannel& b)
{
    a = b;
}
