// CTCLight::OnSerialise(CPersistContext& context, bool flag) -- persist Active (a load with the flag set re-requests the appearance unless the hero state is being serialised) and, for an overridden light, its own light info (a load builds a fresh CLightInfo)
// Grade: RELOCATION_MATCH
// Leaf: OnSerialise
// Retail: Fable.exe @ 0x00641020, 488 bytes, `ret 8`. PDB ?OnSerialise@CTCLight@@UAEXAAVCPersistContext@@_N@Z (Ego_r).
// Callees: CPersistContext::Transfer<bool> 0x004045C0 / Transfer<float> 0x00410620 / Transfer<CRGBColour> 0x006420F0;
//   CTCBase::PeekWorld() const 0x00686A80; 0x004C7590 (name unproven, binding kept); CThing::SetAsFrameUpdated 0x004C7C40;
//   CCountedPointer<CTCLight::CLightInfo>::Reset 0x00641EB0 (engine/CCountedPointer.h, out of line as retail).
// Layout: CTCBase ParentThing +0x04 (local CTCBase as the landed CTCWound callers, for PeekWorld); CTCLight PLightInfo +0x10,
//   Active +0x1c, Overridden +0x1d; CWorld from engine/CWorld.h (SerialisingHeroState +0x102); CPersistContext Mode +0x18.
#include "engine/CCountedPointer.h"
#include "engine/CWorld.h"

class CRGBColour {                                      // D3DCOLOR byte order
public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

class CPersistContext {
public:
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18 (1 / 3: saving)
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // bool 0x004045C0, float 0x00410620, CRGBColour 0x006420F0
};

class CThing {
public:
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                       // +0x6c
    void UpdateGraphicAppearance(long reason);          // 0x004C7590 (name unproven)
    void SetAsFrameUpdated();                           // 0x004C7C40
};

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
protected:
    const CWorld& PeekWorld() const;                    // 0x00686A80
};

class CTCLight : public CTCBase {
public:
    class CLightInfo {                                  // PDB, 24 bytes
    public:
        CRGBColour    Colour;                           // +0x00
        float         InnerRadius;                      // +0x04
        float         OuterRadius;                      // +0x08
        float         Flicker;                          // +0x0c
        unsigned long LightingChannelsEffected;         // +0x10
        bool          Inverted;                         // +0x14
    };
    unsigned char                 _pad08[0x10 - 0x08];
    CCountedPointer<CLightInfo>   PLightInfo;           // +0x10
    const void*                   PLightDef;            // +0x18
    bool                          Active;               // +0x1c
    bool                          Overridden;           // +0x1d
    virtual void OnSerialise(CPersistContext& context, bool flag);
};

void CTCLight::OnSerialise(CPersistContext& context, bool flag)
{
    context.Transfer("Active", Active, false);
    if (flag && !context.IsSaving()) {
        if (!(ParentThing->ThingTypeFlags & 2) || !PeekWorld().SerialisingHeroState) {
            ParentThing->UpdateGraphicAppearance(4);
            ParentThing->SetAsFrameUpdated();
        }
    }
    context.Transfer("Overridden", Overridden, false);
    if (Overridden) {
        if (context.IsSaving()) {
            context.Transfer("Colour", PLightInfo->Colour, CRGBColour(0, 0, 0, 0xff));
            context.Transfer("InnerRadius", PLightInfo->InnerRadius, 0.0f);
            context.Transfer("OuterRadius", PLightInfo->OuterRadius, 0.0f);
            context.Transfer("Flicker", PLightInfo->Flicker, 0.0f);
            context.Transfer("Inverted", PLightInfo->Inverted, false);
        } else {
            PLightInfo.Reset(new CLightInfo);
            CRGBColour colour;
            context.Transfer("Colour", colour, CRGBColour(0, 0, 0, 0xff));
            PLightInfo->Colour = colour;
            context.Transfer("InnerRadius", PLightInfo->InnerRadius, 0.0f);
            context.Transfer("OuterRadius", PLightInfo->OuterRadius, 0.0f);
            context.Transfer("Flicker", PLightInfo->Flicker, 0.0f);
            context.Transfer("Inverted", PLightInfo->Inverted, false);
        }
    }
}