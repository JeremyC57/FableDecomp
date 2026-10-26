// CAppearanceDef::Transfer(CPersistContext& context) -- persist every appearance field through the context's typed transfer helpers, then re-sort the animation set's entry map
// Retail: Fable.exe (TLC, VC7.1) @ 0x00473504, 143 bytes, __fastcall, ret 4 (virtual).
// Leaf: ?Transfer@CAppearanceDef@@UAEXAAVCPersistContext@@@Z
// Callees (CPersistContext typed transfers; their manifest labels are propagated): CEngineGraphic
// 0x00451E11, CAnimationSet 0x0047367A, unsigned char 0x00473593, bool 0x0043314A, ELightingChannel
// 0x004735D6, float 0x00431061 (CPersistContext::Transfer<T>); then CVectorMap<unsigned long, CAnimationEntry,
// CKeyPairCompareLess>::SortAndTrim 0x005DEDC0 on Animation.Anims (sorts the entry map when its dirty byte +0x0D is set).
// Layout: Ego_r.pdb CAppearanceDef with the retail 0x20-byte CAnimationSet (the PDB's is 0x24): the
// 14-byte CEngineGraphic Graphic at the unaligned +0x25 (#pragma pack(1)), Animation +0x34,
// OutlineEffectAlphaRef / Bias +0x54 / +0x55, OverrideLightingChannel +0x56, LightingChannel +0x58,
// FadeOutWhenCloseGraphic +0x5C, HasFadeOutWhenCloseGraphic +0x6A, three fade distances +0x6C..+0x74.
// Shape: the context stays in edi and every member address is formed with `lea` (the last with
// `add esi,74h`); the Animation address is kept in ebx across the transfers for the final call.
// Size-optimised TU: every address goes through eax (`#pragma optimize("s", on)`; the speed build
// rotates eax/ecx/edx).
#pragma optimize("s", on)
#pragma pack(push, 1)
class CEngineGraphic { public:                    // 14 bytes
    unsigned char m_data[14];
};
#pragma pack(pop)

class CAnimationEntry;
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap { public:   // 0x10 bytes
    void* Data[3];                         // +0x00 CArray< SPair<K, V> >
    C Compare;                             // +0x0C
    bool Dirty;                            // +0x0D
    void SortAndTrim();                    // 0x005DEDC0
};

class CAnimationSet { public:                     // retail 0x20 bytes
    CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> > Anims;   // +0x00
    long DefaultFlags;                     // +0x10
    long DefaultTransitionTime;            // +0x14
    long DefaultDelay;                     // +0x18
    long DefaultGroup;                     // +0x1C CDefString
};

enum ELightingChannel { LIGHTING_CHANNEL_DEFAULT = 0 };

class CPersistContext {
public:
    template <class T> void Transfer(T& value);   // one-argument helpers (namepass TR2/DEF1): CEngineGraphic 0x00451E11,
                                                  // CAnimationSet 0x0047367A, unsigned char 0x00473593, bool 0x0043314A,
                                                  // ELightingChannel 0x004735D6, float 0x00431061
};

#pragma pack(push, 1)
class CAppearanceDef {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x21];                          // +0x04 .. +0x24 (CDefBase)
    CEngineGraphic Graphic;                             // +0x25
    unsigned char _pad_0x33;                            // +0x33
    CAnimationSet Animation;                            // +0x34
    unsigned char OutlineEffectAlphaRef;                // +0x54
    unsigned char OutlineEffectAlphaBias;               // +0x55
    bool OverrideLightingChannel;                       // +0x56
    unsigned char _pad_0x57;                            // +0x57
    ELightingChannel LightingChannel;                   // +0x58
    CEngineGraphic FadeOutWhenCloseGraphic;             // +0x5C
    bool HasFadeOutWhenCloseGraphic;                    // +0x6A
    unsigned char _pad_0x6b;                            // +0x6B
    float FadeOutGraphicRenderFadeDistance;             // +0x6C
    float FadeOutGraphicNearRenderFadeDistance;         // +0x70
    float FadeOutGraphicNearRenderClipDistance;         // +0x74
};
#pragma pack(pop)

void CAppearanceDef::Transfer(CPersistContext& context)
{
    context.Transfer(Graphic);
    context.Transfer(Animation);
    context.Transfer(OutlineEffectAlphaRef);
    context.Transfer(OutlineEffectAlphaBias);
    context.Transfer(OverrideLightingChannel);
    context.Transfer(LightingChannel);
    context.Transfer(FadeOutWhenCloseGraphic);
    context.Transfer(HasFadeOutWhenCloseGraphic);
    context.Transfer(FadeOutGraphicRenderFadeDistance);
    context.Transfer(FadeOutGraphicNearRenderFadeDistance);
    context.Transfer(FadeOutGraphicNearRenderClipDistance);
    Animation.Anims.SortAndTrim();
}