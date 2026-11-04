// CQuaternion::SetFromAxisAngle(const C3DVector& axis, float angle) -- set the rotation of `angle` (in turns) about the unit axis: (axis * sin(angle/2), cos(angle/2)) from the 1024-entry interpolated cosine table
// Leaf: ?SetFromAxisAngle@CQuaternion@@QAEXABVC3DVector@@M@Z
// Flags: none
// Retail-asm: 0x00A88D39 `fistp dword ptr [esp]` (GFFastSin) and 0x00A88D9C `fistp dword ptr [esp+10h]` (GFFastCos) -- each table read
//   rounds `angle * 1024 - 0.5` with the engine's inline-assembly float-to-long GFFloatToLongNear (shared engine/GFFloatToLongNear.h:
//   the argument stored to the helper's by-value copy, reloaded, a bare dword fistp). VC7.1 emits no bare dword fistp from C++
//   (`(long)x` = __ftol2, /QIfist = `fistp qword`). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A88D10, 196 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?SetFromAxisAngle@CQuaternion@@QAEXABVC3DVector@@M@Z.
// Inlined: GFFastSin 0x00A147E0 / GFFastCos 0x00A14840 (FableWin ?GFFastSin@@YIMM@Z / ?GFFastCos@@YIMM@Z, the landed bodies) on angle * 0.5.
// Globals: g_CosTable 0x013CD550 (landed spelling); 0.5f 0x0122F59C, 1024.0f 0x01230010, 1.0f 0x0122DED8.
#include "engine/C3DVector.h"
#include "engine/GFFloatToLongNear.h"
extern float g_CosTable[1025];          // 0x013CD550: cos(2*pi*k/1024), k = 0..1024
__forceinline float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}
__forceinline float __fastcall GFFastCos(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i &= 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}
class CQuaternion {
public:
    float X, Y, Z, W;
    void SetFromAxisAngle(const C3DVector& axis, float angle);
};
void CQuaternion::SetFromAxisAngle(const C3DVector& axis, float angle)
{
    angle = angle * 0.5f;
    float s = GFFastSin(angle);
    X = s * axis.X;
    Y = s * axis.Y;
    Z = s * axis.Z;
    W = GFFastCos(angle);
}