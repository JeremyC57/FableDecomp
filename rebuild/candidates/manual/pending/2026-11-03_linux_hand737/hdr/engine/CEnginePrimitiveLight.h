#ifndef FABLE_ENGINE_CENGINEPRIMITIVELIGHT_H
#define FABLE_ENGINE_CENGINEPRIMITIVELIGHT_H
// New shared header (chunk 737, CPSCLight::PreparePrimitives 0x00B19C60): Ego_r.pdb CEnginePrimitiveLight : CEnginePrimitive, sizeof
// 0x28: WorldPos +0x0C, Colour +0x18, Radius +0x1C, SpotRadius +0x20, LightingChannelMask +0x24 (the landed constructor 0x00642000's
// stores). The constructor is out of line in retail: ??0CEnginePrimitiveLight@@QAE@_KABVC3DVector@@ABVCRGBColour@@MMW4EEngineInterpolationSet@@K@Z
// (Ego_r.pdb; 8 stack words, `ret 20h`; the 64-bit handle is not stored).
#include "rebuild_abi.h"
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
class CEnginePrimitiveLight : public CEnginePrimitive {
public:
    CEnginePrimitiveLight(unsigned __int64 handle, const C3DVector& world_pos, const CRGBColour& colour, float radius, float spot_radius,
                          EEngineInterpolationSet interpolation_set, unsigned long lighting_channel_mask);   // 0x00642000
    C3DVector     WorldPos;              // +0x0C
    CRGBColour    Colour;                // +0x18
    float         Radius;                // +0x1C
    float         SpotRadius;            // +0x20
    unsigned long LightingChannelMask;   // +0x24
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveLight) == 0x28);
#endif
