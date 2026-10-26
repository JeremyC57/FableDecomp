// C3DVector::DotProduct(const C3DVector&) -- the scalar product of this vector with another
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C256A0, length 27 bytes. No callees.
// Leaf: DotProduct
// The manifest label "CQuadric::GetDeterminant" is a low-similarity BSim port (0.52) and is rejected: the real
// QuadricOptimiserInternals::CQuadric holds doubles, this body reads three floats from `this` (ecx) and from one
// pointer argument (`ret 4`) and returns their dot product on the x87 stack. C3DVector members X/Y/Z from the PDB.
// Shape: `fld [arg+8] ; fmul [this+8]` (b * a loads the right operand first), Z term first, then Y, then X:
// the sum is written Z + Y + X.
class C3DVector { public:
    float X, Y, Z;
    float DotProduct(const C3DVector& other) const;
};

float C3DVector::DotProduct(const C3DVector& other) const
{
    return Z * other.Z + Y * other.Y + X * other.X;
}