// 0065595C CEnginePrimitive2DClockSprite::CEnginePrimitive2DClockSprite
// Flags: /O1 /Oy- /Gy
// Fidelity: the private instantiation emits the shared constructor in its own object.
// Microsoft LINK discards the private call site when the constructor is linked.
// Leaf: ??0CEnginePrimitive2DClockSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKABVCRGBColour@@KK3W4EEnginePrimitiveBlendMode@@MM11KW4EEngineInterpolationSet@@@Z
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_CLOCK_CORNERS_OWNER_PASS30
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COPY_FORCEINLINE_PASS26
#include "engine/C2DVector.h"
#include "engine/CEnginePrimitive2DClockSprite.h"

#include <cstdio>

static bool SameColour(const CRGBColour& a, const CRGBColour& b)
{
    return a.R == b.R && a.G == b.G && a.B == b.B && a.A == b.A;
}

int main()
{
    for (unsigned i = 0; i < 128; ++i) {
        const float n = static_cast<float>(i);
        const C2DVector centre(n - 64.0f, 65.0f - n);
        const C2DVector start(-n, 0.25f), end(0.5f, n);
        const CRGBColour first(static_cast<unsigned char>(i), 0, 255, 31);
        const CRGBColour second(8, static_cast<unsigned char>(i + 128), 255, 0);
        const EEngine2DLayer layer = static_cast<EEngine2DLayer>(i & 3);
        const EEngineInterpolationSet interp = static_cast<EEngineInterpolationSet>(i & 1);
        const EEnginePrimitiveBlendMode blend = ENGINE_PRIMITIVE_BLEND_ALPHA;
        const unsigned long flags = (i & 1) ? 0xffffffffUL : i;
        CEnginePrimitive2DClockSprite clock(i, centre, layer, 208 + i, i, first, 64 - i, 2 * i,
            second, blend, -0.25f, n, start, end, flags, interp);
        if (static_cast<unsigned>(clock.Type) != 35 || clock.InterpolationSet != interp ||
            clock.ThemeChannelDesc.ChannelIndex0 || clock.ThemeChannelDesc.ChannelIndex1 ||
            clock.ThemeChannelDesc.ChannelBlendFactor || clock.Layer != layer ||
            clock.BankIndex != 208 + i || clock.BankIndex2 != 64 - i ||
            clock.Frame != i || clock.Frame2 != 2 * i || !SameColour(clock.Colour, first) ||
            !SameColour(clock.Colour2, second) || clock.BlendMode != blend || clock.Flags != flags ||
            !clock.ClipBoxIsPos || clock.Filter || clock.AngleStart != -0.25f || clock.AngleEnd != n ||
            clock.ClipBox.TLX != centre.X || clock.ClipBox.BRX != centre.X ||
            clock.ClipBox.TLY != centre.Y || clock.ClipBox.BRY != centre.Y ||
            clock.UVOffsetsStart.X != start.X || clock.UVOffsetsStart.Y != start.Y ||
            clock.UVOffsetsEnd.X != end.X || clock.UVOffsetsEnd.Y != end.Y) return 1;
    }
    std::puts("0065595c_TEST PASS");
    return 0;
}
