#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
#include <string.h>
#include <new>
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { } CBaseClassNonCopyable() { } };
template <class T> struct CAllocator { CAllocator() { } };
template <class T> struct CAllocProxy : CAllocator<T> {
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {
    T* m_pStart; T* m_pFinish; CAllocProxy<T> m_endOfStorage;
    CVectorBase(const CAllocator<T>& alloc) : m_pStart(0), m_pFinish(0), m_endOfStorage(alloc, 0) { }
};
template <class T> struct CSTLportVector : CVectorBase<T> {
    explicit CSTLportVector(const CAllocator<T>& alloc = CAllocator<T>()) : CVectorBase<T>(alloc) { }
};
template <class T> struct CArray : CSTLportVector<T> { };
static int g_inputs = 0;
struct CVertexShaderInput { virtual ~CVertexShaderInput() { } long m_body[6]; CVertexShaderInput() { ++g_inputs; for (int i = 0; i < 6; ++i) m_body[i] = 0; } };
struct CShaderDataBank; struct CShaderRenderManager; struct CShaderBase; struct SElement;
struct CShaderResource : CBaseClassNonCopyable {
    CShaderDataBank* ShaderBank; CArray<CShaderBase*> Shaders; CShaderRenderManager* ShaderRenderManager;
    CVertexShaderInput VertexShaderInput; SElement* VertexShaderDeclaration; long VertexFormat;
    CShaderResource() : ShaderBank(0), VertexShaderDeclaration(0), VertexFormat(0) { ShaderRenderManager = 0; }
};
int main()
{
    unsigned char raw[sizeof(CShaderResource)]; memset(raw, 0xcd, sizeof(raw));
    CShaderResource* r = new (raw) CShaderResource;
    if (g_inputs != 1 || r->ShaderBank || r->Shaders.m_pStart || r->Shaders.m_pFinish || r->Shaders.m_endOfStorage.m_data || r->ShaderRenderManager
        || r->VertexShaderDeclaration || r->VertexFormat != 0) { std::printf("00a5d870_TEST FAIL\n"); return 1; }
    std::printf("00a5d870_TEST PASS\n");
    return 0;
}