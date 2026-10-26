#include <cstdio>
static int g_freed = 0, g_destroyed = 0, g_base = 0;
static void free_x(void* p) { ++g_freed; }
struct CMeshResource { virtual void v00() { } virtual void Destroy() { ++g_destroyed; } long m_refCount; void Release() { if (--m_refCount == 0) Destroy(); } };
struct CIVCountedPointer_Mesh { CMeshResource* m_p; ~CIVCountedPointer_Mesh() { if (m_p) { m_p->Release(); m_p = 0; } } };
class CTCBase { public: unsigned char _pad[0x0c]; ~CTCBase() { ++g_base; } };
struct CMallocBlock { void* m_p; ~CMallocBlock() { if (m_p != 0) free_x(m_p); } };
struct CMeshBase : CTCBase { CIVCountedPointer_Mesh m_resource; CMallocBlock m_vertices; ~CMeshBase() { } };
int main() { CMeshResource res; res.m_refCount = 1; int verts; { CMeshBase m; m.m_resource.m_p = &res; m.m_vertices.m_p = &verts; } if (g_freed != 1 || g_destroyed != 1 || g_base != 1) { std::printf("004e2763_TEST FAIL\n"); return 1; }
    res.m_refCount = 2; { CMeshBase m; m.m_resource.m_p = &res; m.m_vertices.m_p = 0; } if (g_freed != 1 || g_destroyed != 1 || res.m_refCount != 1 || g_base != 2) { std::printf("004e2763_TEST FAIL\n"); return 1; } std::printf("004e2763_TEST PASS\n"); return 0; }