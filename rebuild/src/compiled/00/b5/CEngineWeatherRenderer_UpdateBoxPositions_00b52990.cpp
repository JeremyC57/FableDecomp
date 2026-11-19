// CEngineWeatherRenderer::UpdateBoxPositions() -- unless the particle boxes are locked, rebuild the 9 x 9 box matrix when the camera has moved at least half a box span (floored) from the snapped box centre on any axis
// Leaf: ?UpdateBoxPositions@CEngineWeatherRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B52990, 406 bytes (416 with padding), thiscall `ret`, ebp frame (the GFFloatToLongFloor expansions' frame words).
// Identity: Ego_r.pdb / FableWin.pdb ?UpdateBoxPositions@CEngineWeatherRenderer@@AAEXXZ (private: AAE, the old row bound QAE through a
//   local class model); its one caller is FrameStart 0x00B53F60, its one callee BuildBoxPositionMatrix 0x00B51AD0.
// Locals / scope: Ego_d.pdb {cam_pos (const C3DVector&); half_box_span_X / _Y / _Z, diff_X / _Y / _Z (const long)}, all in one block
//   under the lock test (`if (!ParticleBoxLocked) { ... }`). Members: the shared CEngineWeatherSettings (Settings +0x114):
//   ParticleBoxLocked +0x06 (renderer +0x11A), ParticleBoxSpan +0xC0 (+0x1D4), ParticleBoxCenter (C3DCoordI) +0xDC (+0x1F0).
// Re-port (audit 2026-11-19): replaces the 2026-11-04 text (local `class CEngineWeatherRenderer` pad model, FreezeBoxes / BoxSize /
//   BoxCentre, sizeX / distX locals, early return, `#pragma optimize("y", off)`). The pragma is not needed (the asm helper already gives
//   the ebp frame: RELOCATION_MATCH without it) and, left in FrameStart's TU, gives FrameStart an ebp frame (403v411). This text is the
//   one FrameStart's TU (0x00B53F60) carries as its same-TU strong copy.
// Retail-asm: 0x00B529C7, 0x00B52A01, 0x00B52A3B, 0x00B52A7C, 0x00B52AB5, 0x00B52AEE `fistp dword ptr [ebp-8]` .. -- the shared
//   engine/GFFloatToLongFloor.h inline-assembly floor (six expansions); this row carries no asm of its own.
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BOXMATRIX
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_FRAMESTART
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_BOXCENTER
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#include "fable_compiler.h"
#include "engine/C3DVector.h"
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineCamera.h"
#include "engine/GFFloatToLongFloor.h"
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                          // 0x01436EA0

void CEngineWeatherRenderer::UpdateBoxPositions()
{
    if (!Settings.ParticleBoxLocked) {
        const long half_box_span_X = GFFloatToLongFloor(Settings.ParticleBoxSpan.X * 0.5f);
        const long half_box_span_Y = GFFloatToLongFloor(Settings.ParticleBoxSpan.Y * 0.5f);
        const long half_box_span_Z = GFFloatToLongFloor(Settings.ParticleBoxSpan.Z * 0.5f);
        const C3DVector& cam_pos = NEngineInternal::GPCamera->PeekCameraPos();
        const long diff_X = GFFloatToLongFloor((float)fabs((float)Settings.ParticleBoxCenter.X - cam_pos.X));
        const long diff_Y = GFFloatToLongFloor((float)fabs((float)Settings.ParticleBoxCenter.Y - cam_pos.Y));
        const long diff_Z = GFFloatToLongFloor((float)fabs((float)Settings.ParticleBoxCenter.Z - cam_pos.Z));
        if (diff_X >= half_box_span_X || diff_Y >= half_box_span_Y || diff_Z >= half_box_span_Z)
            BuildBoxPositionMatrix();
    }
}