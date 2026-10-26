// C3DVector::SwapYZ() const -- returns (x, z, y): the vector with its Y and Z components exchanged (Y-up <-> Z-up conversion)
// Leaf: SwapYZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C070, 53 bytes (+11 int3 padding). No callees.
//
// The manifest label `CPhysicalPrimitiveMesh::GetClosestPointToPos2D` is propagated and wrong:
// `this` is a 12-byte vector (three floats at +0/+4/+8), the result goes through the hidden pointer
// (`mov eax,[esp+10h]` after the 12-byte frame, `ret 4`, eax returned). Callers in the render module
// (0x00C1C29D, 0x00C1C2BF, 0x00C1C2E1, 0x00C2744C, 0x00C27591) convert several vectors in a row.
// Shape: the vector is a POD (no user constructors). The swizzled copy is assembled field by field
// in a NAMED local (frame slots x/z/y) and that local is copied out to the hidden result
// (`mov ecx,eax` then three load/store pairs): a POD return copies through the frame. A user
// (memberwise) copy constructor instead folds everything into three x87 stores straight into the
// result (25 bytes), and dword members with constructors elide the frame.
class C3DVector { public:
    float x;
    float y;
    float z;
    C3DVector SwapYZ() const;
};

C3DVector C3DVector::SwapYZ() const
{
    C3DVector result;
    result.x = x;
    result.y = z;
    result.z = y;
    return result;
}