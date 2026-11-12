// CTCSpotLight::OnAppearanceDraw(CTCGraphicAppearance&, const CGraphicAppearanceDrawInfo&) -- the spot light's per-frame draw: on the first
// draw allocate a persistent primitive slot in the owner's graphic appearance, then hand it a CEnginePrimitiveShadowedSpotLight at the
// owner's position along the light direction, coloured by the light colour scaled by the interpolated flicker factor.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00731EA0, 575 bytes, thiscall `ret 8`; vtable 0x01266FA4 slot 0 (the
//   NGraphicAppearance::CAGraphicAppearanceComponent side at +0x0C), so `this` arrives as the component subobject (edi) and the full
//   object is materialised as edi-0x0C (esi) by the first inline accessor.
// Leaf: ?OnAppearanceDraw@CTCSpotLight@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z
// Callees: CBaseClass::CBaseClass 0x0099A2D0 (inside the inline CPrimitive / CEnginePrimitiveHandle ctor),
//   NGraphicAppearance::CTCGraphicAppearance::AddPersistentPrimitive 0x004C0050, NGraphicAppearance::CPrimitive::~CPrimitive 0x004C4160,
//   __ftol2 0x00BFEA70 (CRGBFloatColour::GetRGBColour), CThing::GetPos 0x004C73D0, CTCSpotLight::GetDirection 0x00731CA0,
//   CEnginePrimitiveShadowedSpotLight::CEnginePrimitiveShadowedSpotLight 0x007324A0 (Ego_r / FableWin
//   ??0CEnginePrimitiveShadowedSpotLight@@QAE@_KABVC3DVector@@1ABVCRGBColour@@MMMW4EEngineInterpolationSet@@K@Z),
//   NGraphicAppearance::CTCGraphicAppearance::SetPrimitiveGraphics 0x004BD620.
// Floats: 1/255 0x01231724, 1.0f 0x0122DED8, 255.0f 0x01230014, 0.5f 0x0122F59C.
// Shape: the inline GetColour/GetWidth/GetInnerRadius/GetOuterRadius accessors (by value, PDB) select info vs def on Overridden; the
//   colour is converted to floats before the flicker blend (the flicker lands in the dead draw_info slot); the radii are named before
//   the GetPos call; the owner's position is held by reference and copied into the argument temporary, the owner re-read through a
//   reference after the call for the 64-bit unique id; the primitive is a named local passed to SetPrimitiveGraphics.
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CSPOTLIGHTDEF
#define FABLE_RETAIL_CTCSPOTLIGHT_BASES
#include "engine/CThing.h"
#include "engine/CTCSpotLight.h"
#include "engine/CRGBFloatColour.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CEnginePrimitive.h"
// No shared header: CGraphicAppearanceDrawInfo (Ego_r 36 bytes, Interpolate +0x00) and CEnginePrimitiveShadowedSpotLight (Ego_r 0x38).
namespace NGraphicAppearance { class CGraphicAppearanceDrawInfo { public: float Interpolate; }; }
class CEnginePrimitiveShadowedSpotLight : public CEnginePrimitive {
public:
    CEnginePrimitiveShadowedSpotLight(unsigned __int64 id, const C3DVector& world_pos, const C3DVector& direction, const CRGBColour& colour,
        float radius, float spot_radius, float cone_radius, EEngineInterpolationSet interpolation_set, unsigned long lighting_channel_mask);   // 0x007324A0
    C3DVector WorldPos;                                     // +0x0c
    C3DVector Direction;                                    // +0x18
    CRGBColour Colour;                                      // +0x24
    float Radius;                                           // +0x28
    float SpotRadius;                                       // +0x2c
    float ConeRadius;                                       // +0x30
    unsigned long LightingChannelMask;                      // +0x34
};

bool CTCSpotLight::OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance& tc_appearance, const NGraphicAppearance::CGraphicAppearanceDrawInfo& draw_info)
{
    if (LightPrimitive == 0)
        LightPrimitive = tc_appearance.AddPersistentPrimitive(NGraphicAppearance::CPrimitive((NGraphicAppearance::EPrimitiveType)3));
    CRGBFloatColour colour(GetColour());
    float flicker = (1.0f - draw_info.Interpolate) * OldAccumilativeFlickerFactor + AccumilativeFlickerFactor * draw_info.Interpolate;
    CRGBColour draw_colour = (colour * flicker).GetRGBColour();
    float width = GetWidth();
    float inner_radius = GetInnerRadius();
    float outer_radius = GetOuterRadius();
    const C3DVector& pos = GetParentThing().GetPos();
    const CThing& thing = GetParentThing();
    CEnginePrimitiveShadowedSpotLight spot_light(thing.GetUniqueID(), C3DVector(pos), GetDirection(), draw_colour,
        outer_radius, inner_radius * flicker, width * 0.5f, (EEngineInterpolationSet)0, 0xFFFFFFFF);
    tc_appearance.SetPrimitiveGraphics(LightPrimitive, spot_light, 0);
    return true;
}