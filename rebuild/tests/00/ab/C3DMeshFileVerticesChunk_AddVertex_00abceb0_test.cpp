// Behaviour test for C3DMeshFileVerticesChunk::AddVertex 0x00ABCEB0: each call appends one 32-byte vertex, in order, unchanged.
#include <cstdio>
#include <vector>
struct C3DVector { float x, y, z; };
struct C3DVertex2 { C3DVector Position; C3DVector Normal; float U, V; };
struct C3DMeshFileVerticesChunk {
    std::vector<C3DVertex2> VertexList;
    void AddVertex(const C3DVertex2& vertex) { VertexList.push_back(vertex); }
};
int main()
{
    if (sizeof(C3DVertex2) != 32) { std::printf("00abceb0_TEST FAIL\n"); return 1; }
    C3DMeshFileVerticesChunk c;
    for (int i = 0; i < 5; ++i) { C3DVertex2 v = { { (float)i, 0, 0 }, { 0, 1, 0 }, 0.5f * i, 0.25f }; c.AddVertex(v); }
    if (c.VertexList.size() != 5 || c.VertexList[3].Position.x != 3.0f || c.VertexList[4].U != 2.0f) { std::printf("00abceb0_TEST FAIL\n"); return 1; }
    std::printf("00abceb0_TEST PASS\n");
    return 0;
}