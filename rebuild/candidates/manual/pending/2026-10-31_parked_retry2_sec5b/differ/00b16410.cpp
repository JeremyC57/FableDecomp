// NParticleEngine::CPSCOrbit::COrbitState::Update(const COrbitSettings&, CMatrix3x4&, float) -- advance the orbit angle by the per-tick rotate speed, shift the from/to/next position ring, compute the next orbit offset (squeezed ellipse, optional cycling/expanding radius) in the plane picked by the orbit type, transform or translate it, pre-rotate the matrix about the plane normal and take the half-difference tangent
// Grade: DIFFER(1674v1671) 124/511 -- residue is register colouring (retail keeps settings in ecx, this in esi, ebp=&FromPosition, ebx=0, edi=0x7fffffff for GFFloatAbs) and frame-slot packing (retail homes radius in the dead settings-argument slot [esp+40h], x/y at [esp+24h]/[esp+28h]) plus the translation temp shape (retail fld X, fld Y, Z integer-copied to [esp+2Ch]); every helper call site and constant matches
// Leaf: ?Update@COrbitState@CPSCOrbit@NParticleEngine@@QAEXABVCOrbitSettings@23@AAVCMatrix3x4@@M@Z
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00B164AA `fld dword ptr [esp+1Ch]` + 0x00B164AE `fistp dword ptr [esp+10h]` -- the engine's inline-assembly float-to-long helper (landed FloatToLong) inside the inlined GFFastCos/GFFastSin table lookups; VC7.1 `(long)x` calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B16410, 1671 bytes, `ret 0Ch` (extent from the retail bytes, BOUNDS_FIX.tsv).
// Name: Ego_r.pdb ?Update@COrbitState@CPSCOrbit@NParticleEngine@@QAEXABVCOrbitSettings@23@AAVCMatrix3x4@@M@Z + caller CPSCOrbit::Update; GetOneOverTickRate Ego_r/FableWin ?GetOneOverTickRate@NParticleEngine@@YIMXZ (0x00AEAA00, `fld [0x01399508]`).
// Layout: COrbitSettings Type:2 / bit 2 (LocalSpace: name unproven) / Enabled +4 / Radius +8 / Expand +0xc / CycleFlag +0x10 / CycleTime +0x14 / SqueezeScale +0x18 / SqueezeAngle +0x1c / RotateSpeed +0x20; COrbitState NextAngle, RotateSpeedRandomValue, From/To/NextPosition +8/+0x14/+0x20, From/ToTangent +0x2c/+0x38.
// Levers: GFFloatAbs integer-and form (517->500); __forceinline helpers (334); direct component writes (313); switch value cached in a local + rotation `t = s*a.X` form (191); `+= transform.Position` (127); GetTranslation by-value ctor (124). Inert: operator+= by value (191), explicit temp (196).
#include <math.h>

extern float g_CosTable[1025];          // 0x013CD550: cos(2*pi*k/1024), k = 0..1024
extern const float g_epsilon_0129ba3c;  // 0x0129BA3C (1e-4f)

static __forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static __forceinline float __fastcall GFFastCos(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i &= 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}
static __forceinline float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

inline float __fastcall GFFloatAbs(float f)       // FableWin.pdb ?GFFloatAbs@@YIMM@Z (inline)
{
    *(unsigned long*)&f &= 0x7fffffff;
    return f;
}

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};
class CMatrix3x4 { public:
    C3DVector Right, Up, Forward, Position;
    C3DVector GetTranslation() const { return C3DVector(Position.X, Position.Y, Position.Z); }
    __forceinline void TransformPoint(C3DVector& v) const
    {
        float x = v.X, y = v.Y, z = v.Z;
        v.X = Right.X * x + Up.X * y + Forward.X * z + Position.X;
        v.Y = Right.Y * x + Up.Y * y + Forward.Y * z + Position.Y;
        v.Z = Right.Z * x + Up.Z * y + Forward.Z * z + Position.Z;
    }
    __forceinline void PreXRotate(float angle)
    {
        float c = (float)cos(angle), s = (float)sin(angle);
        float t;
        t = s * Up.X; Up.X = c * Up.X - s * Forward.X; Forward.X = c * Forward.X + t;
        t = s * Up.Y; Up.Y = c * Up.Y - s * Forward.Y; Forward.Y = c * Forward.Y + t;
        t = s * Up.Z; Up.Z = c * Up.Z - s * Forward.Z; Forward.Z = c * Forward.Z + t;
    }
    __forceinline void PreYRotate(float angle)
    {
        float c = (float)cos(angle), s = (float)sin(angle);
        float t;
        t = s * Right.X; Right.X = c * Right.X + s * Forward.X; Forward.X = c * Forward.X - t;
        t = s * Right.Y; Right.Y = c * Right.Y + s * Forward.Y; Forward.Y = c * Forward.Y - t;
        t = s * Right.Z; Right.Z = c * Right.Z + s * Forward.Z; Forward.Z = c * Forward.Z - t;
    }
    __forceinline void PreZRotate(float angle)
    {
        float c = (float)cos(angle), s = (float)sin(angle);
        float t;
        t = s * Right.X; Right.X = c * Right.X - s * Up.X; Up.X = c * Up.X + t;
        t = s * Right.Y; Right.Y = c * Right.Y - s * Up.Y; Up.Y = c * Up.Y + t;
        t = s * Right.Z; Right.Z = c * Right.Z - s * Up.Z; Up.Z = c * Up.Z + t;
    }
};

