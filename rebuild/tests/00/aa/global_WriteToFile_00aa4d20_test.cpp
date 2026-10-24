#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

class CDataOutputStream {
public:
    float Written[16];
    int   Count;
    CDataOutputStream() : Count(0) { }
    void WriteFloat(float value) { if (Count < 16) Written[Count] = value; ++Count; }
};
struct CVertexCompressionParams {
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
};
// the row: WriteToFile(params, stream) -- the eight floats in declaration order
static void __fastcall WriteToFile(const CVertexCompressionParams& params, CDataOutputStream& stream)
{
    stream.WriteFloat(params.PosScaleX);
    stream.WriteFloat(params.PosScaleY);
    stream.WriteFloat(params.PosScaleZ);
    stream.WriteFloat(params.PosScaleW);
    stream.WriteFloat(params.PosBiasX);
    stream.WriteFloat(params.PosBiasY);
    stream.WriteFloat(params.PosBiasZ);
    stream.WriteFloat(params.PosBiasW);
}
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    CVertexCompressionParams p = { 1.0f, 2.0f, 3.0f, 4.0f, -0.5f, -1.5f, -2.5f, 0.125f };
    CDataOutputStream s;
    WriteToFile(p, s);
    Check(s.Count == 8);
    const float* f = &p.PosScaleX;
    for (int k = 0; k < 8; ++k)
        Check(Near(s.Written[k], f[k]));

    if (g_fail) {
        std::printf("00aa4d20_TEST FAIL\n");
        return 1;
    }
    std::printf("00aa4d20_TEST PASS\n");
    return 0;
}