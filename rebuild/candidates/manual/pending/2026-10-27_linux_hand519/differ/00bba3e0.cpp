// CEngineInternalPrimitiveDecal::GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere) -- a sphere centred on the decal's world position with the length of its 2D size as radius; always true
// Grade: DIFFER(87v87) -- scheduling only: retail issues `faddp` before `mov [esp+8],eax` and `fsqrt` before `mov [ecx],edx`; ours one slot later. Named-local, inline C2DVector length helper and single-expression forms all schedule identically
// Leaf: ?GetBoundingSphereWorldSpace@CEngineInternalPrimitiveDecal@@UAE_NAAVC3DBoundingSphere@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBA3E0, 87 bytes, thiscall, `ret 4`.
// Identity (proven): slot 18 of vtable 0x012A5AE4 (RTTI .?AVCEngineInternalPrimitiveDecal@@), the GetBoundingSphereWorldSpace
//   slot of the landed CEngineInternalPrimitiveSprite 0x00BACF00 (vtable 0x012A5444 slot 18); Ego_r.pdb
//   ?GetBoundingSphereWorldSpace@CEngineInternalPrimitiveDecal@@UAE_NAAVC3DBoundingSphere@@@Z.
// Layout (Ego_r.pdb, retail offsets equal: CEngineInternalPrimitiveBase is 0x48): WorldPos C3DVector +0x54, Size C2DVector
//   +0x60 (X), +0x64 (Y).
// Callees: none (fsqrt inline via sqrtf).
// Shape: the landed Sprite model: `sphere = C3DBoundingSphere(WorldPos, sqrtf(x*x + y*y));` with C3DVector's memberwise user
//   copy constructor (the centre copied into the 16-byte frame temporary, then the implicit operator= copies four words out).
#include <math.h>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C2DVector {
public:
    float X, Y;
};

class C3DBoundingSphere {
public:
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() { }
    C3DBoundingSphere(const C3DVector& centre, float radius) : Center(centre), Radius(radius) { }
};

class CEngineInternalPrimitiveDecal {
public:
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere);
    unsigned char _pad04[0x48 - 4];                  // CEngineInternalPrimitiveBase (not touched here)
    bool LandscapeOnlyDecal;                         // +0x48
    long DecalIndex;                                 // +0x4C
    long DecalFrame;                                 // +0x50
    C3DVector WorldPos;                              // +0x54
    C2DVector Size;                                  // +0x60
};

bool CEngineInternalPrimitiveDecal::GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere)
{
    float x = Size.X;
    float y = Size.Y;
    sphere = C3DBoundingSphere(WorldPos, sqrtf(x * x + y * y));
    return true;
}
