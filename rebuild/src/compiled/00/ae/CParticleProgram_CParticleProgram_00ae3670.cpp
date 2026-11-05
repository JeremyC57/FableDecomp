// CParticleProgram::~CParticleProgram() -- clears the program, then destroys its members in reverse order: the loop stack, the constraint stream, the two group arrays, the index / position / texture-coordinate arrays, the render arrays, the export-particle map and the embedded initial simulation
// Leaf: ??1CParticleProgram@@UAE@XZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Grades: 1 (RELOCATION_MATCH first grade)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE3670, 338 bytes, thiscall, plain `ret`.
// Identity: the first vtable store is 0x0129F0D8 = CParticleProgram (rtti-vtables.tsv) and Ego_r.pdb names
//   ??1CParticleProgram@@UAE@XZ (virtual); the landed scalar deleting destructor 0x00AE8520 calls it. The catalog label
//   "CLandscapeBackgroundPatch_destructor" is propagated.
// Callees: CParticleProgram::Clear 0x00AE3360 (the user body), free 0x00BFEA14 (each inline STLport vector dtor),
//   ??1CMemoryDataOutputStream@@UAE@XZ 0x00996B30 (Constraints), std::vector<C3DGroup2>::~vector 0x00AE7EF0 (SimGroups /
//   NonSimGroups; landed), ??1CCharString@@QAE@XZ 0x0099EAE0 (the export map's pair keys), CParticleSimulation::Clear
//   0x00ADF1E0 (the inlined ~CParticleSimulation, as the out-of-line copy 0x00AE0630), operator delete 0x00BFE9BC (the
//   counted render-data pointer's count block).
// Layout: engine/CParticleProgram.h holds the right retail offsets but flattens every member to bytes and the vptr to a
//   `void* __vftable`, so no member destructor can be expressed through it; the members are modelled locally with the PDB
//   names at those offsets (as the landed ctor 0x00AE3550). Retail CVectorMap is 0x10 (12-byte STLport vector, empty
//   comparator, Dirty), CArray a 12-byte STLport vector, CMemoryDataOutputStream 0x18, CParticleSimulation 0x80.
// Shape: CMovableResource's inline user dtor leaves its vtable store (twice: the simulation member, then the base);
//   ~vector<C3DGroup2> stays a call, the other vectors inline (their trivially destroyed elements leave no loop).
class C4DVector { public: float X, Y, Z, W; };
class C2DVector { public: float X, Y; };
class C3DTriangle2 { public: unsigned short VertexIndex[3]; };
class C3DQuad2 { public: unsigned short VertexIndex[4]; };
class C3DClothRenderVertex { public: unsigned short PositionIndex; unsigned short TexCoordIndex; };
class CMovableResourceMemoryPool;
class CParticleSimulationRenderData;

extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
void operator delete(void* p);                               // 0x00BFE9BC

#include "engine/CCountedPointer.h"

#include "engine/CArray.h"


class C3DVertexBlend2 { public: unsigned long VertexIndex; float BlendValue; };
class C3DGroup2 { public:                                     // 16 bytes in retail
    long BoneIndex;                                           // +0x00
    CArray<C3DVertexBlend2> VertexBlends;                     // +0x04
};

class CCharStringData;
class CCharString { public:                                   // 4 bytes
    ~CCharString();                                           // 0x0099EAE0
    CCharStringData* PStringData;
};

#include "engine/CVectorMap.h"

class CMemoryDataOutputStream { public:                       // retail 0x18 bytes, vtable
    virtual ~CMemoryDataOutputStream();                       // 0x00996B30
    unsigned char _pad_0x04[0x4];
    CArray<char> Data;                                        // +0x08
    unsigned long CurrentPosition;                            // +0x14
};

class CMovableResource { public:                              // vtable 0x0129B860
    virtual ~CMovableResource() { }
};

class C3DBoundingBox { public: float Min[3]; float Max[3]; };

class CParticleSimulation : public CMovableResource { public:  // 0x80 bytes, vtable 0x0129F0C4
    CCountedPointer<CParticleSimulationRenderData> RenderData; // +0x04
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
    virtual ~CParticleSimulation() { Clear(); }        // out of line at 0x00AE0630
    void Clear();                                      // 0x00ADF1E0
};

class CParticleProgram : public CMovableResource { public:    // retail 0x140 bytes, vtable 0x0129F0D8
    class CLoopBegin { public: unsigned long Position; unsigned long Count; };   // 8 bytes (Ego_r)
    CParticleSimulation             InitialSimulation;     // +0x08
    CVectorMap<CCharString, unsigned long, CKeyPairCompareLess<CCharString, unsigned long> > ExportParticles;   // +0x88
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
    CArray<CLoopBegin>              LoopStack;             // +0x12c
    long                            MemoryHandle;          // +0x138
    CMovableResourceMemoryPool*     MemoryPool;            // +0x13c
    virtual ~CParticleProgram();
    void Clear();                                          // 0x00AE3360
};

CParticleProgram::~CParticleProgram()
{
    Clear();
}