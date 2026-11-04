// CEngine::ConvertScreenPosToWorldPosAtViewDistance(float distance, const C2DVector& screenPos, C3DVector& worldPos) const -- when the engine is active, turn a screen position into the world point at the given view distance: undo the window centring and half-size scaling at that depth, then transform by the camera's inverse view matrix
// Leaf: ?ConvertScreenPosToWorldPosAtViewDistance@CEngine@@UBE_NMABVC2DVector@@AAVC3DVector@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B24400, 238 bytes, `ret 0Ch`. CEngine vtable 0x012A0F3C slot 12 (FableWin.pdb
//   ?ConvertScreenPosToWorldPosAtViewDistance@CEngine@@UBE_NMABVC2DVector@@AAVC3DVector@@@Z, labels_rtti_port.tsv; virtual const).
// Callees: CIEngine::IsActive() const (virtual slot 51, +0xCC; the draft's "HasCamera" was a guess). Globals: the engine camera 0x01436EA0
//   (Ego_r CEngineCamera: OneOverHalfWindowWidth +0xC0, OneOverHalfWindowHeight +0xC4, WindowCenterX +0xC8, WindowCenterY +0xCC,
//   InverseViewTransform +0xE4).
// Shape: the view point is built in place (screen X/Y and the distance, then X/Y rescaled: screen Y is loaded first, X spilled) and
//   transformed in place by the inline CMatrix3x4::TransformPoint (header overlay FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS: X row
//   parenthesised, Y / Z rows left to right), then copied to worldPos with dword moves -- one 12-byte frame vector, as retail. The camera
//   global is read at each use (a hoisted local loads it before screenPos).
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS
#include "engine/CEngine.h"
#include "engine/C2DVector.h"
#include "engine/CMatrix3x4.h"
class CEngineCamera { public:                    // engine/CEngineCamera.h's retail branch leaves the members opaque
    unsigned char _pad000[0xc0];
    float         OneOverHalfWindowWidth;       // +0xc0
    float         OneOverHalfWindowHeight;      // +0xc4
    float         WindowCenterX;                // +0xc8
    float         WindowCenterY;                // +0xcc
    unsigned char _pad0d0[0xe4 - 0xd0];
    CMatrix3x4    InverseViewTransform;         // +0xe4
};
extern CEngineCamera* g_pEngineCamera;          // 0x01436EA0

bool CEngine::ConvertScreenPosToWorldPosAtViewDistance(float distance, const C2DVector& screenPos, C3DVector& worldPos) const
{
    if (IsActive()) {
        C3DVector viewPos(screenPos.X, screenPos.Y, distance);
        viewPos.X = (viewPos.X - g_pEngineCamera->WindowCenterX) * g_pEngineCamera->OneOverHalfWindowWidth * distance;
        viewPos.Y = (g_pEngineCamera->WindowCenterY - viewPos.Y) * g_pEngineCamera->OneOverHalfWindowHeight * distance;
        g_pEngineCamera->InverseViewTransform.TransformPoint(viewPos);
        worldPos = viewPos;
        return true;
    }
    return false;
}