// CEngine::ClipSphere(const C3DBoundingSphere& sphere) const -- true when the sphere lies wholly outside the current engine camera's view volume: beyond the near or far plane along the look vector, or outside one of the four frustum side planes
// Leaf: ?ClipSphere@CEngine@@UBE_NABVC3DBoundingSphere@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B24150, 327 bytes, thiscall `ret 4` (this unused).
// Identity (proven): Ego_r.pdb ?ClipSphere@CEngine@@UBE_NABVC3DBoundingSphere@@@Z (virtual const); the landed CEngine
//   constructor 0x00B260B0 lists it as vtable slot 21. The body reads the engine camera through the global at
//   0x01436EA0: CEngineCamera (Ego_r) CameraDesc.WorldPosition +0x54, CameraDesc.NearZPlane +0x6C / FarZPlane +0x70,
//   LookVector +0x1B4, FrustrumPlanes[4] +0x1C0 (CPlane, 0x10 each; tested in the order 2, 3, 0, 1).
// Globals: 0x01436EA0, the current CEngineCamera* (no PDB data name; landed under eight spellings -- see NOTES.md; this
//   source uses the bound ?g_pEngineCamera_01436ea0@@3PAVCEngineCamera@@A, a descriptive name whose type the layout proves).
// Shape: radius read first, then a copy of the centre through C3DVector's user copy constructor (z homed in the frame,
//   radius in the dead sphere argument slot), then the camera; one `||` chain so the shared `return false` sits at the
//   end (separate ifs gave an early inline return, grade 1); dot products as the C3DVector operator* member.
// Layout: local CEngine with only the virtual (the generated engine/CEngine.h has no virtuals); a local CEngineCamera view
//   of the members read (engine/CEngineCamera.h flattens CameraDesc / FrustrumPlanes to byte arrays).
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
class C3DBoundingSphere {
public:
    C3DVector Centre;                                      // +0x0
    float Radius;                                          // +0xC
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
    unsigned char _pad_0x04[0x10];                         // +0x04
    CEngineCameraDesc CameraDesc;                          // +0x14
    unsigned char _pad_0x80[0x134];                        // +0x80 ViewTransform .. ProjectionTransform
    C3DVector LookVector;                                  // +0x1B4
    CPlane FrustrumPlanes[4];                              // +0x1C0
};
extern CEngineCamera* g_pEngineCamera_01436ea0;                     // 0x01436EA0

class CEngine {
public:
    virtual bool ClipSphere(const C3DBoundingSphere& sphere) const;
};

bool CEngine::ClipSphere(const C3DBoundingSphere& sphere) const
{
    float radius = sphere.Radius;
    C3DVector centre = sphere.Centre;
    const CEngineCamera* camera = g_pEngineCamera_01436ea0;
    C3DVector d = centre - camera->CameraDesc.WorldPosition;
    float depth = d * camera->LookVector;
    if (depth + radius < camera->CameraDesc.NearZPlane ||
        depth - radius > camera->CameraDesc.FarZPlane ||
        d * camera->FrustrumPlanes[2].Normal > radius ||
        d * camera->FrustrumPlanes[3].Normal > radius ||
        d * camera->FrustrumPlanes[0].Normal > radius ||
        d * camera->FrustrumPlanes[1].Normal > radius)
        return false;
    return true;
}