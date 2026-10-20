// Grade: DIFFER(374v294) -- structural model only: the decoded body (rows -(Forward x Up)*sx, -(Forward*sy), Up*sz, WorldPos; colour; flags) is right, but retail keeps a 0x24 frame (one orientation copy whose Up lands at +0xc and Forward.Z at +0x20, the right vector at +0, the forward product at +0x18, the Z scale homed in the dead argument slot) where this form takes 0x48 and copies the whole set. Next: drop the set copy (read Up/Forward through a reference), build the right vector in place, and negate at the stores.
// CEngineInternalPrimitiveParticleMesh::Update(const CEnginePrimitive& primitive, unsigned long flags) -- apply a particle-mesh descriptor: flags, colour and an object transform built from the orientation set scaled by the size percentages (rows: -(right * x), -(forward * y), up * z, the world position)
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC2480, length 292 bytes (`ret 8`, virtual). 0.01f @ 0x01230A5C.
// Layout: PDB CEnginePrimitiveParticleMesh (WorldPos +0x0c, Size +0x18, RightHandedSet +0x24 (Up, Forward), Colour +0x3c) and
// engine/CEngineInternalPrimitiveParticleMesh.h (EnginePrimitiveFlags +0x0c of the base, ObjectTransform +0x48, Colour +0x78).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    __forceinline C3DVector Cross(const C3DVector& o) const { return C3DVector(Y * o.Z - Z * o.Y, Z * o.X - X * o.Z, X * o.Y - Y * o.X); }
};
struct CRightHandedSet { C3DVector Up, Forward; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CMatrix3x4 { C3DVector Row1, Row2, Row3, Row4; };
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
    C3DVector right = set.Forward.Cross(set.Up) * scale.X;
    C3DVector forward = set.Forward * scale.Y;
    C3DVector up = set.Up * scale.Z;
    ObjectTransform.Row1 = -right;
    ObjectTransform.Row2 = -forward;
    ObjectTransform.Row3 = up;
    ObjectTransform.Row4 = mesh.WorldPos;
}
