// CEngineClothSimulationStaticMesh::CalcAndSetRunState() -- transform the mesh's bounding-sphere centre by the owning static mesh's ObjectTransform, scale its radius by the owner's scale, store the resulting run state and report whether it is not state 1
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B81180, 167 bytes (+ int3 padding). Mangled ?CalcAndSetRunState@CEngineClothSimulationStaticMesh@@MAE_NXZ.
// Callees: CEngineSimulationBase::CalculateRunState(const C3DVector& centre, float radius) @ 0x00B80940 (on `this`, ret 8).
// Layout (retail): CEngineSimulationBase::RunState +0x04, CEngineClothSimulationMeshBase MeshInfo +0xa8, Owner +0xb0;
//   C3DMeshInfo::BoundingSphere +0x28 (Center) / +0x34 (Radius); the owner is a CEngineInternalPrimitiveStaticMesh: Scale +0x58
//   (CEngineInternalPrimitiveMeshBase), ObjectTransform +0x70 (Ego_r.pdb agrees: the mesh base is 0x70 bytes).
// Shape: the landed animated-mesh twin 0x00B81230 with the owner's ObjectTransform in place of CurrentTransform: the centre is
//   a by-value C3DVector local kept on the x87 stack, each world component accumulated Z-term, Y-term, X-term, translation
//   (statement-by-statement `+=`) and stored into the frame vector; the transform is read through `Owner->ObjectTransform` at each use (a
//   reference local reorders the radius product's loads); `return RunState != 1;` through a full register.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

struct C3DBoundingSphere {
    C3DVector Center;                       // +0x00
    float     Radius;                       // +0x0c
};

struct C3DMeshInfo {
    unsigned char     _pad00[0x28];         // vptr, MeshData, BoundingBox
    C3DBoundingSphere BoundingSphere;       // +0x28
};

struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

struct CEngineInternalPrimitiveStaticMesh {   // the cloth owner (CEngineInternalPrimitiveMeshBase is 0x70 bytes)
    unsigned char _pad00[0x58];
    float         Scale;                    // +0x58
    unsigned char _pad5c[0x70 - 0x5c];
    CMatrix3x4    ObjectTransform;          // +0x70
};

class CEngineSimulationBase {
public:
    virtual void v00();
    long          RunState;                 // +0x04 (ERunState)
    unsigned char SimStepOffset;            // +0x08
    long CalculateRunState(const C3DVector& centre, float radius);   // 0x00B80940
};

class CEngineClothSimulationMeshBase : public CEngineSimulationBase {
public:
    unsigned char _pad0c[0x0c];             // +0x0c: ParticleSims (12-byte vector)
    CMatrix3x4    CurrentTransform;         // +0x18
    CMatrix3x4    LastTransform;            // +0x48
    CMatrix3x4    InverseTransform;         // +0x78
    C3DMeshInfo*  MeshInfo;                 // +0xa8
    unsigned long PrimitiveIndex;           // +0xac
    CEngineInternalPrimitiveStaticMesh* Owner; // +0xb0
};

class CEngineClothSimulationStaticMesh : public CEngineClothSimulationMeshBase {
protected:
    virtual bool CalcAndSetRunState();
};

bool CEngineClothSimulationStaticMesh::CalcAndSetRunState()
{
    C3DVector centre = MeshInfo->BoundingSphere.Center;
    C3DVector world;
    float x = centre.Z * Owner->ObjectTransform.E31;
    x += centre.Y * Owner->ObjectTransform.E21;
    x += centre.X * Owner->ObjectTransform.E11;
    x += Owner->ObjectTransform.E41;
    world.X = x;
    float y = centre.Z * Owner->ObjectTransform.E32;
    y += centre.Y * Owner->ObjectTransform.E22;
    y += centre.X * Owner->ObjectTransform.E12;
    y += Owner->ObjectTransform.E42;
    world.Y = y;
    float z = centre.Z * Owner->ObjectTransform.E33;
    z += centre.Y * Owner->ObjectTransform.E23;
    z += centre.X * Owner->ObjectTransform.E13;
    z += Owner->ObjectTransform.E43;
    world.Z = z;
    RunState = CalculateRunState(world, MeshInfo->BoundingSphere.Radius * Owner->Scale);
    return RunState != 1;
}
