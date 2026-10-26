// Behaviour test for CTCSpotLight::OnSerialise (0x007320e0): Overridden gate, save order, load into a fresh info block.
#include <cstdio>
#include <cstring>
#include <stddef.h>
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CPersistContext { public:
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18
    int Calls; char Names[16][16];
    void Note(const char* name) { if (Calls < 16) strcpy(Names[Calls], name); ++Calls; }
    void Transfer(const char* name, bool& value, const bool& defaultValue) { Note(name); if (Mode == 0) value = true; else if (Mode == 2) value = defaultValue; }
    void Transfer(const char* name, CRGBColour& value, const CRGBColour& defaultValue) { Note(name); if (Mode == 0) { value.R = 10; value.G = 20; value.B = 30; value.A = defaultValue.A; } }
    void Transfer(const char* name, float& value, const float& defaultValue) { Note(name); if (Mode == 0) value = (float)Calls + defaultValue; }
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
    void Reset(CSpotLightInfo* data) { Data = data; }
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

int main()
{
    int fails = 0;
    CTCSpotLight light; light.Overridden = false; light.PLightInfo.Data = 0;
    CPersistContext ctx; ctx.Mode = 2; ctx.Calls = 0;
    light.OnSerialise(ctx, true);                                 // binary load of a non-overridden light: only the flag
    if (ctx.Calls != 1 || light.Overridden || light.PLightInfo.Data) ++fails;
    ctx.Mode = 0; ctx.Calls = 0;                                  // text load of an overridden light
    light.OnSerialise(ctx, true);
    if (ctx.Calls != 7 || !light.Overridden || !light.PLightInfo.Data) ++fails;
    else {
        CSpotLightInfo* info = light.PLightInfo.Data;
        if (info->Colour.R != 10 || info->Colour.G != 20 || info->Colour.B != 30 || info->Colour.A != 0xff) ++fails;
        if (info->InnerRadius != 3.0f || info->OuterRadius != 4.0f || info->Angle != 5.0f || info->Width != 6.0f || info->Flicker != 7.0f) ++fails;
        if (strcmp(ctx.Names[1], "Colour") || strcmp(ctx.Names[2], "InnerRadius") || strcmp(ctx.Names[6], "Flicker")) ++fails;
        CSpotLightInfo* before = info;
        ctx.Mode = 1; ctx.Calls = 0;                              // save keeps the block and reads every field
        light.OnSerialise(ctx, false);
        if (ctx.Calls != 7 || light.PLightInfo.Data != before || info->Angle != 5.0f) ++fails;
        delete before;
    }
    if (fails) { printf("007320e0_TEST FAIL %d\n", fails); return 1; }
    printf("007320e0_TEST PASS\n");
    return 0;
}