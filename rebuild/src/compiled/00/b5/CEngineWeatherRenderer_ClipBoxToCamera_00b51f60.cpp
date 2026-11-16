// CEngineWeatherRenderer::ClipBoxToCamera(const C3DVector& pos, float radius) const -- rejects weather boxes outside the distance /
// frustum limits: the box's smallest particle span is its radius; a box whose centre lies further than radius + span from the camera,
// or whose span matrix the camera clips, is WEATHER_BOX_CLIPPED.
// Leaf: ?ClipBoxToCamera@CEngineWeatherRenderer@@ABE?AW4EWeatherBoxVisible@1@ABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B51F60, 292 bytes. Native minimum uses ordered less-than (including NaNs); distance
// rejection treats unordered as outside.
// NF46 (round 7): ported onto the shared headers, as the same body already lives in the landed RenderRain TU 0x00B54890 (chunk 2037).
//   Identity: Ego_r.pdb / FableWin.pdb ?ClipBoxToCamera@CEngineWeatherRenderer@@ABE?AW4EWeatherBoxVisible@1@ABVC3DVector@@M@Z (private
//   const; was landed as CWeatherRendererMethods::ClipBoxToCamera, a local stand-in). The 0x01436EA0 global is NEngineInternal::GPCamera
//   (Ego_r.pdb / FableWin.pdb ?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A; was gWeatherCamera / CWeatherCameraMethods), its position
//   CEngineCamera::PeekCameraPos() (CameraDesc.WorldPosition) and CEngineCamera::ClipBox(const CMatrix3x4&) const 0x00B30610 (Ego_r.pdb);
//   the box span is Settings.ParticleBoxSpan (CEngineWeatherSettings, +0x114 + 0xC0).
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDERRAIN
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CLIPBOX
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineCamera.h"
#include <math.h>

namespace NEngineInternal { extern CEngineCamera* GPCamera; }                          // 0x01436EA0
#define WEATHER_MIN(a, b) ((a) < (b) ? (a) : (b))

CEngineWeatherRenderer::EWeatherBoxVisible CEngineWeatherRenderer::ClipBoxToCamera(const C3DVector& pos, float radius) const
{
    const CEngineCamera* camera = NEngineInternal::GPCamera;
    float box_radius = WEATHER_MIN(WEATHER_MIN(Settings.ParticleBoxSpan.X, Settings.ParticleBoxSpan.Y), Settings.ParticleBoxSpan.Z);
    float x = camera->PeekCameraPos().X - pos.X, y = camera->PeekCameraPos().Y - pos.Y, z = camera->PeekCameraPos().Z - pos.Z;
    float squared = y * y;
    squared += z * z;
    squared += x * x;
    if (sqrt(squared) - box_radius <= radius) {
        CMatrix3x4 box;
        box.E11 = Settings.ParticleBoxSpan.X;
        box.E22 = Settings.ParticleBoxSpan.Y;
        box.E33 = Settings.ParticleBoxSpan.Z;
        box.E12 = 0; box.E13 = 0; box.E21 = 0; box.E23 = 0; box.E31 = 0; box.E32 = 0;
        box.E41 = pos.X; box.E42 = pos.Y; box.E43 = pos.Z;
        if (camera->ClipBox(box))
            return WEATHER_BOX_VISIBLE;
    }
    return WEATHER_BOX_CLIPPED;
}
#undef WEATHER_MIN
