// CTCLight::SetOuterRadius(float value) -- set the light's OuterRadius when it is overridden, then mark the parent thing's graphic appearance changed and put it back on the render list
// Leaf: ?SetOuterRadius@CTCLight@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00640c70, 42 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?SetOuterRadius@CTCLight@@QAEXM@Z (bound ?SetOuterRadius@CTCLight@@QAEXM@Z); the store goes to Ego_r.pdb
//   CTCLight::CLightInfo::OuterRadius (+0x08) through PLightInfo; the CTCLight setters sit together (SetInnerRadius 0x00640C40,
//   SetOuterRadius 0x00640C70, SetFlicker 0x00640CA0, SetInverted 0x00640CD0) and CGameScriptInterface::CreateLight 0x0089FAA0
//   calls the first three.
// Callees: CThing::UpdateGraphicAppearance(long) 0x004C7590 (descriptive name, the spelling landed callers use; no PDB name),
//   CThing::AddToRenderList() 0x004C7AC0 (Ego_r.pdb).
// Layout: engine/CTCBase.h (ParentThing +0x04); Ego_r.pdb CTCLight members PLightInfo +0x10, PLightDef +0x18, Active +0x1C,
//   Overridden +0x1D (the retail offsets agree).
#include "engine/CTCBase.h"
class CThing {
public:
    void UpdateGraphicAppearance(long reason);            // 0x004C7590
    void AddToRenderList();                               // 0x004C7AC0
};
class CCPPointerInfo;
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    T* operator->() const { return Data; }
};
template <class T> class CDefPointer {
public:
    const T* Object;                                      // +0x00
};
class CRGBColour {
public:
    unsigned char R, G, B, A;
};
class CLightDef;
class CTCLight : public CTCBase {
public:
    class CLightInfo {                                    // Ego_r.pdb, 24 bytes
    public:
        CRGBColour Colour;                                // +0x00
        float InnerRadius;                                // +0x04
        float OuterRadius;                                // +0x08
        float Flicker;                                    // +0x0C
        unsigned long LightingChannelsEffected;           // +0x10
        bool Inverted;                                    // +0x14
    };
    unsigned char _pad_0x0c[0x4];                         // +0x0C (Ego_r.pdb: unnamed)
    CCountedPointer<CLightInfo> PLightInfo;               // +0x10
    CDefPointer<CLightDef> PLightDef;                     // +0x18
    bool Active;                                          // +0x1C
    bool Overridden;                                      // +0x1D
    void SetOuterRadius(float value);
};
void CTCLight::SetOuterRadius(float value)
{
    if (Overridden) {
        PLightInfo->OuterRadius = value;
        ParentThing->UpdateGraphicAppearance(4);
        ParentThing->AddToRenderList();
    }
}