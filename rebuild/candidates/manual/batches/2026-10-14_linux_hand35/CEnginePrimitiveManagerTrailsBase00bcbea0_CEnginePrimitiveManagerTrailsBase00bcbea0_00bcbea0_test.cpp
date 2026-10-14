#include <cstdio>
static int g_deleted = 0, g_baseDtor = 0;
struct CRefCountedShader_00bc9a00 { virtual ~CRefCountedShader_00bc9a00() { ++g_deleted; } unsigned char _pad[0xc]; long m_refCount; };
struct CShaderBase { virtual void v00() { } ~CShaderBase() { } };
struct CVertexShader : CShaderBase { CRefCountedShader_00bc9a00* m_pShader; CVertexShader() : m_pShader(0) { } ~CVertexShader() { CRefCountedShader_00bc9a00* shader = m_pShader; if (shader) { if (--shader->m_refCount == 0) delete shader; } } };
struct CBase { virtual void v00() { } ~CBase() { ++g_baseDtor; } };
static int g_owner = 0;
struct CResourceOwner_00bf3160 { unsigned char _body[4]; ~CResourceOwner_00bf3160() { ++g_owner; } };
struct CEngineDynamic3DResourceManager { virtual void v00() { } unsigned char _pad[0xc]; CResourceOwner_00bf3160 m_owner; ~CEngineDynamic3DResourceManager() { } };
struct CEnginePrimitiveManagerTrailsBase_00bcbea0 : CBase { long m_type; bool m_enabled; CVertexShader m_vertexShader; CEngineDynamic3DResourceManager m_resources; ~CEnginePrimitiveManagerTrailsBase_00bcbea0() { } };
int main() {
    CRefCountedShader_00bc9a00* sh = new CRefCountedShader_00bc9a00; sh->m_refCount = 1;
    { CEnginePrimitiveManagerTrailsBase_00bcbea0 m; m.m_vertexShader.m_pShader = sh; } if (g_owner != 1 || g_deleted != 1 || g_baseDtor != 1) { std::printf("00bcbea0_TEST FAIL\n"); return 1; }
    std::printf("00bcbea0_TEST PASS\n"); return 0;
}
