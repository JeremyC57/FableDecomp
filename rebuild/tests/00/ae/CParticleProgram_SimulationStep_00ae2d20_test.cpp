// Behaviour test for 00ae2d20: src/00ae2d20.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CParticleProgram::SimulationStep(CParticleSimulation*, CParticleContext*, C3DAnimationBlendState*, C3DAnimationManager*, C3DVector*, C3DMesh2*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CParticleProgram::SimulationStep(CParticleSimulation*, CParticleContext*, C3DAnimationBlendState*, C3DAnimationManager*, C3DVector*, C3DMesh2*) -- for a non-empty simulation: dragging and forces, cling the particles to the animated mesh, then run the program for the context's step count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE2D20, length 97 bytes (padded to 112).
// Leaf: ?SimulationStep@CParticleProgram@@QAEXPAUCParticleSimulation@@PAUCParticleContext@@PAUC3DAnimationBlendState
// Callees: CParticleSimulation::ApplyDragging / ApplyForces(CParticleContext*) @ 0x00ADF300 / 0x00ADF760, GFInterpolateVectors
// CParticleProgram::ClingToMesh @ 0x00AE1320 (in place of GFInterpolateVectors @ 0x00A5BEF0), the context's virtual slots 5 (time delta, float) and 4 (step count for a time), the program's
// looped execution @ 0x00AE2510.
// Layout: PDB CParticleSimulation (Size +0x18, Positions +0x1c, SimulationAlphas +0x2c); CParticleProgram's InitialSimulation at +8.
// Shape: the time delta is held in a DOUBLE local (the float return widened, no rounding store) and narrowed
// straight into the pushed argument slot (`push ecx ; fstp [esp]`) after the vtable re-read; a float local spills
// through its frame slot first, the nested call caches the vtable across the inner call.
struct C4DVector { float X, Y, Z, W; };
struct C3DVector;
struct C3DAnimationBlendState;
struct C3DAnimationManager;
struct C3DMesh2;
struct CParticleContext {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual long GetStepCount(float time);  // slot 4 (+0x10)
    virtual float GetTimeDelta();           // slot 5 (+0x14)
};
void __cdecl GFInterpolateVectors(C4DVector* out, unsigned long outStride, C4DVector* a, unsigned long aStride,
                                  C4DVector* b, unsigned long bStride, float* t, unsigned long tStride,
                                  unsigned long count);   // 0x00A5BEF0
struct CParticleSimulation {                // Ego_r.pdb layout (0x80 bytes)
    void*         __vftable;                // +0x00
    unsigned char RenderData[8];            // +0x04 CCountedPointer<CParticleSimulationRenderData>
    float         Timestep;                 // +0x0c
    bool          TimestepChanged;          // +0x10
    float         TimestepMultiplier;       // +0x14
    unsigned long Size;                     // +0x18
    C4DVector*    Positions;                // +0x1c
    C4DVector*    LastPositions;            // +0x20
    C4DVector*    SavedPositions;           // +0x24
    C4DVector*    Accelerations;            // +0x28
    float*        SimulationAlphas;         // +0x2c
    unsigned char _rest[0x50];              // +0x30
    void ApplyDragging(CParticleContext* context);   // 0x00ADF300
    void ApplyForces(CParticleContext* context);     // 0x00ADF760
};
struct CParticleProgram {
    void*               __vftable;          // +0x00
    unsigned char       _base_0x04[4];      // +0x04
    CParticleSimulation InitialSimulation;  // +0x08
    void ExecuteLooped(CParticleSimulation* simulation, long steps);   // 0x00AE2510
    void ClingToMesh(CParticleSimulation* simulation, C3DAnimationBlendState* blendState, C3DAnimationManager* animationManager,
                     C3DVector* position, C3DMesh2* mesh);              // 0x00AE1320
    void SimulationStep(CParticleSimulation* simulation, CParticleContext* context);
    void SimulationStep(CParticleSimulation* simulation, CParticleContext* context, C3DAnimationBlendState* blendState,
                        C3DAnimationManager* animationManager, C3DVector* position, C3DMesh2* mesh);
};
void CParticleProgram::SimulationStep(CParticleSimulation* simulation, CParticleContext* context, C3DAnimationBlendState* blendState,
                                      C3DAnimationManager* animationManager, C3DVector* position, C3DMesh2* mesh)
{
    if (simulation->Size) {
        simulation->ApplyDragging(context);
        simulation->ApplyForces(context);
        ClingToMesh(simulation, blendState, animationManager, position, mesh);
        double timeDelta = context->GetTimeDelta();
        ExecuteLooped(simulation, context->GetStepCount((float)timeDelta));
    }
}

