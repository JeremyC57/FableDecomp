// CParticleProgram::RemapBoneIndices(std::map<unsigned long, unsigned long>& remap) -- rewrite the bone indices of the simulated groups, then of the non-simulated groups, through the remap table
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE3A30, length 39 bytes (padded to 48).
// Leaf: RemapBoneIndices
// Label correction: the catalog "_Dest_val<..., std::pair<long, CMusicManager::CRegisteredMusicEntry>>" is propagated.
// Ego_r.pdb lists the pair ?RemapBoneIndices@CParticleProgram@@QAEXAAV?$map@KK...@@@Z (this row) and the private
// static __fastcall ?RemapBoneIndices@CParticleProgram@@CIXAAV?$map@KK...@@AAV?$CArray@VC3DGroup2@@@@@Z (0x00AE3920,
// manifest "InsertFromVector"). Only caller: the forwarding thunk 0x00AC3AF0 (`mov ecx,[ecx+4] ; jmp`), the twin of
// GetUsedBones' thunk 0x00AC3AE0.
// Callees: CParticleProgram::RemapBoneIndices(remap, groups) @ 0x00AE3920, twice.
// Layout: retail-offset local model as in GetUsedBones 0x00AE15D0 (engine/CParticleProgram.h holds the PDB offsets);
// std::map is only named, never touched.
// Shape: plain __fastcall wrapper; `remap` is held in edi and reloaded into ecx for each __fastcall call (no ecx
// knowledge needed here, unlike GetUsedBones).
namespace std {
template <class T> struct less {};
template <class T> struct allocator {};
template <class K, class V> struct pair {};
template <class K, class V, class C, class A> class map;
}
typedef std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > > CBoneRemap;
struct C4DVector;
struct CMovableResourceMemoryPool;
struct C3DTriangle2; struct C3DQuad2; struct C3DClothRenderVertex; struct C2DVector; struct C3DGroup2;
template <class T> struct CArray {                     // retail CArray<T>: STLport vector, three pointers
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
struct CMovableResource {                              // vtable 0x0129B860
    virtual ~CMovableResource();
};
class CParticleProgram : public CMovableResource {     // retail 0x140 bytes, vtable 0x0129F0D8
public:
    unsigned char                   _align_0x04[4];        // +0x04 (CParticleSimulation holds a double: 8-aligned)
    unsigned char                   InitialSimulation[0x80]; // +0x08 CParticleSimulation
    unsigned char                   ExportParticles[0x10];   // +0x88 CVectorMap<CCharString, unsigned long, ...>
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
    unsigned char                   Constraints[0x18];     // +0x114 CMemoryDataOutputStream
    CArray<long>                    LoopStack;             // +0x12c CArray<CParticleProgram::CLoopBegin>
    long                            MemoryHandle;          // +0x138
    CMovableResourceMemoryPool*     MemoryPool;            // +0x13c
    void RemapBoneIndices(CBoneRemap& remap);
private:
    static void __fastcall RemapBoneIndices(CBoneRemap& remap, CArray<C3DGroup2>& groups);   // 0x00AE3920
};
void CParticleProgram::RemapBoneIndices(CBoneRemap& remap)
{
    RemapBoneIndices(remap, SimGroups);
    RemapBoneIndices(remap, NonSimGroups);
}