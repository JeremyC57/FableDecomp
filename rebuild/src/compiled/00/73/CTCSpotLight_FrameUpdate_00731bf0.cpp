// CTCSpotLight::FrameUpdate() -- random-walk the accumulated flicker factor by the (overridden or def) flicker amount, kept between 1 - flicker and 1
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00731BF0, 169 bytes (plain `ret`); `this` in ecx (written as a __fastcall free
// function over the engine header, as the landed CTCSpotLight::SetAngle 0x00731BE0).
// Callees: CTCBase::GetPLocalSeed @ 0x00686DB0 (called on the header struct cast to its CTCBase part), GFFloatRandom(float range, unsigned long* seed) @ 0x004978A0
// (__fastcall: the range on the stack, the seed in ecx).
// Layout: engine/CTCSpotLight.h (PLightInfo_Data +0x10, PLightDef_Object +0x18, Overridden +0x1c, AccumilativeFlickerFactor
// +0x24, OldAccumilativeFlickerFactor +0x28); CSpotLightInfo Flicker +0x10; CSpotLightDef (def at Ego_r.pdb offsets) Flicker
// +0x38, FlickerSpeed +0x3c.
// Shape: the flicker is the selected amount times the double 0.1 stored to a named float (the threshold compare uses the
// unrounded product); the speed is a named float copied before the calls; the clamp is `if (< low) low; else if (> 1) 1`
// with the accumulated value kept on the x87 stack.
#include "engine/CTCSpotLight.h"
struct CSpotLightInfo {
    unsigned char _pad00[0x10];
    float Flicker;                                      // +0x10
};
class CSpotLightDef { public:
    unsigned char _pad00[0x38];
    float Flicker;                                      // +0x38
    float FlickerSpeed;                                 // +0x3c
};
class CTCBase { public:                                        // the engine header flattens the CTCBase part; only this call is modelled
    unsigned long* GetPLocalSeed();                     // 0x00686DB0
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);  // 0x004978A0

void __fastcall FrameUpdate(CTCSpotLight* light)
{
    light->OldAccumilativeFlickerFactor = light->AccumilativeFlickerFactor;
    CSpotLightDef* def = (CSpotLightDef*)light->PLightDef_Object;
    float flicker = (light->Overridden ? light->PLightInfo_Data->Flicker : def->Flicker) * 0.1;
    if (flicker > 0.00390625f) {
        float speed = ((CSpotLightDef*)light->PLightDef_Object)->FlickerSpeed;
        light->AccumilativeFlickerFactor += (GFFloatRandom(1.0f, ((CTCBase*)light)->GetPLocalSeed()) - 0.5f) * speed * flicker;
        float low = 1.0f - flicker;
        if (light->AccumilativeFlickerFactor < low)
            light->AccumilativeFlickerFactor = low;
        else if (light->AccumilativeFlickerFactor > 1.0f)
            light->AccumilativeFlickerFactor = 1.0f;
    }
}