// CParticleProgram::ReadFromFile -- Read particle simulation, render arrays, exports and groups.
// Grade: DIFFER(2310v2310) -- At 00AE40AC the texture-coordinate buffer starts in EDI rather than EAX. Retail copies EAX to EDI after the count test. The later read and the remainder of the function realign.
// Leaf: ?ReadFromFile@CParticleProgram@@QAE_NAAVCDataInputStream@@AAVCMovableResourceMemoryPool@@@Z
// Retail: 0x00AE3C30, 2310 bytes, thiscall.
#define FABLE_RETAIL_CPARTICLEPROGRAM_READ_PASS24
#define FABLE_RETAIL_CPARTICLEPROGRAM_TEXCOORDACCESS
#define FABLE_RETAIL_CPARTICLEPROGRAM_PASS22_CLEARGROUPS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_COPYTO
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM_RESERVE
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_RENDERACCESS
#define FABLE_RETAIL_C3DCLOTHRENDERVERTEX_DEFAULT_CTOR_PASS24
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

bool CParticleProgram::ReadFromFile(CDataInputStream& is, CMovableResourceMemoryPool& alloc)
{
    const unsigned long version = is.ReadULONG();
    Constraints.Clear();
    unsigned long size = is.ReadULONG();
    is.CopyTo(Constraints, size);
    if (!InitialSimulation.ReadFromFile(is, alloc))
        return false;
    unsigned long no_tris = is.ReadULONG();
    RenderTris.resize(no_tris);
    is.Read(RenderTris.GetAsCArray(), RenderTris.size() * sizeof(C3DTriangle2));
    unsigned long no_quads = is.ReadULONG();
    RenderQuads.resize(no_quads);
    is.Read(RenderQuads.GetAsCArray(), RenderQuads.size() * sizeof(C3DQuad2));
    unsigned long no_render_verts = is.ReadULONG();
    RenderVertices.resize(no_render_verts);
    is.Read(RenderVertices.GetAsCArray(), RenderVertices.size() * sizeof(C3DClothRenderVertex));
    NonSimCount = is.ReadULONG();
    if (!AllocateArrays(NonSimCount + InitialSimulation.GetParticleCount(), alloc))
        return false;
    CParticleSimulation::ReadVectorArray(NonSimPositions, NonSimCount, is, 0.0f);
    unsigned long no_texcoord = is.ReadULONG();
    IndexedTextureCoords.resize(no_texcoord);
    {
        C2DVector* data = IndexedTextureCoords.begin();
        if (!(IndexedTextureCoords.size() > 0))
            data = reinterpret_cast<C2DVector*>(&IndexedTextureCoords);
        is.Read(data, IndexedTextureCoords.size() * sizeof(C2DVector));
    }
    unsigned long no_indices = is.ReadULONG();
    ParticleIndices.resize(no_indices);
    is.Read(ParticleIndices.GetAsCArray(), ParticleIndices.size() * sizeof(long));
    no_indices = is.ReadULONG();
    VertexIndices.resize(no_indices);
    is.Read(VertexIndices.GetAsCArray(), VertexIndices.size() * sizeof(long));
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
