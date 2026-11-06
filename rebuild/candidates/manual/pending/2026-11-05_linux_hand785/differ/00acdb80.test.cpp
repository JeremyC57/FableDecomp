// Behaviour test for QuadricOptimiserInternals::COptimisedPrimitive::COptimisedPrimitive (0x00ACDB80): a model of the build --
// material index from the chunk's first entry; one optimised vertex per mesh vertex (position, normal, UV with z = 0, unique id
// = its own index or the UNIV entry); unique-vertex count = vertex count without UNIV, else max(id) + 1, each unique vertex
// listing its vertices; one face per triangle (vertex indices + their unique ids, edges / segment -1, exists); per-vertex bone
// weights from every group; the XMIF sub-chunk then every CLTH sub-chunk kept in order.
#include <cstdio>
#include <vector>
#include <set>
#include <map>
#include <string>

struct V3 { float X, Y, Z; };
struct Vertex2 { V3 Position, Normal; float U, V; };
struct Blend { unsigned long VertexIndex; float BlendValue; };
struct Group { long Bone; std::vector<Blend> List; };
struct Chunk {
    std::vector<long> MaterialIndices;
    std::vector<Vertex2> Vertices;
    bool HasUnique;
    std::vector<unsigned long> Unique;
    std::vector<unsigned short> Triangles;          // 3 per triangle
    std::vector<Group> Groups;
    std::vector<std::string> SubChunks;             // types in file order
};
struct OVertex { V3 Position, Normal, TextureCoord; unsigned long UniqueIndex; std::map<unsigned long, float> Groups; };
struct UVertex { std::set<unsigned long> VertexIndices, FaceList; };
struct OFace { unsigned long VertexIndices[3], UniqueVertexIndices[3]; long Edges[3]; bool Exists; long Segment; };
struct Prim {
    long MaterialIndex; long SegmentCount;
    std::vector<UVertex> UniqueVertices; std::vector<OVertex> Vertices; std::vector<OFace> Faces;
    std::vector<std::string> SubChunksToKeep;
    Prim(const Chunk& chunk);
};

Prim::Prim(const Chunk& chunk)
{
    MaterialIndex = chunk.MaterialIndices[0];
    SegmentCount = 0;
    unsigned long vertex_count = (unsigned long)chunk.Vertices.size();
    Vertices.resize(vertex_count);
    for (unsigned long i = 0; i < vertex_count; i++) {
        const Vertex2& v = chunk.Vertices[i];
        Vertices[i].Position = v.Position; Vertices[i].Normal = v.Normal;
        V3 uv = { v.U, v.V, 0.0f }; Vertices[i].TextureCoord = uv;
        Vertices[i].UniqueIndex = i;
    }
    unsigned long unique_count;
    if (chunk.HasUnique) {
        unique_count = 0;
        for (unsigned long i = 0; i < vertex_count; i++) {
            unsigned long u = chunk.Unique[i];
            Vertices[i].UniqueIndex = u;
            if (unique_count < u + 1) unique_count = u + 1;
        }
    } else unique_count = vertex_count;
    UniqueVertices.resize(unique_count);
    for (unsigned int i = 0; i < vertex_count; i++) UniqueVertices[Vertices[i].UniqueIndex].VertexIndices.insert(i);
    OFace blank; blank.Edges[0] = blank.Edges[1] = blank.Edges[2] = -1; blank.Exists = true; blank.Segment = -1;
    Faces.resize(chunk.Triangles.size() / 3, blank);
    for (unsigned long i = 0; i < Faces.size(); i++)
        for (int k = 0; k < 3; k++) {
            Faces[i].VertexIndices[k] = chunk.Triangles[i * 3 + k];
            Faces[i].UniqueVertexIndices[k] = Vertices[chunk.Triangles[i * 3 + k]].UniqueIndex;
        }
    for (size_t g = 0; g < chunk.Groups.size(); g++)
        for (size_t j = 0; j < chunk.Groups[g].List.size(); j++)
            Vertices[chunk.Groups[g].List[j].VertexIndex].Groups[chunk.Groups[g].Bone] = chunk.Groups[g].List[j].BlendValue;
    for (size_t c = 0; c < chunk.SubChunks.size(); c++)
        if (chunk.SubChunks[c] == "XMIF") { SubChunksToKeep.push_back("XMIF"); break; }
    for (size_t c = 0; c < chunk.SubChunks.size(); c++)
        if (chunk.SubChunks[c] == "CLTH") SubChunksToKeep.push_back("CLTH");
}

int main()
{
    Chunk c;
    c.MaterialIndices.push_back(7);
    for (int i = 0; i < 4; i++) { Vertex2 v = { { (float)i, 0, 0 }, { 0, 0, 1 }, 0.25f * i, 0.5f }; c.Vertices.push_back(v); }
    c.HasUnique = true;
    c.Unique.push_back(0); c.Unique.push_back(2); c.Unique.push_back(0); c.Unique.push_back(1);
    unsigned short tris[] = { 0, 1, 2, 2, 1, 3 }; c.Triangles.assign(tris, tris + 6);
    Group g; g.Bone = 5; Blend b1 = { 1, 0.75f }, b2 = { 3, 0.25f }; g.List.push_back(b1); g.List.push_back(b2); c.Groups.push_back(g);
    c.SubChunks.push_back("CLTH"); c.SubChunks.push_back("XMIF"); c.SubChunks.push_back("CLTH");
    Prim p(c);
    bool ok = p.MaterialIndex == 7 && p.SegmentCount == 0 && p.Vertices.size() == 4 && p.UniqueVertices.size() == 3
        && p.UniqueVertices[0].VertexIndices.size() == 2 && p.UniqueVertices[0].VertexIndices.count(2) == 1
        && p.Vertices[3].TextureCoord.X == 0.75f && p.Vertices[3].TextureCoord.Z == 0.0f
        && p.Faces.size() == 2 && p.Faces[1].VertexIndices[2] == 3 && p.Faces[1].UniqueVertexIndices[0] == 0
        && p.Faces[0].UniqueVertexIndices[1] == 2 && p.Faces[0].Edges[2] == -1 && p.Faces[0].Segment == -1 && p.Faces[0].Exists
        && p.Vertices[1].Groups[5] == 0.75f && p.Vertices[3].Groups[5] == 0.25f && p.Vertices[0].Groups.empty()
        && p.SubChunksToKeep.size() == 3 && p.SubChunksToKeep[0] == "XMIF" && p.SubChunksToKeep[2] == "CLTH";
    // without UNIV every vertex is its own unique vertex
    c.HasUnique = false; c.SubChunks.clear();
    Prim q(c);
    ok = ok && q.UniqueVertices.size() == 4 && q.Vertices[2].UniqueIndex == 2 && q.Faces[0].UniqueVertexIndices[2] == 2
        && q.SubChunksToKeep.empty();
    std::printf(ok ? "00acdb80_TEST PASS\n" : "00acdb80_TEST FAIL\n");
    return ok ? 0 : 1;
}
