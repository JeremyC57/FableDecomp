// GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b) -- |a.position - b.position|^2 (no square root)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBE583, length 61 bytes.
// Leaf: ?GetSquaredDistanceBetweenThings@@
// Grade: DIFFER(61v61) retry1 -- sum spellings canonicalised; residue: vptr eax/edx + square order
// Retail bytes: mov eax,[edx] ; push esi ; push edi ; mov edi,ecx ; mov ecx,edx ; call [eax+18h] ; mov esi,eax ; mov eax,[edi] ; mov ecx,edi ; call [eax+18h] ;
//               fld [eax] ; fsub [esi] ; pop edi ; fld [eax+4] ; fsub [esi+4] ; fld [eax+8] ; fsub [esi+8] ; pop esi ; fld st(0) ; fmul st(1) ; fld st(3) ; fmul st(4) ;
//               faddp ; fld st(2) ; fmul st(3) ; faddp ; fstp st(3) ; fstp st(0) ; fstp st(0) ; ret
// Callees: CScriptThing virtual slot 6 (+0x18) on each argument -- the thing's position (a `const C3DVector&`).
// The manifest label "CScriptBase::Activate" is propagated (CScriptBase::Activate takes no arguments, `QAEXXZ`). The
// script module's distance helpers sit together: 0x00CBE45C / 0x00CBE4B7 (thing-to-position over/under), 0x00CBE512 (the
// square-rooted distance between two things; the manifest gives it this body's name), 0x00CBE551 (thing to position,
// square-rooted) and this one, the squared distance between two things. `__fastcall`: a in ecx, b in edx, `ret`.
// Shape: b's position is fetched first (operator- evaluates its right operand first), the difference is built
// component by component in a named local (C3DVector has a memberwise user copy constructor, so the
// returned copy stays on the x87 stack) and its squared length leaves Z*Z, X*X, Y*Y on the x87 stack in that order.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}   // memberwise: the copy folds into x87 registers
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

inline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    C3DVector result;
    result.X = a.X - b.X;
    result.Y = a.Y - b.Y;
    result.Z = a.Z - b.Z;
    return result;
}

struct CScriptThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPosition() const;   // slot 6 (+0x18)
};

float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b)
{
    return (a.GetPosition() - b.GetPosition()).GetLengthSquared();
}
