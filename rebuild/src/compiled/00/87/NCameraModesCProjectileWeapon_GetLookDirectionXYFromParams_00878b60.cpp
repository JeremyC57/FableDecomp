// NCameraModes::CProjectileWeapon::GetLookDirectionXYFromParams(float phi, float heightParam) const -- the camera's horizontal look direction for an orbit angle: the XY offset a quarter turn on (less the def's initial phi offset), turned to face back along the orbit
// Leaf: ?GetLookDirectionXYFromParams@CProjectileWeapon@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00878B60, length 128 bytes (`ret 0Ch`: hidden C3DVector result pointer,
//   phi, heightParam -- heightParam is unused).
// Identity: Ego_r.pdb lists `?GetLookDirectionXYFromParams@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@MM@Z`
//   (const, C3DVector by value, two floats); its only caller 0x00878E13 is in the same TU, and it calls the landed
//   0x00878A80 on `this` -- which the PDB name set only fits as `GetCamPosOffsetXY(float) const` (C3DVector by value,
//   one float; the landed file's "GetRotatedPosition" is a descriptive name). The catalog label
//   "CProjectileWeapon_CalcPitchDirection" is an auto label.
// Callee: NCameraModes::CProjectileWeapon::GetCamPosOffsetXY(float) const @ 0x00878A80 (landed). Constants: 1/360
//   0x01238E00, 0.25f 0x0123078C, -1.0f 0x0122DEE0.
// Layout: CBase PCameraModeDef +0x44 (CDefPointer, Object at +0); CCameraModeDef
//   ProjectileCameraInitialPhiOffsetForCamera +0xA0 (as the landed SetUpParams 0x00878CE0).
// Shape: the angle is `phi - (offset * (1/360) - 0.25f)` (`fsubr` against the phi argument); the offset is a named
//   local that is overwritten by a `C3DVector(y, -x, 0) * -1.0f` temporary (implicit operator=: dword copies through
//   eax/ecx/edx) and then returned by copy (no NRVO in VC7.1; user copy constructor). Both live components get
//   `fmul dword [-1.0f]` and the constant Z folds to -0.0f (0x80000000). Returning the product directly drops both
//   frame vectors (85 bytes).
#pragma pack(push, 1)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
#pragma pack(pop)

class CCameraModeDef { public:
    unsigned char _pad00[0xa0];
    float         ProjectileCameraInitialPhiOffsetForCamera;    // +0xa0
    float         ProjectileCameraInitialHeightParamForCamera;  // +0xa4
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

namespace NCameraModes {
#pragma pack(push, 1)
class CBase { public:
    virtual void v00();
    unsigned char _pad04[0x44 - 0x04];
    CDefPointer<const CCameraModeDef> PCameraModeDef;   // +0x44
};
class CProjectileWeapon : public CBase { public:                      // vtable 0x01277804
    C3DVector GetCamPosOffsetXY(float phi) const;       // 0x00878A80
    C3DVector GetLookDirectionXYFromParams(float phi, float heightParam) const;
};
#pragma pack(pop)
}

static const float ONE_OVER_360 = 1.0f / 360.0f;       // 0x01238E00

C3DVector NCameraModes::CProjectileWeapon::GetLookDirectionXYFromParams(float phi, float heightParam) const
{
    C3DVector direction = GetCamPosOffsetXY(phi - (PCameraModeDef->ProjectileCameraInitialPhiOffsetForCamera * ONE_OVER_360 - 0.25f));
    direction = C3DVector(direction.Y, -direction.X, 0.0f) * -1.0f;
    return direction;
}