// Test: N3DPrimitive::CVertexShaderInputStatic scalar deleting destructor -- frees the D3D declaration array, destroys each stream info
// (freeing its element table) and frees the stream array, runs the base destructor, and frees the object only when bit 0 is set.
#include <cstdio>
#include <new>
#include <cstdlib>
static int g_frees = 0, g_baseDtors = 0, g_deletes = 0;
static void Free(void* p) { if (p) { ++g_frees; std::free(p); } }
class CBaseClass { public: virtual ~CBaseClass() { ++g_baseDtors; } };
struct CVertexFormat : CBaseClass { void* m_pElements; CVertexFormat() : m_pElements(0) {} ~CVertexFormat() { if (m_pElements) Free(m_pElements); } };
struct CStreamInfo { unsigned long m_00; CVertexFormat Format; };
struct CStreamInfoArray {
    CStreamInfo* m_pFirst; CStreamInfo* m_pLast;
    ~CStreamInfoArray() { for (CStreamInfo* p = m_pFirst; p != m_pLast; ++p) p->~CStreamInfo(); if (m_pFirst) Free(m_pFirst); }
};
struct CVertexElementArray { void* m_pFirst; ~CVertexElementArray() { if (m_pFirst) Free(m_pFirst); } };
class CVertexShaderInput : public CBaseClass { public: CStreamInfoArray DataStreams; CVertexElementArray D3DDeclaration; };
struct CVertexShaderInputStatic : CVertexShaderInput { };
static CVertexShaderInputStatic* ScalarDeletingDestructor(CVertexShaderInputStatic* self, unsigned int flags)
{
    self->~CVertexShaderInputStatic();
    if (flags & 1) { ++g_deletes; std::free(self); }
    return self;
}
static CVertexShaderInputStatic* Make(int streams)
{
    CVertexShaderInputStatic* o = (CVertexShaderInputStatic*)std::malloc(sizeof(CVertexShaderInputStatic)); new (o) CVertexShaderInputStatic;
    CStreamInfo* a = (CStreamInfo*)std::malloc(sizeof(CStreamInfo) * (streams ? streams : 1));
    for (int i = 0; i < streams; ++i) { new (&a[i]) CStreamInfo; a[i].Format.m_pElements = std::malloc(8); }
    o->DataStreams.m_pFirst = a; o->DataStreams.m_pLast = a + streams;
    o->D3DDeclaration.m_pFirst = std::malloc(16);
    return o;
}
int main()
{
    bool ok = true;
    CVertexShaderInputStatic* o = Make(2);
    ScalarDeletingDestructor(o, 1);
    ok = ok && g_frees == 4 && g_deletes == 1 && g_baseDtors == 3;   // 2 formats + object base
    g_frees = g_baseDtors = g_deletes = 0;
    CVertexShaderInputStatic* k = Make(0);
    ScalarDeletingDestructor(k, 0);
    ok = ok && g_frees == 2 && g_deletes == 0 && g_baseDtors == 1;
    std::free(k);
    std::puts(ok ? "00a98760_TEST PASS" : "00a98760_TEST FAIL");
    return ok ? 0 : 1;
}