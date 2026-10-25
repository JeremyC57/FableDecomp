// Behaviour test (0x00a6c560): CPhysicsMesh::GetNoTriangles is the number of 6-byte triangles in TotalFaceIndices
#include <cstdio>
class C3DTriangle2 { public: unsigned short VertexIndex[3]; };
class CTriangleArray {
public:
    C3DTriangle2* m_pFirst; C3DTriangle2* m_pLast; C3DTriangle2* m_pEnd;
    unsigned int size() const { return (unsigned int)(m_pLast - m_pFirst); }
};
class CPhysicsMesh {
public:
    CTriangleArray TotalFaceIndices;
    long GetNoTriangles() const { return TotalFaceIndices.size(); }
};
int main()
{
    if (sizeof(C3DTriangle2) != 6) { std::printf("00a6c560_TEST FAIL\n"); return 1; }
    C3DTriangle2 tris[10];
    CPhysicsMesh mesh; mesh.TotalFaceIndices.m_pFirst = tris; mesh.TotalFaceIndices.m_pEnd = tris + 10;
    mesh.TotalFaceIndices.m_pLast = tris + 7;
    if (mesh.GetNoTriangles() != 7) { std::printf("00a6c560_TEST FAIL\n"); return 1; }
    mesh.TotalFaceIndices.m_pLast = tris;
    if (mesh.GetNoTriangles() != 0) { std::printf("00a6c560_TEST FAIL\n"); return 1; }
    std::printf("00a6c560_TEST PASS\n");
    return 0;
}