namespace NParticleEngine {

float __fastcall GetOneOverTickRate();      // 0x00AEAA00

class CPSCOrbit { public:
    class COrbitSettings { public:
        unsigned long Type : 2;             // +0x00
        unsigned long LocalSpace : 1;       // bit 2 (unproven name: transform the orbit point, else translate)
        bool          Enabled;              // +0x04
        float         Radius;               // +0x08
        float         Expand;               // +0x0c
        bool          CycleFlag;            // +0x10
        float         CycleTime;            // +0x14
        float         SqueezeScale;         // +0x18
        float         SqueezeAngle;         // +0x1c
        float         RotateSpeed;          // +0x20
        float         RotateStart;          // +0x24
        float         RotateSpeedRandom;    // +0x28
        float         RotateStartRandom;    // +0x2c
    };
    class COrbitState { public:
        float     NextAngle;                // +0x00
        float     RotateSpeedRandomValue;   // +0x04
        C3DVector FromPosition;             // +0x08
        C3DVector ToPosition;               // +0x14
        C3DVector NextPosition;             // +0x20
        C3DVector FromTangent;              // +0x2c
        C3DVector ToTangent;                // +0x38
        void Update(const COrbitSettings& settings, CMatrix3x4& transform, float time);
    };
};

void CPSCOrbit::COrbitState::Update(const COrbitSettings& settings, CMatrix3x4& transform, float time)
{
    NextAngle += (settings.RotateSpeed + RotateSpeedRandomValue) * GetOneOverTickRate();
    FromPosition = ToPosition;
    ToPosition = NextPosition;
    float x, y;
    if (settings.Enabled) {
        if (GFFloatAbs(settings.SqueezeAngle) > g_epsilon_0129ba3c) {
            float c = GFFastCos(NextAngle + settings.SqueezeAngle);
            float s = GFFastSin(NextAngle + settings.SqueezeAngle) * settings.SqueezeScale;
            float c2 = GFFastCos(-settings.SqueezeAngle);
            float s2 = GFFastSin(-settings.SqueezeAngle);
            x = c * c2 + s * s2;
            y = s * c2 - c * s2;
        }
        else {
            x = GFFastCos(NextAngle);
            y = GFFastSin(NextAngle) * settings.SqueezeScale;
        }
        float radius = settings.Radius;
        if (GFFloatAbs(settings.Expand) > g_epsilon_0129ba3c && GFFloatAbs(settings.CycleTime) > g_epsilon_0129ba3c) {
            if (settings.CycleFlag)
                radius += (1.0f - GFFastCos(time / settings.CycleTime)) * settings.Expand * 0.5f;
            else {
                if (time > settings.CycleTime)
                    time = settings.CycleTime;
                radius += GFFastSin(time / settings.CycleTime * 0.25f) * settings.Expand;
            }
        }
        x *= radius;
        y *= radius;
    }
    else {
        x = 0.0f;
        y = 0.0f;
    }
    long type = settings.Type;
    switch (type) {
    case 0: NextPosition.X = x; NextPosition.Y = y; NextPosition.Z = 0.0f; break;
    case 1: NextPosition.X = x; NextPosition.Y = 0.0f; NextPosition.Z = y; break;
    case 2: NextPosition.X = 0.0f; NextPosition.Y = x; NextPosition.Z = y; break;
    }
    if (settings.LocalSpace)
        transform.TransformPoint(NextPosition);
    else
        NextPosition += transform.GetTranslation();
    switch (type) {
    case 0: transform.PreZRotate(-NextAngle * 6.28318530718); break;
    case 1: transform.PreYRotate(-NextAngle * 6.28318530718); break;
    case 2: transform.PreXRotate(-NextAngle * 6.28318530718); break;
    }
    transform.Position = NextPosition;
    FromTangent = ToTangent;
    ToTangent = (NextPosition - FromPosition) * 0.5f;
}

} // namespace NParticleEngine
