// Behaviour test for CEngineWeatherRenderer::~CEngineWeatherRenderer() @ 0x00B513B0: the implicit member destruction order
// (last member first: Settings, then the mist/rain/snow shaders and buffers), each buffer Uninitialise'd, shader data released.
#include <cstdio>
#include <string>
namespace model {
std::string g_order;
struct CVertexBufferWin32 { const char* n; CVertexBufferWin32(const char* name) : n(name) {} void Uninitialise() { g_order += n; g_order += ';'; } ~CVertexBufferWin32() { Uninitialise(); } };
struct CIndexBuffer { const char* n; CIndexBuffer(const char* name) : n(name) {} void Uninitialise() { g_order += n; g_order += ';'; } ~CIndexBuffer() { Uninitialise(); } };
struct CShaderData { int refs; CShaderData() : refs(1) {} bool RemoveReference() { return --refs != 0; } };
struct CShader { const char* n; CShaderData* Data; CShader(const char* name, CShaderData* d) : n(name), Data(d) {}
    ~CShader() { if (Data) { CShaderData* current = Data; if (!current->RemoveReference()) { delete current; g_order += n; g_order += ";"; } } } };
struct CEngineWeatherSettings { ~CEngineWeatherSettings() { g_order += "Settings;"; } };
struct CEngineComponent { virtual ~CEngineComponent() { g_order += "Component;"; } };
struct CEngineWeatherRenderer : CEngineComponent {
    CVertexBufferWin32 RainVertexBuffer; CIndexBuffer RainIndexBuffer; CShader RainVertexShader; CShader MistPixelShader; CEngineWeatherSettings Settings;
    CEngineWeatherRenderer() : RainVertexBuffer("RainVB"), RainIndexBuffer("RainIB"), RainVertexShader("RainVS", new CShaderData), MistPixelShader("MistPS", new CShaderData) {}
    ~CEngineWeatherRenderer() {}
};
}
int main()
{
    { model::CEngineWeatherRenderer renderer; }
    bool ok = model::g_order == "Settings;MistPS;RainVS;RainIB;RainVB;Component;";
    if (!ok) { std::printf("00b513b0_TEST FAIL %s\n", model::g_order.c_str()); return 1; }
    std::printf("00b513b0_TEST PASS\n");
    return 0;
}