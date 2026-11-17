// CThemeChannel::operator=(const CThemeChannel&) -- compiler-generated memberwise assignment of an environment theme channel: 19 colours, 30 scalers, then the 3 texture sets (two CArray assignments each)
// Leaf: ??4CThemeChannel
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B6180, 208 bytes incl. padding (ret 4).
// Callees: CArray<long>::operator= @ 0x00454886, CArray<float>::operator= @ 0x0045EF2D (incremental-link thunks;
//          manifest "CActiveFile::operator=" is propagated).
// The manifest label "CBlendedEnvironmentTheme::operator=" is propagated: this is the sibling CThemeChannel (no TimeOfDay).
// Layout (Ego_r.pdb CThemeChannel with the retail 12-byte CArray, so 0x1F0 not 0x208):
//          +0 Colours CRGBFloatColour[19], +0x130 Scalers float[30], +0x1A8 TextureSets CIDrawEnvironmentTextureSet[3]
//          (+0 Textures CArray<long>, +0xC Blends CArray<float>).
// NF48 (round 7): CArray<T> is the shared engine/CArray.h (a public STLport 4.6.2 std::vector<T>, was a local CArray template with
// its own operator=); FABLE_RETAIL_STLP_VECTOR_ASSIGN declares vector<long>::operator= (0x00454886) and vector<float>::operator=
// (0x0045EF2D) out of line, which CArray's implicit operator= calls.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "engine/CArray.h"
class CRGBFloatColour { public:
    float R, G, B, A;
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

void AssignChannel(CThemeChannel& a, const CThemeChannel& channel)
{
    a = channel;
}