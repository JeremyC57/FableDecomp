// NParticleEngine::CParticleSimulation::SatisfyDistanceConstraints(const CParticleDistanceConstraint* constraints, unsigned long count) -- one relaxation pass over distance constraints: for each pair move both particles along their separation, weighted by inverse mass, by stiffness * (length - rest) / ((wA + wB) * length) (denominator clamped to a minimum), clear both w components and track the largest stretch seen; an SSE path does the same four-wide when the global SSE flag is set
// Grade: DIFFER(759v766) 28 diff lines (asm oracle; base 466v766, SSE branch missing) -- /Zi realigned prologue (exact), the hand SSE loop as one __asm block over named aligned locals, x87 fallback rewritten as C3DVector p1/p2/d with operator-/operator*/+=/-= and C4DVector::Set(v, 0.0f) write-back (130->33), X-first length spelling (VC reverses: 33->28). Residue: the constraint IV bias (+4 v retail +8: A/B/RestLength/Stiffness offsets shift) and the scalar kept on the FPU stack through all three products then popped (fstp st(0)) where VC7.1 folds the last product; tried const float& / out-of-class / named s1,s2 / memberwise result (no change or worse)
// Retail-asm: 0x00ADF9DD..0x00ADFAAE `mov esi,[ebx+8] ; ... movaps xmm0,[ebp-40h] ; shufps xmm0,xmm0,0 ; ... sqrtps xmm4,xmm4 ; maxss xmm6,xmm5 ; push esi ; mov esi,[ebp-0Ch] ; ... pop esi ; ... divps xmm1,xmm0 ; ... add esi,[ebp-10h] ; dec dword ptr [ebp-4] ; jne` -- a hand SSE loop (push esi/pop esi inside the block, counter decremented in memory, stride added from a frame local): no C++/intrinsic form emits push/pop or a memory dec loop.
// Leaf: SatisfyDistanceConstraints
// Flags: /Zi
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADF960, 766 bytes (+ int3 padding), `ret 8`, 16-byte aligned ebx/ebp frame.
// Callees: none. Global: the SSE-enabled byte 0x013D2880; minimum denominator 0x0129F0BC; 1e-4f (0x38D1B717) immediates.
// Layout: CParticleSimulation: Positions (16-byte C4DVector array) +0x1c, InverseMasses (float array) +0x2c, MaxStretch
//   +0x30. CParticleDistanceConstraint (16 bytes): particle indices +0x00/+0x04, RestLength +0x08, Stiffness +0x0c.
// Note: the SSE branch is an inline __asm block (movaps/shufps/sqrtps/maxss on frame copies of the members, with the
//   `push esi / pop esi` and the aligned frame it needs) -- not reconstructible without __asm: lane gap: inline __asm SSE.
//   This is the behavioural C++ (the SSE branch written as the same scalar maths).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
};
struct C4DVector {
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    void Set(const C3DVector& v, float w) { X = v.X; Y = v.Y; Z = v.Z; W = w; }
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
    if (g_bParticleUseSSE) {
        float* masses = InverseMasses;
        unsigned long stride = sizeof(CParticleDistanceConstraint);
        unsigned long remaining = count;
        __declspec(align(16)) float epsilon[4];
        epsilon[0] = 1e-4f;
        epsilon[1] = 1e-4f;
        epsilon[2] = 1e-4f;
        epsilon[3] = 1e-4f;
        __declspec(align(16)) float maxStretch[4];
        maxStretch[0] = MaxStretch;
        maxStretch[1] = 0.0f;
        maxStretch[2] = 0.0f;
        maxStretch[3] = 0.0f;
        C4DVector* positions = Positions;
        __declspec(align(16)) float pair[4];
        __asm {
            mov     esi, constraints
            mov     edi, positions
        next:
            mov     eax, [esi + 8]
            mov     edx, [esi + 0Ch]
            mov     dword ptr pair[0], eax
            mov     dword ptr pair[4], edx
            movaps  xmm0, pair
            movaps  xmm1, pair
            shufps  xmm0, xmm0, 0
            shufps  xmm1, xmm1, 55h
            mov     eax, [esi]
            mov     edx, [esi + 4]
            shl     eax, 2
            shl     edx, 2
            mov     dword ptr [edi + eax*4 + 0Ch], 0
            mov     dword ptr [edi + edx*4 + 0Ch], 0
            movaps  xmm2, [edi + eax*4]
            movaps  xmm3, [edi + edx*4]
            movaps  xmm4, xmm3
            subps   xmm4, xmm2
            mulps   xmm4, xmm4
            movaps  xmm5, xmm4
            shufps  xmm5, xmm5, 4Eh
            addps   xmm4, xmm5
            movaps  xmm5, xmm4
            shufps  xmm5, xmm5, 11h
            addps   xmm4, xmm5
            sqrtps  xmm4, xmm4
            movaps  xmm6, maxStretch
            movaps  xmm5, xmm4
            subps   xmm5, xmm0
            maxss   xmm6, xmm5
            mulps   xmm5, xmm1
            movaps  maxStretch, xmm6
            push    esi
            mov     esi, masses
            mov     ecx, [esi + eax]
            mov     dword ptr pair[0], ecx
            mov     ecx, [esi + edx]
            mov     dword ptr pair[4], ecx
            pop     esi
            movaps  xmm6, pair
            movaps  xmm7, pair
            shufps  xmm6, xmm6, 0
            shufps  xmm7, xmm7, 55h
            movaps  xmm0, xmm6
            addps   xmm0, xmm7
            mulps   xmm0, xmm4
            maxps   xmm0, epsilon
            movaps  xmm1, xmm5
            divps   xmm1, xmm0
            movaps  xmm0, xmm3
            subps   xmm0, xmm2
            mulps   xmm0, xmm1
            mulps   xmm6, xmm0
            mulps   xmm7, xmm0
            addps   xmm2, xmm6
            subps   xmm3, xmm7
            movaps  [edi + eax*4], xmm2
            movaps  [edi + edx*4], xmm3
            add     esi, stride
            dec     remaining
            jne     next
        }
        MaxStretch = maxStretch[0];
    } else {
        for (unsigned long i = 0; i < count; ++i) {
            const CParticleDistanceConstraint& c = constraints[i];
            C3DVector p1(Positions[c.A].X, Positions[c.A].Y, Positions[c.A].Z);
            C3DVector p2(Positions[c.B].X, Positions[c.B].Y, Positions[c.B].Z);
            float w1 = InverseMasses[c.A];
            float w2 = InverseMasses[c.B];
            C3DVector d = p2 - p1;
            float length = (float)sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z);
            float denominator = (w2 + w1) * length;
            if (denominator < PARTICLE_MIN_CONSTRAINT_DENOMINATOR)
                denominator = PARTICLE_MIN_CONSTRAINT_DENOMINATOR;
            float stretch = length - c.RestLength;
            float scale = c.Stiffness / denominator * stretch;
            p1 += d * (w1 * scale);
            p2 -= d * (scale * w2);
            MaxStretch = MaxStretch > stretch ? MaxStretch : stretch;
            Positions[c.A].Set(p1, 0.0f);
            Positions[c.B].Set(p2, 0.0f);
        }
    }
}
}
