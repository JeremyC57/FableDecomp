// Test: CLandscapeLayerMesh::RelocateData -- moving the index buffer repoints it and runs its post-relocation hook; moving the foreground vertex buffer only repoints; anything else is ignored.
#include <cstdio>
static int g_post = 0;
class CIndexBuffer { public: int x; void OnPostRelocation() { ++g_post; } };
struct CVertexBuffer { int y; };
template <class T> void RelocatePointer(T*& p, void* oldData, void* newData) { p = (T*)((char*)p + ((char*)newData - (char*)oldData)); }
struct CMesh { CIndexBuffer* IndexBuffer; CVertexBuffer* ForegroundVertexBuffer;
    void RelocateData(int, void* oldData, void* newData, int) {
        if (oldData == IndexBuffer) { RelocatePointer(IndexBuffer, oldData, newData); IndexBuffer->OnPostRelocation(); return; }
        if (oldData == ForegroundVertexBuffer) RelocatePointer(ForegroundVertexBuffer, oldData, newData); } };
int main()
{
    CIndexBuffer ib[2]; CVertexBuffer vb[2]; int other;
    CMesh m; m.IndexBuffer = &ib[0]; m.ForegroundVertexBuffer = &vb[0];
    m.RelocateData(1, &ib[0], &ib[1], 4);
    bool ok = m.IndexBuffer == &ib[1] && g_post == 1;
    m.RelocateData(1, &vb[0], &vb[1], 4);
    ok = ok && m.ForegroundVertexBuffer == &vb[1] && g_post == 1;
    m.RelocateData(1, &other, &vb[0], 4);
    ok = ok && m.IndexBuffer == &ib[1] && m.ForegroundVertexBuffer == &vb[1] && g_post == 1;
    std::printf(ok ? "00bfde80_TEST PASS\n" : "00bfde80_TEST FAIL\n");
    return ok ? 0 : 1;
}