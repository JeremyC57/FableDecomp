// CGameCameraBase::GetRotationAngle(const C3DVector& from, const C3DVector& to) -- signed rotation in turns from `from` to the XY projection of `to`: acos of the normalised dot / 2pi, negative when `to` lies clockwise; 0 outside the acos domain
// Leaf: ?GetRotationAngle@CGameCameraBase@@
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLength respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FD4F0, 227 bytes, `ret 8`.
// Label correction: catalog "Vector3_AngleBetween" -> CGameCameraBase::GetRotationAngle (Ego_r.pdb name set).
// Callees: _CIacos 0x00BFEBA2 (acos intrinsic). Data: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, -1.0f 0x0122DEE0,
//   1/(2*pi) 0x01237F08.
// Shape: a `result = 0.0f` local declared first (the 0.0 preloaded), the flattened target as a named C3DVector,
//   Dot/GetLength inline members (sqrt intrinsic), `sign` defaulting to 1.0f.
extern "C" double __cdecl sqrt(double x);
extern "C" double __cdecl acos(double x);
#pragma intrinsic(sqrt, acos)

#include "engine/C3DVector.h"

class CGameCameraBase {
public:
    virtual ~CGameCameraBase();
    float GetRotationAngle(const C3DVector& from, const C3DVector& to);
};

float CGameCameraBase::GetRotationAngle(const C3DVector& from, const C3DVector& to)
{
    float result = 0.0f;
    C3DVector flatTo(to.X, to.Y, 0.0f);
    float cosAngle = flatTo.Dot(from) / (from.GetMagnitude() * flatTo.GetMagnitude());
    if (cosAngle <= 1.0f && cosAngle >= -1.0f) {
        float angle = (float)acos(cosAngle) * 0.15915494f;
        float sign = 1.0f;
        if (flatTo.X * from.Y - flatTo.Y * from.X < 0.0f)
            sign = -1.0f;
        result = angle * sign;
    }
    return result;
}