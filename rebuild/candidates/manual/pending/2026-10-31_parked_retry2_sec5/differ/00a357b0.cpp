// C3DMeshObject::SetPositionMatrix(const C3DVector& position, const CRightHandedSet& axes, float scale) -- build the object's position matrix from a basis, a translation and a uniform scale
// Grade: DIFFER(307v318) -- the scale is applied by a loop over the nine rotation floats (`float* e = &m.E11; for (k <
//   9) e[k] *= scale`): VC7.1 unrolls it into retail's reload-scale / fmul element / store sequence instead of folding
//   the scale into the row stores; residue: the cross product (right = Forward ^ Up) operand order and the position
//   add/copy scheduling
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A357B0, 318 bytes (`ret 0Ch`). No callees (1.0f stored as 0x3F800000).
// Ego_r.pdb: ?SetPositionMatrix@C3DMeshObject@@QAEXABVC3DVector@@ABVCRightHandedSet@@M@Z.
// Shape: copies of Up and Forward (X, Y on the x87 stack, Z through a register to the frame), the right vector
// Forward ^ Up from the set itself, the matrix reset to identity, the position added to its translation, the rows
// set to -right, -forward, up, then the three axis rows scaled element by element.
// Layout: Ego_r.pdb C3DMeshObject PositionMatrix +0x10 (CMatrix3x4 E11..E43); CRightHandedSet Up +0, Forward +0xc.
// Leaf: SetPositionMatrix@C3DMeshObject
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    C3DVector operator^(const C3DVector& o) const { return C3DVector(Y * o.Z - Z * o.Y, Z * o.X - X * o.Z, X * o.Y - Y * o.X); }
    void operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; }
};

struct CRightHandedSet {
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};

struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    void SetIdentity()
    {
        E12 = E13 = E21 = E23 = E31 = E32 = E41 = E42 = E43 = 0.0f;
        E11 = E22 = E33 = 1.0f;
    }
    void Translate(const C3DVector& v)
    {
        C3DVector p = v;
        E41 += p.X;
        E42 += p.Y;
        E43 += p.Z;
    }
};

struct C3DMeshObject {
    void* __vftable;                               // +0x00
    void* MeshInfo;                                // +0x04
    void* Mesh_Data;                               // +0x08 CCountedPointer<const C3DMeshStats>
    void* Mesh_Info;                               // +0x0c
    CMatrix3x4 PositionMatrix;                     // +0x10
    void SetPositionMatrix(const C3DVector& position, const CRightHandedSet& axes, float scale);
};

void C3DMeshObject::SetPositionMatrix(const C3DVector& position, const CRightHandedSet& axes, float scale)
{
    C3DVector up = axes.Up;
    C3DVector forward = axes.Forward;
    C3DVector right = axes.Forward ^ axes.Up;
    CMatrix3x4& m = PositionMatrix;
    m.SetIdentity();
    m.Translate(position);
    m.E11 = -right.X;
    m.E12 = -right.Y;
    m.E13 = -right.Z;
    m.E21 = -forward.X;
    m.E22 = -forward.Y;
    m.E23 = -forward.Z;
    m.E31 = up.X;
    m.E32 = up.Y;
    m.E33 = up.Z;
    float* e = &m.E11;
    for (int k = 0; k < 9; ++k)
        e[k] *= scale;
}
