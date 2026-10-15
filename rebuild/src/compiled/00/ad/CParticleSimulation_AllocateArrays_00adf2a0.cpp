// CParticleSimulation::AllocateArrays(CMovableResourceMemoryPool*) -- allocates one movable block of Size * 0x44 bytes from the pool and carves it into the positions, last positions, saved positions, accelerations and alphas arrays
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADF2A0, 86 bytes (`ret 4`).
// Callees: CMovableResourceMemoryPool::AllocateResource(long*, CMovableResource*, ulong, ulong) @ 0x00A62E40,
// CHandleBasedMemoryPool::GetPointerFromHandle(long) @ 0x00A026B0.
// Shape: the PDB layout holds at the retail offsets (Size +0x18, the five arrays +0x1c..+0x2c, MemoryHandle +0x74,
// MemoryPool +0x78). The pool argument goes straight into ecx for the allocate call and its store to MemoryPool
// sinks below the pushes; the failure path returns the call's own `al`. The handle is read into a local before
// the pool (`mov eax,[edi] ; mov ecx,[esi+78h] ; push eax`; the member read inline is pushed before the pool
// load). The arrays are chained pointer additions of Size C4DVectors (16 bytes) each; the accelerations pointer
// is stored AFTER the alphas are derived from it (`add ecx,eax` keeps it live in eax): a local stored last.
struct C4DVector {
    float X, Y, Z, W;
};

struct CMovableResource {
    virtual void v00();
};

struct CHandleBasedMemoryPool {
    void* GetPointerFromHandle(long handle);         // 0x00A026B0
};

struct CMovableResourceMemoryPool : CHandleBasedMemoryPool {
    bool AllocateResource(long* handle, CMovableResource* owner, unsigned long size, unsigned long alignment);   // 0x00A62E40
};

struct CParticleSimulation : CMovableResource {
    unsigned char               RenderData[0x08];    // +0x04 CCountedPointer<CParticleSimulationRenderData>
    float                       Timestep;            // +0x0c
    bool                        TimestepChanged;     // +0x10
    float                       TimestepMultiplier;  // +0x14
    unsigned long               Size;                // +0x18
    C4DVector*                  Positions;           // +0x1c
    C4DVector*                  LastPositions;       // +0x20
    C4DVector*                  SavedPositions;      // +0x24
    C4DVector*                  Accelerations;       // +0x28
    float*                      SimulationAlphas;    // +0x2c
    unsigned char               m_pad30[0x44];
    long                        MemoryHandle;        // +0x74
    CMovableResourceMemoryPool* MemoryPool;          // +0x78
    bool AllocateArrays(CMovableResourceMemoryPool* pool);
};

bool CParticleSimulation::AllocateArrays(CMovableResourceMemoryPool* pool)
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