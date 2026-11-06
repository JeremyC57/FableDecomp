// QuadricOptimiserInternals::COptimisedPrimitive::COptimisedPrimitive(C3DMeshFilePrimitiveChunk& chunk) -- build the quadric optimiser's primitive from a mesh-file primitive chunk: vertices (position, normal, UV), unique-vertex ids and their vertex lists, faces from the triangle list, per-vertex bone weights from the groups, and keep the XMIF / CLTH sub-chunks
// Grade: DIFFER(2658v2674) -- every call, temporary and STLport expansion is retail-shaped (struct diff 79 lines); residue is VC7.1 register colouring: retail keeps `this` in edi and &Faces in esi (ours swaps them, so the prologue pushes edi late), the first two vertex loops schedule their strided loads differently, and retail hoists the converted bone key (`mov ecx,[esp+14h]; mov [esp+14h],ecx` before the weight loop, plus a `lea esp,[esp]` pad) where ours re-reads the bone slot
// Next idea: the source-level Lionhead helpers (COptimisedVertex / CUniqueVertex setters, C3DMeshFilePrimitiveChunk::FindFirstCloth / FindNextCloth wrappers from Ego_r) could change the usage weights that decide the esi/edi colouring; the key hoist wants a `long` bone converted once outside the loop without the copy being propagated away
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
// Layout: row-local models (engine/COptimisedPrimitive.h is the flattened Ego_r layout with opaque byte arrays and no namespace, and
//   there is no QuadricOptimiserInternals header); the chunk classes carry only the members this body reads.
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"

extern "C" void* __cdecl malloc(unsigned int size);

class CChunkedFileChunk { public:
    unsigned char _body[0x10];
};
class CChunkedFileSuperChunk : public CChunkedFileChunk { public:
    unsigned char _pad_0x10[0x1c];
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type);                                          // 0x00AA80E0
    CCountedPointer<CChunkedFileChunk> FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous);   // 0x00AA8190
};
class C3DVertex2 { public: C3DVector Position; C3DVector Normal; float U; float V; };
class C3DTriangle2 { public: unsigned short VertexIndex[3]; };
class C3DVertexBlend2 { public: unsigned long VertexIndex; float BlendValue; };
class C3DMeshFileVerticesChunk : public CChunkedFileChunk { public:
    CArray<C3DVertex2> VertexList;                                  // +0x10
    unsigned long GetCount() const;                                 // 0x00ABA360
};
class C3DMeshFileUniqueVerticesChunk : public CChunkedFileChunk { public:
    CArray<unsigned long> UniqueVerticesList;                       // +0x10
};
class C3DMeshFileTrianglesChunk : public CChunkedFileChunk { public:
    unsigned char _pad_0x10[0x1c];
    CArray<C3DTriangle2> TriangleList;                              // +0x2c
    unsigned long GetCount() const;                         // 0x00ABA370
};
class C3DMeshFileGroupChunk : public CChunkedFileChunk { public:
    long GetBoneIndex() const;                                      // 0x00AB9F60
    unsigned long GetCount() const;                                 // 0x00ABA350
    const C3DVertexBlend2* GetList() const;                         // 0x00ABC370
};
class C3DMeshFilePrimitiveChunk : public CChunkedFileSuperChunk { public:
    CArray<long> MaterialIndices;                                   // +0x2c
    CCountedPointer<const C3DMeshFileVerticesChunk> PeekVertices() const;                 // 0x00ABAFE0
    bool HasUniqueVertices() const;                                                       // 0x00ABB610
    CCountedPointer<const C3DMeshFileUniqueVerticesChunk> PeekUniqueVertices() const;     // 0x00ABB720
    CCountedPointer<const C3DMeshFileTrianglesChunk> PeekTriangles() const;               // 0x00ABB100
    CCountedPointer<C3DMeshFileGroupChunk> FindFirstGroup();                              // 0x00ABB7B0
    CCountedPointer<C3DMeshFileGroupChunk> FindNextGroup(const CCountedPointer<const C3DMeshFileGroupChunk>& previous);   // 0x00ABB840
};

