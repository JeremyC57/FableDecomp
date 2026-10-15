// CRightHandedSet::GetRight() const -- the right vector of the frame: the cross product Forward x Up, returned by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049CE00, length 55 bytes.
// Retail bytes:  fld [ecx+0Ch] ; mov eax,[esp+4] ; fmul [ecx+4] ; fld [ecx+10h] ; fmul [ecx] ; fsubp st(1),st ; fld [ecx+14h] ;
//                fmul [ecx] ; fld [ecx+8] ; fmul [ecx+0Ch] ; fsubp st(1),st ; fld [ecx+10h] ; fmul [ecx+8] ; fld [ecx+14h] ;
//                fmul [ecx+4] ; fsubp st(1),st ; fstp [eax] ; fstp [eax+4] ; fstp [eax+8] ; ret 4
// PDB layout (struct_layouts_egor.tsv, CRightHandedSet = 24 bytes): +0 C3DVector Up, +0xC C3DVector Forward.
// Result: x = Up.Z*Forward.Y - Up.Y*Forward.Z, y = Up.X*Forward.Z - Up.Z*Forward.X, z = Up.Y*Forward.X - Up.X*Forward.Y.
// Shape: an inline CrossProduct(a, b) helper called as CrossProduct(Forward, Up) builds the result with C3DVector's (x, y, z)
// constructor straight into the hidden return slot; its arguments are evaluated right to left (z first), so the three
// components sit on the x87 stack and are stored x, y, z. The x87 load order of each product follows the helper's
// reference parameters: the same products written directly on the members load Up.Y before Forward.X in the first
// product (VC7.1 does not keep the source operand order of a `*` there).
struct C3DVector {
    float X;                                 // +0x00
    float Y;                                 // +0x04
    float Z;                                 // +0x08
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
};

struct CRightHandedSet {
    C3DVector Up;                            // +0x00
    C3DVector Forward;                       // +0x0C
    C3DVector GetRight() const;
};

inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y,
                     a.Z * b.X - a.X * b.Z,
                     a.X * b.Y - a.Y * b.X);
}

C3DVector CRightHandedSet::GetRight() const
{
    return CrossProduct(Forward, Up);
}
