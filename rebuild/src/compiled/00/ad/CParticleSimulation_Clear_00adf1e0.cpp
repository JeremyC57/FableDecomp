// CParticleSimulation::Clear (Ego_r.pdb `?Clear@CParticleSimulation@@QAEXXZ`): resets the simulation
// state and frees the pooled particle buffers. Member names/offsets from Ego_r.pdb
// (ghidra_out/struct_layouts_egor.tsv, sizeof 0x80); no shared engine header exists for this class.
// Callee 0xA62ED0 is CMovableResourceMemoryPool::FreeResource (5-byte jmp to CHandleBasedMemoryPool::Free).
// 0.0 is VC7.1's shared `__real@0000000000000000` constant (0x122ED70, 188 readers).
// Leaf: ?Clear@CParticleSimulation@@QAEXXZ
class CParticleSimulationRenderData;
class CMovableResourceMemoryPool {
public:
    void FreeResource(long handle);
};
class C4DVector;

class CMovableResource {                            // retail RTTI: .?AVCParticleSimulation@@ : .?AVCMovableResource@@
public:                                             // (vtable 0x0129F0C4)
    virtual ~CMovableResource();
};

class CParticleSimulation : public CMovableResource {   // base vptr +0x00
public:
    void Clear();


    CParticleSimulationRenderData* RenderData[2];   // +0x04 CCountedPointer<CParticleSimulationRenderData>
    float Timestep;                                 // +0x0c
    bool TimestepChanged;                           // +0x10
    float TimestepMultiplier;                       // +0x14
    unsigned long Size;                             // +0x18
    C4DVector* Positions;                           // +0x1c
    C4DVector* LastPositions;                       // +0x20
    C4DVector* SavedPositions;                      // +0x24
    C4DVector* Accelerations;                       // +0x28
    float* SimulationAlphas;                        // +0x2c
    float MaximumAdjustment;                        // +0x30
    float GravityStrength;                          // +0x34
    float WindStrength;                             // +0x38
    bool DraggingEnable;                            // +0x3c
    bool DraggingRotational;                        // +0x3d
    float DraggingStrength;                         // +0x40
    bool AccelerationEnable;                        // +0x44
    float GlobalDamping;                            // +0x48
    double CurrentTime;                             // +0x50
    float BoundingBox[6];                           // +0x58 C3DBoundingBox
    bool BoundingBoxValid;                          // +0x70
    long MemoryHandle;                              // +0x74
    CMovableResourceMemoryPool* MemoryPool;         // +0x78
};

void CParticleSimulation::Clear()
{
    TimestepChanged = false;
    CurrentTime = 0.0;
    MaximumAdjustment = 0.0f;
    Positions = 0;
    LastPositions = 0;
    SavedPositions = 0;
    Accelerations = 0;
    SimulationAlphas = 0;
    Timestep = 1.0f / 15.0f;
    TimestepMultiplier = 1.0f;
    GlobalDamping = 0.25f;
    if (MemoryHandle != -1) {
        MemoryPool->FreeResource(MemoryHandle);
        MemoryHandle = -1;
    }
}
