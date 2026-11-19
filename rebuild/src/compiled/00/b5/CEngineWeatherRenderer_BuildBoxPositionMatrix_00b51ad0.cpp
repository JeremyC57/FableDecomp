// CEngineWeatherRenderer::BuildBoxPositionMatrix() -- snap the particle-box centre to the box grid around the camera (floor each camera coordinate, then round it to the nearest whole box span; Z only when the box is not Z-locked) and lay out the 9 x 9 box positions around that centre
// Leaf: ?BuildBoxPositionMatrix@CEngineWeatherRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B51AD0, 969 bytes, thiscall `ret`, ebp frame (the GFFloatToLongFloor expansions' frame words).
// Identity: Ego_r.pdb ?BuildBoxPositionMatrix@CEngineWeatherRenderer@@AAEXXZ (private; Ego_r 0x009F2D10, 967 bytes, object order 14 of
//   Release\Engine_Weather_Renderer.obj); its one caller is the landed UpdateBoxPositions 0x00B52990. The snap helper is Ego_r's private
//   ?GetBoxCoord@CEngineWeatherRenderer@@AAEXAAJJ@Z (Ego_r 0x009F2330, object order 5), inline in retail.
// Locals / names: Ego_d.pdb BuildBoxPositionMatrix {cam_pos (const C3DVector&), half_side_length (const long); {c_x; {c_y;
//   {this_mul_x, this_mul_y (const float), index (const long)}}}} and GetBoxCoord (coord, box_span; distance_to_nearest_box_centre,
//   half_box_span, both const long). Members: Ego_r.pdb CEngineWeatherSettings ParticleBoxZLocked (+0x07), ParticleBoxSpan (+0xC0),
//   ParticleBoxCenter (C3DCoordI, +0xDC), ParticleBoxPos[81] (+0xE8), at renderer Settings +0x114.
// Globals: NEngineInternal::GPCamera 0x01436EA0 (Ego_r.pdb / FableWin.pdb), position through CEngineCamera::PeekCameraPos (+0x54).
// Shape: the loops run c_x / c_y from 0 to the side length (2 * half_side_length + 1) with this_mul_* = coordinate - half_side_length
//   and index = c_x * side + c_y: VC7.1 then keeps the outer counter as a 9-trip down-counter (`mov edx,9 ... dec edx ; jne`, 0x00B51CD3 /
//   0x00B51E58) and carries this_mul_x in the frame word [ebp-4] from -4 (0x00B51CC6), with the pointer IV at +0x20C (0x00B51CCD). The
//   symmetric spelling `for (c_x = -half_side_length; c_x <= half_side_length; ...)` keeps c_x itself in edx with `cmp edx,4 ; jle`
//   and releases ebx / esi / edi before the loop (965v969, as in the parked drafts); the inner loop is fully unrolled either way.
// Retail-asm: 0x00B51AFC, 0x00B51B39, 0x00B51B9A, 0x00B51BD7, 0x00B51C46, 0x00B51C83 `fistp dword ptr [ebp-4]` -- the shared
//   engine/GFFloatToLongFloor.h inline-assembly floor (as in UpdateBoxPositions 0x00B52990). This row carries no asm of its own.
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BOXMATRIX
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_BOXCENTER
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#include "fable_compiler.h"
#include "engine/C3DVector.h"
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineCamera.h"
#include "engine/GFFloatToLongFloor.h"
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                          // 0x01436EA0

inline void CEngineWeatherRenderer::GetBoxCoord(long& coord, long box_span)
{
    const long distance_to_nearest_box_centre = coord % box_span;
    const long half_box_span = box_span / 2;
    coord = coord / box_span * box_span;
    if (distance_to_nearest_box_centre >= half_box_span)
        coord += box_span;
}

void CEngineWeatherRenderer::BuildBoxPositionMatrix()
{
    const C3DVector& cam_pos = NEngineInternal::GPCamera->PeekCameraPos();
    const long half_side_length = 4;
    Settings.ParticleBoxCenter.X = GFFloatToLongFloor(cam_pos.X);
    GetBoxCoord(Settings.ParticleBoxCenter.X, GFFloatToLongFloor(Settings.ParticleBoxSpan.X));
    Settings.ParticleBoxCenter.Y = GFFloatToLongFloor(cam_pos.Y);
    GetBoxCoord(Settings.ParticleBoxCenter.Y, GFFloatToLongFloor(Settings.ParticleBoxSpan.Y));
    if (!Settings.ParticleBoxZLocked) {
        Settings.ParticleBoxCenter.Z = GFFloatToLongFloor(cam_pos.Z);
        GetBoxCoord(Settings.ParticleBoxCenter.Z, GFFloatToLongFloor(Settings.ParticleBoxSpan.Z));
    }
    for (long c_x = 0; c_x < half_side_length * 2 + 1; c_x++) {
        for (long c_y = 0; c_y < half_side_length * 2 + 1; c_y++) {
            const float this_mul_x = (float)(c_x - half_side_length);
            const float this_mul_y = (float)(c_y - half_side_length);
            const long index = c_x * (half_side_length * 2 + 1) + c_y;
            Settings.ParticleBoxPos[index].X = this_mul_x * Settings.ParticleBoxSpan.X + Settings.ParticleBoxCenter.X;
            Settings.ParticleBoxPos[index].Y = this_mul_y * Settings.ParticleBoxSpan.Y + Settings.ParticleBoxCenter.Y;
            Settings.ParticleBoxPos[index].Z = (float)Settings.ParticleBoxCenter.Z;
        }
    }
}