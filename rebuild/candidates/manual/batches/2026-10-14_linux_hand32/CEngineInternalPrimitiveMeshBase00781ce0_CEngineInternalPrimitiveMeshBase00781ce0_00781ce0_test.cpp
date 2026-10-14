#include <cstdio>
static int g_freed = 0, g_destroyed = 0, g_base = 0;
static void free_x(void* p) { ++g_freed; }
struct CMeshResource { virtual void v00() { } virtual void Destroy() { ++g_destroyed; } long m_refCount; void Release() { if (--m_refCount == 0) Destroy(); } };
struct CIVCountedPointer_Mesh { CMeshResource* m_p; ~CIVCountedPointer_Mesh() { if (m_p) { m_p->Release(); m_p = 0; } } };
struct CTCBase { unsigned char _pad[0x0c]; ~CTCBase() { ++g_base; } };
struct CRetailAllocator { CRetailAllocator() { } }; struct SMeshElement { unsigned char _body[8]; }; static int g_ranged = 0;
static void DestroyRange(SMeshElement* f, SMeshElement* l, const CRetailAllocator&) { g_ranged += (int)(l - f); }
struct CRetailElementVector { SMeshElement* m_pFirst; SMeshElement* m_pLast; SMeshElement* m_pEnd; ~CRetailElementVector() { DestroyRange(m_pFirst, m_pLast, CRetailAllocator()); if (m_pFirst) free_x(m_pFirst); } };
struct CMeshBase : CTCBase { CIVCountedPointer_Mesh m_resource; CRetailElementVector m_elements; ~CMeshBase() { } };
int main() { CMeshResource res; res.m_refCount = 1; SMeshElement els[2]; { CMeshBase m; m.m_resource.m_p = &res; m.m_elements.m_pFirst = els; m.m_elements.m_pLast = els + 2; m.m_elements.m_pEnd = els + 2; }
    if (g_ranged != 2 || g_freed != 1 || g_destroyed != 1 || g_base != 1) { std::printf("00781ce0_TEST FAIL\n"); return 1; } std::printf("00781ce0_TEST PASS\n"); return 0; }
