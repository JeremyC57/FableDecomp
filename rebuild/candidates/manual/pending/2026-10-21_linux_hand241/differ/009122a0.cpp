// CBezierCurve::GetPointOnCurve(float t) -- evaluate the cubic Bezier at t: P0 (1-t)^3 + 3 P1 t (1-t)^2 + 3 P2 t^2 (1-t) + P3 t^3
// Grade: DIFFER(ours v retail) -- 358v390: the 3 P1 t (1-t)^2 term: retail reloads (1-t) before the P1*3 products and spills (1-t)^2 into the dead t argument slot (memory-operand multiplies); ours keeps it on the x87 stack; 4 forms tried (NRVO operators, ctor operators, named (1-t) local, by-reference scalar factor)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009122A0, length 390 bytes (`ret 8`: the hidden C3DVector result pointer and t).
// Callees: pow @ 0x00BFEB40 (MSVCR71, x2: t^3 and (1-t)^3 with the double 3.0 @ 0x012368F0); constants 3.0f @ 0x0122DED4
//          and 1.0f @ 0x0122DED8.
// Layout: CBezierCurve: four C3DVector control points P0 +0, P1 +0xc, P2 +0x18, P3 +0x24 (descriptive names).
// Shape: the four terms are summed left to right with the inline by-value C3DVector operators, so the right-most (P3) term is
//          evaluated first; the scalar factors t*t and (1-t)*(1-t) are parenthesised products reused across the three
//          components, (1 - t) is a common subexpression the compiler keeps in a frame slot, and the double pow results feed the
//          float multiplies directly.
extern "C" double __cdecl pow(double x, double y);     // 0x00BFEB40

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};

struct CBezierCurve {
    C3DVector P0;                                       // +0x00
    C3DVector P1;                                       // +0x0c
    C3DVector P2;                                       // +0x18
    C3DVector P3;                                       // +0x24
    C3DVector GetPointOnCurve(float t) const;
};

C3DVector CBezierCurve::GetPointOnCurve(float t) const
{
    return P0 * pow(1.0f - t, 3.0)
         + P1 * 3.0f * t * ((1.0f - t) * (1.0f - t))
         + P2 * 3.0f * (t * t) * (1.0f - t)
         + P3 * pow(t, 3.0);
}
