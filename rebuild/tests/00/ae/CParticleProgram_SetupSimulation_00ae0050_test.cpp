// Test for 0x00AE0050 CParticleProgram::SetupSimulation (shared headers): the simulation copies the program's InitialSimulation
// (+0x08) with the given pool, and the result is CopyFrom's.
#include <cstdio>
#include <string.h>
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_UNSKINNED_SIMULATION
#define FABLE_RETAIL_CPARTICLESIMULATION
#include "engine/CParticleSimulation.h"
#include "engine/CParticleProgram.h"

static const CParticleSimulation* g_captured_source = 0;
static CParticleSimulation* g_captured_dest = 0;
static CMovableResourceMemoryPool* g_captured_pool = 0;
static bool g_ret = false;

bool CParticleSimulation::CopyFrom(const CParticleSimulation& other, CMovableResourceMemoryPool& pool)
{
    g_captured_source = &other;
    g_captured_dest = this;
    g_captured_pool = &pool;
    return g_ret;
}

static double g_prog_storage[(sizeof(CParticleProgram) + 7) / 8];
static double g_sim_storage[(sizeof(CParticleSimulation) + 7) / 8];

int main()
{
    memset(g_prog_storage, 0, sizeof(g_prog_storage));
    const CParticleProgram& prog = *(const CParticleProgram*)g_prog_storage;
    CParticleSimulation& sim = *(CParticleSimulation*)g_sim_storage;
    CMovableResourceMemoryPool& pool = *(CMovableResourceMemoryPool*)0xABCD;

    g_ret = true;
    bool r1 = prog.SetupSimulation(sim, pool);
    g_ret = false;
    bool r2 = prog.SetupSimulation(sim, pool);

    bool ok = r1 && !r2
           && g_captured_source == (const CParticleSimulation*)((const char*)&prog + 8)
           && g_captured_dest == &sim
           && g_captured_pool == &pool;
    printf(ok ? "SETUPSIM_OK\n" : "SETUPSIM_FAIL\n");
    return ok ? 0 : 1;
}
