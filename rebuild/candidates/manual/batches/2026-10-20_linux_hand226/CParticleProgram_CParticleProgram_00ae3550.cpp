// CParticleProgram::CParticleProgram() -- default constructor: the embedded initial simulation (counted render data cleared, no memory block, then reset), the export-particle map, the render / index / group arrays, the constraint stream and the loop stack all empty, no memory block, then Clear()
// Leaf: ??0CParticleProgram
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE3550, 285 bytes, `ret`. Called from 0x00AC3D4F and 0x00AC3E40.
// Callees: CParticleSimulation::Reset 0x00ADF1E0 (the inlined CParticleSimulation constructor's body),
//   CMemoryDataOutputStream::CMemoryDataOutputStream() 0x00996AF0, CParticleProgram::Clear 0x00AE3360 (PDB
//   ?Clear@CParticleProgram@@QAEXXZ; catalogued as a propagated "allocator::construct"). Vtables: CParticleProgram 0x0129F0D8,
//   CParticleSimulation 0x0129F0C4 (rtti-vtables.tsv).
// Layout: engine/CParticleProgram.h carries the Ego_r.pdb offsets, which hold only up to +0x88: retail CVectorMap is 0x10
//   bytes (12-byte STLport vector, empty comparator, Dirty +0x0D), every CArray is 12 bytes and CMemoryDataOutputStream 0x18,
//   so from ExportParticles on everything sits lower (RenderTris +0x98, IndexedTextureCoords +0xC4, BezierEnable +0xF4,
//   SimGroups +0xFC, Constraints +0x114, LoopStack +0x12C, MemoryHandle +0x138, MemoryPool +0x13C; 0x140 bytes). A local
//   model with the PDB names at the retail offsets is used. CParticleSimulation (0x80, PDB layout holds) keeps its vptr in
//   the CMovableResource base so its CurrentTime double makes it 8-aligned at +8 without padding its own vptr.
// Shape: the vptr is stored before the member constructors; the inlined CParticleSimulation constructor stores its vtable
//   (the CMovableResource one is dead), the counted pointer's two zero words, MemoryHandle -1 / MemoryPool 0 and calls Reset;
//   the vector-map and arrays are STLport vectors (`_Vector_base` with the end-of-storage word in an `_STLP_alloc_proxy`
//   built from an `allocator()` temporary: that model places the Constraints `lea` before the last zero word) sharing
//   the ebx zero register.
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
    virtual ~CParticleSimulation();                    // 0x00AE0630
    void Reset();                                      // 0x00ADF1E0
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
    CMemoryDataOutputStream();                         // 0x00996AF0
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
    virtual ~CParticleProgram();
    void Clear();                                      // 0x00AE3360
};

CParticleProgram::CParticleProgram()
    : MemoryHandle(-1), MemoryPool(0)
{
    Clear();
}
