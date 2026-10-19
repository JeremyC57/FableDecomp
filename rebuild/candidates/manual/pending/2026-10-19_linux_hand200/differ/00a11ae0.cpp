// Grade: DIFFER(ours v retail) -- 168v185: retail keeps a 0x20 frame with an extra copy of the halved centre (X through `fstp [esp+8] ; mov eax,[esp+8] ; mov [esp],eax`, Z spilled to [esp+10h]) before the transform; ours folds it into a 0x18 frame -- tried a named centre local and a by-value Transform argument (outlined the helper)
// CPhysicsMeshObject::GetBoundingBoxCentre() const -- the centre of the mesh's bounding box transformed into the object's frame (rotation rows plus position)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A11AE0, 185 bytes (+ int3 padding), `ret 4` (C3DVector by hidden pointer).
//   Constant 0.5f @ 0x0122F59C.
// Callees: none.
// Shape: the sibling of the landed GetBoundingBoxZEdge 0x00A11BA0 / GetCentreOfGravity 0x00A11EC0 (same CMatrix3x4 model,
//   memberwise C3DVector copy constructor, the box reached through a reference: `add eax,84h`). The centre is
//   the box's inline GetCentre() `(Min + Max) * 0.5f` (called on the box it loads Min first, as retail); the transform is the inline CMatrix3x4::Transform
//   (Rotate's three sums plus the position row), whose named result folds into the three x87 sums stored last.
//   Layout (PDB): Mesh +4, PositionMatrix +8 (rows, then Position +0x2C); the mesh's box at retail +0x84.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct C3DBoundingBox {
    C3DVector Min;
    C3DVector Max;
    C3DVector GetCentre() const { return (Min + Max) * 0.5f; }
};
struct CMatrix3x4 {
    C3DVector Row[3];                              // the rotation rows
    C3DVector Position;
    C3DVector Transform(const C3DVector& v) const
    {
        C3DVector r;
        r.X = v.X * Row[0].X + v.Y * Row[1].X + v.Z * Row[2].X + Position.X;
        r.Y = v.X * Row[0].Y + v.Y * Row[1].Y + v.Z * Row[2].Y + Position.Y;
        r.Z = v.X * Row[0].Z + v.Y * Row[1].Z + v.Z * Row[2].Z + Position.Z;
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
    C3DVector GetBoundingBoxCentre() const;
};
C3DVector CPhysicsMeshObject::GetBoundingBoxCentre() const
{
    const C3DBoundingBox& box = Mesh->BoundingBox;
    return PositionMatrix.Transform(box.GetCentre());
}
