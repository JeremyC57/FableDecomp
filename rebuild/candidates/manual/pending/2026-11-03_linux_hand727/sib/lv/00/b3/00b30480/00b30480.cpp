// CEngineCamera::ClipSphere(const C3DVector& centre, float radius, unsigned long* clipFlags) const -- incremental frustum test for a sphere against the four side planes, skipping planes the flags already mark as passed: false (and the 0x40 "outside" flag set) when the sphere lies beyond a plane, a plane's bit (4 << plane) set when the sphere lies wholly inside it, true otherwise
// Leaf: ?ClipSphere@CEngineCamera@@QBE_NABVC3DVector@@MPAK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B30480, 397 bytes, thiscall `ret 0Ch`.
// Identity (proven): Ego_r.pdb ?ClipSphere@CEngineCamera@@QBE_NABVC3DVector@@MPAK@Z (const; centre, radius, flags pointer --
//   the row's three stack arguments and bool result; donor 0x009D0BF0, 0x1B0 bytes), the CEngineCamera members it reads
//   (CameraDesc.WorldPosition +0x54, FrustrumPlanes[4] +0x1C0, tested 2, 3, 0, 1 as CEngine::ClipSphere 0x00B24150 does).
//   The catalog row name "ClipSphere" had the class right but no signature.
// Layout: local CEngineCamera view (engine/CEngineCamera.h flattens CameraDesc / FrustrumPlanes to byte arrays and a
//   method cannot be declared on it).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float operator*(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }   // dot
};
class CPlane {
public:
    C3DVector Normal;                                      // +0x0
    float D;                                               // +0xC
};
struct CEngineCameraDesc {
    unsigned char Window[0x10];                            // +0x00 C2DBoxF
    unsigned char RotationMatrix[0x30];                    // +0x10 CMatrix3x4
    C3DVector WorldPosition;                               // +0x40
    float HorizontalFOV;                                   // +0x4C
    float VerticalFOV;                                     // +0x50
    bool Use2DFOV;                                         // +0x54
    float NearZPlane;                                      // +0x58
    float FarZPlane;                                       // +0x5C
    float ZBufferStart;                                    // +0x60
    float ZBufferEnd;                                      // +0x64
    bool StateValid;                                       // +0x68
};
class CEngineCamera {
public:
    virtual ~CEngineCamera();
    bool ClipSphere(const C3DVector& centre, float radius, unsigned long* clipFlags) const;
    unsigned char _pad_0x04[0x10];                         // +0x04
    CEngineCameraDesc CameraDesc;                          // +0x14
    unsigned char _pad_0x80[0x134];                        // +0x80 ViewTransform .. ProjectionTransform
    C3DVector LookVector;                                  // +0x1B4
    CPlane FrustrumPlanes[4];                              // +0x1C0
};

bool CEngineCamera::ClipSphere(const C3DVector& centre, float radius, unsigned long* clipFlags) const
{
    unsigned long flags = *clipFlags;
    if (flags & 0x40)
        return false;
    if ((flags & 0x3C) == 0x3C)
        return true;
    C3DVector d = centre - CameraDesc.WorldPosition;
    float distance;
    if (!(*clipFlags & 0x10)) {
        distance = d * FrustrumPlanes[2].Normal;
        if (distance > radius) {
            *clipFlags |= 0x40;
            return false;
        }
        if (distance < -radius)
            *clipFlags |= 0x10;
    }
    if (!(*clipFlags & 0x20)) {
        distance = d * FrustrumPlanes[3].Normal;
        if (distance > radius) {
            *clipFlags |= 0x40;
            return false;
        }
        if (distance < -radius)
            *clipFlags |= 0x20;
    }
    if (!(*clipFlags & 0x04)) {
        distance = d * FrustrumPlanes[0].Normal;
        if (distance > radius) {
            *clipFlags |= 0x40;
            return false;
        }
        if (distance < -radius)
            *clipFlags |= 0x04;
    }
    if (!(*clipFlags & 0x08)) {
        distance = d * FrustrumPlanes[1].Normal;
        if (distance > radius) {
            *clipFlags |= 0x40;
            return false;
        }
        if (distance < -radius)
            *clipFlags |= 0x08;
    }
    return true;
}