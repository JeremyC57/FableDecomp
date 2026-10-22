// Grade: DIFFER(422v726) -- lane gap: inline __asm SSE (the 0x013D2880 branch of the bone-matrix product is a hand-written movaps/shufps/mulps/addps block reading pointers the C code stored to frame slots, on a 16-byte aligned ebp frame, as in ClingToMesh 0x00AE1320); the source covers the x87 branch's maths only
// NParticleEngine::CParticleProgram::SetupSimulation(CParticleSimulation* simulation, C3DAnimationBlendState* blendState, C3DAnimationManager* animation, const C3DVector* position, C3DMesh2* mesh, CMovableResourceMemoryPool* pool, CParticleContext* context) -- start a cloth-style simulation on an animated mesh: initialise the simulation from the program's template (false on failure), compute this frame's bone matrices premultiplied by the inverse rest-pose bone matrices, skin the program's vertices into the simulation positions, run the program's setup pass with the context's time step, and copy the positions into the previous positions; true on success
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
struct CMatrix4x4 { float M[4][4]; };
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
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                matrices[i].M[r][c] = bone.M[r][1] * inverse.M[1][c] + bone.M[r][3] * inverse.M[3][c] +
                                      bone.M[r][2] * inverse.M[2][c] + bone.M[r][0] * inverse.M[0][c];
    }
    TransformVertices(simulation->Positions, Vertices, simulation->NumParticles, matrices, &Groups);
    RunSetup(simulation, context->GetSetupSteps(context->GetTimeStep()));
    memcpy(simulation->OldPositions, simulation->Positions, simulation->NumParticles * sizeof(C4DVector));
    return true;
}
}
