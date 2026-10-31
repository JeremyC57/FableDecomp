// NParticleEngine::CParticleProgram::SetupSimulation(CParticleSimulation* simulation, C3DAnimationBlendState* blendState, C3DAnimationManager* animation, const C3DVector* position, C3DMesh2* mesh, CMovableResourceMemoryPool* pool, CParticleContext* context) -- start a cloth-style simulation on an animated mesh: initialise the simulation from the program's template (false on failure), compute this frame's bone matrices premultiplied by the inverse rest-pose bone matrices, skin the program's vertices into the simulation positions, run the program's setup pass with the context's time step, and copy the positions into the previous positions; true on success
// Grade: DIFFER(710v726) 94 diff lines (asm oracle; base 422v726, 196) -- the shared MultiplySSE __forceinline helper (pointers homed/reloaded), aligned CMatrix4x4, stepwise x87 sum (107->94). Residue: this/simulation ebx/edi colouring, homing-store order, x87 fallback per-column term order (canonicalised)
// Retail-asm: 0x00AE2A7A..0x00AE2B7x `lea ecx,[esp+30h] ; mov [esp+28h],ecx ; mov [esp+24h],eax ; ... mov eax,[esp+28h] ; mov edx,[esp+24h] ; mov edi,[esp+20h] ; movaps xmm0,[edx] ... shufps ... movaps [edi+30h],xmm4` -- the hand SSE 4x4 multiply (twin of 0x00AE13CA / 0x0098929D).
// Leaf: SetupSimulation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE29D0, 726 bytes (+ int3 padding), `ret 1Ch`, 16-byte aligned ebp frame.
// Callees: CParticleSimulation::InitializeFromTemplate 0x00ADF610, the scratch matrix buffer getter 0x00AE04E0 (__fastcall),
//   C3DAnimationManager::CalcMeshTransformations 0x00A9F540, CParticleProgram::TransformVertices 0x00AE0E60, the context's
//   virtual slots 5 (+0x14, float) and 4 (+0x10, taking that float), the program pass 0x00AE2510 (label
//   "ParticleProgram_ExecuteLooped"). Global: the SSE-enabled byte 0x013D2880.
// Layout: CParticleProgram template +0x08, Vertices +0x24, Groups +0xfc; C3DMesh2 NumBones +0x98, InverseBoneMatrices
//   +0xe0; CParticleSimulation NumParticles +0x18, Positions +0x1c, OldPositions +0x20.
// Note: the SSE branch of the bone-matrix product is an inline __asm block (as in ClingToMesh 0x00AE1320) -- lane gap:
//   inline __asm SSE. This is the behavioural C++ (x87 branch maths only).
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
struct C3DVector { float X, Y, Z; };
struct C4DVector { float X, Y, Z, W; };
struct __declspec(align(16)) CMatrix4x4 { float M[4][4]; };
extern bool g_SSEAvailable;                        // 0x013D2880
static __forceinline void MultiplySSE(CMatrix4x4* out, const CMatrix4x4* left, const CMatrix4x4* right)
{
    __asm {
        mov     eax, left
        mov     edx, right
        mov     edi, out
        movaps  xmm0, [edx]
        movaps  xmm1, [edx+10h]
        movaps  xmm2, [edx+20h]
        movaps  xmm3, [edx+30h]
        movaps  xmm4, [eax]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi], xmm4
        movaps  xmm4, [eax+10h]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi+10h], xmm4
        movaps  xmm4, [eax+20h]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi+20h], xmm4
        movaps  xmm4, [eax+30h]
        movaps  xmm5, xmm4
        movaps  xmm6, xmm4
        movaps  xmm7, xmm4
        shufps  xmm4, xmm4, 0
        shufps  xmm5, xmm5, 55h
        shufps  xmm6, xmm6, 0AAh
        shufps  xmm7, xmm7, 0FFh
        mulps   xmm4, xmm0
        mulps   xmm5, xmm1
        mulps   xmm6, xmm2
        mulps   xmm7, xmm3
        addps   xmm4, xmm5
        addps   xmm6, xmm7
        addps   xmm4, xmm6
        movaps  [edi+30h], xmm4
    }
}

