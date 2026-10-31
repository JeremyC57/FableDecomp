// CPhysicsMeshObject::GetBoundingBoxXEdge() const -- half the mesh bounding box's X extent along the object's first basis row, rotated into world orientation (no translation)
// Grade: DIFFER(104v103) -- a three-float Rotate overload (no C3DVector temporary): the frame is now retail's single
//   push-ecx word (9 -> 6 diff lines); residue: retail keeps the half extent on the x87 stack after storing it (fst
//   [esp] ; fxch) for the first row, ours fstp + fld [esp]; a named half and a reordered row sum give the same
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A11C20, length 103 bytes (`ret 4`, C3DVector by hidden pointer). Constants 0.5f @ 0x0122F59C, 0.0f @ 0x0122DEDC.
// Identity: Ego_r.pdb `?GetBoundingBoxXEdge@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ`; the X/Y/Z edges are 0x00A11C20 / 0x00A11C90 / 0x00A11BA0
// (callers 0x004BC9A5.. use all three in a row). The manifest's "CEngineInternalPrimitiveParticleLine::GetWorldPosition" is propagated and wrong.
// Layout (PDB): CPhysicsMeshObject = vptr, Mesh +4 (CIVCountedPointer<const CPhysicsMesh>), PositionMatrix CMatrix3x4 +8; the mesh's
// bounding box sits at retail +0x84 (Min) / +0x90 (Max) (12-byte containers before it, like CentreOfGravity at +0xA8).
// Shape: `PositionMatrix.Rotate(C3DVector(...))` -- the landed CMatrix3x4::Rotate of GetCentreOfGravity (0x00A11EC0, NAMED result,
// memberwise copy constructor); the two zero components fold into `(a + b) * 0.0f` per row (the 0.0f pool constant), the half
// extent is the only frame word of the temporary vector; the box is reached through a reference (`add eax,84h`).
// Tried: (a) temporary vector [this file]; (b) Rotate taking the vector by value (not inlined, 57 bytes); (c) a named half-width
// float (same as a); (d) no user copy constructor (Rotate result goes through a frame copy, 73 bytes). Next: lane gap (frame trimming of
// a partly constant temporary whose live word is the first one).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct C3DBoundingBox {
    C3DVector Min;
    C3DVector Max;
};

struct CMatrix3x4 {
    C3DVector Row[3];                              // the rotation rows
    C3DVector Position;
    C3DVector Rotate(float x, float y, float z) const
    {
        C3DVector r;
        r.X = x * Row[0].X + y * Row[1].X + z * Row[2].X;
        r.Y = x * Row[0].Y + y * Row[1].Y + z * Row[2].Y;
        r.Z = x * Row[0].Z + y * Row[1].Z + z * Row[2].Z;
        return r;
    }
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
    unsigned char  _pad00[0x84];
    C3DBoundingBox BoundingBox;                    // +0x84 (retail; 12-byte containers before it)
};

struct CPhysicsMeshObject {
    virtual void v00();
    const CPhysicsMesh* Mesh;                      // +0x04 CIVCountedPointer<const CPhysicsMesh>
    CMatrix3x4          PositionMatrix;            // +0x08
    float               Scale;                     // +0x38
    float               OO_Scale;                  // +0x3c
    C3DVector GetBoundingBoxXEdge() const;
    C3DVector GetBoundingBoxYEdge() const;
    C3DVector GetBoundingBoxZEdge() const;
};

C3DVector CPhysicsMeshObject::GetBoundingBoxXEdge() const
{
    const C3DBoundingBox& box = Mesh->BoundingBox;
    return PositionMatrix.Rotate((box.Max.X - box.Min.X) * 0.5f, 0.0f, 0.0f);
}
