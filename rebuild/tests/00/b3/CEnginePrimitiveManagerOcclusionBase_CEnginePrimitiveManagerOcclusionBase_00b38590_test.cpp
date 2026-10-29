#include <cstdio>
static int g_deleted = 0, g_baseDtor = 0;
struct CRefCountedShader_00bc9a00 { virtual ~CRefCountedShader_00bc9a00() { ++g_deleted; } unsigned char _pad[0xc]; long m_refCount; };
class CShaderBase { public: virtual void v00() { } ~CShaderBase() { } };
class CVertexShader : public CShaderBase { public: CRefCountedShader_00bc9a00* m_pShader; CVertexShader() : m_pShader(0) { } ~CVertexShader() { CRefCountedShader_00bc9a00* shader = m_pShader; if (shader) { if (--shader->m_refCount == 0) delete shader; } } };
class CBaseClassNonCopyable {
public:   virtual ~CBaseClassNonCopyable() { ++g_baseDtor; } };
static int g_destroyed = 0; static void Destroy(void*) { ++g_destroyed; }
struct SCountBlock_00b38590 { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct CCountedPointer_00b38590 { void* m_pObject; SCountBlock_00b38590* m_pCount; CCountedPointer_00b38590() : m_pObject(0), m_pCount(0) { }
    ~CCountedPointer_00b38590() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } m_pObject = 0; m_pCount = 0; } };
struct CEnginePrimitiveManagerOcclusionBase : CBaseClassNonCopyable { long m_type; bool m_enabled; CVertexShader m_vertexShader; CCountedPointer_00b38590 m_occlusionData; ~CEnginePrimitiveManagerOcclusionBase() { } };
int main() {
    CRefCountedShader_00bc9a00* sh = new CRefCountedShader_00bc9a00; sh->m_refCount = 1; SCountBlock_00b38590* blk = new SCountBlock_00b38590; blk->m_references = 1; blk->m_pfnDestroy = &Destroy; blk->m_pObject = sh;
    { CEnginePrimitiveManagerOcclusionBase m; m.m_vertexShader.m_pShader = sh; m.m_occlusionData.m_pObject = sh; m.m_occlusionData.m_pCount = blk; }
    if (g_destroyed != 1 || g_deleted != 1 || g_baseDtor != 1) { std::printf("00b38590_TEST FAIL\n"); return 1; }
    std::printf("00b38590_TEST PASS\n"); return 0;
}