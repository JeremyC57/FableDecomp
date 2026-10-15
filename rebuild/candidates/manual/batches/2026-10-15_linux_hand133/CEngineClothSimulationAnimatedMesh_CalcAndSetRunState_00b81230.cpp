// CEngineClothSimulationAnimatedMesh::CalcAndSetRunState() -- transform the mesh's bounding-sphere centre by CurrentTransform, scale its radius by the owner's scale, store the resulting run state and report whether it is not state 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B81230, length 143 bytes (+ int3 padding).
// Callees: CEngineSimulationBase::CalculateRunState(const C3DVector& centre, float radius) @ 0x00B80940 (__thiscall on `this`, ret 8).
// Layout (retail): CEngineSimulationBase::RunState +0x04 (PDB), CurrentTransform +0x18 (PDB +0x1c; retail CArray 12 bytes), MeshInfo +0xa8,
// Owner +0xb0; C3DMeshInfo::BoundingSphere +0x28 (Center) / +0x34 (Radius); CEngineInternalPrimitiveMeshBase::Scale +0x58.
// Shape: the sphere centre is copied into three x87 registers (a by-value C3DVector local) and reused by `fld st(n)`; each world
// component is accumulated Z-term, Y-term, X-term, then the translation, and stored straight into the 12-byte frame vector before the next
// row (statement-by-statement `+=` fixes VC7.1's term order). The radius is `MeshInfo->Radius * Owner->Scale` (`fld [owner] ; fmul [mesh]`).
// `xor ecx,ecx ; cmp eax,1 ; setne cl ; mov al,cl` is `return RunState != 1;` (a bool result through a full register). The mangled name is
// ?CalcAndSetRunState@CEngineClothSimulationAnimatedMesh@@MAE_NXZ (protected virtual).
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

struct CEngineInternalPrimitiveMeshBase {
    unsigned char _pad00[0x58];
    float         Scale;                    // +0x58
};

struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
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
    unsigned char _pad0c[0x0c];             // +0x0c: ParticleSims (12-byte CArray)
    CMatrix3x4    CurrentTransform;         // +0x18
    CMatrix3x4    LastTransform;            // +0x48
    CMatrix3x4    InverseTransform;         // +0x78
    C3DMeshInfo*  MeshInfo;                 // +0xa8
    unsigned long PrimitiveIndex;           // +0xac
    CEngineInternalPrimitiveMeshBase* Owner; // +0xb0
};

class CEngineClothSimulationAnimatedMesh : public CEngineClothSimulationMeshBase {
protected:
    virtual bool CalcAndSetRunState();
};

bool CEngineClothSimulationAnimatedMesh::CalcAndSetRunState()
{
    C3DVector centre = MeshInfo->BoundingSphere.Center;
    C3DVector world;
    float x = centre.Z * CurrentTransform.E31;
    x += centre.Y * CurrentTransform.E21;
    x += centre.X * CurrentTransform.E11;
    x += CurrentTransform.E41;
    world.X = x;
    float y = centre.Z * CurrentTransform.E32;
    y += centre.Y * CurrentTransform.E22;
    y += centre.X * CurrentTransform.E12;
    y += CurrentTransform.E42;
    world.Y = y;
    float z = centre.Z * CurrentTransform.E33;
    z += centre.Y * CurrentTransform.E23;
    z += centre.X * CurrentTransform.E13;
    z += CurrentTransform.E43;
    world.Z = z;
    RunState = CalculateRunState(world, MeshInfo->BoundingSphere.Radius * Owner->Scale);
    return RunState != 1;
}
