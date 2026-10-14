#include <cstdio>
static int g_copies = 0;
struct CRetailVectorA { void* m_pFirst; void* m_pLast; void* m_pEnd; CRetailVectorA() { } CRetailVectorA(const CRetailVectorA& o) : m_pFirst(o.m_pFirst), m_pLast(o.m_pLast), m_pEnd(o.m_pEnd) { ++g_copies; } };
struct CSurface { unsigned char _body[0x10]; CSurface() { } CSurface(const CSurface& o) { _body[0] = o._body[0]; ++g_copies; } };
struct CTexture { unsigned char _body[8]; CTexture() { } CTexture(const CTexture& o) { _body[0] = o._body[0]; ++g_copies; } };
struct CRetailVectorB { void* m_pFirst; void* m_pLast; void* m_pEnd; CRetailVectorB() { } CRetailVectorB(const CRetailVectorB& o) : m_pFirst(o.m_pFirst), m_pLast(o.m_pLast), m_pEnd(o.m_pEnd) { ++g_copies; } };
struct CEditPrimitiveStats { long m_primitiveCount; long m_vertexCount; CRetailVectorA m_counts; CSurface m_surface; CTexture m_texture; long m_flags; CRetailVectorB m_sizes;
    CEditPrimitiveStats() { }
    CEditPrimitiveStats(const CEditPrimitiveStats& o) : m_primitiveCount(o.m_primitiveCount), m_vertexCount(o.m_vertexCount), m_counts(o.m_counts), m_surface(o.m_surface), m_texture(o.m_texture), m_flags(o.m_flags), m_sizes(o.m_sizes) { } };
int main() { CEditPrimitiveStats a; int x; a.m_primitiveCount = 1; a.m_vertexCount = 2; a.m_counts.m_pFirst = &x; a.m_surface._body[0] = 3; a.m_texture._body[0] = 4; a.m_flags = 5; a.m_sizes.m_pEnd = &x;
    CEditPrimitiveStats b(a); if (g_copies != 4 || b.m_primitiveCount != 1 || b.m_vertexCount != 2 || b.m_counts.m_pFirst != &x || b.m_surface._body[0] != 3 || b.m_texture._body[0] != 4 || b.m_flags != 5 || b.m_sizes.m_pEnd != &x) { std::printf("0082cac0_TEST FAIL\n"); return 1; } std::printf("0082cac0_TEST PASS\n"); return 0; }
