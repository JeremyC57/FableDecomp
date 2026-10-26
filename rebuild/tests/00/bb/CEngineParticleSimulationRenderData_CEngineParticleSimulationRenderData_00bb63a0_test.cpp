// Behaviour test for CEngineParticleSimulationRenderData::CEngineParticleSimulationRenderData @ 0x00BB63A0.
#include <cstdio>
static int g_baseCtors = 0;
struct CBase { virtual int Kind() { return 0; } CBase() { ++g_baseCtors; } virtual ~CBase() { } };
class CVertexCompressionParams { public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW, PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams() : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
        PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};
struct CVertexBufferBase : CBase {
    void* PVertexBuffer; unsigned int Length; long Type; unsigned char Stride; CVertexCompressionParams CompressionParams;
    CVertexBufferBase() : Length(0), Type(0), CompressionParams() { }
};
class CVertexBufferWin32 : public CVertexBufferBase { public:
    virtual int Kind() { return 2; }
    CVertexBufferWin32() : CVertexBufferBase() { PVertexBuffer = 0; Stride = 0; }
};
struct CEngineParticleSimulationRenderData : CBase {
    CVertexBufferWin32 VertexBuffers[2]; long CurrentVB;
    virtual int Kind() { return 1; }
    CEngineParticleSimulationRenderData() : CurrentVB(0) { }
};
int main()
{
    CEngineParticleSimulationRenderData d;
    int ok = g_baseCtors == 3 && d.Kind() == 1 && d.CurrentVB == 0;
    for (int i = 0; i < 2; ++i) {
        const CVertexBufferWin32& v = d.VertexBuffers[i];
        ok = ok && d.VertexBuffers[i].Kind() == 2 && v.PVertexBuffer == 0 && v.Length == 0 && v.Type == 0 && v.Stride == 0
             && v.CompressionParams.PosScaleX == 1.0f && v.CompressionParams.PosScaleW == 1.0f
             && v.CompressionParams.PosBiasX == 0.0f && v.CompressionParams.PosBiasW == 0.0f;
    }
    if (!ok) { std::printf("00bb63a0_TEST FAIL\n"); return 1; }
    std::printf("00bb63a0_TEST PASS\n");
    return 0;
}