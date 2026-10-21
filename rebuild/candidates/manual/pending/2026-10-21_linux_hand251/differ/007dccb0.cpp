// Grade: DIFFER(390v390) -- scheduling only: the same CRightHandedSet tail as 0x007DE030 (Up.X fstp and the Forward.Y int copy swapped); 288-combination search over copy-ctor / set-ctor / cross spellings and the QFE compiler all leave it
// CTCCreatureFlyOnSpline::FrameUpdate() -- advance along the flight spline: step the progress (wrapping to its fractional part when looping, then clamping to [0, 1]), move the creature to the new spline position and, when it moved noticeably, orient it along the normalised travel direction (up = (dir.Y, -dir.X, 0) x dir)
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DCCB0, 400 bytes (`ret`).
// Callees: __ftol2 @ 0x00BFEA70, CCardinalSpline::GetSplinePosition(C3DVector*, float) @ 0x00A567A0 (twice), the physics
// component's virtual slots 30 SetPosition (+0x78) and 75 SetRHSet (+0x12c, {up, forward}), C3DVector::Normalise() @ 0x00A14440.
// Globals: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, the epsilon 1e-4f @ 0x0129BA3C (variable, squared).
// Layout: as the landed SetSpline 0x007DCFD0 (PSpline +0x0c, Progress +0x20, DeltaProgress +0x24, Looping +0x28; the thing's
// physics component at +0x60).
// Shape: the spline pointer's object is tested and cached for the first position call; the previous progress is a named local
// that the looping path overwrites with the wrapped progress (sharing the __ftol2 integer's slot); the physics component is a
// local taken before the orientation is built (the CTCCreatureFlyInCircles::FrameUpdate 0x007DE030 orientation code).
extern float g_epsilon;                                 // 0x0129BA3C (1e-4f)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    void Normalise();                                   // 0x00A14440
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
    CRightHandedSet(const CRightHandedSet& o) : Up(o.Up), Forward(o.Forward) { }
};
struct CTCPhysicsBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2)
    virtual void SetPosition(const C3DVector& pos);     // slot 30 (+0x78)
    VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39)
    VF10(4) VF10(5) VF10(6)
    VF(70) VF(71) VF(72) VF(73) VF(74)
    virtual void SetRHSet(const CRightHandedSet& set);  // slot 75 (+0x12c)
#undef VF10
#undef VF
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
};
struct CCardinalSpline {
    void GetSplinePosition(C3DVector* position, float progress);    // 0x00A567A0
};
struct SCountBlock;
template <class T> struct CCountedPointer {
    T* Data;                                            // +0
    SCountBlock* m_pCount;                              // +4
    T* operator->() const { return Data; }
};
struct CTCCreatureFlyOnSpline {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CCountedPointer<CCardinalSpline> PSpline;           // +0x0c
    void* EngineHandles[3];                             // +0x14 vector<CEnginePrimitiveHandle>
    float Progress;                                     // +0x20
    float DeltaProgress;                                // +0x24
    bool  Looping;                                      // +0x28
    void FrameUpdate();
};

void CTCCreatureFlyOnSpline::FrameUpdate()
{
    if (!PSpline.Data)
        return;
    float lastProgress = Progress;
    Progress += DeltaProgress;
    if (Looping) {
        Progress -= (long)Progress;
        lastProgress = Progress;
    }
    if (Progress < 0.0f)
        Progress = 0.0f;
    else if (Progress > 1.0f)
        Progress = 1.0f;
    C3DVector lastPos;
    PSpline->GetSplinePosition(&lastPos, lastProgress);
    C3DVector pos;
    PSpline->GetSplinePosition(&pos, Progress);
    ParentThing->PhysicsTC->SetPosition(pos);
    C3DVector dir = pos - lastPos;
    if (dir.GetLengthSquared() > g_epsilon * g_epsilon) {
        dir.Normalise();
        CTCPhysicsBase* physics = ParentThing->PhysicsTC;
        CRightHandedSet set(CrossProduct(C3DVector(dir.Y, -dir.X, 0.0f), dir), dir);
        physics->SetRHSet(set);
    }
}
