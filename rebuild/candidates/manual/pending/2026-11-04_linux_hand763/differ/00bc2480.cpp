// CEngineInternalPrimitiveParticleMesh::Update(const CEnginePrimitive& primitive, unsigned long flags) -- apply a particle-mesh descriptor: flags, colour and an object transform built from the orientation set scaled by the size percentages (rows: -(right * x), -(forward * y), up * z, the world position)
// Grade: DIFFER(294v294) -- dl=38: retail keeps a 0x24 frame (the CRightHandedSet copy scaled in place, Z scale homed in the dead flags slot, cross operands lower-offset first); ours takes 0x3c with separate forward/up locals and VC7.1 canonicalises the cross products the other way
// Leaf: ?Update@CEngineInternalPrimitiveParticleMesh@@UAEXABVCEnginePrimitive@@K@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
struct CRGBColour { unsigned char B, G, R, A; };
class CMatrix3x4 { public:
    float M[4][3];
};
class CEnginePrimitive { public: long Type; long InterpolationSet; unsigned long ThemeChannelDesc; };
class CEnginePrimitiveParticleMesh : public CEnginePrimitive {
public:
    C3DVector       WorldPos;                      // +0x0c
    C3DVector       Size;                          // +0x18
    CRightHandedSet RightHandedSet;                // +0x24
    CRGBColour      Colour;                        // +0x3c
};
class CEngineInternalPrimitiveParticleMesh {
public:
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);
    unsigned char _pad04[8];
    unsigned long EnginePrimitiveFlags;            // +0x0c
    unsigned char _pad10[0x38];
    CMatrix3x4    ObjectTransform;                 // +0x48
    CRGBColour    Colour;                          // +0x78
};
void CEngineInternalPrimitiveParticleMesh::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    const CEnginePrimitiveParticleMesh& mesh = static_cast<const CEnginePrimitiveParticleMesh&>(primitive);
    EnginePrimitiveFlags = flags;
    Colour = mesh.Colour;
    C3DVector scale = mesh.Size * 0.01f;
    CRightHandedSet set = mesh.RightHandedSet;
    const C3DVector& u = mesh.RightHandedSet.GetUp();
    const C3DVector& f = mesh.RightHandedSet.GetForward();
    C3DVector right(u.Z * f.Y - u.Y * f.Z, u.X * f.Z - u.Z * f.X, u.Y * f.X - u.X * f.Y);
    right *= scale.X;
    C3DVector forward = set.GetForward() * scale.Y;
    C3DVector up = set.GetUp() * scale.Z;
    ObjectTransform.M[0][0] = -right.X;
    ObjectTransform.M[0][1] = -right.Y;
    ObjectTransform.M[0][2] = -right.Z;
    ObjectTransform.M[1][0] = -forward.X;
    ObjectTransform.M[1][1] = -forward.Y;
    ObjectTransform.M[1][2] = -forward.Z;
    ObjectTransform.M[2][0] = up.X;
    ObjectTransform.M[2][1] = up.Y;
    ObjectTransform.M[2][2] = up.Z;
    ObjectTransform.M[3][0] = mesh.WorldPos.X;
    ObjectTransform.M[3][1] = mesh.WorldPos.Y;
    ObjectTransform.M[3][2] = mesh.WorldPos.Z;
}
