// NParticleEngine::CPSCSpline::WriteBinary(CDataOutputStream*) -- writes the bounce and speed-scaling flags and the animation speed, then the CPSCSplineBase data
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1AFF0, length 58 bytes.
// Callees: CDataOutputStream::WriteBool @ 0x00993E30 (x2), CDataOutputStream::WriteFloat @ 0x00993EE0,
// NParticleEngine::CPSCSplineBase::WriteBinary @ 0x00B21630.
// Identity: slot 16 of NParticleEngine::CPSCSpline's vtable 0x012A0934 (rtti-vtables.tsv).
// Layout: Ego_r.pdb CPSCSpline (SplineCurrentPosition, ReverseSpline, SplineBounce, ScaleSplineSpeed,
// SplineAnimSpeed) sits 4 bytes lower in retail because the base's CCardinalSpline holds a 12-byte
// CArray: +0x34, +0x38, +0x39, +0x3a, +0x3c (the landed constructor 0x00B1B350 clears the same five).
// Shape: the bool members widen with `xor reg,reg ; mov reg8` into the argument; the float is pushed
// as a dword; the base write keeps `this` in ecx.
struct CBase {
    virtual ~CBase();
};

struct CDataOutputStream {
    void WriteBool(bool value);             // 0x00993E30
    void WriteFloat(float value);           // 0x00993EE0
};

namespace NParticleEngine {
struct CPSCSplineBase : CBase {
    unsigned char _body[0x30];              // +0x04 .. +0x34 (CPSCBase part, SplineAnimSpeed, Spline, the two counted data pointers)
    void WriteBinary(CDataOutputStream* out);   // 0x00B21630
};

struct CPSCSpline : CPSCSplineBase {        // vtable 0x012A0934
    float SplineCurrentPosition;            // +0x34
    bool  ReverseSpline;                    // +0x38
    bool  SplineBounce;                     // +0x39
    bool  ScaleSplineSpeed;                 // +0x3a
    float SplineAnimSpeed;                  // +0x3c
    void WriteBinary(CDataOutputStream* out);
};

void CPSCSpline::WriteBinary(CDataOutputStream* out)
{
    out->WriteBool(SplineBounce);
    out->WriteBool(ScaleSplineSpeed);
    out->WriteFloat(SplineAnimSpeed);
    CPSCSplineBase::WriteBinary(out);
}
}