// Behaviour test for 00ae2cb0: src/00ae2cb0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CParticleProgram::SimulationStep(CParticleSimulation*, CParticleContext*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CParticleProgram::SimulationStep(CParticleSimulation*, CParticleContext*) -- for a non-empty simulation: dragging and forces, blend the positions toward the initial simulation by the per-particle alphas, then run the program for the context's step count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE2CB0, length 102 bytes (padded to 112).
// Leaf: ?SimulationStep@CParticleProgram@@QAEXPAUCParticleSimulation@@PAUCParticleContext@@@Z
// Callees: CParticleSimulation::ApplyDragging / ApplyForces(CParticleContext*) @ 0x00ADF300 / 0x00ADF760, GFInterpolateVectors
// @ 0x00A5BEF0 (cdecl, 9 args), the context's virtual slots 5 (time delta, float) and 4 (step count for a time), the program's
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
void CParticleProgram::SimulationStep(CParticleSimulation* simulation, CParticleContext* context)
{
    if (simulation->Size) {
        simulation->ApplyDragging(context);
        simulation->ApplyForces(context);
        GFInterpolateVectors(simulation->Positions, 16, InitialSimulation.Positions, 16, simulation->Positions, 16,
                             simulation->SimulationAlphas, 4, InitialSimulation.Size);
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
static C4DVector *g_out, *g_a, *g_b; static float* g_t; static unsigned long g_os, g_as, g_bs, g_ts, g_count;
void __cdecl GFInterpolateVectors(C4DVector* out, unsigned long outStride, C4DVector* a, unsigned long aStride,
                                  C4DVector* b, unsigned long bStride, float* t, unsigned long tStride, unsigned long count)
{ g_out = out; g_os = outStride; g_a = a; g_as = aStride; g_b = b; g_bs = bStride; g_t = t; g_ts = tStride; g_count = count; Log('I'); }
int main()
{
    static CParticleProgram program; static CParticleSimulation sim;
    C4DVector pos[2], initial[2]; float alphas[2];
    sim.Size = 0; sim.Positions = pos; sim.SimulationAlphas = alphas;
    program.InitialSimulation.Positions = initial; program.InitialSimulation.Size = 2;
    CTestContext ctx; ctx.delta = 0.25f; ctx.stepCalls = 0;
    program.SimulationStep(&sim, &ctx);
    CHECK(g_logn == 0);                                     // empty simulation: nothing
    sim.Size = 5;
    program.SimulationStep(&sim, &ctx);
    CHECK(strcmp(g_log, "DFITCE") == 0);
    CHECK(g_dragSim == &sim); CHECK(g_dragCtx == &ctx); CHECK(g_forceCtx == &ctx);
    CHECK(g_out == pos); CHECK(g_os == 16); CHECK(g_a == initial); CHECK(g_as == 16); CHECK(g_b == pos); CHECK(g_bs == 16);
    CHECK(g_t == alphas); CHECK(g_ts == 4); CHECK(g_count == 2);
    CHECK(ctx.seenTime == 0.25f); CHECK(ctx.stepCalls == 1);
    CHECK(g_execThis == &program); CHECK(g_execSim == &sim); CHECK(g_execSteps == 25);
    if (fails == 0) printf("00ae2cb0_TEST PASS\n"); else printf("00ae2cb0_TEST FAIL\n");
    return fails;
}