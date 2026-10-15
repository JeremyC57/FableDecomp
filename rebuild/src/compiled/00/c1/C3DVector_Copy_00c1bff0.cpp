// C3DVector::Copy() const -- returns a copy of the vector (x, y, z) through the hidden result pointer, assembled field by field in a local
// Leaf: Copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BFF0, 53 bytes (+11 int3 padding). No callees.
//
// The manifest label `CPhysicalPrimitiveMesh::GetClosestPointToPos2D` is propagated and wrong:
// `this` is a 12-byte vector (three floats at +0/+4/+8), the result goes through the hidden pointer
// (`mov eax,[esp+10h]` after the 12-byte frame, `ret 4`, eax returned). Callers in the render module
// (0x00C1C5B2) use it beside the Y/Z swizzle 0x00C1C070 (identical body with +4/+8 exchanged).
// Shape: the vector is a POD (no user constructors). The copy is assembled field by field
// in a NAMED local (frame slots x/y/z) and that local is copied out to the hidden result
// (`mov ecx,eax` then three load/store pairs): a POD return copies through the frame. A user
// (memberwise) copy constructor instead folds everything into three x87 stores straight into the
// result (25 bytes), and dword members with constructors elide the frame.
struct C3DVector {
    float x;
    float y;
    float z;
    C3DVector Copy() const;
};

C3DVector C3DVector::Copy() const
{
    C3DVector result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}