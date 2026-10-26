// CTCSpotLight::OnSerialise(CPersistContext& context, bool loading) -- transfer the Overridden flag; when overridden, save the light info's colour, inner/outer radius, angle, width and flicker, or (loading) give the light a fresh info block and load them into it
// Leaf: OnSerialise
// Retail: Fable.exe @ 0x007320E0, 480 bytes (`ret 8`). Label correction: the catalog says vfunc_1; vtable 0x01266FBC slot 1
//   is CTCBase::OnSerialise (33 TC classes).
// Callees: CPersistContext::Transfer<bool> 0x004045C0 / <CRGBColour> 0x006420F0 / <float> 0x00410620, operator new 0x00BFEA1A,
//   CCountedPointer<CSpotLightInfo>::Reset 0x007323D0.
// Layout: engine/CTCSpotLight.h (PLightInfo +0x10, Overridden +0x1c); CSpotLightInfo (0x18 bytes): Angle +0x00, Width +0x04,
//   InnerRadius +0x08, OuterRadius +0x0c, Flicker +0x10, Colour +0x14; CPersistContext::Mode +0x18 (1/3 = saving).
// Shape: the save test is a named bool (an inline `Mode == 1 || Mode == 3` condition puts the load arm first); the colour
//   default is a CRGBColour temporary whose ctor assigns R, G, B, A (byte stores 2, 1, 0, 3); the loaded colour goes through
//   a local and is copied into the fresh info block (the info has no ctor: `new` is not null-tested).
#include <stddef.h>
void* operator new(size_t size);                       // 0x00BFEA1A
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CPersistContext { public:
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18
    void Transfer(const char* name, bool& value, const bool& defaultValue);               // 0x004045C0
    void Transfer(const char* name, CRGBColour& value, const CRGBColour& defaultValue);   // 0x006420F0
    void Transfer(const char* name, float& value, const float& defaultValue);             // 0x00410620
};
struct CSpotLightInfo {
    float      Angle;                                   // +0x00
    float      Width;                                   // +0x04
    float      InnerRadius;                             // +0x08
    float      OuterRadius;                             // +0x0c
    float      Flicker;                                 // +0x10
    CRGBColour Colour;                                  // +0x14
};
class CCPPointerInfo;
struct CSpotLightInfoPointer {                          // CCountedPointer<CTCSpotLight::CSpotLightInfo>
    CSpotLightInfo* Data;
    CCPPointerInfo* Info;
    void Reset(CSpotLightInfo* data);                   // 0x007323D0
    CSpotLightInfo* operator->() const { return Data; }
};
class CTCSpotLight { public:
    void*         __vftable;
    unsigned char _pad04[0x0c];
    CSpotLightInfoPointer PLightInfo;                   // +0x10
    void*         PLightDef_Object;                     // +0x18
    bool          Overridden;                           // +0x1c
    void OnSerialise(CPersistContext& context, bool loading);
};

void CTCSpotLight::OnSerialise(CPersistContext& context, bool loading)
{
    context.Transfer("Overridden", Overridden, false);
    if (!Overridden)
        return;
    bool saving = context.Mode == 1 || context.Mode == 3;
    if (saving)
    {
        context.Transfer("Colour", PLightInfo->Colour, CRGBColour(0, 0, 0, 0xff));
        context.Transfer("InnerRadius", PLightInfo->InnerRadius, 0.0f);
        context.Transfer("OuterRadius", PLightInfo->OuterRadius, 0.0f);
        context.Transfer("Angle", PLightInfo->Angle, 0.0f);
        context.Transfer("Width", PLightInfo->Width, 0.0f);
        context.Transfer("Flicker", PLightInfo->Flicker, 0.0f);
    }
    else
    {
        PLightInfo.Reset(new CSpotLightInfo);
        CRGBColour colour;
        context.Transfer("Colour", colour, CRGBColour(0, 0, 0, 0xff));
        PLightInfo->Colour = colour;
        context.Transfer("InnerRadius", PLightInfo->InnerRadius, 0.0f);
        context.Transfer("OuterRadius", PLightInfo->OuterRadius, 0.0f);
        context.Transfer("Angle", PLightInfo->Angle, 0.0f);
        context.Transfer("Width", PLightInfo->Width, 0.0f);
        context.Transfer("Flicker", PLightInfo->Flicker, 0.0f);
    }
}