// C3DVector::CrossProduct(const C3DVector& other) const -- returns this x other through the hidden result pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C25860, length 57 bytes.
// Leaf: ?CrossProduct@C3DVector
// Grade: DIFFER(57v57) -- one operand order: the Y component's second product is retail `fld [ecx] ; fmul [eax+8]` (X loaded, other.Z multiplied), the lane emits `fld [eax+8] ; fmul [ecx]` for both `X * other.Z` and `other.Z * X`. Tried: both operand orders, an inline Cross(a, b) helper, a field-by-field named result.
// Retail bytes: mov eax,[esp+8] ; fld [ecx] ; fmul [eax+4] ; fld [ecx+4] ; fmul [eax] ; fsubp ; (Y) ; (X) ; mov eax,[esp+4] ; fsubp ; fstp [eax] ; fstp [eax+4] ; fstp [eax+8] ; ret 8
// The manifest label "CRightHandedSet::GetRight" is propagated: the body reads `this` as a bare C3DVector and
// takes one more vector by reference (`ret 8` = hidden result + argument), so it is the vector cross product.
// Shape: the result is a C3DVector with a user constructor (hidden-pointer return); the constructor's three
// arguments are evaluated right to left (Z first) and stay on the x87 stack until the three stores. The lane canonicalises the
// operand order of each product (both spellings give the same code).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector CrossProduct(const C3DVector& other) const;
};

C3DVector C3DVector::CrossProduct(const C3DVector& other) const
{
    return C3DVector(Y * other.Z - Z * other.Y,
                     Z * other.X - other.Z * X,
                     X * other.Y - Y * other.X);
}
