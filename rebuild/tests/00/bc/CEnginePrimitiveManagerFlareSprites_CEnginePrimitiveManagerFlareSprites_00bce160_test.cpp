// Behaviour test for 0x00BCE160 (see src/00bce160.cpp): the flare-sprite manager destructor releasing its shader and freeing the map.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00bce160_TEST FAIL\n"); return 1; } } while (0)

static int g_shaderDeletes = 0, g_erases = 0, g_frees = 0, g_baseDtor = 0;
struct CShaderData { int RefCount; };
class CVertexShader { public:
    CShaderData* Data;
    ~CVertexShader() { CShaderData* d = Data; if (d) { if (--d->RefCount == 0) ++g_shaderDeletes; } }
};
struct SNode { int colour; SNode* parent; SNode* left; SNode* right; };
struct CVertexBufferMap {
    SNode* header; unsigned int count;
    void Erase(SNode* n) { if (n) ++g_erases; }
    void clear() { if (count != 0) { Erase(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; } }
    ~CVertexBufferMap() { clear(); if (header) { ++g_frees; delete header; } }
};
namespace N_0099a300 { struct CBase { virtual ~CBase() { ++g_baseDtor; } }; } using N_0099a300::CBase;
class CEnginePrimitiveManagerFlareSprites : public CBase { public: CVertexBufferMap FlareVertexBuffers; CVertexShader VertexShaderFlareTest; float FadeInSpeed; };
int main()
{
    CShaderData shader = { 1 }; SNode root;
    CEnginePrimitiveManagerFlareSprites* m = new CEnginePrimitiveManagerFlareSprites;
    m->FlareVertexBuffers.header = new SNode; m->FlareVertexBuffers.header->parent = &root; m->FlareVertexBuffers.count = 2;
    m->VertexShaderFlareTest.Data = &shader;
    delete m;
    CHECK(g_shaderDeletes == 1 && shader.RefCount == 0 && g_erases == 1 && g_frees == 1 && g_baseDtor == 1);
    shader.RefCount = 2;
    m = new CEnginePrimitiveManagerFlareSprites;
    m->FlareVertexBuffers.header = 0; m->FlareVertexBuffers.count = 0; m->VertexShaderFlareTest.Data = &shader;
    delete m;
    CHECK(g_shaderDeletes == 1 && shader.RefCount == 1 && g_erases == 1 && g_frees == 1 && g_baseDtor == 2);
    std::printf("00bce160_TEST PASS\n");
    return 0;
}