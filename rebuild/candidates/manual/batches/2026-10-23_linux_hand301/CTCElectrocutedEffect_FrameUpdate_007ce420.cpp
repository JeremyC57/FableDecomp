// CTCElectrocutedEffect::FrameUpdate() -- fades the electrocution tint and highlight in and out on the thing and its tint list, then removes the effect when the fade-out ends
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CE420, 784 bytes, `ret`.
// Callees: CThing::SetActive(bool)? @ 0x004C7590, CThing::SetAsFrameUpdated() @ 0x004C7C40, _ftol @ 0x00BFEA70,
//          NGraphicAppearance::CTCGraphicAppearance::SetTintColour(const CRGBColour&, void*) @ 0x004C00B0,
//          NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float, float, const CRGBColour&, bool, bool,
//          EAppearanceMeshEffectPriority, void*) @ 0x004C04C0, CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50,
//          CThing::RemoveTC(long) @ 0x004C9840. Float pool: 1.0f @ 0x0122DED8, 255.0f @ 0x01230014.
// Layout: Ego_r.pdb CTCElectrocutedEffect (matches retail): PDef +0x0C, CurTime +0x10, FadeInTime +0x14,
//         FadeOutTimeStart +0x18, FadeOutTime +0x1C, PEffect +0x20, TintList +0x28 (12-byte STLport vector);
//         retail CSpecialAbilitiesElectrocutedEffectDef (PDB - 4, 12-byte vectors): Color +0x48, HighlightColor +0x54,
//         HighlightWidth +0x60; CThing's cached graphic appearance +0x64.
// Shape: the two fade branches inline one tint helper (fade factor t: tint = Color * t + (1 - t) * 255 whitened,
//        highlight = HighlightColor with alpha Color[3] * t, width HighlightWidth * t), applied to the thing and every
//        live thing of the tint list. The colour constructor assigns R, G, B, A in its body (a member-initialiser list
//        reorders the four _ftol channel conversions), the highlight channels are read straight through the def's vector
//        (a named element pointer takes ebp), and the loop caches the list end in a local.
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
namespace NGraphicAppearance {
    enum EAppearanceMeshEffectPriority { PRIORITY_2 = 2 };
    class CTCGraphicAppearance {
    public:
        void SetTintColour(const CRGBColour& colour, void* owner);   // 0x004C00B0
        void SetAsHighlighted(float width, float start, const CRGBColour& colour, bool includeCarried, bool flag,
                              EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C04C0
    };
}
class CThing {
public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Flags91;                          // +0x91 (bit 0: being destroyed)
    void SetActive(bool active);                    // 0x004C7590
    void SetAsFrameUpdated();                       // 0x004C7C40
    void RemoveTC(long type);                       // 0x004C9840
};
class CBaseIntelligentPointer {
public:
    void* _vfptr;
    void* PData;
    CThing* GetPItem() const;                       // 0x00A01B50
};
template<class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
};
class CSpecialAbilitiesElectrocutedEffectDef {
public:
    unsigned char _pad00[0x3c];
    float FadeInTimeSecs;                           // +0x3C
    float FadeOutTimeStartSecs;                     // +0x40
    float FadeOutTimeSecs;                          // +0x44
    vector<long> Color;                             // +0x48
    vector<long> HighlightColor;                    // +0x54
    float HighlightWidth;                           // +0x60
};

class CTCElectrocutedEffect {
public:
    virtual void v00();
    CThing* Thing;                                  // +0x04
    unsigned char _pad08[4];
    const CSpecialAbilitiesElectrocutedEffectDef* PDef;   // +0x0C
    long CurTime;                                   // +0x10
    long FadeInTime;                                // +0x14
    long FadeOutTimeStart;                          // +0x18
    long FadeOutTime;                               // +0x1C
    CBaseIntelligentPointer PEffect;                // +0x20
    vector<CBaseIntelligentPointer> TintList;       // +0x28
    static __forceinline unsigned char Channel(long colour, float t, long white)
    {
        return (unsigned char)(colour * t + white);
    }
    __forceinline void ApplyTint(CThing* thing, float t)
    {
        long white = (long)((1.0f - t) * 255.0f);
        const CSpecialAbilitiesElectrocutedEffectDef* def = PDef;
        const long* colour = def->Color._M_start;
        CRGBColour tint(Channel(colour[0], t, white), Channel(colour[1], t, white), Channel(colour[2], t, white),
                        Channel(colour[3], t, white));
        CRGBColour highlight((unsigned char)def->HighlightColor._M_start[0], (unsigned char)def->HighlightColor._M_start[1],
                             (unsigned char)def->HighlightColor._M_start[2], (unsigned char)(def->Color._M_start[3] * t));
        float width = t * def->HighlightWidth;
        NGraphicAppearance::CTCGraphicAppearance* appearance = thing->PGraphicAppearance;
        if (appearance) {
            appearance->SetTintColour(tint, this);
            appearance->SetAsHighlighted(width, 0.0f, highlight, false, false, NGraphicAppearance::PRIORITY_2, this);
        }
        CBaseIntelligentPointer* end = TintList._M_finish;
        for (CBaseIntelligentPointer* it = TintList._M_start; it != end; ++it) {
            CThing* other = it->GetPItem();
            if (other && !(other->Flags91 & 1)) {
                NGraphicAppearance::CTCGraphicAppearance* otherAppearance = other->PGraphicAppearance;
                if (otherAppearance) {
                    otherAppearance->SetTintColour(tint, this);
                    otherAppearance->SetAsHighlighted(width, 0.0f, highlight, false, false, NGraphicAppearance::PRIORITY_2, this);
                }
            }
        }
    }
    void FrameUpdate();
};

void CTCElectrocutedEffect::FrameUpdate()
{
    CThing* thing = Thing;
    thing->SetActive(true);
    thing->SetAsFrameUpdated();
    if (CurTime <= FadeInTime) {
        float t = (float)CurTime / FadeInTime;
        if (!FadeInTime)
            t = 1.0f;
        ApplyTint(thing, t);
    } else if (CurTime >= FadeOutTimeStart) {
        float t = 1.0f - (float)(CurTime - FadeOutTimeStart) / FadeOutTime;
        if (!FadeOutTime)
            t = 1.0f;
        ApplyTint(thing, t);
    }
    if (++CurTime > FadeOutTimeStart + FadeOutTime)
        thing->RemoveTC(0x8a);
}
