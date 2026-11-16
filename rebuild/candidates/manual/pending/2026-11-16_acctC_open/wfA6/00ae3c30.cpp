// CParticleProgram::ReadFromFile(CDataInputStream&, CMovableResourceMemoryPool&) -- load a particle program in WriteToFile order: version, the raw constraint stream, the initial simulation, the render triangle / quad / vertex arrays, the non-simulated positions, texture coordinates, particle and vertex indices, patch size, bezier flag, the exported particle names, then both group arrays
// Grade: DIFFER(2310v2310) -- (wfA6n) real 12, equal length, frame 0x10, const 0 in ebx and all 11 this reloads = retail. Left: the 6 lines 0x00AE40AC..0x00AE40B8 of the IndexedTextureCoords data select: retail `mov eax,[esi] ; mov ecx,[esi+4] ; sub ecx,eax ; sar ecx,3 ; test ecx,ecx ; mov edi,eax ; ja`, ours `mov edi,[esi] ; mov eax,[esi+4] ; mov ebx,edi ; sub eax,ebx ; sar eax,3 ; test eax,eax ; ja` (who owns the _M_start load). FITTED: that site spelled as an explicit select (data = begin(); if (!(size() > 0)) data = zero-size pointer) instead of GetAsCArray(): it defines the data pointer (edi) in the join block, which ends the esi piece of `this` before the site (see NOTES wfA6n). FITTED (wfA6j): declared count n.
// Next idea (wfA6o): the ternary GetAsCArray at the IndexedTextureCoords site is EVIDENCED (all five retail sites byte-identical) but no lever found that ends the esi `this` piece with it (k 0..7, dead asserts at 4 positions, ReadVectorArray / resize(n) bodies visible, NonSim/AllocateArrays respellings, array views, /Oa: all inert at real 127); the select (FITTED) is the only known closer. wfA6o dropped FABLE_RETAIL_CARRAY_GETASCARRAY_START: the default `&front()` text (= FableWin 19i) is byte-identical here.
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
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_RESERVE
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_READ
#define FABLE_RETAIL_CPARTICLEPROGRAM_RENDERACCESS
#define FABLE_RETAIL_C3DCLOTHRENDERVERTEX_CTOR
#define FABLE_RETAIL_CPARTICLESIMULATION
#define FABLE_RETAIL_CPARTICLESIMULATION_READFROMFILE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_SORT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR                      // EVIDENCED: retail TUs see STLport _M_fill_insert (frame 0x10, temp at [esp+10h])
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
    unsigned long n;
    in.ReadULONG();
    Constraints.Clear();
    in.CopyTo(Constraints, in.ReadULONG());
    if (!InitialSimulation.ReadFromFile(in, pool))
        return false;
    n = in.ReadULONG();
    RenderTris.resize(n);
    in.Read(RenderTris.GetAsCArray(), RenderTris.size() * sizeof(C3DTriangle2));
    n = in.ReadULONG();
    RenderQuads.resize(n);
    in.Read(RenderQuads.GetAsCArray(), RenderQuads.size() * sizeof(C3DQuad2));
    n = in.ReadULONG();
    RenderVertices.resize(n);
    in.Read(RenderVertices.GetAsCArray(), RenderVertices.size() * sizeof(C3DClothRenderVertex));
    NonSimCount = in.ReadULONG();
    if (!AllocateArrays(NonSimCount + InitialSimulation.GetParticleCount(), pool))
        return false;
    CParticleSimulation::ReadVectorArray(NonSimPositions, NonSimCount, in, 0.0f);
    n = in.ReadULONG();
    IndexedTextureCoords.resize(n);
    {
        C2DVector* data = IndexedTextureCoords.begin();
        if (!(IndexedTextureCoords.size() > 0))
            data = reinterpret_cast<C2DVector*>(&IndexedTextureCoords);
        in.Read(data, IndexedTextureCoords.size() * sizeof(C2DVector));
    }
    n = in.ReadULONG();
    ParticleIndices.resize(n);
    in.Read(ParticleIndices.GetAsCArray(), ParticleIndices.size() * sizeof(long));
    n = in.ReadULONG();
    VertexIndices.resize(n);
    in.Read(VertexIndices.GetAsCArray(), VertexIndices.size() * sizeof(long));
    AveragePatchSize = in.ReadFloat();
    BezierEnable = in.ReadEBOOL();
    unsigned long no_export_particles = in.ReadULONG();
    ExportParticles.resize(no_export_particles);
    for (unsigned long c = 0; c < no_export_particles; ++c) {
        CCharString name = in.ReadNullTerminatedString();
        unsigned long index = in.ReadULONG();
        ExportParticles.Insert(name, index);
    }
    ExportParticles.Update();
    ReadGroups(in, SimGroups);
    ReadGroups(in, NonSimGroups);
    return true;
}
