// Grade: DIFFER(466v766) -- lane gap: inline __asm SSE (the g_bParticleUseSSE branch is a hand-written movaps/shufps/sqrtps/maxss block with push esi/pop esi on a 16-byte aligned ebx/ebp frame); the source covers the x87 branch's maths only, so the SSE branch, the frame and the flag test are missing
// NParticleEngine::CParticleSimulation::SatisfyDistanceConstraints(const CParticleDistanceConstraint* constraints, unsigned long count) -- one relaxation pass over distance constraints: for each pair move both particles along their separation, weighted by inverse mass, by stiffness * (length - rest) / ((wA + wB) * length) (denominator clamped to a minimum), clear both w components and track the largest stretch seen; an SSE path does the same four-wide when the global SSE flag is set
// Leaf: SatisfyDistanceConstraints
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADF960, 766 bytes (+ int3 padding), `ret 8`, 16-byte aligned ebx/ebp frame.
// Callees: none. Global: the SSE-enabled byte 0x013D2880; minimum denominator 0x0129F0BC; 1e-4f (0x38D1B717) immediates.
// Layout: CParticleSimulation: Positions (16-byte C4DVector array) +0x1c, InverseMasses (float array) +0x2c, MaxStretch
//   +0x30. CParticleDistanceConstraint (16 bytes): particle indices +0x00/+0x04, RestLength +0x08, Stiffness +0x0c.
// Note: the SSE branch is an inline __asm block (movaps/shufps/sqrtps/maxss on frame copies of the members, with the
//   `push esi / pop esi` and the aligned frame it needs) -- not reconstructible without __asm: lane gap: inline __asm SSE.
//   This is the behavioural C++ (the SSE branch written as the same scalar maths).
struct C4DVector {
    float X, Y, Z, W;
};
struct CParticleDistanceConstraint {
    unsigned long A;                               // +0x00
    unsigned long B;                               // +0x04
    float         RestLength;                      // +0x08
    float         Stiffness;                       // +0x0c
};
extern bool g_bParticleUseSSE;                     // 0x013D2880
extern const float PARTICLE_MIN_CONSTRAINT_DENOMINATOR;   // 0x0129F0BC
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
namespace NParticleEngine {
class CParticleSimulation {
public:
    unsigned char _pad00[0x1c];
    C4DVector*    Positions;                       // +0x1c
    unsigned char _pad20[0xc];
    float*        InverseMasses;                   // +0x2c
    float         MaxStretch;                      // +0x30
    void SatisfyDistanceConstraints(const CParticleDistanceConstraint* constraints, unsigned long count);
};

void CParticleSimulation::SatisfyDistanceConstraints(const CParticleDistanceConstraint* constraints, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i) {
        const CParticleDistanceConstraint& c = constraints[i];
        C4DVector p1 = Positions[c.A];
        C4DVector p2 = Positions[c.B];
        float w1 = InverseMasses[c.A];
        float w2 = InverseMasses[c.B];
        float dx = p2.X - p1.X;
        float dy = p2.Y - p1.Y;
        float dz = p2.Z - p1.Z;
        float length = (float)sqrt(dz * dz + dy * dy + dx * dx);
        float denominator = (w1 + w2) * length;
        if (denominator < PARTICLE_MIN_CONSTRAINT_DENOMINATOR)
            denominator = PARTICLE_MIN_CONSTRAINT_DENOMINATOR;
        float stretch = length - c.RestLength;
        float scale = c.Stiffness / denominator * stretch;
        float s1 = w1 * scale;
        p1.X += dx * s1;
        p1.Y += dy * s1;
        p1.Z += dz * s1;
        float s2 = scale * w2;
        p2.X -= dx * s2;
        p2.Y -= dy * s2;
        p2.Z -= dz * s2;
        MaxStretch = MaxStretch > stretch ? MaxStretch : stretch;
        p1.W = 0.0f;
        Positions[c.A] = p1;
        p2.W = 0.0f;
        Positions[c.B] = p2;
    }
}
}
