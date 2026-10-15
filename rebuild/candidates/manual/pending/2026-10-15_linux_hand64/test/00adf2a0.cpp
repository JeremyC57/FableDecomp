#include <cstdio>
// Behaviour of CParticleSimulation::AllocateArrays (0x00ADF2A0): the pool is remembered, one Size*0x44-byte block
// is allocated for the simulation, and the five arrays are carved from it back to back; a failed allocation
// returns false and leaves the arrays alone.
struct C4DVector { float X, Y, Z, W; };
struct CMovableResource { virtual ~CMovableResource() {} };
static unsigned char g_block[0x44 * 8];
struct CHandleBasedMemoryPool {
    void* GetPointerFromHandle(long handle) { return handle == 5 ? g_block : 0; }
};
struct CMovableResourceMemoryPool : CHandleBasedMemoryPool {
    bool m_fail; CMovableResource* m_owner; unsigned long m_size;
    bool AllocateResource(long* handle, CMovableResource* owner, unsigned long size, unsigned long)
    {
        if (m_fail) return false;
        *handle = 5; m_owner = owner; m_size = size; return true;
    }
};
struct CParticleSimulation : CMovableResource {
    unsigned long Size;
    C4DVector* Positions; C4DVector* LastPositions; C4DVector* SavedPositions; C4DVector* Accelerations; float* SimulationAlphas;
    long MemoryHandle;
    CMovableResourceMemoryPool* MemoryPool;
    bool AllocateArrays(CMovableResourceMemoryPool* pool)
    {
        MemoryPool = pool;
        if (!pool->AllocateResource(&MemoryHandle, this, Size * 0x44, 0))
            return false;
        long handle = MemoryHandle;
        Positions = (C4DVector*)MemoryPool->GetPointerFromHandle(handle);
        LastPositions = Positions + Size;
        SavedPositions = LastPositions + Size;
        C4DVector* accelerations = SavedPositions + Size;
        SimulationAlphas = (float*)(accelerations + Size);
        Accelerations = accelerations;
        return true;
    }
};
int main()
{
    CMovableResourceMemoryPool pool; pool.m_fail = false;
    CParticleSimulation s; s.Size = 8; s.Positions = 0; s.Accelerations = 0;
    if (!s.AllocateArrays(&pool) || s.MemoryPool != &pool || pool.m_owner != &s || pool.m_size != 8 * 0x44 || s.MemoryHandle != 5) { std::printf("00adf2a0_TEST FAIL\n"); return 1; }
    unsigned char* b = g_block;
    if ((unsigned char*)s.Positions != b || (unsigned char*)s.LastPositions != b + 8 * 16 || (unsigned char*)s.SavedPositions != b + 16 * 16 ||
        (unsigned char*)s.Accelerations != b + 24 * 16 || (unsigned char*)s.SimulationAlphas != b + 32 * 16) { std::printf("00adf2a0_TEST FAIL\n"); return 1; }
    CParticleSimulation t; t.Size = 2; t.Positions = 0; pool.m_fail = true;
    if (t.AllocateArrays(&pool) || t.Positions != 0 || t.MemoryPool != &pool) { std::printf("00adf2a0_TEST FAIL\n"); return 1; }
    std::printf("00adf2a0_TEST PASS\n");
    return 0;
}
