// C3DMeshPartitionElement::WriteToFile(CDataOutputStream&) -- write the box, sphere and triangle range, then the sub-element tree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC27A0, length 384 bytes.
// Callees: CDataOutputStream::WriteFloat @ 0x00993EE0, WriteLong @ 0x00993EB0, WriteBool @ 0x00993E30,
//          itself (recursion on SubElements[0]).
// Shape: the six box floats are written straight from the members; the sphere centre comes from three
//          by-value GetCenter() temporaries (12 bytes each at frame +0/+0xC/+0x18: only the component used
//          is stored, the 0x24-byte frame), then the radius and the two triangle-range longs. A present
//          first child writes `true`, recurses into SubElements[0] and tail-recurses into SubElements[1],
//          which VC7.1 turns into the rotated loop; a leaf writes `false`.
// Layout: PDB (struct_layouts_egor.tsv) C3DMeshPartitionElement, 0x38 bytes: BoundingBox +0x00 (6 floats),
//          StartTriangle +0x18, TriangleCount +0x1C, SubElements[2] +0x20, BoundingSphere +0x28 (Center, Radius).
//          No engine header exists for the class.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

struct C3DBoundingBox { C3DVector Min; C3DVector Max; };

struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    C3DVector GetCenter() const { return Center; }
    float GetRadius() const { return Radius; }
};

struct CDataOutputStream {
    void WriteBool(bool value);            // 0x00993E30
    void WriteLong(long value);            // 0x00993EB0
    void WriteFloat(float value);          // 0x00993EE0
};

struct C3DMeshPartitionElement {
    C3DBoundingBox           BoundingBox;      // +0x00
    long                     StartTriangle;    // +0x18
    long                     TriangleCount;    // +0x1C
    C3DMeshPartitionElement* SubElements[2];   // +0x20
    C3DBoundingSphere        BoundingSphere;   // +0x28
    void WriteToFile(CDataOutputStream& out) const;
};

void C3DMeshPartitionElement::WriteToFile(CDataOutputStream& out) const
{
    out.WriteFloat(BoundingBox.Min.X);
    out.WriteFloat(BoundingBox.Min.Y);
    out.WriteFloat(BoundingBox.Min.Z);
    out.WriteFloat(BoundingBox.Max.X);
    out.WriteFloat(BoundingBox.Max.Y);
    out.WriteFloat(BoundingBox.Max.Z);
    out.WriteFloat(BoundingSphere.GetCenter().X);
    out.WriteFloat(BoundingSphere.GetCenter().Y);
    out.WriteFloat(BoundingSphere.GetCenter().Z);
    out.WriteFloat(BoundingSphere.GetRadius());
    out.WriteLong(StartTriangle);
    out.WriteLong(TriangleCount);
    if (SubElements[0]) {
        out.WriteBool(true);
        SubElements[0]->WriteToFile(out);
        SubElements[1]->WriteToFile(out);
    }
    else
        out.WriteBool(false);
}