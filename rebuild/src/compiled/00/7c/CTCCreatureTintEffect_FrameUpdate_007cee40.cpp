// CTCCreatureTintEffect::FrameUpdate() -- fade the tint in from white over FadeInTime, or (once fade-out started) back to white over FadeOutTime, tinting the parent and every live thing in TintList; remove the component when the fade-out ends
// Leaf: FrameUpdate
// Retail 0x007CEE40, 508 bytes (plain `ret`); catalog "vfunc_9" = Ego_r.pdb ?FrameUpdate@CTCCreatureTintEffect@@UAEXXZ.
// Callees: __ftol2 @ 0x00BFEA70, NGraphicAppearance::CTCGraphicAppearance::SetTintColour(const CRGBColour&, void*)
// @ 0x004C00B0 (the thing's cached appearance at +0x64), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CThing::RemoveTC(long) @ 0x004C9840 (0x8b = CTCCreatureTintEffect). Constants: 1.0f @ 0x0122DED8, 255.0f @ 0x01230014.
// Layout: as the landed Reset 0x007CEDE0 / ctor 0x007CF110 (CurTime +0x10, FadeInTime +0x14, StartFadeOut +0x18,
// FadeOutTimeStart +0x1c, FadeOutTime +0x20, TintList +0x24 12-byte vector, Color[4] +0x30).
struct CRGBColour {
    unsigned char B;                                    // +0
    unsigned char G;                                    // +1
    unsigned char R;                                    // +2
    unsigned char A;                                    // +3
};
namespace NGraphicAppearance {
    struct CTCGraphicAppearance {
        void SetTintColour(const CRGBColour& colour, void* owner);   // 0x004C00B0
    };
}
struct CThing {
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    void RemoveTC(long interfaceType);                  // 0x004C9840
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
};
struct CTCCreatureTintEffect {
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    void* PDef;                             // +0x0c
    long CurTime;                           // +0x10
    long FadeInTime;                        // +0x14
    bool StartFadeOut;                      // +0x18
    long FadeOutTimeStart;                  // +0x1c
    long FadeOutTime;                       // +0x20
    vector<CBaseIntelligentPointer> TintList;   // +0x24
    long Color[4];                          // +0x30
    void SetTint(CThing* parent, float amount);
    void FrameUpdate();
};

__forceinline void CTCCreatureTintEffect::SetTint(CThing* parent, float amount)
{
    float white = (float)(long)((1.0f - amount) * 255.0f);
    CRGBColour colour;
    colour.R = (unsigned char)(long)(Color[0] * amount + white);
    colour.G = (unsigned char)(long)(Color[1] * amount + white);
    colour.B = (unsigned char)(long)(Color[2] * amount + white);
    colour.A = (unsigned char)(long)(Color[3] * amount + white);
    if (parent->PGraphicAppearance)
        parent->PGraphicAppearance->SetTintColour(colour, this);
    CBaseIntelligentPointer* it = TintList.begin();
    CBaseIntelligentPointer* end = TintList.end();
    for (; it != end; ++it) {
        CThing* thing = it->GetPItem();
        if (thing && !(thing->m_flags91 & 1) && thing->PGraphicAppearance)
            thing->PGraphicAppearance->SetTintColour(colour, this);
    }
}

void CTCCreatureTintEffect::FrameUpdate()
{
    CThing* parent = ParentThing;
    if (CurTime <= FadeInTime) {
        float amount = (float)CurTime / (float)FadeInTime;
        if (FadeInTime == 0)
            amount = 1.0f;
        SetTint(parent, amount);
    } else if (StartFadeOut) {
        float amount = 1.0f - (float)(CurTime - FadeOutTimeStart) / (float)FadeOutTime;
        if (FadeOutTime == 0)
            amount = 1.0f;
        SetTint(parent, amount);
        if (CurTime > FadeOutTimeStart + FadeOutTime)
            parent->RemoveTC(0x8b);
    }
    ++CurTime;
}