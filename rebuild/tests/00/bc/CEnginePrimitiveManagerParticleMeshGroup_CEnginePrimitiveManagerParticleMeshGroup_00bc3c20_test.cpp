// Behaviour test for CEnginePrimitiveManagerParticleMeshGroup::~CEnginePrimitiveManagerParticleMeshGroup (0x00BC3C20, compiler-generated): the five pixel shaders in reverse and the displacement vertex shader (shared shader data: drop a reference, delete on the last), the point-light vertex-shader array (virtual element dtors forward, then storage), then ~CEnginePrimitiveManagerOcclusionBase.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
static char g_log[256];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct ShaderDataModel { char Tag; unsigned long ReferenceCount; virtual ~ShaderDataModel() { Log(Tag); } };
struct ShaderBaseModel { virtual ~ShaderBaseModel() { } };
struct ShaderModel : ShaderBaseModel {
    ShaderDataModel* Data;
    ShaderModel() : Data(0) { }
    virtual ~ShaderModel() { ShaderDataModel* d = Data; if (d) { if (--d->ReferenceCount == 0) delete d; } }
};
struct ShaderArrayModel {
    ShaderModel* Start; ShaderModel* Finish;
    ~ShaderArrayModel() { for (ShaderModel* s = Start; s != Finish; ++s) s->~ShaderModel(); if (Start) { Log('F'); ::operator delete(Start); } }
};
struct OcclusionBaseModel { virtual ~OcclusionBaseModel() { Log('O'); } };
struct ParticleMeshGroupModel : OcclusionBaseModel {
    ShaderArrayModel VertexShadersMeshPointLights;
    ShaderModel      VertexShaderDisplacement;
    ShaderModel      PixelShader;
    ShaderModel      PixelShaderStipple;
    ShaderModel      PixelShaderAdditiveAlpha;
    ShaderModel      PixelShaderAdditiveAlphaStipple;
    ShaderModel      PixelShaderDisplacement;
};
static ShaderDataModel* NewData(char tag, unsigned long refs) { ShaderDataModel* d = new ShaderDataModel; d->Tag = tag; d->ReferenceCount = refs; return d; }
int main()
{
    ParticleMeshGroupModel* g = new ParticleMeshGroupModel;
    ShaderModel* lights = (ShaderModel*)::operator new(2 * sizeof(ShaderModel));
    new (&lights[0]) ShaderModel; lights[0].Data = NewData('x', 1);
    new (&lights[1]) ShaderModel; lights[1].Data = NewData('y', 1);
    g->VertexShadersMeshPointLights.Start = lights; g->VertexShadersMeshPointLights.Finish = lights + 2;
    g->VertexShaderDisplacement.Data = NewData('d', 1);
    g->PixelShader.Data = NewData('1', 1);
    ShaderDataModel* shared = NewData('2', 2);
    g->PixelShaderStipple.Data = shared;
    g->PixelShaderAdditiveAlpha.Data = NewData('3', 1);
    g->PixelShaderAdditiveAlphaStipple.Data = NewData('4', 1);
    g->PixelShaderDisplacement.Data = NewData('5', 1);
    OcclusionBaseModel* p = g;
    delete p;
    if (std::strcmp(g_log, "5431dxyFO") != 0 || shared->ReferenceCount != 1) { std::printf("00bc3c20_TEST FAIL\n"); return 1; }
    delete shared;
    std::printf("00bc3c20_TEST PASS\n");
    return 0;
}