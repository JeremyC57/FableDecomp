// Grade: DIFFER(632v634) -- colour channel order: retail converts c[3], c[2], c[1], c[0]; ours c[3], c[1], c[0], c[2]; retail also spills parent (ebp) to the frame and uses ebp for the kill-loop index (frame 0x14 v 0x10)
// CTCBurntEffect::FrameUpdate() -- fade the burn tint in over FadeInTime; at FadeOutTimeStart fade out or kill every live
// effect in Effects, then fade the tint back to white over FadeOutTime; remove the component once the fade-out ends
// Leaf: FrameUpdate
// Retail 0x007CD9A0, 0x27A bytes (plain `ret`). Callees: __ftol2 0x00BFEA70, CTCGraphicAppearance::SetTintColour
// (const CRGBColour&, void*) 0x004C00B0 (the thing's cached appearance at +0x64), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, FadeOrKillEffect(CIntelligentPointer<CThing>&) 0x0072ABD0 (__fastcall), CThing::RemoveTC(long) 0x004C9840
// (0x89 = CTCBurntEffect). Constants: 1.0f 0x0122DED8, 255.0f 0x01230014.
// Layout: the landed CTCBurntEffect ctor 0x007CDFE0 (PDef +0x0c, CurTime +0x10, FadeInTime +0x14, FadeOutTimeStart
// +0x18, FadeOutTime +0x1c, Effects +0x20, TintList +0x2c); the def's colour vector<long> at +0x48. Shape: the landed
// sibling CTCCreatureTintEffect::FrameUpdate 0x007CEE40.
#include "engine/CTCBase.h"

struct CRGBColour {
    unsigned char B;                                    // +0
    unsigned char G;                                    // +1
    unsigned char R;                                    // +2
    unsigned char A;                                    // +3
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) {}
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
void __fastcall FadeOrKillEffect(CBaseIntelligentPointer& effect);   // 0x0072ABD0
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return *(_M_start + i); }
};
struct CSpecialAbilitiesBurntEffectDef {
    unsigned char _pad00[0x48];
    vector<long> Colour;                                // +0x48 R, G, B, A
};
struct CTCBurntEffect : CTCBase {
    CSpecialAbilitiesBurntEffectDef* PDef;              // +0x0c
    long CurTime;                                       // +0x10
    long FadeInTime;                                    // +0x14
    long FadeOutTimeStart;                              // +0x18
    long FadeOutTime;                                   // +0x1c
    vector<CBaseIntelligentPointer> Effects;            // +0x20
    vector<CBaseIntelligentPointer> TintList;           // +0x2c
    void SetTint(CThing* parent, float amount);
    void FrameUpdate();
};

__forceinline void CTCBurntEffect::SetTint(CThing* parent, float amount)
{
    float white = (float)(long)((1.0f - amount) * 255.0f);
    const long* c = PDef->Colour._M_start;
    CRGBColour colour((unsigned char)(long)(c[0] * amount + white), (unsigned char)(long)(c[1] * amount + white),
                      (unsigned char)(long)(c[2] * amount + white), (unsigned char)(long)(c[3] * amount + white));
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

void CTCBurntEffect::FrameUpdate()
{
    CThing* parent = ParentThing;
    if (CurTime <= FadeInTime) {
        float amount = (float)CurTime / (float)FadeInTime;
        if (FadeInTime == 0)
            amount = 1.0f;
        SetTint(parent, amount);
    } else if (CurTime >= FadeOutTimeStart) {
        if (CurTime == FadeOutTimeStart) {
            for (unsigned int i = 0; i < Effects.size(); ++i) {
                if (Effects[i].GetPItem() && !(Effects[i].GetPItem()->m_flags91 & 1))
                    FadeOrKillEffect(Effects[i]);
            }
        }
        float amount = 1.0f - (float)(CurTime - FadeOutTimeStart) / (float)FadeOutTime;
        if (FadeOutTime == 0)
            amount = 1.0f;
        SetTint(parent, amount);
    }
    ++CurTime;
    if (CurTime > FadeOutTimeStart + FadeOutTime)
        parent->RemoveTC(0x89);
}
