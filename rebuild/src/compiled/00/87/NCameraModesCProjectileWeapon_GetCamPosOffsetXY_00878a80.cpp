// NCameraModes::CProjectileWeapon::GetCamPosOffsetXY(float fraction) const -- the camera position offset at angle fraction*2pi on the mode's X/Y axis circle: XAxis*sin(a) + YAxis*cos(a)
// Leaf: ?GetCamPosOffsetXY@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00878A80, 119 bytes (`ret 8`: hidden C3DVector result pointer + the float).
// Naming: Ego_r.pdb ?GetCamPosOffsetXY@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@M@Z (private const, C3DVector by
//   value + one float = `ret 8`), the only CProjectileWeapon member of that signature; FINDINGS chunk 404. The earlier
//   name "GetRotatedPosition" came from the manifest auto label "CThing::GetRotatedPosition". Both callers (0x00878B85 in
//   CProjectileWeapon_CalcPitchDirection 0x00878B60 and 0x00878C1E in CProjectileWeapon::GetPosFromParams 0x00878BE0)
//   call it on the NCameraModes::CProjectileWeapon `this` (vtable 0x01277804), and the two byte-packed vectors it
//   reads match Ego_r.pdb CProjectileWeapon XAxis (+0xcd) / YAxis (+0xd9), 0x24 lower in retail: XAxis +0xa9,
//   YAxis +0xb5 (ZAxis +0xc1). Double pool 0x0124F2B8 = 6.2831854820251465 = (double)(2pi as float).
// Shape: the angle is computed in double (`fmul qword`), cos and sin share it (intrinsics); the two scaled vectors are
//   by-value operator* temporaries (cos term first) summed by an inline operator+ (sin term + cos term).
#include <math.h>
#pragma intrinsic(cos, sin)
#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
#pragma pack(pop)
static const float TWO_PI = 6.2831855f;
namespace NCameraModes {
#pragma pack(push, 1)                           // genuine: Ego_r.pdb CProjectileWeapon XAxis sits at the odd offset +0xcd
class CProjectileWeapon {
public:
    unsigned char _pad[0xa9];
    C3DVector XAxis;                            // +0xa9
    C3DVector YAxis;                            // +0xb5
    C3DVector ZAxis;                            // +0xc1
private:
    C3DVector GetCamPosOffsetXY(float fraction) const;
};
#pragma pack(pop)

C3DVector CProjectileWeapon::GetCamPosOffsetXY(float fraction) const
{
    double angle = fraction * (double)TWO_PI;
    return XAxis * (float)sin(angle) + YAxis * (float)cos(angle);
}
}