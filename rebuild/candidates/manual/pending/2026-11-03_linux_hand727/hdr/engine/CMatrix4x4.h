#ifndef FABLE_ENGINE_CMATRIX4X4_H
#define FABLE_ENGINE_CMATRIX4X4_H
// CMatrix4x4 (chunk 727): Ego_r.pdb class, 0x40 bytes, a union whose struct arm is float E11..E44 in rows of four
// (struct_layouts_egor.tsv CMatrix4x4_u_0_s_0); the flat members below are that arm (identical layout, as engine/CMatrix3x4.h).
// SetInverse(const CMatrix4x4&): FableWin/Ego_r ?SetInverse@CMatrix4x4@@QAEXABV1@@Z, out of line in retail at 0x00A5CB00 (the
// 4x4 copy into a frame matrix and the Gauss-Jordan elimination; called by CEngineShadowRenderer::CalcClipVolume 0x00B4BD32 on
// the world-to-shadow-buffer matrix). PerspectiveTransformPoint(C3DVector&): FableWin ?PerspectiveTransformPoint@CMatrix4x4@@
// QAEXAAVC3DVector@@@Z (non-const in the PDB), inline in retail (0x00B4BD37: the point times the matrix, divided by w).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
class CMatrix4x4 {
public:
    float E11,E12,E13,E14,E21,E22,E23,E24,E31,E32,E33,E34,E41,E42,E43,E44;
    CMatrix4x4() { }
    void SetInverse(const CMatrix4x4& m);                                // 0x00A5CB00
    __forceinline void PerspectiveTransformPoint(C3DVector& point)
    {
        const float x = point.X * E11 + point.Y * E21 + point.Z * E31 + E41;
        const float y = point.X * E12 + point.Y * E22 + point.Z * E32 + E42;
        const float z = point.X * E13 + point.Y * E23 + point.Z * E33 + E43;
        const float w = point.X * E14 + point.Y * E24 + point.Z * E34 + E44;
        const float ooW = 1.0f / w;
        point.X = x * ooW;
        point.Y = y * ooW;
        point.Z = z * ooW;
    }
};
FABLE_STATIC_ASSERT(sizeof(CMatrix4x4) == 0x40);
#endif // FABLE_ENGINE_CMATRIX4X4_H
