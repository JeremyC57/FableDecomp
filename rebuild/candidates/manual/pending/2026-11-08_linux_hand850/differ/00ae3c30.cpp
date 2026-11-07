// CParticleProgram::ReadFromFile(CDataInputStream&, CMovableResourceMemoryPool&) -- load a particle program in WriteToFile order: version, the raw constraint stream, the initial simulation, the render triangle / quad / vertex arrays, the non-simulated positions, texture coordinates, particle and vertex indices, patch size, bezier flag, the exported particle names, then both group arrays
// Grade: DIFFER(2278v2310) -- frame-slot / register allocation: retail shares the never-written default C3DTriangle2 resize temp with the saved-this slot (frame 0x10) and gives ebx the zero constant once `pool` is dead; ours puts the temp on the pair slot (frame 0xc) and keeps `this` in ebx (273 diff lines)
// Next idea: a resize(n) instance spelled with the STLport fill value built by the caller (explicit `C3DTriangle2 fill;` hoisted above the first resize, tried) did not move it; next try the CArray resize as a same-TU noinline instance of the real template (0x00A97A50) or reorder the member declarations of the reader TU
// Leaf: ?ReadFromFile@CParticleProgram@@QAE_NAAVCDataInputStream@@AAVCMovableResourceMemoryPool@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE3C30, 2310 bytes, `ret 8`.
// Identity: Ego_r.pdb ?ReadFromFile@CParticleProgram@@QAE_NAAVCDataInputStream@@AAVCMovableResourceMemoryPool@@@Z; the mirror of the
//   landed CParticleProgram::WriteToFile 0x00AE2230 (same field order; version 333 written, read and dropped here).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of the inline Read), CMemoryDataOutputStream::Clear
//   0x00996B60, CDataInputStream::CopyTo 0x009964E0, CParticleSimulation::ReadFromFile 0x00AE1BC0, the STLport vector resize
//   instances (resize(n, x): C3DTriangle2 0x00A97A50, C3DClothRenderVertex 0x00AE7830, long 0x004428F0; resize(n): C3DQuad2
//   0x00AE7AA0, C2DVector 0x00AE7B10, pair<CCharString,unsigned long> 0x00AE7D10), AllocateArrays 0x00ADF460,
//   CParticleSimulation::ReadVectorArray 0x00AE1730, the stream's virtual ReadNullTerminatedString (slot 6), CCharString copy
//   0x0099EC30 / dtor 0x0099EAE0, vector<pair<CCharString,unsigned long> >::_M_insert_overflow 0x009B71B0, the CVectorMap sort
//   0x009B8EF0 (__fastcall), ReadGroups 0x00AE3A60 (__fastcall).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_COPYTO
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_CLEAR
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_READ
#define FABLE_RETAIL_CPARTICLESIMULATION
#define FABLE_RETAIL_CPARTICLESIMULATION_READ
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CDataInputStream.h"
#include "engine/CParticleProgram.h"

// Retail calls these resize(n) instances out of line (the default value built inside them); the others expand resize(n).
template<> void std::vector<C3DQuad2>::resize(size_type n);
template<> void std::vector<C2DVector>::resize(size_type n);
template<> void std::vector<std::pair<CCharString,unsigned long> >::resize(size_type n);

bool CParticleProgram::ReadFromFile(CDataInputStream& in, CMovableResourceMemoryPool& pool)
{
    in.ReadULONG();
    Constraints.Clear();
    in.CopyTo(Constraints, in.ReadULONG());
    if (!InitialSimulation.ReadFromFile(in, pool))
        return false;
    RenderTris.resize(in.ReadULONG());
    in.Read(RenderTris.GetAsCArray(), RenderTris.size() * sizeof(C3DTriangle2));
    RenderQuads.resize(in.ReadULONG());
    in.Read(RenderQuads.GetAsCArray(), RenderQuads.size() * sizeof(C3DQuad2));
    RenderVertices.resize(in.ReadULONG());
    in.Read(RenderVertices.GetAsCArray(), RenderVertices.size() * sizeof(C3DClothRenderVertex));
    NonSimCount = in.ReadULONG();
    if (!AllocateArrays(NonSimCount + InitialSimulation.GetParticleCount(), pool))
        return false;
    CParticleSimulation::ReadVectorArray(NonSimPositions, NonSimCount, in, 0.0f);
    IndexedTextureCoords.resize(in.ReadULONG());
    in.Read(IndexedTextureCoords.GetAsCArray(), IndexedTextureCoords.size() * sizeof(C2DVector));
    ParticleIndices.resize(in.ReadULONG());
    in.Read(ParticleIndices.GetAsCArray(), ParticleIndices.size() * sizeof(long));
    VertexIndices.resize(in.ReadULONG());
    in.Read(VertexIndices.GetAsCArray(), VertexIndices.size() * sizeof(long));
    AveragePatchSize = in.ReadFloat();
    BezierEnable = in.ReadEBOOL();
    unsigned long no_export_particles = in.ReadULONG();
    ExportParticles.resize(no_export_particles);
    for (unsigned long c = 0; c < no_export_particles; ++c) {
        CCharString name = in.ReadNullTerminatedString();
        unsigned long index = in.ReadULONG();
        ExportParticles.Insert(std::pair<CCharString,unsigned long>(name, index));
    }
    ExportParticles.Update();
    ReadGroups(in, SimGroups);
    ReadGroups(in, NonSimGroups);
    return true;
}
