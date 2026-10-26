// C4DVector::Interpolate(const C4DVector& to, float t) -- linear interpolation between this vector and `to`: this + (to - this) * t, returned by value
// Leaf: Interpolate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5B7D0, 117 bytes (`ret 0Ch`: hidden result, `to`, t). Catalog: "CTCPhysicsStandard::
//   GetFacingVector" (propagated, wrong): `this` is read as four floats, the body sits in the vector-math TU next to
//   GFInitVectorMath 0x00A5B850 / GFInterpolateVectors 0x00A5B870, and FableWin.pdb names
//   `?Interpolate@C4DVector@@QBE?BV1@ABV1@M@Z` (const C4DVector C4DVector::Interpolate(const C4DVector&, float) const).
// Callees: none.
// Layout: C4DVector X +0, Y +4, Z +8, W +0xc.
// Shape: the inline member operators (FableWin.pdb: operator-, operator*(float), operator+, all `const C4DVector` by value)
//   build each result through the four-float ctor; the difference and the scaled vector are frame temporaries, the final sum
//   evaluates its ctor arguments right to left (W first) and stores X..W.
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    const C4DVector operator-(const C4DVector& o) const { return C4DVector(X - o.X, Y - o.Y, Z - o.Z, W - o.W); }
    const C4DVector operator+(const C4DVector& o) const { return C4DVector(X + o.X, Y + o.Y, Z + o.Z, W + o.W); }
    const C4DVector operator*(float s) const { return C4DVector(X * s, Y * s, Z * s, W * s); }
    const C4DVector Interpolate(const C4DVector& to, float t) const;
};

const C4DVector C4DVector::Interpolate(const C4DVector& to, float t) const
{
    return (to - *this) * t + *this;
}