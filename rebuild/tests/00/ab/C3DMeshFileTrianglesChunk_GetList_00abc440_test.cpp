#include <cstdio>
// Behaviour test: GetList returns the first triangle when the list holds any, else the container's own address.
struct C3DTriangle2 { unsigned short VertexIndex[3]; };
template <class T> struct CArray {
    T* m_first; T* m_last; T* m_end;
    long size() const { return (long)(m_last - m_first); }
};
struct C3DMeshFileTrianglesChunk {
    void* __vftable;
    unsigned char _chunkBase[0x28];
    CArray<C3DTriangle2> TriangleList;
    const C3DTriangle2* GetList() const
    {
        const CArray<C3DTriangle2>* list = &TriangleList;
        return list->size() != 0 ? list->m_first : (const C3DTriangle2*)list;
    }
};
int main()
{
    C3DTriangle2 tris[4];
    C3DMeshFileTrianglesChunk c;
    c.TriangleList.m_first = tris; c.TriangleList.m_last = tris; c.TriangleList.m_end = tris + 4;
    if (c.GetList() != (const C3DTriangle2*)&c.TriangleList) { std::printf("00abc440_TEST FAIL\n"); return 1; }
    c.TriangleList.m_last = tris + 3;
    if (c.GetList() != tris) { std::printf("00abc440_TEST FAIL\n"); return 1; }
    if (sizeof(C3DTriangle2) != 6) { std::printf("00abc440_TEST FAIL\n"); return 1; }
    std::printf("00abc440_TEST PASS\n");
    return 0;
}