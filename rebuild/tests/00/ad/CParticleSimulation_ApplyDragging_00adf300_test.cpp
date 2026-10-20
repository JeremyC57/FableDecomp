#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define ALIGN16 __declspec(align(16))
#else
#define ALIGN16 __attribute__((aligned(16)))
#endif
struct CMatrix4x4Fwd;
struct C4DVector { float X, Y, Z, W; };
struct CMatrix3x4 { float m[4][3]; };                  // rows: right, up, forward, position

class CMatrix4x4;
static const CMatrix4x4* g_to; static float g_t, g_lastW, g_lastM; static int g_transforms; static const void* g_lastIn; static const void* g_lastOut; static unsigned long g_lastCount;
class ALIGN16 CMatrix4x4 {
public:
    float m[4][4];
    CMatrix4x4() {}
    CMatrix4x4(const CMatrix3x4& o)
    {
        m[0][0] = o.m[0][0]; m[0][1] = o.m[1][0]; m[0][2] = o.m[2][0]; m[0][3] = o.m[3][0];
        m[1][0] = o.m[0][1]; m[1][1] = o.m[1][1]; m[1][2] = o.m[2][1]; m[1][3] = o.m[3][1];
        m[2][0] = o.m[0][2]; m[2][1] = o.m[1][2]; m[2][2] = o.m[2][2]; m[2][3] = o.m[3][2];
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }
    void AlignedSetInterpolate(const CMatrix4x4& from, const CMatrix4x4& to, float t) { g_to = &to; g_t = t; for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) m[r][c] = from.m[r][c] * (1.0f - t) + to.m[r][c] * t; }
    void InterpolateTo(const CMatrix4x4& to, float t) { AlignedSetInterpolate(*this, to, t); }
    void AlignedTransformTransposedAssumeW(const C4DVector* in, C4DVector* out, unsigned long count, float w) const { ++g_transforms; g_lastIn = in; g_lastOut = out; g_lastCount = count; g_lastW = w; g_lastM = m[0][3]; }
};

extern const CMatrix4x4 g_ParticleDragReference;

class CParticleContext {
public:
    CMatrix3x4 value;
    virtual void GetTransform(CMatrix3x4& transform) const { transform = value; }
    virtual ~CParticleContext() {}
};

struct CMovableResource { virtual ~CMovableResource() {} };
struct CMovableResourceMemoryPool;

class CParticleSimulation : public CMovableResource {
public:
    unsigned char               RenderData[8];          // +0x04
    float                       Timestep;               // +0x0c
    bool                        TimestepChanged;        // +0x10
    float                       TimestepMultiplier;     // +0x14
    unsigned long               Size;                   // +0x18
    C4DVector*                  Positions;              // +0x1c
    C4DVector*                  LastPositions;          // +0x20
    C4DVector*                  SavedPositions;         // +0x24
    C4DVector*                  Accelerations;          // +0x28
    float*                      SimulationAlphas;       // +0x2c
    float                       MaximumAdjustment;      // +0x30
    float                       GravityStrength;        // +0x34
    float                       WindStrength;           // +0x38
    bool                        DraggingEnable;         // +0x3c
    bool                        DraggingRotational;     // +0x3d
    float                       DraggingStrength;       // +0x40
    void ApplyDragging(const CParticleContext& context);
};

void CParticleSimulation::ApplyDragging(const CParticleContext& context)
{
    if (!DraggingEnable)
        return;
    CMatrix3x4 transform;
    context.GetTransform(transform);
    CMatrix4x4 drag(transform);
    drag.InterpolateTo(g_ParticleDragReference, 1.0f - DraggingStrength);
    drag.AlignedTransformTransposedAssumeW(Positions, Positions, Size, 1.0f);
    drag.AlignedTransformTransposedAssumeW(LastPositions, LastPositions, Size, 1.0f);
}

static CMatrix4x4 MakeReference() { CMatrix3x4 z; std::memset(&z, 0, sizeof(z)); z.m[0][0] = 1; z.m[1][1] = 1; z.m[2][2] = 1; return CMatrix4x4(z); }
const CMatrix4x4 g_ParticleDragReference = MakeReference();
int main()
{
    static C4DVector pos[3], last[3];
    CParticleSimulation s; s.Size = 3; s.Positions = pos; s.LastPositions = last; s.DraggingEnable = false; s.DraggingStrength = 0.25f;
    CParticleContext c;
    for (int r = 0; r < 4; ++r) for (int k = 0; k < 3; ++k) c.value.m[r][k] = (float)(r * 3 + k);
    s.ApplyDragging(c);
    if (g_transforms != 0) { std::printf("00adf300_TEST FAIL\n"); return 1; }
    s.DraggingEnable = true;
    s.ApplyDragging(c);
    /* m[0][3] = pos.X = 9 blended with the identity's 0 by t = 0.75: 9 * 0.25 */
    if (g_transforms != 2 || g_to != &g_ParticleDragReference || g_t != 0.75f || g_lastIn != last || g_lastOut != last || g_lastCount != 3 || g_lastW != 1.0f || g_lastM != 9.0f * 0.25f) { std::printf("00adf300_TEST FAIL\n"); return 1; }
    std::printf("00adf300_TEST PASS\n");
    return 0;
}