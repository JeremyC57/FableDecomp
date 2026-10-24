// CParticleSimulation::CParticleSimulation() -- construct an empty cloth/particle simulation: no render data, no movable-resource memory (handle -1), then Clear() the simulation parameters
// Leaf: ??0CParticleSimulation@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE0600, 0x24 bytes (+ int3 padding), thiscall, returns this.
// Identity: stores vtable 0x0129F0C4 (RTTI .?AVCParticleSimulation@@ over CMovableResource; slot 1 the landed
// RelocateData 0x00ADFC60); Ego_r.pdb ??0CParticleSimulation@@QAE@XZ with CParticleSimulation (128 bytes):
// RenderData CCountedPointer<CParticleSimulationRenderData> +4, MemoryHandle +0x74, MemoryPool +0x78. Callers
// 0x00B82033 / 0x00B82243. Catalog "GetDefaultVal<CPhysicalPrimitiveInit>" is propagated.
// Callee: CParticleSimulation::Clear() @ 0x00ADF1E0 (landed as "CLandscapeBackgroundPatch_ClearTextures", a
// propagated label: its stores are Clear's -- TimestepChanged +0x10, CurrentTime +0x50, the arrays, Timestep 1/15,
// TimestepMultiplier 1, GlobalDamping 0.25, and the MemoryHandle release through MemoryPool).
// Shape: the derived vtable store (the empty inline CMovableResource constructor's store is dead), the counted pointer's
// two zero words, `MemoryHandle(-1), MemoryPool(0)` from one zero register, then `Clear();` in the body.
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    CCountedPointer() : Data(0), Info(0) { }
};

class CMovableResourceMemoryPool;

class CMovableResource {                                    // 4 bytes (the vtable)
public:
    CMovableResource() { }
    virtual ~CMovableResource() { }
    virtual void RelocateData(long offset, void* oldBase, void* newBase, long size) = 0;
};

struct C4DVector { float X, Y, Z, W; };
struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox { C3DVector Min; C3DVector Max; };
class CParticleSimulationRenderData;

class CParticleSimulation : public CMovableResource {      // Ego_r.pdb, 128 bytes
public:
    CCountedPointer<CParticleSimulationRenderData> RenderData;   // +0x04
    float                       Timestep;                   // +0x0C
    bool                        TimestepChanged;            // +0x10
    float                       TimestepMultiplier;         // +0x14
    unsigned long               Size;                       // +0x18
    C4DVector*                  Positions;                  // +0x1C
    C4DVector*                  LastPositions;              // +0x20
    C4DVector*                  SavedPositions;             // +0x24
    C4DVector*                  Accelerations;              // +0x28
    float*                      SimulationAlphas;           // +0x2C
    float                       MaximumAdjustment;          // +0x30
    float                       GravityStrength;            // +0x34
    float                       WindStrength;               // +0x38
    bool                        DraggingEnable;             // +0x3C
    bool                        DraggingRotational;         // +0x3D
    float                       DraggingStrength;           // +0x40
    bool                        AccelerationEnable;         // +0x44
    float                       GlobalDamping;              // +0x48
    double                      CurrentTime;                // +0x50
    C3DBoundingBox              BoundingBox;                // +0x58
    bool                        BoundingBoxValid;           // +0x70
    long                        MemoryHandle;               // +0x74
    CMovableResourceMemoryPool* MemoryPool;                 // +0x78

    CParticleSimulation();
    virtual ~CParticleSimulation();
    virtual void RelocateData(long offset, void* oldBase, void* newBase, long size);   // 0x00ADFC60
    void Clear();                                           // 0x00ADF1E0
};

CParticleSimulation::CParticleSimulation()
    : MemoryHandle(-1), MemoryPool(0)
{
    Clear();
}