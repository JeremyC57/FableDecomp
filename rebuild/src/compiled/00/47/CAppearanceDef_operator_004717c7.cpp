// CAppearanceDef::operator=(const CAppearanceDef&) -- def copy: CDefClassBase part, then the CAppearanceDef fields memberwise in declaration order
// Leaf: ??4CAppearanceDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004717C7, 107 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x004717C2 in slot 19 of vtable 0x012354DC = CAppearanceDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10, CAnimationSet::operator= @ 0x00471832.
// Layout (engine/CAppearanceDef.h, PDB minus 4 after Animation): Graphic +0x25, Animation +0x34, OutlineEffectAlphaRef +0x54, OutlineEffectAlphaBias +0x55, OverrideLightingChannel +0x56, LightingChannel +0x58, FadeOutWhenCloseGraphic +0x5c, HasFadeOutWhenCloseGraphic +0x6a, FadeOutGraphicRenderFadeDistance +0x6c, FadeOutGraphicNearRenderFadeDistance +0x70, FadeOutGraphicNearRenderClipDistance +0x74.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
// The two 14-byte CEngineGraphic records copy as `movsd x3 ; movsw` (size TU); LightingChannel is the 4-byte ELightingChannel enum (long).
#pragma optimize("s", on)

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
#pragma pack(push, 1)
struct CEngineGraphic {                 // 14 bytes, byte-aligned (PDB: Graphic at +0x25)
    unsigned long BankIndex;            // +0x00
    float AnimStep;                     // +0x04
    float RenderSizeX;                  // +0x08
    bool AdditiveAlpha;                 // +0x0c
    unsigned char Type;                 // +0x0d
};
#pragma pack(pop)
struct CAnimationSet {                  // retail 0x20 bytes (PDB 0x24: its CVectorMap loses 4)
    unsigned long m_words[8];
    CAnimationSet& operator=(const CAnimationSet& other);   // 0x00471832
};

struct CAppearanceDef : CDefClassBase {
    CEngineGraphic           Graphic;                     // +0x25
    CAnimationSet            Animation;                   // +0x34
    unsigned char            OutlineEffectAlphaRef;       // +0x54
    unsigned char            OutlineEffectAlphaBias;      // +0x55
    bool                     OverrideLightingChannel;     // +0x56
    long                     LightingChannel;             // +0x58
    CEngineGraphic           FadeOutWhenCloseGraphic;     // +0x5c
    bool                     HasFadeOutWhenCloseGraphic;  // +0x6a
    float                    FadeOutGraphicRenderFadeDistance; // +0x6c
    float                    FadeOutGraphicNearRenderFadeDistance; // +0x70
    float                    FadeOutGraphicNearRenderClipDistance; // +0x74
    CAppearanceDef& operator=(const CAppearanceDef& other);
};

CAppearanceDef& CAppearanceDef::operator=(const CAppearanceDef& other)
{
    CDefClassBase::operator=(other);
    Graphic = other.Graphic;
    Animation = other.Animation;
    OutlineEffectAlphaRef = other.OutlineEffectAlphaRef;
    OutlineEffectAlphaBias = other.OutlineEffectAlphaBias;
    OverrideLightingChannel = other.OverrideLightingChannel;
    LightingChannel = other.LightingChannel;
    FadeOutWhenCloseGraphic = other.FadeOutWhenCloseGraphic;
    HasFadeOutWhenCloseGraphic = other.HasFadeOutWhenCloseGraphic;
    FadeOutGraphicRenderFadeDistance = other.FadeOutGraphicRenderFadeDistance;
    FadeOutGraphicNearRenderFadeDistance = other.FadeOutGraphicNearRenderFadeDistance;
    FadeOutGraphicNearRenderClipDistance = other.FadeOutGraphicNearRenderClipDistance;
    return *this;
}