// Link test for 00ae2d20 CParticleProgram::SimulationStep(sim&, const context&, blend, anim, pos, mesh) const: calls the
// source object; callees recorded in order (dragging, forces, cling, slot 5 time delta, slot 4 iterations, ApplyConstraints).
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; std::printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class C4DVector {
public:
    float X, Y, Z, W;
};
class C3DVector;
class C3DAnimationBlendState;
class C3DAnimationManager;
class C3DMesh2;
class CMovableResourceMemoryPool;
class CParticleSimulationRenderData;

namespace NVectorMath {
void __cdecl GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* a, unsigned long aStride,
                                  const C4DVector* b, unsigned long bStride, const float* t, unsigned long tStride,
                                  unsigned long count);   // 0x00A5BEF0
}

class CParticleContext {                    // Ego_r.pdb: sizeof 4 (vptr only); slot 0 is the virtual destructor (??_E)
public:
    virtual ~CParticleContext();
    virtual void Slot1() const;             // slots 1-3 unused here
    virtual void Slot2() const;
    virtual void Slot3() const;
    virtual long GetStepCount() const;      // slot 4 (+0x10), descriptive: constraint iterations for this step
    virtual float GetTimeDelta() const;     // slot 5 (+0x14), descriptive
};

class CMovableResource {                    // retail RTTI base of CParticleSimulation (vtable 0x0129F0C4) and
public:                                     // CParticleProgram (vtable 0x0129F0D8)
    virtual ~CMovableResource();
};

class CParticleSimulation : public CMovableResource {   // Ego_r.pdb layout (0x80 bytes; the base's vptr is +0x00)
    friend class CParticleProgram;
public:
    CParticleSimulationRenderData* RenderData[2];   // +0x04 CCountedPointer<CParticleSimulationRenderData>
    float         Timestep;                 // +0x0c
    bool          TimestepChanged;          // +0x10
    float         TimestepMultiplier;       // +0x14
    unsigned long Size;                     // +0x18
    C4DVector*    Positions;                // +0x1c
    C4DVector*    LastPositions;            // +0x20
    C4DVector*    SavedPositions;           // +0x24
    C4DVector*    Accelerations;            // +0x28
    float*        SimulationAlphas;         // +0x2c
    float         MaximumAdjustment;        // +0x30
    float         GravityStrength;          // +0x34
    float         WindStrength;             // +0x38
    bool          DraggingEnable;           // +0x3c
    bool          DraggingRotational;       // +0x3d
    float         DraggingStrength;         // +0x40
    bool          AccelerationEnable;       // +0x44
    float         GlobalDamping;            // +0x48
    double        CurrentTime;              // +0x50
    float         BoundingBox[6];           // +0x58 C3DBoundingBox
    bool          BoundingBoxValid;         // +0x70
    long          MemoryHandle;             // +0x74
    CMovableResourceMemoryPool* MemoryPool; // +0x78
private:
    void ApplyDragging(const CParticleContext& context);   // 0x00ADF300
    void ApplyForces(const CParticleContext& context);     // 0x00ADF760
};

class CParticleProgram : public CMovableResource {   // Ego_r.pdb: InitialSimulation +8 (8-aligned), sizeof 0x170
public:
    CParticleSimulation InitialSimulation;  // +0x08
    void SimulationStep(CParticleSimulation& simulation, const CParticleContext& context) const;
    void SimulationStep(CParticleSimulation& simulation, const CParticleContext& context,
                        const C3DAnimationBlendState& blendState, C3DAnimationManager& animationManager,
                        const C3DVector* position, const C3DMesh2& mesh) const;
private:
    void ApplyConstraints(CParticleSimulation& simulation, long iterations, float timeDelta) const;   // 0x00AE2510
    void ClingToMesh(CParticleSimulation& simulation, const C3DAnimationBlendState& blendState,
                     C3DAnimationManager& animationManager, const C3DVector* position,
                     const C3DMesh2& mesh) const;                                                      // 0x00AE1320
};

static char g_log[32]; static int g_n = 0;
static const void* g_ctx = 0; static const void* g_sim = 0;
static long g_iter = -1; static float g_dt = -1.0f;
static const void* g_interp[5]; static unsigned long g_count = 0, g_strides[4];
static const void* g_cling[5];
CParticleContext::~CParticleContext() { }
void CParticleContext::Slot1() const { }
void CParticleContext::Slot2() const { }
void CParticleContext::Slot3() const { }
long CParticleContext::GetStepCount() const { return 0; }
float CParticleContext::GetTimeDelta() const { return 0.0f; }
class CTestContext : public CParticleContext {
public:
    long GetStepCount() const { g_log[g_n++] = 'S'; return 7; }
    float GetTimeDelta() const { g_log[g_n++] = 'T'; return 0.25f; }
};
void CParticleSimulation::ApplyDragging(const CParticleContext& c) { g_log[g_n++] = 'D'; g_sim = this; g_ctx = &c; }
void CParticleSimulation::ApplyForces(const CParticleContext& c) { g_log[g_n++] = 'F'; if (g_ctx != &c || g_sim != this) ++fails; }
void __cdecl NVectorMath::GFInterpolateVectors(C4DVector* out, unsigned long s0, const C4DVector* a, unsigned long s1,
                                               const C4DVector* b, unsigned long s2, const float* t, unsigned long s3,
                                               unsigned long count)
{ g_log[g_n++] = 'I'; g_interp[0] = out; g_interp[1] = a; g_interp[2] = b; g_interp[3] = t; g_count = count;
  g_strides[0] = s0; g_strides[1] = s1; g_strides[2] = s2; g_strides[3] = s3; }
CMovableResource::~CMovableResource() { }
void CParticleProgram::ApplyConstraints(CParticleSimulation& s, long iterations, float dt) const
{ g_log[g_n++] = 'C'; if (&s != g_sim) ++fails; g_iter = iterations; g_dt = dt; }
void CParticleProgram::ClingToMesh(CParticleSimulation& s, const C3DAnimationBlendState& bs, C3DAnimationManager& am,
                                   const C3DVector* pos, const C3DMesh2& mesh) const
{ g_log[g_n++] = 'M'; g_cling[0] = &s; g_cling[1] = &bs; g_cling[2] = &am; g_cling[3] = pos; g_cling[4] = &mesh; }

int main()
{
    static CParticleProgram prog;
    static CParticleSimulation sim;
    CTestContext ctx;
    static char bs[4], am[4], mesh[4]; static char posv[12];
    sim.Size = 2;
    prog.SimulationStep(sim, ctx, *(const C3DAnimationBlendState*)bs, *(C3DAnimationManager*)am, (const C3DVector*)posv,
                        *(const C3DMesh2*)mesh);
    g_log[g_n] = 0;
    CHECK(std::strcmp(g_log, "DFMTSC") == 0);
    CHECK(g_cling[0] == &sim && g_cling[1] == bs && g_cling[2] == am && g_cling[3] == posv && g_cling[4] == mesh);
    CHECK(g_iter == 7 && g_dt == 0.25f);
    g_n = 0; sim.Size = 0;
    prog.SimulationStep(sim, ctx, *(const C3DAnimationBlendState*)bs, *(C3DAnimationManager*)am, (const C3DVector*)posv,
                        *(const C3DMesh2*)mesh);
    CHECK(g_n == 0);
    if (fails == 0) std::printf("00ae2d20_TEST PASS\n"); else std::printf("00ae2d20_TEST FAIL\n");
    return fails != 0;
}
