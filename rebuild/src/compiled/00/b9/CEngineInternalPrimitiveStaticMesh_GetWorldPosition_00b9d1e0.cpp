// CEngineInternalPrimitiveStaticMesh::GetWorldPosition(C3DVector& position) -- the translation row of the object transform; always true
// Leaf: ?GetWorldPosition@CEngineInternalPrimitiveStaticMesh@@UAE_NAAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9D1E0, 63 bytes, thiscall, `ret 4`.
// Identity (proven): slot 15 of vtable 0x012A425C (RTTI .?AVCEngineInternalPrimitiveStaticMesh@@); Ego_r.pdb
//   ?GetWorldPosition@CEngineInternalPrimitiveStaticMesh@@UAE_NAAVC3DVector@@@Z (the catalog row already carries it).
// Layout: as engine/CEngineInternalPrimitiveStaticMesh.h: ObjectTransform CMatrix3x4 at +0x70, its translation row
//   (E41..E43) at +0x94. Local model: the header carries the vptr as a data member, so the virtual cannot be declared
//   on it (Header debt).
// Shape: `position = ObjectTransform.GetTranslation();` -- FableWin.pdb ?GetTranslation@CMatrix3x4@@QBE?AVC3DVector@@XZ,
//   inline, returning a C3DVector by value: the 12-byte frame temporary is built and then copied into the reference.
class C3DVector {
public:
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    float X, Y, Z;
};

class CMatrix3x4 {
public:
    C3DVector GetTranslation() const { return C3DVector(E41, E42, E43); }
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

class CEngineInternalPrimitiveStaticMesh {
public:
    virtual bool GetWorldPosition(C3DVector& position);
    unsigned char _pad04[0x70 - 4];                  // CEngineInternalPrimitiveMeshBase members (not touched here)
    CMatrix3x4 ObjectTransform;                      // +0x70
};

bool CEngineInternalPrimitiveStaticMesh::GetWorldPosition(C3DVector& position)
{
    position = ObjectTransform.GetTranslation();
    return true;
}