// CParticleProgram::ReadFromFile(CDataInputStream& is, CMovableResourceMemoryPool& alloc) -- read a particle program: version, constraints, the initial simulation, the render triangle / quad / vertex arrays, the non-simulated positions, texture coordinates, particle and vertex index arrays, patch size, Bezier flag, the named export particles and the two group lists; false when the simulation or the array allocation fails
// Shape: FableWin.pdb line table (lib_particle_simulation.obj, lines 953..1011) and Ego_d / FableWin locals (no array-pointer local):
//   every array read is the one statement `is.Read(X.GetAsCArray(), X.GetByteSize());` (38 debug bytes each), the allocation test is
//   `!AllocateArrays(GetVertexPositionCount(), alloc)` (28). Line 955 is an ASSERT in the debug build (277 bytes); it is left out here:
//   its release dead arm emits nothing and changes nothing (graded both ways).
// What closed it: FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE (existing shared opt-in: CDataInputStream::Read with FableWin's
//   own locals dst / no_bytes / overflow_check and the whole body under `if (no_bytes > 0)`). With the plain Read model `this` keeps esi
//   across the texcoord ReadULONG (0x00AE4071) and later takes an ebx piece, so const 0 loses ebx (2278v2310).
// Callees out of line as in retail: vector<C3DQuad2> / vector<C2DVector> / vector<pair<CCharString, unsigned long> >::resize(n)
//   (the other arrays expand STLport's resize).
// Leaf: ?ReadFromFile@CParticleProgram@@QAE_NAAVCDataInputStream@@AAVCMovableResourceMemoryPool@@@Z
// Retail: 0x00AE3C30, 2310 bytes, thiscall `ret 8`.
#define FABLE_RETAIL_CPARTICLEPROGRAM_READ_PASS24
#define FABLE_RETAIL_CPARTICLEPROGRAM_TEXCOORDACCESS
#define FABLE_RETAIL_CPARTICLEPROGRAM_PASS22_CLEARGROUPS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CDATAINPUTSTREAM_COPYTO
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_RESERVE
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_RENDERACCESS
#define FABLE_RETAIL_C3DCLOTHRENDERVERTEX_DEFAULT_CTOR_PASS24
#define FABLE_RETAIL_CPARTICLESIMULATION
#define FABLE_RETAIL_CPARTICLESIMULATION_READFROMFILE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETBYTESIZE
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

bool CParticleProgram::ReadFromFile(CDataInputStream& is, CMovableResourceMemoryPool& alloc)
{
    unsigned long version = is.ReadULONG();
    Constraints.Clear();
    unsigned long size = is.ReadULONG();
    is.CopyTo(Constraints, size);
    if (!InitialSimulation.ReadFromFile(is, alloc))
        return false;
    unsigned long no_tris = is.ReadULONG();
    RenderTris.resize(no_tris);
    is.Read(RenderTris.GetAsCArray(), RenderTris.GetByteSize());
    unsigned long no_quads = is.ReadULONG();
    RenderQuads.resize(no_quads);
    is.Read(RenderQuads.GetAsCArray(), RenderQuads.GetByteSize());
    unsigned long no_render_verts = is.ReadULONG();
    RenderVertices.resize(no_render_verts);
    is.Read(RenderVertices.GetAsCArray(), RenderVertices.GetByteSize());
    NonSimCount = is.ReadULONG();
    if (!AllocateArrays(GetVertexPositionCount(), alloc))
        return false;
    CParticleSimulation::ReadVectorArray(NonSimPositions, NonSimCount, is, 0.0f);
    unsigned long no_texcoord = is.ReadULONG();
    IndexedTextureCoords.resize(no_texcoord);
    is.Read(IndexedTextureCoords.GetAsCArray(), IndexedTextureCoords.GetByteSize());
    unsigned long no_indices = is.ReadULONG();
    ParticleIndices.resize(no_indices);
    is.Read(ParticleIndices.GetAsCArray(), ParticleIndices.GetByteSize());
    no_indices = is.ReadULONG();
    VertexIndices.resize(no_indices);
    is.Read(VertexIndices.GetAsCArray(), VertexIndices.GetByteSize());
    AveragePatchSize = is.ReadFloat();
    BezierEnable = is.ReadEBOOL();
    unsigned long no_exp = is.ReadULONG();
    ExportParticles.resize(no_exp);
    for (unsigned long c1 = 0; c1 < no_exp; ++c1) {
        CCharString name = is.ReadNullTerminatedString();
        unsigned long index = is.ReadULONG();
        ExportParticles.Insert(name, index);
    }
    ExportParticles.Update();
    ReadGroups(is, SimGroups);
    ReadGroups(is, NonSimGroups);
    return true;
}