// QuadricOptimiserInternals::COptimisedPrimitive::COptimisedPrimitive(C3DMeshFilePrimitiveChunk& chunk) -- build the quadric optimiser's primitive from a mesh-file primitive chunk: vertices (position, normal, UV), unique-vertex ids and their vertex lists, faces from the triangle list, per-vertex bone weights from the groups, and keep the XMIF / CLTH sub-chunks
// Leaf: ??0COptimisedPrimitive@QuadricOptimiserInternals@@QAE@AAVC3DMeshFilePrimitiveChunk@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ACDB80, 2674 bytes, thiscall `ret 4`, frame 0x17C, no EH. The TU is /Gr (the landed
//   sibling vector<CCountedPointer<CChunkedFileChunk> >::_M_insert_overflow 0x00AA94E0 carries `// Flags: /Gr`; here __copy
//   0x00AD6FF0 and __destroy_aux 0x00AA9140 are __fastcall, and only /Gr lets the two push_back overflows expand inline as retail).
// Identity: Ego_r.pdb ??0COptimisedPrimitive@QuadricOptimiserInternals@@QAE@AAVC3DMeshFilePrimitiveChunk@@@Z; members per the landed
//   ~COptimisedPrimitive 0x00AD84D0 (MaterialIndex +0, UniqueVertices +4, Vertices +0x10, Faces +0x1C, Edges +0x28,
//   VertexPairContractions +0x34, SegmentCount +0x40, SegmentSizes +0x44, SubChunksToKeep +0x50); the "XMIF" 0x0129DF48 and "CLTH"
//   0x0129DF30 literals; COptimisedVertex UniqueIndex +0x94 / Groups +0x9C (Ego_r COptimisedVertex with 12-byte STLport containers).
// Callees: C3DMeshFilePrimitiveChunk::PeekVertices 0x00ABAFE0 / HasUniqueVertices 0x00ABB610 / PeekUniqueVertices 0x00ABB720 /
//   PeekTriangles 0x00ABB100 / FindFirstGroup 0x00ABB7B0 / FindNextGroup 0x00ABB840, CChunkedFileSuperChunk::FindFirstSubChunkOfType
//   0x00AA80E0 / FindNextSubChunkOfType 0x00AA8190, C3DMeshFileVerticesChunk::GetCount 0x00ABA360, C3DMeshFileTrianglesChunk::GetCount 0x00ABA370,
//   C3DMeshFileGroupChunk::GetBoneIndex 0x00AB9F60 / GetCount 0x00ABA350 / GetList 0x00ABC370 (Ego_r names), COptimisedVertex ctor 0x00AD7A00 / dtor 0x00AD7400, vector<COptimisedVertex>::resize 0x00AD87A0,
//   CUniqueVertex dtor 0x00AD4A70, __copy<CUniqueVertex*> 0x00AD6FF0, vector<CUniqueVertex>::_M_fill_insert 0x00AD7E80,
//   vector<COptimisedFace>::erase 0x00AD3D60 / _M_fill_insert 0x00AD54F0, set<unsigned long> insert_unique 0x00514C10,
//   map<unsigned long,float>::operator[] 0x00AD6420, __destroy_aux<CCountedPointer<CChunkedFileChunk>*> 0x00AA9140, malloc 0x00BFEA0E,
//   free 0x00BFEA14, operator delete 0x00BFE9BC.
// Layout: the mesh-file chunk classes from their shared headers (retail branches + the wfA3 accessor opt-ins: Ego_r out-of-line
//   getters as declarations, FableWin's always-inlined PeekVertex / PeekTriangle / GetUniqueVertexIndex / GetMaterialIndex /
//   C3DVertexBlend2 getters as bodies, each the FableWin /Od size); the STLport set / map / multimap of stlport/fable_map.h
//   (FABLE_RETAIL_STLP_MAP: map::operator[] and _Rb_tree::insert_unique visible, VC7.1 keeps both out of line as retail does and
//   hoists the loop-invariant key store). The QuadricOptimiserInternals classes from the shared engine/QuadricOptimiserTypes.h +
//   engine/COptimisedPrimitive.h (FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE, as the landed 0x00AC82A0 / 0x00AC8AE0) with the wfA3
//   opt-ins _CUNIQUEVERTEX (CIndexList / CUniqueVertex), _COPTIMISEDVERTEX_GROUPS (FaceList / Groups typed, ctors, AddGroup),
//   _COPTIMISEDFACE_CTOR and _COPTIMISEDPRIMITIVE_CTOR (ctor + GetVertex); CIndexList::AddIndex, COptimisedVertex::AddGroup,
//   COptimisedFace() and COptimisedPrimitive::GetVertex are FableWin.pdb inline members (fwsize SAME for AddGroup 14i/36b and
//   GetVertex 12i/28b).
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_FINDNEXTSUBCHUNKOFTYPE
#define FABLE_RETAIL_C3DMESHFILEPRIMITIVECHUNK
#define FABLE_RETAIL_C3DMESHFILEPRIMITIVECHUNK_ACCESSORS
#define FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK
#define FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK_PEEKVERTEX
#define FABLE_RETAIL_C3DMESHFILEUNIQUEVERTICESCHUNK
#define FABLE_RETAIL_C3DMESHFILEUNIQUEVERTICESCHUNK_GETUNIQUEVERTEXINDEX
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK_ACCESSORS
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK_PEEKTRIANGLE
#define FABLE_RETAIL_C3DMESHFILEGROUPCHUNK
#define FABLE_RETAIL_C3DMESHFILEGROUPCHUNK_ACCESSORS
#define FABLE_RETAIL_C3DVERTEXBLEND2_GETTERS
#define FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX
#define FABLE_RETAIL_QUADRIC_COPTIMISEDVERTEX_GROUPS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDFACE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDFACE_CTOR
#define FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_CTOR
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/C3DMeshFilePrimitiveChunk.h"
#include "engine/C3DMeshFileVerticesChunk.h"
#include "engine/C3DMeshFileUniqueVerticesChunk.h"
#include "engine/C3DMeshFileTrianglesChunk.h"
#include "engine/C3DMeshFileGroupChunk.h"
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"

