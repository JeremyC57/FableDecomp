// CPhysicsMeshObject::GetHelperPointPositionStatic(const CCharString& name, long index) -- a named helper point of the physics mesh, transformed by the object's position matrix
// Leaf: ?GetHelperPointPositionStatic@CPhysicsMeshObject@@QBE?AVC3DVector@@ABVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A11760, 150 bytes, `ret 0Ch` (hidden C3DVector return + name + index).
//   Callers 0x004BF6E1, 0x0050A0CF. PDB name (Ego_r) confirmed: ?GetHelperPointPositionStatic@CPhysicsMeshObject@@QBE...
// Callees: CPhysicsMesh::GetHelperPointPositionStatic 0x00A6C9C0 (thiscall, hidden-pointer return).
// Shape: the mesh-space point is fetched into a local (C3DVector has a user copy ctor, so it returns through a
//   hidden pointer), x/y are cached, then the point is pushed through PositionMatrix (rows Right/Up/Forward/Position).
class CCharString { public: void* PStringData; };

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& p) : X(p.X), Y(p.Y), Z(p.Z) {}
};

class CPhysicsMesh {
public:
    C3DVector GetHelperPointPositionStatic(const CCharString& name, long index) const;   // 0x00A6C9C0
};

class CIVCountedPointeeBase;

template <class T> class CIVCountedPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
};

class CMatrix3x4 {
public:
    float M[12];                                        // Right, Up, Forward, Position (row vectors)
};

class CPhysicsMeshObject {
public:
    void*                                 VTable;       // +0x00
    CIVCountedPointer<const CPhysicsMesh> Mesh;         // +0x04
    CMatrix3x4                            PositionMatrix;   // +0x08
    float                                 Scale;        // +0x38
    float                                 OO_Scale;     // +0x3C
    C3DVector GetHelperPointPositionStatic(const CCharString& name, long index) const;
};

C3DVector CPhysicsMeshObject::GetHelperPointPositionStatic(const CCharString& name, long index) const
{
    C3DVector point(Mesh->GetHelperPointPositionStatic(name, index));
    const float x = point.X;
    const float y = point.Y;
    point.X = x * PositionMatrix.M[0] + y * PositionMatrix.M[3] + point.Z * PositionMatrix.M[6] + PositionMatrix.M[9];
    point.Y = x * PositionMatrix.M[1] + y * PositionMatrix.M[4] + point.Z * PositionMatrix.M[7] + PositionMatrix.M[10];
    point.Z = x * PositionMatrix.M[2] + y * PositionMatrix.M[5] + point.Z * PositionMatrix.M[8] + PositionMatrix.M[11];
    return point;
}