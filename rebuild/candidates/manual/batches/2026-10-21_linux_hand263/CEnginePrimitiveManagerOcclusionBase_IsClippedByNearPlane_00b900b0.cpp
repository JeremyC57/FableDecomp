// CEnginePrimitiveManagerOcclusionBase::IsClippedByNearPlane(const CMatrix3x4& box) -- map the eight corners of the unit cube through the box transform into camera space and report whether any corner lies in front of the near clip distance
// Leaf: IsClippedByNearPlane
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B900B0, length 448 bytes (body 0x1be + int3 padding), `ret 4` (`this` unused).
// Globals: the engine camera @ 0x01436EA0 (NoScaleViewTransform CMatrix3x4 at +0x114, NearClip at +0x6c = CameraDesc +0x58).
// Layout: engine/CEngineCamera.h keeps CameraDesc and NoScaleViewTransform as opaque byte arrays, so the camera is
//   modelled locally at the same offsets.
// Shape: the corners are a local C3DVector[8] of +-1; the box-to-camera matrix is a full inline CMatrix3x4 product written
//   component-wise (z term first) of which only the Z column survives dead-store elimination; the per-corner depth is
//   `x*R.z + y*U.z + z*F.z + P.z` in source order (VC7.1 evaluates the z product first).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct CMatrix3x4 {
    C3DVector Right;                               // +0x00
    C3DVector Up;                                  // +0x0c
    C3DVector Forward;                             // +0x18
    C3DVector Pos;                                 // +0x24
};

__forceinline void Multiply(CMatrix3x4& r, const CMatrix3x4& a, const CMatrix3x4& b)
{
    r.Right.X = a.Right.Z * b.Forward.X + a.Right.Y * b.Up.X + a.Right.X * b.Right.X;
    r.Right.Y = a.Right.Z * b.Forward.Y + a.Right.Y * b.Up.Y + a.Right.X * b.Right.Y;
    r.Right.Z = a.Right.Z * b.Forward.Z + a.Right.Y * b.Up.Z + a.Right.X * b.Right.Z;
    r.Up.X = a.Up.Z * b.Forward.X + a.Up.Y * b.Up.X + a.Up.X * b.Right.X;
    r.Up.Y = a.Up.Z * b.Forward.Y + a.Up.Y * b.Up.Y + a.Up.X * b.Right.Y;
    r.Up.Z = a.Up.Z * b.Forward.Z + a.Up.Y * b.Up.Z + a.Up.X * b.Right.Z;
    r.Forward.X = a.Forward.Z * b.Forward.X + a.Forward.Y * b.Up.X + a.Forward.X * b.Right.X;
    r.Forward.Y = a.Forward.Z * b.Forward.Y + a.Forward.Y * b.Up.Y + a.Forward.X * b.Right.Y;
    r.Forward.Z = a.Forward.Z * b.Forward.Z + a.Forward.Y * b.Up.Z + a.Forward.X * b.Right.Z;
    r.Pos.X = a.Pos.Z * b.Forward.X + a.Pos.Y * b.Up.X + a.Pos.X * b.Right.X + b.Pos.X;
    r.Pos.Y = a.Pos.Z * b.Forward.Y + a.Pos.Y * b.Up.Y + a.Pos.X * b.Right.Y + b.Pos.Y;
    r.Pos.Z = a.Pos.Z * b.Forward.Z + a.Pos.Y * b.Up.Z + a.Pos.X * b.Right.Z + b.Pos.Z;
}

struct CEngineCamera {
    unsigned char _pad000[0x6c];
    float         NearClip;                        // +0x6c (CameraDesc +0x58)
    unsigned char _pad070[0x114 - 0x70];
    CMatrix3x4    NoScaleViewTransform;            // +0x114
};
extern CEngineCamera* g_pEngineCamera;              // 0x01436EA0

struct CEnginePrimitiveManagerOcclusionBase {
    bool IsClippedByNearPlane(const CMatrix3x4& box);
};

bool CEnginePrimitiveManagerOcclusionBase::IsClippedByNearPlane(const CMatrix3x4& box)
{
    C3DVector corners[8] = {
        C3DVector(-1.0f, -1.0f, -1.0f), C3DVector(-1.0f, -1.0f, 1.0f), C3DVector(-1.0f, 1.0f, -1.0f), C3DVector(-1.0f, 1.0f, 1.0f),
        C3DVector(1.0f, -1.0f, -1.0f), C3DVector(1.0f, -1.0f, 1.0f), C3DVector(1.0f, 1.0f, -1.0f), C3DVector(1.0f, 1.0f, 1.0f),
    };
    CMatrix3x4 toCamera;
    Multiply(toCamera, box, g_pEngineCamera->NoScaleViewTransform);
    for (unsigned long i = 0; i < 8; ++i) {
        const C3DVector& c = corners[i];
        if (c.X * toCamera.Right.Z + c.Y * toCamera.Up.Z + c.Z * toCamera.Forward.Z + toCamera.Pos.Z < g_pEngineCamera->NearClip)
            return true;
    }
    return false;
}
