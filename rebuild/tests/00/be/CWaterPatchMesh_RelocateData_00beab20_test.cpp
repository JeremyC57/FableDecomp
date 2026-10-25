// Behaviour test for CWaterPatchMesh::RelocateData (0x00BEAB20): only a vertex buffer pointer equal to the moved block
// is rebased; other blocks and a null pointer are left alone.
#include <cstdio>

class CVertexBufferWin32 { public: int dummy[4]; };
template <class T> inline void RelocatePointer(T*& p, void* oldData, void* newData)
{
    if (p)
        p = (T*)((char*)p - (char*)oldData + (char*)newData);
}
class CWaterPatchMesh {
public:
    virtual ~CWaterPatchMesh() {}
    virtual void RelocateData(long handle, void* oldData, void* newData, long size);
    void*               Map;
    long                OffsetX, OffsetY;
    CVertexBufferWin32* VertexBuffer;
    void*               VertexHeightData;
};
void CWaterPatchMesh::RelocateData(long handle, void* oldData, void* newData, long size)
{
    if (oldData == VertexBuffer)
        RelocatePointer(VertexBuffer, oldData, newData);
}

int main()
{
    CVertexBufferWin32 a, b, c;
    CWaterPatchMesh m; m.VertexBuffer = &a; m.VertexHeightData = 0;
    m.RelocateData(1, &c, &b, 16);              // another block moved
    if (m.VertexBuffer != &a) { std::printf("00beab20_TEST FAIL\n"); return 1; }
    m.RelocateData(1, &a, &b, 16);              // our block moved
    if (m.VertexBuffer != &b) { std::printf("00beab20_TEST FAIL\n"); return 1; }
    m.VertexBuffer = 0;
    m.RelocateData(1, 0, &c, 16);               // null stays null
    if (m.VertexBuffer != 0) { std::printf("00beab20_TEST FAIL\n"); return 1; }
    std::printf("00beab20_TEST PASS\n");
    return 0;
}