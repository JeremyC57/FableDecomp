// Leaf: ?ClipBoxToWeatherMaskSphere@CEngineWeatherRenderer@@ABE?AW4EWeatherBoxVisible@1@ABVC3DVector@@@Z
// CEngineWeatherRenderer::ClipBoxToWeatherMaskSphere(const C3DVector& pos) const -- WEATHER_BOX_CLIPPED (1) when the point lies within any of the three weather masks' bounding spheres grown by the weather box's bounding-sphere radius
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local Length respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B52090, 142 bytes (+ int3 padding).
// Callees: CEngineWeatherRenderer::GetBoxBoundingSphereRadiusSquared() @ 0x00B50E50 (float), the `fsqrt` intrinsic.
// NF35 (2026-11-16): renamed from the descriptive IsPointInPrecipitation to Ego_r + FableWin
//   ?ClipBoxToWeatherMaskSphere@CEngineWeatherRenderer@@ABE?AW4EWeatherBoxVisible@1@ABVC3DVector@@@Z (private const, enum result) and moved
//   onto engine/CEngineWeatherRenderer.h (RENDERRAIN typed layout + CLIPBODIES: GetBoxBoundingSphereRadiusSquared private const, ABEMXZ).
// Old layout note: engine/CEngineWeatherRenderer.h keeps WeatherMaskList (CWeatherMask[3], 0x5c bytes each) at +0x66c as opaque bytes;
//   a CWeatherMask starts with its C3DBoundingSphere BSphere { C3DVector Centre; float Radius; } (Ego_r.pdb), so the
//   class is modelled locally with the masks typed.
// Shape: the box radius is a named float local (stored once to the frame); the loop walks the masks by pointer (stride
//   0x5c) with an int counter; the sphere centre comes through a BY-VALUE getter (`GetCentre()`, a memberwise-copied C3DVector
//   temporary: x/y stay on the FPU, z is copied as a dword into the frame) and `(point - centre).Length()` builds the
//   difference through C3DVector's value constructor (dx spilled to the frame), VC summing z*z + y*y before x*x; the 0x1c
//   frame is the box radius plus the two vector temporaries. `Length() < boxRadius + Radius` is `fcompp ; test ah,41h ;
//   je`; the result is an int 0/1. (Reading `sphere.Centre` directly folds the subtraction into memory operands.)
extern "C" double __cdecl sqrt(double value);
#pragma intrinsic(sqrt)

#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDERRAIN
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_CLIPBODIES
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#include "engine/C3DVector.h"
#include "engine/CEngineWeatherRenderer.h"

CEngineWeatherRenderer::EWeatherBoxVisible CEngineWeatherRenderer::ClipBoxToWeatherMaskSphere(const C3DVector& pos) const
{
    float boxRadius = (float)sqrt(GetBoxBoundingSphereRadiusSquared());
    for (long i = 0; i < 3; ++i) {
        const C3DBoundingSphere& sphere = WeatherMaskList[i].BSphere;
        if ((pos - sphere.GetCenter()).GetMagnitude() < boxRadius + sphere.Radius)
            return WEATHER_BOX_CLIPPED;
    }
    return WEATHER_BOX_VISIBLE;
}
