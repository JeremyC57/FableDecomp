#include <cstdio>
static int g_deleted = 0, g_baseDtor = 0;
struct CRefCountedShader_00bc9a00 { virtual ~CRefCountedShader_00bc9a00() { ++g_deleted; } unsigned char _pad[0xc]; long m_refCount; };
class CShaderBase { public: virtual void v00() { } ~CShaderBase() { } };
class CVertexShader : public CShaderBase { public: CRefCountedShader_00bc9a00* m_pShader; CVertexShader() : m_pShader(0) { } ~CVertexShader() { CRefCountedShader_00bc9a00* shader = m_pShader; if (shader) { if (--shader->m_refCount == 0) delete shader; } } };
namespace N_0099a300 { struct CBase { virtual void v00() { } ~CBase() { ++g_baseDtor; } }; } using N_0099a300::CBase;
class CEnginePrimitiveManagerLines : public CBase { public: long m_type; bool m_enabled; CVertexShader m_vertexShader; ~CEnginePrimitiveManagerLines() { } };
int main() {
    CRefCountedShader_00bc9a00* sh = new CRefCountedShader_00bc9a00; sh->m_refCount = 2;
    { CEnginePrimitiveManagerLines m; m.m_vertexShader.m_pShader = sh; } if (g_deleted != 0 || sh->m_refCount != 1 || g_baseDtor != 1) { std::printf("00bc9a00_TEST FAIL\n"); return 1; }
    { CEnginePrimitiveManagerLines m; m.m_vertexShader.m_pShader = sh; } if (g_deleted != 1 || g_baseDtor != 2) { std::printf("00bc9a00_TEST FAIL\n"); return 1; }
    { CEnginePrimitiveManagerLines m; } if (g_deleted != 1 || g_baseDtor != 3) { std::printf("00bc9a00_TEST FAIL\n"); return 1; }
    std::printf("00bc9a00_TEST PASS\n"); return 0;
}