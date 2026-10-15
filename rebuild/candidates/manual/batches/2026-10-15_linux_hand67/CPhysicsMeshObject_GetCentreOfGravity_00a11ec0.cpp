// CPhysicsMeshObject::GetCentreOfGravity() const -- the physics mesh's centre of gravity rotated into the object's frame (the 3x3 part of the position matrix at +8; no translation)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A11EC0, length 91 bytes.
// Identity: Ego_r.pdb `?GetCentreOfGravity@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ`.
// Callees: none (C3DVector returned through the hidden pointer, `ret 4`).
// Layout (PDB): CPhysicsMeshObject = vptr, Mesh CIVCountedPointer<const CPhysicsMesh> +4,
// PositionMatrix CMatrix3x4 +8 (three basis rows then the position), Scale +0x38, OO_Scale +0x3C;
// CPhysicsMesh::CentreOfGravity is PDB +0xC4, retail +0xA8 (12-byte containers).
// Shape: the rotation is an inline CMatrix3x4 member taking the vector by const reference, whose
// result is a NAMED local filled field by field and returned through C3DVector's memberwise copy
// (it folds into the three x87 sums left on the stack: `fxch st(2)` then three `fstp`). VC7.1
// reorders the terms of each sum on its own: written directly in GetCentreOfGravity (any term or
// operand order) every sum starts with the X term; only through the inline helper does it start
// with Z (`fld [ecx+20h] ; fmul [eax+0B0h]`) as in retail.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CMatrix3x4 {
    C3DVector Row[3];                              // the rotation rows
    C3DVector Position;
    C3DVector Rotate(const C3DVector& v) const
    {
        C3DVector r;
        r.X = v.X * Row[0].X + v.Y * Row[1].X + v.Z * Row[2].X;
        r.Y = v.X * Row[0].Y + v.Y * Row[1].Y + v.Z * Row[2].Y;
        r.Z = v.X * Row[0].Z + v.Y * Row[1].Z + v.Z * Row[2].Z;
        return r;
    }
};
struct CPhysicsMesh {
    unsigned char _pad00[0xa8];
    C3DVector     CentreOfGravity;                 // +0xa8 (PDB +0xc4)
};

struct CPhysicsMeshObject {
    virtual void v00();
    const CPhysicsMesh* Mesh;                      // +0x04 CIVCountedPointer<const CPhysicsMesh>
    CMatrix3x4          PositionMatrix;            // +0x08
    float               Scale;                     // +0x38
    float               OO_Scale;                  // +0x3c
    C3DVector GetCentreOfGravity() const;
};

C3DVector CPhysicsMeshObject::GetCentreOfGravity() const
{
    return PositionMatrix.Rotate(Mesh->CentreOfGravity);
}