struct C3DAnimationBlendState;
struct CMovableResourceMemoryPool;
struct CGroupArray { void* Data; unsigned long Size; unsigned long Capacity; };
struct C3DMesh2 {
    unsigned char _pad00[0x98];
    unsigned long NumBones;                        // +0x98
    unsigned char _pad9c[0xe0 - 0x9c];
    CMatrix4x4*   InverseBoneMatrices;             // +0xe0
};
struct C3DAnimationManager {
    void CalcMeshTransformations(C3DAnimationBlendState* blendState, const C3DVector* position, C3DMesh2* mesh, CMatrix4x4* matrices, unsigned long count, bool world);   // 0x00A9F540
};
CMatrix4x4* __fastcall GetGroupMatrixBuffer(unsigned long count);  // 0x00AE04E0
struct CParticleContext {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual long GetSetupSteps(float timeStep);    // slot 4 (+0x10)
    virtual float GetTimeStep();                   // slot 5 (+0x14)
};
namespace NParticleEngine {
struct CParticleSimulationTemplate;
struct CParticleSimulation {
    unsigned char _pad00[0x18];
    unsigned long NumParticles;                    // +0x18
    C4DVector*    Positions;                       // +0x1c
    C4DVector*    OldPositions;                    // +0x20
    bool InitializeFromTemplate(const CParticleSimulationTemplate* simulationTemplate, CMovableResourceMemoryPool* pool);   // 0x00ADF610
};
struct CParticleSimulationTemplate { unsigned char _body[0x18]; };
class CParticleProgram {
public:
    unsigned char               _pad00[8];
    CParticleSimulationTemplate Template;          // +0x08
    unsigned char               _pad20[4];
    C4DVector*                  Vertices;          // +0x24
    unsigned char               _pad28[0xfc - 0x28];
    CGroupArray                 Groups;            // +0xfc
    void TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* matrices, const CGroupArray* groups);   // 0x00AE0E60
    void RunSetup(CParticleSimulation* simulation, long steps);   // 0x00AE2510
    bool SetupSimulation(CParticleSimulation* simulation, C3DAnimationBlendState* blendState, C3DAnimationManager* animation, const C3DVector* position, C3DMesh2* mesh, CMovableResourceMemoryPool* pool, CParticleContext* context);
};

bool CParticleProgram::SetupSimulation(CParticleSimulation* simulation, C3DAnimationBlendState* blendState, C3DAnimationManager* animation, const C3DVector* position, C3DMesh2* mesh, CMovableResourceMemoryPool* pool, CParticleContext* context)
{
    if (!simulation->InitializeFromTemplate(&Template, pool))
        return false;
    unsigned long numBones = mesh->NumBones;
    CMatrix4x4* matrices = GetGroupMatrixBuffer(numBones);
    animation->CalcMeshTransformations(blendState, position, mesh, matrices, numBones, true);
    for (unsigned long i = 0; i < numBones; ++i) {
        CMatrix4x4 bone = matrices[i];
        const CMatrix4x4& inverse = mesh->InverseBoneMatrices[i];
        if (g_SSEAvailable) {
            MultiplySSE(&matrices[i], &bone, &inverse);
        } else {
            for (int r = 0; r < 4; ++r)
                for (int c = 0; c < 4; ++c) {
                    float v = bone.M[r][3] * inverse.M[3][c];
                    v += inverse.M[2][c] * bone.M[r][2];
                    v += bone.M[r][1] * inverse.M[1][c];
                    v += inverse.M[0][c] * bone.M[r][0];
                    matrices[i].M[r][c] = v;
                }
        }
    }
    TransformVertices(simulation->Positions, Vertices, simulation->NumParticles, matrices, &Groups);
    RunSetup(simulation, context->GetSetupSteps(context->GetTimeStep()));
    memcpy(simulation->OldPositions, simulation->Positions, simulation->NumParticles * sizeof(C4DVector));
    return true;
}
}
