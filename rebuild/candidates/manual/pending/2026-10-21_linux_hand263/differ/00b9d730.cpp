// Grade: DIFFER(429v429) -- one fmul operand order: retail loads the sphere radius first (`fld [ecx+18h]`, then the camera into ecx, `fmul [eax+58h]`, homed in the bound slot), ours loads the mesh scale first (tried both operand orders, radius before the transform/after the camera, `*=`, no camera local, operator- for rel)
// CEnginePrimitiveManagerStaticMeshesBase::IsInsideFustrum(C3DPrimitive2* bound, CEngineInternalPrimitiveStaticMesh* mesh) -- transform the bounding sphere into the world by the mesh's object transform, then test it against the camera's near/far depth range and the four side planes
// Leaf: IsInsideFustrum
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9D730, length 432 bytes (body 0x1ad + int3 padding), `ret 8` (`this` unused).
// Globals: the engine camera @ 0x01436EA0.
// Layout: engine/CEngineCamera.h flattens CameraDesc (+0x14, 0x6c bytes); the desc's camera position (+0x54), near (+0x6c)
//   and far (+0x70) distances are read here, with LookVector +0x1b4 and FrustrumPlanes[4] +0x1c0 (0x10-byte CPlane:
//   normal + d), so the camera is modelled locally at those offsets. The sphere is C3DPrimitive2 (centre +0xc, radius
//   +0x18); the mesh's uniform scale sits at +0x58 and its ObjectTransform (CMatrix3x4, rows Right/Up/Forward + Pos) at
//   +0x70 (engine/CEngineInternalPrimitiveStaticMesh.h names +0x58 a retail-only long; it is read as a float here).
// Shape: the side planes are tested in the order 2, 3, 0, 1 against the normal only (the planes pass through the eye).
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

__forceinline C3DVector operator*(const C3DVector& v, const CMatrix3x4& m)
{
    float x = v.X, y = v.Y, z = v.Z;
    return C3DVector(z * m.Forward.X + y * m.Up.X + x * m.Right.X + m.Pos.X,
                     z * m.Forward.Y + y * m.Up.Y + x * m.Right.Y + m.Pos.Y,
                     z * m.Forward.Z + y * m.Up.Z + x * m.Right.Z + m.Pos.Z);
}

struct CPlane {
    C3DVector Normal;
    float     D;
};

struct C3DPrimitive2 {
    unsigned char _pad00[0xc];
    C3DVector     Centre;                          // +0x0c
    float         Radius;                          // +0x18
};

struct CEngineInternalPrimitiveStaticMesh {
    unsigned char _pad00[0x58];
    float         Scale;                           // +0x58
    unsigned char _pad5c[0x70 - 0x5c];
    CMatrix3x4    ObjectTransform;                 // +0x70
};

struct CEngineCamera {
    unsigned char _pad000[0x54];
    C3DVector     Position;                        // +0x54 (CameraDesc +0x40)
    unsigned char _pad060[0x6c - 0x60];
    float         NearClip;                        // +0x6c
    float         FarClip;                         // +0x70
    unsigned char _pad074[0x1b4 - 0x74];
    C3DVector     LookVector;                      // +0x1b4
    CPlane        FrustrumPlanes[4];               // +0x1c0
};
extern CEngineCamera* g_pEngineCamera;              // 0x01436EA0

static inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct CEnginePrimitiveManagerStaticMeshesBase {
    bool IsInsideFustrum(C3DPrimitive2* bound, CEngineInternalPrimitiveStaticMesh* mesh);
};

bool CEnginePrimitiveManagerStaticMeshesBase::IsInsideFustrum(C3DPrimitive2* bound, CEngineInternalPrimitiveStaticMesh* mesh)
{
    C3DVector centre = bound->Centre * mesh->ObjectTransform;
    float radius = bound->Radius * mesh->Scale;
    CEngineCamera* camera = g_pEngineCamera;
    C3DVector rel;
    rel.X = centre.X - camera->Position.X;
    rel.Y = centre.Y - camera->Position.Y;
    rel.Z = centre.Z - camera->Position.Z;
    float depth = Dot(rel, camera->LookVector);
    if (depth + radius < camera->NearClip || depth - radius > camera->FarClip
        || Dot(rel, camera->FrustrumPlanes[2].Normal) > radius
        || Dot(rel, camera->FrustrumPlanes[3].Normal) > radius
        || Dot(rel, camera->FrustrumPlanes[0].Normal) > radius
        || Dot(rel, camera->FrustrumPlanes[1].Normal) > radius)
        return false;
    return true;
}
