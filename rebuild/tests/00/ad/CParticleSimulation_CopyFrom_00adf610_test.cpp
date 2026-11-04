#include <cstdio>
#include <cstring>
class C4DVector;
static bool g_failAlloc;
extern C4DVector g_buf[64];
class C4DVector { public: float X, Y, Z, W; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
class C3DBoundingBox { public:
    C3DVector Min;
    C3DVector Max;
};
class CMovableResourceMemoryPool;

class CMovableResource { public:
    virtual ~CMovableResource() {}
};

struct CCountedPointer_RenderData {                    // CCountedPointer<CParticleSimulationRenderData>
    void* Object;
    void* Info;
};

class CParticleSimulation : public CMovableResource { public:        // 0x80 bytes, vtable 0x0129F0C4
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
    bool AllocateArrays(CMovableResourceMemoryPool* pool)
    {
        MemoryPool = pool;
        if (g_failAlloc) return false;
        Positions = g_buf; LastPositions = g_buf + Size; SavedPositions = g_buf + 2 * Size; Accelerations = g_buf + 3 * Size;
        SimulationAlphas = (float*)(g_buf + 4 * Size);
        return true;
    }
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

C4DVector g_buf[64];
int main()
{
    static C4DVector src[4][3]; static float alphas[3];
    for (int a = 0; a < 4; ++a) for (int i = 0; i < 3; ++i) { src[a][i].X = (float)(a * 10 + i); src[a][i].Y = 1; src[a][i].Z = 2; src[a][i].W = 3; }
    for (int i = 0; i < 3; ++i) alphas[i] = 0.5f + i;
    CParticleSimulation t; std::memset(&t.RenderData, 0, sizeof(t.RenderData));
    t.Timestep = 0.25f; t.TimestepChanged = true; t.TimestepMultiplier = 2.0f; t.Size = 3;
    t.Positions = src[0]; t.LastPositions = src[1]; t.SavedPositions = src[2]; t.Accelerations = src[3]; t.SimulationAlphas = alphas;
    t.MaximumAdjustment = 9.0f; t.GravityStrength = 4.0f; t.WindStrength = 5.0f; t.DraggingEnable = true; t.DraggingRotational = false;
    t.DraggingStrength = 6.0f; t.AccelerationEnable = true; t.GlobalDamping = 7.0f; t.CurrentTime = 12.5;
    t.BoundingBox.Min.X = -1; t.BoundingBox.Min.Y = -2; t.BoundingBox.Min.Z = -3; t.BoundingBox.Max.X = 1; t.BoundingBox.Max.Y = 2; t.BoundingBox.Max.Z = 3;
    t.BoundingBoxValid = true;
    CParticleSimulation d; d.MaximumAdjustment = 42.0f; d.GravityStrength = 0; d.BoundingBoxValid = false;
    CMovableResourceMemoryPool* pool = (CMovableResourceMemoryPool*)&g_buf[63];
    g_failAlloc = true;
    if (d.InitializeFromTemplate(t, pool) != false || d.Size != 3 || d.Timestep != 0.25f || d.GravityStrength != 0 || d.MemoryPool != pool) { std::printf("00adf610_TEST FAIL\n"); return 1; }
    g_failAlloc = false;
    if (d.InitializeFromTemplate(t, pool) != true) { std::printf("00adf610_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (d.Positions[i].X != (float)i || d.LastPositions[i].X != (float)(10 + i) || d.SavedPositions[i].X != (float)(20 + i) ||
            d.Accelerations[i].X != (float)(30 + i) || d.Accelerations[i].W != 3 || d.SimulationAlphas[i] != 0.5f + i) { std::printf("00adf610_TEST FAIL\n"); return 1; }
    if (d.TimestepChanged != true || d.TimestepMultiplier != 2.0f || d.MaximumAdjustment != 42.0f || d.GravityStrength != 4.0f || d.WindStrength != 5.0f ||
        d.DraggingEnable != true || d.DraggingRotational != false || d.DraggingStrength != 6.0f || d.AccelerationEnable != true || d.GlobalDamping != 7.0f ||
        d.CurrentTime != 12.5 || d.BoundingBox.Min.Z != -3 || d.BoundingBox.Max.Y != 2 || d.BoundingBoxValid != true) { std::printf("00adf610_TEST FAIL\n"); return 1; }
    std::printf("00adf610_TEST PASS\n");
    return 0;
}