// C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe&, ulong, float) const -- slerped rotation (table acos/sin) and lerped word-packed position between frame and frame+1
// Grade: DIFFER(968v1006) -- retail_asm candidate: the C++ (int) cast stand-in replaced by the landed __forceinline
//   FloatToLong inline-asm helper (fld / bare dword fistp, Retail-asm evidence line) inside the inlined FastAcos /
//   FastSin; 224 diff lines from 338; residue: the slerp body's x87 scheduling and frame slots (-38 bytes)
// Retail-asm: 0x00A4C2F2 `fistp dword ptr [esp + 0x18]` (and the other fistp sites of the inlined FastAcos / FastSin) -- the engine's inline-assembly FloatToLong (value stored to a frame word, reloaded by the asm fld, rounded by a bare dword fistp, as the landed FloatToLong of CPSCEmitterGeneric::CalcIsFinishedGeneratingParticles 0x00B10220); VC7.1 emits no bare dword fistp from C++.
// Leaf: GetKeyframeInterpolate
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

extern float g_AcosTable[];
extern float g_SinTable[];
extern const float g_SlerpEpsilon;
extern const float g_AcosIndexScale;
extern const float g_AcosValueScale;
extern const float g_SinIndexScale;

__forceinline long FloatToLong(float value)   // the engine's inline-assembly float-to-long (current rounding mode)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

static __forceinline float FastAcos(float x)
{
    float f = (x + 1.0f) * g_AcosIndexScale;
    int i = FloatToLong(f - 0.5f);
    float frac = f - (float)i;
    return ((1.0f - frac) * g_AcosTable[i] + frac * g_AcosTable[i + 1]) * g_AcosValueScale;
}

static __forceinline float FastSin(float x)
{
    float f = x * g_SinIndexScale;
    int r = FloatToLong(f - 0.5f);
    float frac = f - (float)r;
    int i = (r - 256) & 0x3ff;
    return (1.0f - frac) * g_SinTable[i] + frac * g_SinTable[i + 1];
}

struct CQuaternion { float X, Y, Z, W; };
struct C4DVector { float X, Y, Z, W; };
struct C3DVectorWord { short X, Y, Z; };

class C3DKeyframe {
public:
    CQuaternion Rotation;
    C4DVector Position;
    C4DVector Scaling;
};

class C3DAnimationSequenceData {
public:
    char* BoneName;
    float SamplesPerSecond;
    unsigned short FrameCount;
    unsigned char BoneType : 5;
    unsigned char EnabledFlag : 1;
    unsigned char AnimatedFlag : 1;
    unsigned char IdentityFlag : 1;
    unsigned char RotationTrackMode : 2;
    unsigned char PositionTrackMode : 2;
    unsigned char ScalingTrackMode : 2;
    float PositionFactor;
    float ScalingFactor;
    CQuaternion* RotationTrack;
    unsigned short RotationTrackSize;
    unsigned short PositionTrackSize;
    unsigned short PalettedRotationTrackSize;
    unsigned short PalettedPositionTrackSize;
    C3DVectorWord* PositionTrack;
    unsigned char* PalettedRotationTrack;
    unsigned char* PalettedPositionTrack;

    void GetKeyframeInterpolate(C3DKeyframe& key, unsigned long frame, float t) const;
};

void C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe& key, unsigned long frame, float t) const
{
    float oneMinusT = 1.0f - t;
    unsigned long next = frame + 1 < FrameCount ? frame + 1 : 0;
    if (RotationTrackMode & 2) {
        unsigned long a = frame;
        unsigned long b = next;
        if (RotationTrackMode == 3) {
            a = PalettedRotationTrack[frame];
            b = PalettedRotationTrack[next];
            if (a == b) {
                key.Rotation = RotationTrack[a];
                goto position;
            }
        }
        {
            const CQuaternion& q0 = RotationTrack[a];
            const CQuaternion& q1 = RotationTrack[b];
            float sign = 1.0f;
            float dot = q1.W * q0.W + q0.X * q1.X + q1.Z * q0.Z + q1.Y * q0.Y;
            if (dot < 0.0f) {
                sign = -1.0f;
                dot = -dot;
            }
            float s0, s1;
            if (1.0f - dot > g_SlerpEpsilon) {
                float theta = FastAcos(dot);
                float sinTheta = (float)sqrt(1.0f - dot * dot);
                float inv = 1.0f / sinTheta;
                s0 = FastSin(oneMinusT * theta) * inv;
                s1 = FastSin(theta * t) * inv;
            } else {
                s0 = oneMinusT;
                s1 = t;
            }
            s1 *= sign;
            key.Rotation.X = s1 * q1.X + s0 * q0.X;
            key.Rotation.Y = s1 * q1.Y + s0 * q0.Y;
            key.Rotation.Z = s1 * q1.Z + s0 * q0.Z;
            key.Rotation.W = s0 * q0.W + s1 * q1.W;
        }
    } else if (RotationTrackMode == 1) {
        key.Rotation = RotationTrack[0];
    } else {
        key.Rotation.X = 0.0f;
        key.Rotation.Y = 0.0f;
        key.Rotation.Z = 0.0f;
        key.Rotation.W = 1.0f;
    }
position:
    if (PositionTrackMode & 2) {
        unsigned long a = frame;
        unsigned long b = next;
        if (PositionTrackMode == 3) {
            a = PalettedPositionTrack[frame];
            b = PalettedPositionTrack[next];
        }
        const C3DVectorWord& pb = PositionTrack[b];
        const C3DVectorWord& pa = PositionTrack[a];
        float fb = t * PositionFactor;
        float bx = (float)pb.X * fb, by = (float)pb.Y * fb, bz = (float)pb.Z * fb;
        float fa = oneMinusT * PositionFactor;
        float ax = (float)pa.X * fa, ay = (float)pa.Y * fa, az = (float)pa.Z * fa;
        key.Position.X = ax + bx;
        key.Position.Y = ay + by;
        key.Position.Z = az + bz;
        key.Position.W = 0.0f;
    } else if (PositionTrackMode == 1) {
        float f = PositionFactor;
        key.Position.X = (float)PositionTrack[0].X * f;
        key.Position.Y = (float)PositionTrack[0].Y * f;
        key.Position.Z = (float)PositionTrack[0].Z * f;
        key.Position.W = 0.0f;
    } else {
        key.Position.X = 0.0f;
        key.Position.Y = 0.0f;
        key.Position.Z = 0.0f;
        key.Position.W = 0.0f;
    }
}
