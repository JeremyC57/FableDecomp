// CAngleInterpolator::SetTarget(float target, float targetVelocity, float duration) -- (manifest label "CMatrix::UpdateInversePerspective" is propagated) start a smooth move of an angle-like value: a duration under 0.001 snaps to the target; otherwise the target is wrapped into (-pi, pi] when wrapping is on (the current value is shifted by 2pi to stay within pi of it), the start value/velocity and duration are recorded and the polynomial coefficients are solved from the inverse of the [t^4/24 t^3/6 t^2/2; t^3/6 t^2/2 t; t^2/2 t 1] system
// Grade: DIFFER(417v435) -- the right-hand-side vector: retail keeps the `* 0.0f` terms of the inverse * (r0, r1, 0) product (`fmul [0x0122DEDC]`), homes r0/r1 in the dead target/duration argument slots and copies the result's Z through a frame word; ours folds the zero terms away. Also b/Velocity load order in the bookkeeping stores; prologue, wrap logic and the system matrix match
// Leaf: c0b570
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0B570, length 435 bytes (`ret 0Ch`); 0x34-byte elements of an owner array
//   (callers 0x00C0B3CD etc. pass `this` = owner + 4 + i*0x34).
// Callees: the snap 0x00C0B510 (thiscall, one float), the angle wrap 0x00C0B4C0 (__stdcall float(float)), the 3x4 inverse
//   0x00C1D1C0 (__fastcall out, in; manifest label C3DMeshObject::SetInversePositionMatrixFromPositionMatrix is propagated).
// Layout (descriptive, from this body and the snap 0x00C0B510): Current +0, Target +4, TargetVelocity +8, Velocity +0xc, Time +0x14,
//   Duration +0x18, StartValue +0x1c, StartVelocity +0x20, coefficients +0x24/+0x28/+0x2c, Wrap +0x30 (bool).
// Constants: 0.001f, pi, 2pi, -pi, 0.5f, 1/3f, 1/6f, 0.0f.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CMatrix3x4 {
    C3DVector Rows[4];
    CMatrix3x4() {}
    CMatrix3x4(const C3DVector& a, const C3DVector& b, const C3DVector& c, const C3DVector& d) { Rows[0] = a; Rows[1] = b; Rows[2] = c; Rows[3] = d; }
    C3DVector operator*(const C3DVector& v) const
    {
        return C3DVector(v.X * Rows[0].X + v.Y * Rows[1].X + v.Z * Rows[2].X + Rows[3].X,
                         v.X * Rows[0].Y + v.Y * Rows[1].Y + v.Z * Rows[2].Y + Rows[3].Y,
                         v.X * Rows[0].Z + v.Y * Rows[1].Z + v.Z * Rows[2].Z + Rows[3].Z);
    }
};
void __fastcall InvertMatrix(CMatrix3x4* out, const CMatrix3x4* in);   // 0x00C1D1C0
float __stdcall WrapAngle(float angle);                               // 0x00C0B4C0
struct CAngleInterpolator {
    float Current;                                 // +0x00
    float Target;                                  // +0x04
    float TargetVelocity;                          // +0x08
    float Velocity;                                // +0x0c
    float _10;                                     // +0x10
    float Time;                                    // +0x14
    float Duration;                                // +0x18
    float StartValue;                              // +0x1c
    float StartVelocity;                           // +0x20
    float C2;                                      // +0x24
    float C1;                                      // +0x28
    float C0;                                      // +0x2c
    bool  Wrap;                                    // +0x30
    void Snap(float target);                       // 0x00C0B510
    void SetTarget(float target, float targetVelocity, float duration);
};

void CAngleInterpolator::SetTarget(float target, float targetVelocity, float duration)
{
    if (duration < 0.001f) {
        Snap(target);
        return;
    }
    float angle;
    if (Wrap) {
        angle = WrapAngle(target);
        float diff = angle - Current;
        if (diff >= 3.1415927f)
            Current += 6.2831855f;
        else if (diff < -3.1415927f)
            Current -= 6.2831855f;
    } else
        angle = target;
    Target = angle;
    StartValue = Current;
    TargetVelocity = targetVelocity;
    StartVelocity = Velocity;
    Duration = duration;
    Time = 0.0f;
    float t2 = duration * duration * 0.5f;
    float t3 = duration * t2 * (1.0f / 3.0f);
    float t4 = t2 * t2 * (1.0f / 6.0f);
    CMatrix3x4 system;
    system.Rows[0].X = t4;
    system.Rows[0].Y = t3;
    system.Rows[0].Z = t2;
    system.Rows[1].X = t3;
    system.Rows[1].Y = t2;
    system.Rows[1].Z = duration;
    system.Rows[2].X = t2;
    system.Rows[2].Y = duration;
    system.Rows[2].Z = 1.0f;
    system.Rows[3].Z = 0.0f;
    system.Rows[3].Y = 0.0f;
    system.Rows[3].X = 0.0f;
    CMatrix3x4 inverse;
    InvertMatrix(&inverse, &system);
    C3DVector rhs(Target - StartValue - StartVelocity * Duration, TargetVelocity - StartVelocity, 0.0f);
    C3DVector coefficients = inverse * rhs;
    C2 = coefficients.Z;
    C1 = coefficients.Y;
    C0 = coefficients.X;
}