namespace QuadricOptimiserInternals {

COptimisedPrimitive::COptimisedPrimitive(C3DMeshFilePrimitiveChunk& chunk)
{
    MaterialIndex = chunk.GetMaterialIndex(0);
    SegmentCount = 0;
    const C3DMeshFileVerticesChunk* vertices = chunk.PeekVertices().Get();
    unsigned long vertex_count = vertices->GetCount();
    Vertices.resize(vertex_count, COptimisedVertex());
    for (unsigned long i = 0; i < vertex_count; i++) {
        COptimisedVertex& optimised_vertex = Vertices[i];
        const C3DVertex2& vertex = vertices->PeekVertex(i);
        optimised_vertex.Position = vertex.GetPosition();
        optimised_vertex.Normal = vertex.GetNormal();
        optimised_vertex.TextureCoord = C3DVector(vertex.GetU(), vertex.GetV(), 0.0f);
        optimised_vertex.UniqueIndex = i;
    }
    unsigned long unique_count;
    if (chunk.HasUniqueVertices()) {
        unique_count = 0;
        const C3DMeshFileUniqueVerticesChunk* unique_vertices = chunk.PeekUniqueVertices().Get();
        for (unsigned long i = 0; i < vertex_count; i++) {
            COptimisedVertex& vertex = Vertices[i];
            vertex.UniqueIndex = unique_vertices->GetUniqueVertexIndex(i);
            if (unique_count < vertex.UniqueIndex + 1) unique_count = vertex.UniqueIndex + 1;
        }
    } else {
        unique_count = vertex_count;
    }
    UniqueVertices.resize(unique_count, CUniqueVertex());
    for (unsigned int i = 0; i < vertex_count; i++) {
        CUniqueVertex& unique_vertex = UniqueVertices[GetVertex(i).UniqueIndex];
        unique_vertex.VertexIndices.AddIndex(i);
    }
    const C3DMeshFileTrianglesChunk* triangles = chunk.PeekTriangles().Get();
    unsigned long triangle_count = triangles->GetCount();
    Faces.resize(triangle_count, COptimisedFace());
    for (unsigned long i = 0; i < Faces.size(); i++) {
        COptimisedFace& face = Faces[i];
        const C3DTriangle2& triangle = triangles->PeekTriangle(i);
        face.VertexIndices[0] = triangle.GetVertexIndex(0);
        face.VertexIndices[1] = triangle.GetVertexIndex(1);
        face.VertexIndices[2] = triangle.GetVertexIndex(2);
        face.UniqueVertexIndices[0] = Vertices[triangle.GetVertexIndex(0)].UniqueIndex;
        face.UniqueVertexIndices[1] = Vertices[triangle.GetVertexIndex(1)].UniqueIndex;
        face.UniqueVertexIndices[2] = Vertices[triangle.GetVertexIndex(2)].UniqueIndex;
    }
    CCountedPointer<const C3DMeshFileGroupChunk> group;
    for (group = chunk.FindFirstGroup(); group.Get(); group = chunk.FindNextGroup(group)) {
        long bone = group->GetBoneIndex();
        unsigned long count = group->GetCount();
        const C3DVertexBlend2* list = group->GetList();
        for (unsigned long j = 0; j < count; j++) {
            unsigned long vertex_index = list[j].GetVertexIndex();
            float weight = list[j].GetBlendValue();
            GetVertex(vertex_index).AddGroup(bone, weight);
        }
    }
    CCountedPointer<CChunkedFileChunk> mesh_info = chunk.FindFirstSubChunkOfType("XMIF");
    if (mesh_info.Get()) SubChunksToKeep.push_back(mesh_info);
    CCountedPointer<CChunkedFileChunk> cloth = chunk.FindFirstSubChunkOfType("CLTH");
    while (cloth.Get()) {
        SubChunksToKeep.push_back(cloth);
        cloth = chunk.FindNextSubChunkOfType(cloth);
    }
}

}
