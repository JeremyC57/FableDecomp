// CParticleProgram::GetUsedBones(CArray<bool>& used) const -- mark the bones used by the simulated groups, then by the non-simulated groups
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE15D0, length 33 bytes (padded to 48).
// Leaf: ?GetUsedBones@CParticleProgram@@QBE
// Label correction: the catalog "_Dest_val<..., std::pair<long, CMusicManager::CRegisteredMusicEntry>>" is propagated.
// Ego_r.pdb lists the pair ?GetUsedBones@CParticleProgram@@QBEXAAV?$CArray@_N@@@Z (this row) and the private static
// __fastcall ?GetUsedBones@CParticleProgram@@CIXAAV?$CArray@_N@@ABV?$CArray@VC3DGroup2@@@@@Z (0x00AE1210, manifest
// "GetUsedBones"). Only caller: the forwarding thunk 0x00AC3AE0 (`mov ecx,[ecx+4] ; jmp`, the owning resource's
// pointer). SimGroups / NonSimGroups sit at retail +0xFC / +0x108.
// Callees: CParticleProgram::GetUsedBones(used, groups) @ 0x00AE1210, twice.
// Layout: engine/CParticleProgram.h carries the Ego_r.pdb offsets (16-byte CArrays, SimGroups +0x11C); retail CArrays
// are 12 bytes, so the retail-offset model of the landed constructor 0x00AE3550 is used locally (InitialSimulation is
// 8-aligned at +8 because CParticleSimulation holds a double).
// Shape: retail does NOT reload ecx for the second call: whole-program knowledge that 0x00AE1210 leaves ecx intact.
// The static helper's body is compiled in this TU as a stand-in under `#pragma auto_inline(off)` (GOTCHAS 1063 /
// wave 25 precedent 0x75D2B0) so VC7.1 sees the same; it is written from the retail bytes of 0x00AE1210 (signed
// `size() > 0` test = `test ebx,0FFFFFFF8h ; jle`, the group array re-read after every byte store). Without the
// stand-in the build keeps `used` in edi and reloads ecx (39v33).
struct C4DVector;
struct CMovableResourceMemoryPool;
struct C3DTriangle2; struct C3DQuad2; struct C3DClothRenderVertex; struct C2DVector;
template <class T> struct CArray {                     // retail CArray<T>: STLport vector, three pointers
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int i) { return _M_start[i]; }
    const T& operator[](unsigned int i) const { return _M_start[i]; }
};
struct C3DVertexBlend2 { long Vertex; float Weight; };   // 8 bytes
struct C3DGroup2 {                                     // 16 bytes in retail
    long                     BoneIndex;                // +0x0
    CArray<C3DVertexBlend2>  VertexBlends;             // +0x4
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
    void GetUsedBones(CArray<bool>& used) const;
private:
    static void __fastcall GetUsedBones(CArray<bool>& used, const CArray<C3DGroup2>& groups);   // 0x00AE1210
};
#pragma auto_inline(off)
void __fastcall CParticleProgram::GetUsedBones(CArray<bool>& used, const CArray<C3DGroup2>& groups)
{
    for (unsigned int i = 0; i < groups.size(); ++i)
        if ((long)groups[i].VertexBlends.size() > 0)
            used[groups[i].BoneIndex] = true;
}
#pragma auto_inline(on)
void CParticleProgram::GetUsedBones(CArray<bool>& used) const
{
    GetUsedBones(used, SimGroups);
    GetUsedBones(used, NonSimGroups);
}