#include <cstdio>
static int g_baseCtor = 0; static long g_init[5] = { -1, -1, -1, -1, -1 };
struct CBase { virtual void VSlot0() { } CBase() { ++g_baseCtor; } };
struct IDirect3DVertexBuffer9;
struct CVertexCompressionParams {
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW, PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams() : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f), PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};
struct CVertexBufferBase : CBase {
    IDirect3DVertexBuffer9* PVertexBuffer; unsigned long Length; long Type; unsigned char Stride; CVertexCompressionParams CompressionParams;
    CVertexBufferBase() : Length(0), Type(0), CompressionParams() { PVertexBuffer = 0; Stride = 0; }
};
struct CVertexBufferWin32_Retail : CVertexBufferBase {
    virtual void VSlot1() { }
    CVertexBufferWin32_Retail() : CVertexBufferBase() { }
    CVertexBufferWin32_Retail(long type, long a, long b, unsigned long c, long pool) : CVertexBufferBase() { Initialise(type, a, b, c, pool); }
    bool Initialise(long type, long a, long b, unsigned long c, long pool) { g_init[0] = type; g_init[1] = a; g_init[2] = b; g_init[3] = (long)c; g_init[4] = pool; return true; }
};
static bool Shell(const CVertexBufferWin32_Retail& v)
{
    const CVertexCompressionParams& p = v.CompressionParams;
    return v.PVertexBuffer == 0 && v.Length == 0 && v.Type == 0 && v.Stride == 0 && p.PosScaleX == 1.0f && p.PosScaleY == 1.0f && p.PosScaleZ == 1.0f && p.PosScaleW == 1.0f && p.PosBiasX == 0.0f && p.PosBiasY == 0.0f && p.PosBiasZ == 0.0f && p.PosBiasW == 0.0f;
}
inline void* operator new(unsigned int, void* where);
int main()
{
    unsigned char raw[sizeof(CVertexBufferWin32_Retail)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xee;
    CVertexBufferWin32_Retail* v = new (raw) CVertexBufferWin32_Retail(2, 30, 40, 50, 1);
    if (g_baseCtor != 1 || !Shell(*v) || g_init[0] != 2 || g_init[1] != 30 || g_init[2] != 40 || g_init[3] != 50 || g_init[4] != 1) { std::printf("009e2830_TEST FAIL\n"); return 1; }
    std::printf("009e2830_TEST PASS\n"); return 0;
}
inline void* operator new(unsigned int, void* where) { return where; }