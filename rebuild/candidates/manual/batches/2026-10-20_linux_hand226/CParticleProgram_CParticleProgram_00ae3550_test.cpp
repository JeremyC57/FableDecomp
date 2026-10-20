#include <cstdio>
#include <cstring>
static char g_order[8]; static int g_n, g_streamCtors; static long g_resetSawHandle, g_clearSawHandle;
#include <new>
struct C4DVector;
struct CMovableResourceMemoryPool;

struct CMovableResource {                              // vtable 0x0129B860
    virtual ~CMovableResource() {}
};

struct CCountedPointer_RenderData {                    // CCountedPointer<CParticleSimulationRenderData>
    void* Object;
    void* Info;
    CCountedPointer_RenderData() : Object(0), Info(0) {}
};

struct C3DBoundingBox { float Min[3]; float Max[3]; };

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
    CParticleSimulation() : MemoryHandle(-1), MemoryPool(0) { Reset(); }
    virtual ~CParticleSimulation() {}
    void Reset() { g_order[g_n++] = 'R'; g_resetSawHandle = MemoryHandle; }
};

template <class T> struct CAllocator {                 // STLport allocator<T>: empty
    CAllocator() {}
};
template <class T> struct CAllocProxy : CAllocator<T> {  // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* _M_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), _M_data(p) {}
};
template <class T> struct CVectorBase {                // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) {}
};
template <class T> struct CVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) {}
};
template <class T> struct CArray : CVector<T> {        // retail 12 bytes: a vector
    CArray() {}
};

struct CKeyPairCompareLess {};
struct CVectorMap_ExportParticles {                    // CVectorMap<CCharString, unsigned long, CKeyPairCompareLess<...>>, 0x10
    CVector<void*>      Vector;                        // +0x00 vector<pair<CCharString, unsigned long> >
    CKeyPairCompareLess Compare;                       // +0x0c
    bool                Dirty;                         // +0x0d
    CVectorMap_ExportParticles() : Dirty(false) {}
};

struct CMemoryDataOutputStream {                       // retail 0x18 bytes
    CMemoryDataOutputStream() { ++g_streamCtors; g_order[g_n++] = 'S'; }
    unsigned char _body[0x18];
};

struct C3DTriangle2; struct C3DQuad2; struct C3DClothRenderVertex; struct C2DVector; struct C3DGroup2;

struct CParticleProgram : CMovableResource {           // retail 0x140 bytes, vtable 0x0129F0D8
    CParticleSimulation             InitialSimulation;     // +0x08
    CVectorMap_ExportParticles      ExportParticles;       // +0x88
    CArray<C3DTriangle2>            RenderTris;            // +0x98
    CArray<C3DQuad2>                RenderQuads;           // +0xa4
    CArray<C3DClothRenderVertex>    RenderVertices;        // +0xb0
    unsigned long                   NonSimCount;           // +0xbc
    C4DVector*                      NonSimPositions;       // +0xc0
    CArray<C2DVector>               IndexedTextureCoords;  // +0xc4
    CArray<C4DVector>               TempVertexPositions;   // +0xd0
    CArray<long>                    ParticleIndices;       // +0xdc
    CArray<long>                    VertexIndices;         // +0xe8
    bool                            BezierEnable;          // +0xf4
    float                           AveragePatchSize;      // +0xf8
    CArray<C3DGroup2>               SimGroups;             // +0xfc
    CArray<C3DGroup2>               NonSimGroups;          // +0x108
    CMemoryDataOutputStream         Constraints;           // +0x114
    CArray<long>                    LoopStack;             // +0x12c CArray<CParticleProgram::CLoopBegin>
    long                            MemoryHandle;          // +0x138
    CMovableResourceMemoryPool*     MemoryPool;            // +0x13c
    CParticleProgram();
    virtual ~CParticleProgram() {}
    void Clear() { g_order[g_n++] = 'C'; g_clearSawHandle = MemoryHandle; BezierEnable = false; AveragePatchSize = 0.0f; NonSimCount = 0; NonSimPositions = 0; }
};

CParticleProgram::CParticleProgram()
    : MemoryHandle(-1), MemoryPool(0)
{
    Clear();
}

static bool Empty(const void* const* p) { return p[0] == 0 && p[1] == 0 && p[2] == 0; }
template <class T> static bool EmptyV(const CVector<T>& v) { return v._M_start == 0 && v._M_finish == 0 && v._M_end_of_storage._M_data == 0; }
int main()
{
    static unsigned char raw[sizeof(CParticleProgram)];
    std::memset(raw, 0xcd, sizeof(raw));
    CParticleProgram* p = new (raw) CParticleProgram;
    if (std::strcmp(g_order, "RSC") || g_streamCtors != 1 || g_resetSawHandle != -1 || g_clearSawHandle != -1) { std::printf("00ae3550_TEST FAIL\n"); return 1; }
    const CParticleSimulation& s = p->InitialSimulation;
    if (s.RenderData.Object != 0 || s.RenderData.Info != 0 || s.MemoryHandle != -1 || s.MemoryPool != 0) { std::printf("00ae3550_TEST FAIL\n"); return 1; }
    if (!EmptyV(p->ExportParticles.Vector) || p->ExportParticles.Dirty != false) { std::printf("00ae3550_TEST FAIL\n"); return 1; }
    if (!EmptyV(p->RenderTris) || !EmptyV(p->RenderQuads) || !EmptyV(p->RenderVertices) || !EmptyV(p->IndexedTextureCoords) ||
        !EmptyV(p->TempVertexPositions) || !EmptyV(p->ParticleIndices) || !EmptyV(p->VertexIndices) || !EmptyV(p->SimGroups) ||
        !EmptyV(p->NonSimGroups) || !EmptyV(p->LoopStack)) { std::printf("00ae3550_TEST FAIL\n"); return 1; }
    if (p->MemoryHandle != -1 || p->MemoryPool != 0) { std::printf("00ae3550_TEST FAIL\n"); return 1; }
    (void)Empty;
    std::printf("00ae3550_TEST PASS\n");
    return 0;
}
