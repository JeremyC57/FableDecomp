// behaviour test for 0x00AC44D0 CVertexStreamConverter::ConvertFrom
#include <stdio.h>
#include <math.h>
#include "engine/C4DVector.h"
#include "engine/CVertexCompressionParams.h"
enum EVertexStreamDataType {
    VSDT_FLOAT1 = 0, VSDT_FLOAT2 = 1, VSDT_FLOAT3 = 2, VSDT_FLOAT4 = 3, VSDT_D3DCOLOR = 4, VSDT_SHORT2 = 5,
    VSDT_SHORT4 = 6, VSDT_SHORT2_UV = 7, VSDT_SHORT1N = 8, VSDT_SHORT2N = 9, VSDT_SHORT3N = 10, VSDT_SHORT4N = 11,
    VSDT_DEC3N = 12, VSDT_DEC3N_SCALED = 13
};
class CVertexStreamConverter { public:
    void ConvertFrom(const char* data, EVertexStreamDataType type, C4DVector& v, const CVertexCompressionParams& params);
    void ConvertTo(char* data, EVertexStreamDataType type, const C4DVector& v, const CVertexCompressionParams& params);
};

void CVertexStreamConverter::ConvertFrom(const char* data, EVertexStreamDataType type, C4DVector& v,
                                         const CVertexCompressionParams& params)
{
    v = C4DVector(0.0f, 0.0f, 0.0f, 1.0f);
    switch (type) {
    case VSDT_FLOAT1:
        v.X = ((const float*)data)[0];
        return;
    case VSDT_FLOAT2:
        v.X = ((const float*)data)[0];
        v.Y = ((const float*)data)[1];
        return;
    case VSDT_FLOAT3:
        v.X = ((const float*)data)[0];
        v.Y = ((const float*)data)[1];
        v.Z = ((const float*)data)[2];
        return;
    case VSDT_FLOAT4:
        v.X = ((const float*)data)[0];
        v.Y = ((const float*)data)[1];
        v.Z = ((const float*)data)[2];
        v.W = ((const float*)data)[3];
        return;
    case VSDT_D3DCOLOR:
        v.X = (float)((const unsigned char*)data)[2] * (1.0f / 255.0f);
        v.Y = (float)((const unsigned char*)data)[1] * (1.0f / 255.0f);
        v.Z = (float)((const unsigned char*)data)[0] * (1.0f / 255.0f);
        v.W = (float)((const unsigned char*)data)[3] * (1.0f / 255.0f);
        return;
    case VSDT_SHORT2:
        v.X = (float)((const short*)data)[0];
        v.Y = (float)((const short*)data)[1];
        return;
    case VSDT_SHORT4:
        v.X = (float)((const short*)data)[0];
        v.Y = (float)((const short*)data)[1];
        v.Z = (float)((const short*)data)[2];
        v.W = (float)((const short*)data)[3];
        return;
    case VSDT_SHORT2_UV:
        v.X = (float)((const short*)data)[0] * (1.0f / 2048.0f) - 8.0f;
        v.Y = (float)((const short*)data)[1] * (1.0f / 2048.0f) - 8.0f;
        return;
    case VSDT_SHORT1N:
        v.X = (float)((const short*)data)[0] * (1.0f / 32768.0f);
        return;
    case VSDT_SHORT2N:
        v.X = (float)((const short*)data)[0] * (1.0f / 32768.0f);
        v.Y = (float)((const short*)data)[1] * (1.0f / 32768.0f);
        return;
    case VSDT_SHORT3N:
        v.X = (float)((const short*)data)[0] * (1.0f / 32768.0f);
        v.Y = (float)((const short*)data)[1] * (1.0f / 32768.0f);
        v.Z = (float)((const short*)data)[2] * (1.0f / 32768.0f);
        return;
    case VSDT_SHORT4N:
        v.X = (float)((const short*)data)[0] * (1.0f / 32768.0f);
        v.Y = (float)((const short*)data)[1] * (1.0f / 32768.0f);
        v.Z = (float)((const short*)data)[2] * (1.0f / 32768.0f);
        v.W = (float)((const short*)data)[3] * (1.0f / 32768.0f);
        return;
    case VSDT_DEC3N: {
        unsigned long packed = *(const unsigned long*)data;
        long x = packed & 0x7FF;
        long y = (packed >> 11) & 0x7FF;
        long z = packed >> 22;
        x |= (x & 0x400) ? 0xFFFFF800 : 0;
        y |= (y & 0x400) ? 0xFFFFF800 : 0;
        z |= (z & 0x200) ? 0xFFFFFC00 : 0;
        v.X = (float)x * (1.0f / 1023.0f);
        v.Y = (float)y * (1.0f / 1023.0f);
        v.Z = (float)z * (1.0f / 511.0f);
        return;
    }
    case VSDT_DEC3N_SCALED: {
        unsigned long packed = *(const unsigned long*)data;
        long x = packed & 0x7FF;
        long y = (packed >> 11) & 0x7FF;
        long z = packed >> 22;
        x |= (x & 0x400) ? 0xFFFFF800 : 0;
        y |= (y & 0x400) ? 0xFFFFF800 : 0;
        z |= (z & 0x200) ? 0xFFFFFC00 : 0;
        v.X = (float)x * params.PosScaleX * (1.0f / 1023.0f) + params.PosBiasX;
        v.Y = (float)y * params.PosScaleY * (1.0f / 1023.0f) + params.PosBiasY;
        v.Z = (float)z * params.PosScaleZ * (1.0f / 511.0f) + params.PosBiasZ;
        return;
    }
    }
}

