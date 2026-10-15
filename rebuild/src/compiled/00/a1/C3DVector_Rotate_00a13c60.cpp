// C3DVector::Rotate(const C3DVector& axis, float angle) -- rotates this vector in place by `angle` about `axis` (builds the rotation matrix, then multiplies the old components through its axes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A13C60, length 120 bytes.
// Leaf: Rotate
// Callees: 0x00A55F90 CMatrix3x4::CMatrix3x4(const C3DVector& axis, float angle) (the axis-angle rotation; manifest
// label "Matrix_RotationAroundAxis"), building the 48-byte matrix in the frame.
// Shape: the old components are taken into a copy first (VC keeps them on the x87 stack: three `fld` up front,
// popped at the end); each new component is `x * XAxis + y * YAxis + z * ZAxis` of the matrix's column, the copied
// component on the LEFT (the matrix-first form gives `fld st(n) ; fmul [m]` for the y and x products; VC7.1
// emits the z product first, as in 0x00A35F80) stored straight into this vector.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
    void Rotate(const C3DVector& axis, float angle);
};
struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4(const C3DVector& axis, float angle);      // 0x00A55F90
};

void C3DVector::Rotate(const C3DVector& axis, float angle)
{
    CMatrix3x4 rotation(axis, angle);
    C3DVector v(*this);
    X = v.X * rotation.XAxis.X + v.Y * rotation.YAxis.X + v.Z * rotation.ZAxis.X;
    Y = v.X * rotation.XAxis.Y + v.Y * rotation.YAxis.Y + v.Z * rotation.ZAxis.Y;
    Z = v.X * rotation.XAxis.Z + v.Y * rotation.YAxis.Z + v.Z * rotation.ZAxis.Z;
}