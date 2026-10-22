// Grade: DIFFER(892v890) -- the inlined CMatrix3x4::SetInverseNoTranslate keeps a11 and 1/det differently on the x87 stack (fld st(0); fmul st(2) vs fxch; fmul st(1), one extra fstp st(0)) and its cofactor fmul operand order is VC7.1's canonical mirror of retail; the length tail pops the deltas with fstp st(0) x3 where retail does fstp st(3) after fsqrt; the sleep path, both interpolations, Transform.Set and the camera call match
// NParticleEngine::CParticleEmitter::StartFrame(float time, const CEnginePrimitivePrepareInfo* info) -- per-frame emitter setup: nothing while sleeping; an emitter waiting to sleep with no particles and no decals left clears its primitives and goes to sleep; otherwise interpolate the position and orientation for the frame, rebuild Transform and its rotation-only inverse, and work out the distance to the particle camera (0 for 2D emitters)
// Leaf: StartFrame
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEEBA0, 890 bytes (+ int3 padding), `ret 8`.
// Callees: CParticleEmitter::GetNoParticles 0x00AEDCA0 (manifest "GetNoDecals" is propagated) / GetNoDecals 0x00AEDCD0,
//   ClearAllPrimitives(CIEngine*) 0x00AEE940, CRightHandedSet::Interpolate(const CRightHandedSet&, float) 0x00A23190,
//   the particle camera getter 0x00AEAB10 (global 0x013D2E2C) and its virtual GetPosition slot 6 (+0x18). 1.0f 0x0122DED8.
// Layout: Ego_r.pdb CParticleEmitter minus 0x0c (landed constructor 0x00AF0380): flag word +0x40 (WaitingToSleep bit 9,
//   Sleeping bit 10, Emitter2D bit 11), OrientationInterpolator +0x60 (Start +0x64, End +0x7c), PositionInterpolator +0x94
//   (Start +0x98, End +0xa4), Transform +0xb0, InverseTransform +0xe0, DistanceToCam +0x154. Camera model from the landed
//   CParticleEmitter::ShouldSpawn 0x00AECF90.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator^(const C3DVector& v) const { return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
struct CRightHandedSet {
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
    CRightHandedSet Interpolate(const CRightHandedSet& end, float t) const;   // 0x00A23190
};
struct C3DOrientationInterpolator {
    void*           __vftable;                     // +0x00
    CRightHandedSet StartOrientation;              // +0x04
    CRightHandedSet EndOrientation;                // +0x1c
    __forceinline CRightHandedSet Interpolate(float t) const { return StartOrientation.Interpolate(EndOrientation, t); }
};
struct C3DPositionInterpolator {
    void*     __vftable;                           // +0x00
    C3DVector StartPosition;                       // +0x04
    C3DVector EndPosition;                         // +0x10
    __forceinline C3DVector Interpolate(float t) const { return StartPosition + (EndPosition - StartPosition) * t; }
};
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    __forceinline void Set(const C3DVector& right, const C3DVector& forward, const C3DVector& up, const C3DVector& pos)
    {
        E11 = right.X; E12 = right.Y; E13 = right.Z;
        E21 = forward.X; E22 = forward.Y; E23 = forward.Z;
        E31 = up.X; E32 = up.Y; E33 = up.Z;
        E41 = pos.X; E42 = pos.Y; E43 = pos.Z;
    }
    __forceinline void Set(const CRightHandedSet& o, const C3DVector& pos) { Set(o.Forward ^ o.Up, o.Forward, o.Up, pos); }
    __forceinline void SetInverseNoTranslate(const CMatrix3x4& m)
    {
        float a11 = m.E22 * m.E33 - m.E32 * m.E23;
        float det = (m.E32 * m.E13 - m.E33 * m.E12) * m.E21 + (m.E23 * m.E12 - m.E22 * m.E13) * m.E31 + a11 * m.E11;
        float inv = 1.0f / det;
        E11 = a11 * inv;
        E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
        E31 = (m.E32 * m.E21 - m.E22 * m.E31) * inv;
        E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
        E22 = (m.E11 * m.E33 - m.E13 * m.E31) * inv;
        E32 = (m.E31 * m.E12 - m.E11 * m.E32) * inv;
        E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
        E23 = (m.E21 * m.E13 - m.E11 * m.E23) * inv;
        E33 = (m.E11 * m.E22 - m.E21 * m.E12) * inv;
        E41 = 0;
        E42 = 0;
        E43 = 0;
    }
};
struct CParticleCamera {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual C3DVector GetPosition() const;         // slot 6 (+0x18), hidden result
};
CParticleCamera* GetParticleCamera();              // 0x00AEAB10
struct CIEngine;
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CIEngine*     Engine;                          // +0x34
};
namespace NParticleEngine {
class CParticleEmitter {
public:
    void*                      __vftable;              // +0x00
    unsigned char              _state04[0x3c];
    unsigned short Priority : 3;                       // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    unsigned short Emitter2DLayer : 7;                 // +0x42
    unsigned char              _state44[0x1c];
    C3DOrientationInterpolator OrientationInterpolator;   // +0x60
    C3DPositionInterpolator    PositionInterpolator;      // +0x94
    CMatrix3x4                 Transform;                 // +0xb0
    CMatrix3x4                 InverseTransform;          // +0xe0
    CMatrix3x4                 EndTransform;              // +0x110
    void*                      Name;                      // +0x140
    float                      BSphere[4];                // +0x144
    float                      DistanceToCam;             // +0x154
    bool IsSleeping() const { return Sleeping; }
    bool IsWaitingToSleep() const { return WaitingToSleep; }
    bool IsEmitter2D() const { return Emitter2D; }
    long GetNoParticles();                         // 0x00AEDCA0
    long GetNoDecals();                            // 0x00AEDCD0
    void ClearAllPrimitives(CIEngine* engine);     // 0x00AEE940
    void StartFrame(float time, const CEnginePrimitivePrepareInfo* info);
};

void CParticleEmitter::StartFrame(float time, const CEnginePrimitivePrepareInfo* info)
{
    if (IsSleeping())
        return;
    if (IsWaitingToSleep() && GetNoParticles() == 0 && GetNoDecals() == 0) {
        ClearAllPrimitives(info->Engine);
        Sleeping = true;
        return;
    }
    C3DVector pos = PositionInterpolator.Interpolate(time);
    CRightHandedSet orientation = OrientationInterpolator.Interpolate(time);
    Transform.Set(orientation, pos);
    InverseTransform.SetInverseNoTranslate(Transform);
    if (IsEmitter2D())
        DistanceToCam = 0.0f;
    else {
        C3DVector camera = GetParticleCamera()->GetPosition();
        DistanceToCam = (camera - pos).GetLength();
    }
}
}
