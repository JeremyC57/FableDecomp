// CBezierCurve::GetPointOnCurve(float t) const -- evaluate the cubic Bezier at t: Point1 (1-t)^3 + 3 Point2 t (1-t)^2 + 3 Point3 t^2 (1-t) + Point4 t^3
// Grade: DIFFER(358v390) -- x87 scheduling of the 3 Point2 t (1-t)^2 term: retail loads the (1-t) CSE before the Point2*3 products, keeps it on the stack, and spills (1-t)^2 into the dead t argument slot (memory-operand multiplies); ours loads (1-t) at its use and keeps (1-t)^2 on the x87 stack. This wave: PDB member names Point1..4 and the PDB operator shapes (`const C3DVector operator*(float) const`, `operator+(const C3DVector&) const`, user copy ctor) (=); named (1-t) local (362, worse). Earlier: operator*(const float&) (390v390, scalars hoisted), ungrouped (1-t)(1-t) (414), NRVO/ctor operators, by-ref factor. x87 operand/CSE placement family: park.
// Leaf: ?GetPointOnCurve@CBezierCurve@@QBE?AVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009122A0, length 390 bytes (`ret 8`: the hidden C3DVector result pointer and t).
// Callees: pow @ 0x00BFEB40 (MSVCR71, x2: t^3 and (1-t)^3 with the double 3.0 @ 0x012368F0); constants 3.0f @ 0x0122DED4
//          and 1.0f @ 0x0122DED8.
// Layout: CBezierCurve: four C3DVector control points P0 +0, P1 +0xc, P2 +0x18, P3 +0x24 (descriptive names).
// Shape: the four terms are summed left to right with the inline by-value C3DVector operators, so the right-most (P3) term is
//          evaluated first; the scalar factors t*t and (1-t)*(1-t) are parenthesised products reused across the three
//          components, (1 - t) is a common subexpression the compiler keeps in a frame slot, and the double pow results feed the
//          float multiplies directly.
extern "C" double __cdecl pow(double x, double y);     // 0x00BFEB40

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    const C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    const C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};

class CBezierCurve { public:
    C3DVector Point1;                                   // +0x00
    C3DVector Point2;                                   // +0x0c
    C3DVector Point3;                                   // +0x18
    C3DVector Point4;                                   // +0x24
    C3DVector GetPointOnCurve(float t) const;
};

C3DVector CBezierCurve::GetPointOnCurve(float t) const
{
    return Point1 * pow(1.0f - t, 3.0)
         + Point2 * 3.0f * t * ((1.0f - t) * (1.0f - t))
         + Point3 * 3.0f * (t * t) * (1.0f - t)
         + Point4 * pow(t, 3.0);
}
