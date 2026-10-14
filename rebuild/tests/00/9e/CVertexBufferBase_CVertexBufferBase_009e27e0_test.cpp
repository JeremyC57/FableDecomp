#include <cstdio>
#include <stddef.h>
struct IDirect3DVertexBuffer9;
static int g_baseCtors = 0;
struct CBaseClassNonCopyable { virtual void v00() { } CBaseClassNonCopyable() { ++g_baseCtors; } };
struct CVertexCompressionParams { float PosScaleX, PosScaleY, PosScaleZ, PosScaleW, PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams() : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f), PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { } };
struct CVertexBufferBase : CBaseClassNonCopyable {
    IDirect3DVertexBuffer9* PVertexBuffer; unsigned long Length; long Type; unsigned char Stride; CVertexCompressionParams CompressionParams;
    virtual void v00() { }
    CVertexBufferBase() : CBaseClassNonCopyable(), PVertexBuffer(0), Length(0), Type(0), Stride(0), CompressionParams() { }
};
int main()
{
    CVertexBufferBase b;
    if (g_baseCtors != 1 || b.PVertexBuffer != 0 || b.Length != 0 || b.Type != 0 || b.Stride != 0 || b.CompressionParams.PosScaleX != 1.0f || b.CompressionParams.PosScaleW != 1.0f
        || b.CompressionParams.PosBiasX != 0.0f || b.CompressionParams.PosBiasW != 0.0f || offsetof(CVertexBufferBase, CompressionParams) != 0x14 || sizeof(b) != 0x34) { std::printf("009e27e0_TEST FAIL\n"); return 1; }
    std::printf("009e27e0_TEST PASS\n"); return 0;
}