// ---- stubs and driver ----
void CParticleContext::v00() {}
void CParticleContext::v01() {}
void CParticleContext::v02() {}
void CParticleContext::v03() {}
static char g_log[64]; static int g_logn = 0;
static void Log(char c) { if (g_logn < 63) { g_log[g_logn++] = c; g_log[g_logn] = 0; } }
long CParticleContext::GetStepCount(float) { return -1; }
float CParticleContext::GetTimeDelta() { return -1.0f; }
struct CTestContext : CParticleContext {
    float delta; float seenTime; int stepCalls;
    virtual long GetStepCount(float time) { seenTime = time; ++stepCalls; Log('C'); return (long)(time * 100.0f); }
    virtual float GetTimeDelta() { Log('T'); return delta; }
};
static CParticleContext* g_dragCtx; static CParticleSimulation* g_dragSim; static CParticleContext* g_forceCtx;
void CParticleSimulation::ApplyDragging(CParticleContext* context) { g_dragSim = this; g_dragCtx = context; Log('D'); }
void CParticleSimulation::ApplyForces(CParticleContext* context) { g_forceCtx = context; Log('F'); }
static CParticleProgram* g_execThis; static CParticleSimulation* g_execSim; static long g_execSteps = -7;
void CParticleProgram::ExecuteLooped(CParticleSimulation* simulation, long steps) { g_execThis = this; g_execSim = simulation; g_execSteps = steps; Log('E'); }
static CParticleProgram* g_clingThis; static CParticleSimulation* g_clingSim; static C3DAnimationBlendState* g_bs; static C3DAnimationManager* g_am;
static C3DVector* g_pos; static C3DMesh2* g_mesh;
void CParticleProgram::ClingToMesh(CParticleSimulation* simulation, C3DAnimationBlendState* blendState, C3DAnimationManager* animationManager,
                                   C3DVector* position, C3DMesh2* mesh)
{ g_clingThis = this; g_clingSim = simulation; g_bs = blendState; g_am = animationManager; g_pos = position; g_mesh = mesh; Log('M'); }
int main()
{
    static CParticleProgram program; static CParticleSimulation sim;
    sim.Size = 0;
    CTestContext ctx; ctx.delta = 0.5f; ctx.stepCalls = 0;
    C3DAnimationBlendState* bs = (C3DAnimationBlendState*)0x11; C3DAnimationManager* am = (C3DAnimationManager*)0x22;
    C3DVector* p = (C3DVector*)0x33; C3DMesh2* m = (C3DMesh2*)0x44;
    program.SimulationStep(&sim, &ctx, bs, am, p, m);
    CHECK(g_logn == 0);
    sim.Size = 1;
    program.SimulationStep(&sim, &ctx, bs, am, p, m);
    CHECK(strcmp(g_log, "DFMTCE") == 0);
    CHECK(g_dragSim == &sim); CHECK(g_dragCtx == &ctx); CHECK(g_forceCtx == &ctx);
    CHECK(g_clingThis == &program); CHECK(g_clingSim == &sim); CHECK(g_bs == bs); CHECK(g_am == am); CHECK(g_pos == p); CHECK(g_mesh == m);
    CHECK(ctx.seenTime == 0.5f); CHECK(g_execThis == &program); CHECK(g_execSim == &sim); CHECK(g_execSteps == 50);
    if (fails == 0) printf("00ae2d20_TEST PASS\n"); else printf("00ae2d20_TEST FAIL\n");
    return fails;
}