namespace QuadricOptimiserInternals {

struct CTreeNodeBase { char _M_color; CTreeNodeBase* _M_parent; CTreeNodeBase* _M_left; CTreeNodeBase* _M_right; };
template<class V> struct CTreeNode : public CTreeNodeBase { V _M_value_field; };
struct CTreeIterator { CTreeNodeBase* _M_node; };
struct CInsertResult {
    CTreeIterator first; bool second;
    CInsertResult() { }
};
template<class V> class CRbTree { public:                           // STLport _Rb_tree: header, count, empty compare
    CTreeNodeBase* _M_header;
    unsigned long _M_node_count;
    char _M_key_compare;
    CRbTree() : _M_header(0)
    {
        _M_header = (CTreeNode<V>*)malloc(sizeof(CTreeNode<V>));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    ~CRbTree();
    CRbTree& operator=(const CRbTree& other);
    CInsertResult insert_unique(const V& v);                        // 0x00514C10 (unsigned long)
};
class CIndexList { public:
    CRbTree<unsigned long> _M_t;
    CInsertResult insert(const unsigned long& v) { return _M_t.insert_unique(v); }
};
class CGroupMap { public:                                           // map<unsigned long, float>
    CRbTree<unsigned long[3]> _M_t;
    float& operator[](const unsigned long& key);                    // 0x00AD6420
};
class CContractionMultimap { public:                                // multimap<float, CCountedPointer<CVertexPairContraction>>
    CRbTree<unsigned long[3]> _M_t;
};

class CUniqueVertex { public:
    bool Exists;
    CIndexList VertexIndices;                                       // +0x04
    CIndexList FaceList;                                            // +0x10
    ~CUniqueVertex();                                               // 0x00AD4A70
};
class COptimisedVertex { public:
    C3DVector Position;                                             // +0x00
    C3DVector Normal;                                               // +0x0c
    C3DVector TextureCoord;                                         // +0x18
    unsigned char _pad_0x24[0x70];
    unsigned long UniqueIndex;                                      // +0x94
    unsigned long OutputIndex;                                      // +0x98
    CGroupMap Groups;                                               // +0x9c
    unsigned char _pad_0xa8[0x10];
    COptimisedVertex();                                             // 0x00AD7A00
    COptimisedVertex(const COptimisedVertex& other);
    ~COptimisedVertex();                                            // 0x00AD7400
};
class COptimisedFace { public:
    unsigned long VertexIndices[3];
    unsigned long UniqueVertexIndices[3];
    long Edges[3];
    C3DVector UTangent;
    C3DVector VTangent;
    bool Exists;
    unsigned long OutputIndex;
    long Segment;
    COptimisedFace() : Exists(true), Segment(-1) { Edges[0] = Edges[1] = Edges[2] = -1; }
};
class COptimisedEdge { public: long Vertex[2]; long Face[2]; };
class COptimisationParameters { public:
    float Scaling, EdgeWeight, EdgeCost, GroupWeight, TextureWeight, TexturePenalty, UnsplitTextureWeight, UnsplitNormalWeight, HighPenalty;
    unsigned long MaximumBonesPerVertex;
};

class COptimisedPrimitive { public:
    long MaterialIndex;                                             // +0x00
    CArray<CUniqueVertex> UniqueVertices;                           // +0x04
    CArray<COptimisedVertex> Vertices;                              // +0x10
    CArray<COptimisedFace> Faces;                                   // +0x1c
    CArray<COptimisedEdge> Edges;                                   // +0x28
    CContractionMultimap VertexPairContractions;                    // +0x34
    long SegmentCount;                                              // +0x40
    CArray<float> SegmentSizes;                                     // +0x44
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunksToKeep;    // +0x50
    COptimisationParameters Parameters;                             // +0x5c
    COptimisedPrimitive(C3DMeshFilePrimitiveChunk& chunk);
};

}


namespace QuadricOptimiserInternals {

COptimisedPrimitive::COptimisedPrimitive(C3DMeshFilePrimitiveChunk& chunk)
{
    MaterialIndex = chunk.MaterialIndices[0];
    SegmentCount = 0;
    const C3DMeshFileVerticesChunk* vertices = chunk.PeekVertices().Get();
    unsigned long vertex_count = vertices->GetCount();
    Vertices.resize(vertex_count, COptimisedVertex());
    for (unsigned long i = 0; i < vertex_count; i++) {
        const C3DVertex2& vertex = vertices->VertexList[i];
        COptimisedVertex& optimised_vertex = Vertices[i];
        optimised_vertex.Position = C3DVector(vertex.Position);
        optimised_vertex.Normal = C3DVector(vertex.Normal);
        optimised_vertex.TextureCoord = C3DVector(vertex.U, vertex.V, 0.0f);
        optimised_vertex.UniqueIndex = i;
    }
    unsigned long unique_count;
    if (chunk.HasUniqueVertices()) {
        unique_count = 0;
        const C3DMeshFileUniqueVerticesChunk* unique_vertices = chunk.PeekUniqueVertices().Get();
        for (unsigned long i = 0; i < vertex_count; i++) {
            unsigned long unique_index = unique_vertices->UniqueVerticesList[i];
            Vertices[i].UniqueIndex = unique_index;
            if (unique_count < unique_index + 1) unique_count = unique_index + 1;
        }
    } else {
        unique_count = vertex_count;
    }
    UniqueVertices.resize(unique_count, CUniqueVertex());
    for (unsigned int i = 0; i < vertex_count; i++) {
        UniqueVertices[Vertices[i].UniqueIndex].VertexIndices.insert(i);
    }
    const C3DMeshFileTrianglesChunk* triangles = chunk.PeekTriangles().Get();
    unsigned long triangle_count = triangles->GetCount();
    Faces.resize(triangle_count, COptimisedFace());
    for (unsigned long i = 0; i < Faces.size(); i++) {
        COptimisedFace& face = Faces[i];
        const C3DTriangle2& triangle = triangles->TriangleList[i];
        face.VertexIndices[0] = triangle.VertexIndex[0];
        face.VertexIndices[1] = triangle.VertexIndex[1];
        face.VertexIndices[2] = triangle.VertexIndex[2];
        face.UniqueVertexIndices[0] = Vertices[triangle.VertexIndex[0]].UniqueIndex;
        face.UniqueVertexIndices[1] = Vertices[triangle.VertexIndex[1]].UniqueIndex;
        face.UniqueVertexIndices[2] = Vertices[triangle.VertexIndex[2]].UniqueIndex;
    }
    CCountedPointer<const C3DMeshFileGroupChunk> group;
    for (group = chunk.FindFirstGroup(); group.Get(); group = chunk.FindNextGroup(group)) {
        unsigned long bone = group->GetBoneIndex();
        unsigned long count = group->GetCount();
        const C3DVertexBlend2* list = group->GetList();
        for (unsigned long j = 0; j < count; j++) {
            float weight = list[j].BlendValue;
            Vertices[list[j].VertexIndex].Groups[bone] = weight;
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