static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }
static bool Is(const C4DVector& v, float x, float y, float z, float w) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z) && Near(v.W, w); }
int main()
{
    CVertexStreamConverter c; CVertexCompressionParams p; C4DVector v(9.0f, 9.0f, 9.0f, 9.0f);
    bool ok = true;
    float f[4] = { 1.5f, -2.0f, 3.25f, 4.0f };
    c.ConvertFrom((const char*)f, VSDT_FLOAT1, v, p); ok = ok && Is(v, 1.5f, 0.0f, 0.0f, 1.0f);
    c.ConvertFrom((const char*)f, VSDT_FLOAT3, v, p); ok = ok && Is(v, 1.5f, -2.0f, 3.25f, 1.0f);
    c.ConvertFrom((const char*)f, VSDT_FLOAT4, v, p); ok = ok && Is(v, 1.5f, -2.0f, 3.25f, 4.0f);
    unsigned char col[4] = { 0, 51, 255, 102 };   // B, G, R, A
    c.ConvertFrom((const char*)col, VSDT_D3DCOLOR, v, p); ok = ok && Is(v, 1.0f, 0.2f, 0.0f, 0.4f);
    short s[4] = { -3, 7, 16384, -32768 };
    c.ConvertFrom((const char*)s, VSDT_SHORT4, v, p); ok = ok && Is(v, -3.0f, 7.0f, 16384.0f, -32768.0f);
    c.ConvertFrom((const char*)s, VSDT_SHORT4N, v, p); ok = ok && Is(v, -3.0f / 32768.0f, 7.0f / 32768.0f, 0.5f, -1.0f);
    short uv[2] = { 16384, 0 };
    c.ConvertFrom((const char*)uv, VSDT_SHORT2_UV, v, p); ok = ok && Is(v, 0.0f, -8.0f, 0.0f, 1.0f);
    unsigned int packed = (511u << 22) | ((0x7FFu & (unsigned int)-1023) << 11) | 1023u;   // z +1, y -1, x +1
    c.ConvertFrom((const char*)&packed, VSDT_DEC3N, v, p); ok = ok && Is(v, 1.0f, -1.0f, 1.0f, 1.0f);
    p.PosScaleX = 2.0f; p.PosBiasX = 0.5f; p.PosScaleZ = 4.0f; p.PosBiasZ = -1.0f;
    c.ConvertFrom((const char*)&packed, VSDT_DEC3N_SCALED, v, p); ok = ok && Is(v, 2.5f, -1.0f, 3.0f, 1.0f);
    c.ConvertFrom((const char*)f, (EVertexStreamDataType)20, v, p); ok = ok && Is(v, 0.0f, 0.0f, 0.0f, 1.0f);
    printf(ok ? "00ac44d0_TEST PASS\n" : "00ac44d0_TEST FAIL\n");
    return ok ? 0 : 1;
}