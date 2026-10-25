#include <cstdio>
#include <cstring>
class CParticleSimulationRenderData;
class C4DVector;
class CMovableResourceMemoryPool {
public:
    void FreeResource(long handle);
};
static int g_freed = 0;
static long g_handle = -999;
static void* g_pool = 0;
void CMovableResourceMemoryPool::FreeResource(long handle) { g_freed++; g_handle = handle; g_pool = this; }

class CMovableResource {
public:
    virtual ~CMovableResource();
};
class CParticleSimulation : public CMovableResource {
public:
    void Clear();
    CParticleSimulationRenderData* RenderData[2];
    float Timestep;
    bool TimestepChanged;
    float TimestepMultiplier;
    unsigned long Size;
    C4DVector* Positions;
    C4DVector* LastPositions;
    C4DVector* SavedPositions;
    C4DVector* Accelerations;
    float* SimulationAlphas;
    float MaximumAdjustment;
    float GravityStrength;
    float WindStrength;
    bool DraggingEnable;
    bool DraggingRotational;
    float DraggingStrength;
    bool AccelerationEnable;
    float GlobalDamping;
    double CurrentTime;
    float BoundingBox[6];
    bool BoundingBoxValid;
    long MemoryHandle;
    CMovableResourceMemoryPool* MemoryPool;
};

int main() {
    if (sizeof(CParticleSimulation) != 0x80) { std::printf("FAIL size %d\n", (int)sizeof(CParticleSimulation)); return 1; }
    unsigned char buf[0x80];
    std::memset(buf, 0xAB, sizeof buf);
    CParticleSimulation* p = (CParticleSimulation*)buf;
    CMovableResourceMemoryPool* pool = (CMovableResourceMemoryPool*)0x12345678;
    p->MemoryHandle = 42;
    p->MemoryPool = pool;
    p->Clear();
    if (g_freed != 1 || g_handle != 42 || g_pool != pool) { std::printf("FAIL free\n"); return 1; }
    if (p->MemoryHandle != -1 || p->TimestepChanged != false || p->CurrentTime != 0.0) { std::printf("FAIL a\n"); return 1; }
    if (p->Positions || p->LastPositions || p->SavedPositions || p->Accelerations || p->SimulationAlphas || p->MaximumAdjustment != 0.0f) { std::printf("FAIL b\n"); return 1; }
    if (*(unsigned int*)(buf + 0x0c) != 0x3d888889u || p->TimestepMultiplier != 1.0f || p->GlobalDamping != 0.25f) { std::printf("FAIL c\n"); return 1; }
    if (*(unsigned int*)(buf + 0x18) != 0xABABABABu) { std::printf("FAIL size touched\n"); return 1; }
    g_freed = 0;
    p->Clear();
    if (g_freed != 0) { std::printf("FAIL freed on -1\n"); return 1; }
    std::printf("00adf1e0_TEST PASS\n");
    return 0;
}
