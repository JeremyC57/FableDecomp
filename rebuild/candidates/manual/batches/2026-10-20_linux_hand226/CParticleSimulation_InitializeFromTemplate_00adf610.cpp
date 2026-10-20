// CParticleSimulation::InitializeFromTemplate(const CParticleSimulation& source, CMovableResourceMemoryPool* pool) -- copy the timestep settings and particle count from a template simulation, allocate this simulation's arrays from the pool (false on failure), then copy the five particle arrays and the remaining tuning fields, time and bounding box
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADF610, 286 bytes, `ret 8`.
// Callees: CParticleSimulation::AllocateArrays(CMovableResourceMemoryPool*) 0x00ADF2A0 (landed).
// Layout: CParticleSimulation from Ego_r.pdb (0x80 bytes; the landed AllocateArrays 0x00ADF2A0 confirms the offsets):
//   Timestep +0x0C, TimestepChanged +0x10, TimestepMultiplier +0x14, Size +0x18, Positions/LastPositions/SavedPositions/
//   Accelerations +0x1C..+0x28 (C4DVector, 16 bytes), SimulationAlphas +0x2C (float), GravityStrength +0x34, WindStrength
//   +0x38, DraggingEnable/DraggingRotational +0x3C/+0x3D, DraggingStrength +0x40, AccelerationEnable +0x44, GlobalDamping
//   +0x48, CurrentTime +0x50 (double), BoundingBox +0x58, BoundingBoxValid +0x70. MaximumAdjustment (+0x30) is not copied.
//   No engine header exists for CParticleSimulation: a local model with the PDB names is used (vptr in the CMovableResource
//   base, so the double keeps the class 8-aligned without padding the vptr).
// Shape: each array is an intrinsic memcpy of Size elements (`shl ; shr 2 ; rep movsd ; and 3 ; rep movsb`), Size re-read
//   after every copy (the copy may alias it). The bounding box is assigned through C3DVector's implicit operator= next to
//   a user copy ctor, which gives the `lea` source/destination pointer copy of six dwords.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct C4DVector { float X, Y, Z, W; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
struct C3DBoundingBox {
    C3DVector Min;
    C3DVector Max;
};
struct CMovableResourceMemoryPool;

struct CMovableResource {
    virtual ~CMovableResource();
};

struct CCountedPointer_RenderData {                    // CCountedPointer<CParticleSimulationRenderData>
    void* Object;
    void* Info;
};

struct CParticleSimulation : CMovableResource {        // 0x80 bytes, vtable 0x0129F0C4
    CCountedPointer_RenderData  RenderData;            // +0x04
    float                       Timestep;              // +0x0c
    bool                        TimestepChanged;       // +0x10
    float                       TimestepMultiplier;    // +0x14
    unsigned long               Size;                  // +0x18
    C4DVector*                  Positions;             // +0x1c
    C4DVector*                  LastPositions;         // +0x20
    C4DVector*                  SavedPositions;        // +0x24
    C4DVector*                  Accelerations;         // +0x28
    float*                      SimulationAlphas;      // +0x2c
    float                       MaximumAdjustment;     // +0x30
    float                       GravityStrength;       // +0x34
    float                       WindStrength;          // +0x38
    bool                        DraggingEnable;        // +0x3c
    bool                        DraggingRotational;    // +0x3d
    float                       DraggingStrength;      // +0x40
    bool                        AccelerationEnable;    // +0x44
    float                       GlobalDamping;         // +0x48
    double                      CurrentTime;           // +0x50
    C3DBoundingBox              BoundingBox;           // +0x58
    bool                        BoundingBoxValid;      // +0x70
    long                        MemoryHandle;          // +0x74
    CMovableResourceMemoryPool* MemoryPool;            // +0x78
    bool AllocateArrays(CMovableResourceMemoryPool* pool);   // 0x00ADF2A0
    bool InitializeFromTemplate(const CParticleSimulation& source, CMovableResourceMemoryPool* pool);
};

bool CParticleSimulation::InitializeFromTemplate(const CParticleSimulation& source, CMovableResourceMemoryPool* pool)
{
    Timestep = source.Timestep;
    TimestepChanged = source.TimestepChanged;
    TimestepMultiplier = source.TimestepMultiplier;
    Size = source.Size;
    if (!AllocateArrays(pool))
        return false;
    memcpy(Positions, source.Positions, Size * sizeof(C4DVector));
    memcpy(LastPositions, source.LastPositions, Size * sizeof(C4DVector));
    memcpy(SavedPositions, source.SavedPositions, Size * sizeof(C4DVector));
    memcpy(Accelerations, source.Accelerations, Size * sizeof(C4DVector));
    memcpy(SimulationAlphas, source.SimulationAlphas, Size * sizeof(float));
    GravityStrength = source.GravityStrength;
    WindStrength = source.WindStrength;
    DraggingEnable = source.DraggingEnable;
    DraggingRotational = source.DraggingRotational;
    DraggingStrength = source.DraggingStrength;
    AccelerationEnable = source.AccelerationEnable;
    GlobalDamping = source.GlobalDamping;
    CurrentTime = source.CurrentTime;
    BoundingBox = source.BoundingBox;
    BoundingBoxValid = source.BoundingBoxValid;
    return true;
}
