// CEnginePrimitiveShadowedSpotLight::CEnginePrimitiveShadowedSpotLight(unsigned __int64, const C3DVector&, const C3DVector&, const CRGBColour&, float, float, float, EEngineInterpolationSet, unsigned long) -- primitive type 0x10 (ENGINE_PRIMITIVE_SHADOWED_SPOT_LIGHT): world position, direction, colour, radius, spot radius, cone radius and lighting channel mask
// Leaf: ??0CEnginePrimitiveShadowedSpotLight@@QAE@_KABVC3DVector@@1ABVCRGBColour@@MMMW4EEngineInterpolationSet@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007324A0, length 134 bytes (+ int3 padding). No callees.
// Naming (agentNF31, 2026-11-12): was landed as the CEnginePrimitiveLine constructor. The type immediate 0x10 is
//   ENGINE_PRIMITIVE_SHADOWED_SPOT_LIGHT (=16; ENGINE_PRIMITIVE_LINE is 6), the object is 0x38 bytes = Ego_r sizeof
//   CEnginePrimitiveShadowedSpotLight (CEnginePrimitiveLine is 0x3c), and the stores are its members (WorldPos +0xc, Direction +0x18,
//   Colour +0x24, Radius +0x28, SpotRadius +0x2c, ConeRadius +0x30, LightingChannelMask +0x34); Ego_r.pdb 0x0074A620 + FableWin.pdb
//   0x02581D70 ??0CEnginePrimitiveShadowedSpotLight@@QAE@_KABVC3DVector@@1ABVCRGBColour@@MMMW4EEngineInterpolationSet@@K@Z (10 stack
//   words, `ret 28h`), the symbol the landed caller CTCSpotLight::OnAppearanceDraw 0x00731EA0 binds. The CEnginePrimitiveLine
//   constructor is Ego_r 0x004C5F60 (not this address). No engine header models this class; the row keeps its local base models.
// Shape: base inline constructor (interpolation set argument, type immediate, zero channel bytes), vectors
// copied memberwise, the colour through the inline Set(r, g, b, a) copy.


#include "engine/C3DVector.h"
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
class CRGBColour { public:                                      // the union's channel view: D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

class CEnginePrimitiveThemeChannelDesc { public:                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
class CEnginePrimitive { public:                                // 12 bytes (PDB)
    long                             Type;               // +0x00 EEnginePrimitiveType
    long                             InterpolationSet;   // +0x04 EEngineInterpolationSet
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(long type, long interpolationSet = 0) : Type(type), InterpolationSet(interpolationSet) { }
};

#include "engine/EEngineInterpolationSet.h"
class CEnginePrimitiveShadowedSpotLight : public CEnginePrimitive { public:   // Ego_r sizeof 0x38
    C3DVector     WorldPos;                              // +0x0c
    C3DVector     Direction;                             // +0x18
    CRGBColour    Colour;                                // +0x24
    float         Radius;                                // +0x28
    float         SpotRadius;                            // +0x2c
    float         ConeRadius;                            // +0x30
    unsigned long LightingChannelMask;                   // +0x34
    CEnginePrimitiveShadowedSpotLight(unsigned __int64 id, const C3DVector& world_pos, const C3DVector& direction,
        const CRGBColour& colour, float radius, float spot_radius, float cone_radius,
        EEngineInterpolationSet interpolation_set, unsigned long lighting_channel_mask);
};

CEnginePrimitiveShadowedSpotLight::CEnginePrimitiveShadowedSpotLight(unsigned __int64 id, const C3DVector& world_pos,
        const C3DVector& direction, const CRGBColour& colour, float radius, float spot_radius, float cone_radius,
        EEngineInterpolationSet interpolation_set, unsigned long lighting_channel_mask)
    : CEnginePrimitive(0x10, interpolation_set)          // ENGINE_PRIMITIVE_SHADOWED_SPOT_LIGHT
    , WorldPos(world_pos)
    , Direction(direction)
    , Colour(colour)
    , Radius(radius)
    , SpotRadius(spot_radius)
    , ConeRadius(cone_radius)
    , LightingChannelMask(lighting_channel_mask)
{
}
