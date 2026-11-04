// behaviour test for 0x00AC4910 CVertexStreamConverter::ConvertTo (round-to-nearest packing through GFFloatToLongNear)
#include <stdio.h>
#include <string.h>
#include "engine/C4DVector.h"
#include "engine/CVertexCompressionParams.h"
#include "engine/GFFloatToLongNear.h"
enum EVertexStreamDataType {
    VSDT_FLOAT1 = 0, VSDT_FLOAT2 = 1, VSDT_FLOAT3 = 2, VSDT_FLOAT4 = 3, VSDT_D3DCOLOR = 4, VSDT_SHORT2 = 5,
    VSDT_SHORT4 = 6, VSDT_SHORT2_UV = 7, VSDT_SHORT1N = 8, VSDT_SHORT2N = 9, VSDT_SHORT3N = 10, VSDT_SHORT4N = 11,
    VSDT_DEC3N = 12, VSDT_DEC3N_SCALED = 13
};
class CVertexStreamConverter { public:
    void ConvertFrom(const char* data, EVertexStreamDataType type, C4DVector& v, const CVertexCompressionParams& params);
    void ConvertTo(char* data, EVertexStreamDataType type, const C4DVector& v, const CVertexCompressionParams& params);
};

void CVertexStreamConverter::ConvertTo(char* data, EVertexStreamDataType type, const C4DVector& v,
                                       const CVertexCompressionParams& params)
{
    switch (type) {
    case VSDT_FLOAT1:
        ((float*)data)[0] = v.X;
        return;
    case VSDT_FLOAT2:
        ((float*)data)[0] = v.X;
        ((float*)data)[1] = v.Y;
        return;
    case VSDT_FLOAT3:
        ((float*)data)[0] = v.X;
        ((float*)data)[1] = v.Y;
        ((float*)data)[2] = v.Z;
        return;
    case VSDT_FLOAT4:
        ((float*)data)[0] = v.X;
        ((float*)data)[1] = v.Y;
        ((float*)data)[2] = v.Z;
        ((float*)data)[3] = v.W;
        return;
    case VSDT_D3DCOLOR:
        ((unsigned char*)data)[2] = (unsigned char)GFFloatToLongNear(v.X * 255.0f);
        ((unsigned char*)data)[1] = (unsigned char)GFFloatToLongNear(v.Y * 255.0f);
        ((unsigned char*)data)[0] = (unsigned char)GFFloatToLongNear(v.Z * 255.0f);
        ((unsigned char*)data)[3] = (unsigned char)GFFloatToLongNear(v.W * 255.0f);
        return;
    case VSDT_SHORT2:
        ((short*)data)[0] = (short)GFFloatToLongNear(v.X);
        ((short*)data)[1] = (short)GFFloatToLongNear(v.Y);
        return;
    case VSDT_SHORT4:
        ((short*)data)[0] = (short)GFFloatToLongNear(v.X);
        ((short*)data)[1] = (short)GFFloatToLongNear(v.Y);
        ((short*)data)[2] = (short)GFFloatToLongNear(v.Z);
        ((short*)data)[3] = (short)GFFloatToLongNear(v.W);
        return;
    case VSDT_SHORT2_UV:
        ((short*)data)[0] = (short)GFFloatToLongNear((v.X + 8.0f) * 2048.0f);
        ((short*)data)[1] = (short)GFFloatToLongNear((v.Y + 8.0f) * 2048.0f);
        return;
    case VSDT_SHORT1N:
        ((short*)data)[0] = (short)GFFloatToLongNear(v.X * 32768.0f);
        return;
    case VSDT_SHORT2N:
        ((short*)data)[0] = (short)GFFloatToLongNear(v.X * 32768.0f);
        ((short*)data)[1] = (short)GFFloatToLongNear(v.Y * 32768.0f);
        return;
    case VSDT_SHORT3N:
        ((short*)data)[0] = (short)GFFloatToLongNear(v.X * 32768.0f);
        ((short*)data)[1] = (short)GFFloatToLongNear(v.Y * 32768.0f);
        ((short*)data)[2] = (short)GFFloatToLongNear(v.Z * 32768.0f);
        return;
    case VSDT_SHORT4N:
        ((short*)data)[0] = (short)GFFloatToLongNear(v.X * 32768.0f);
        ((short*)data)[1] = (short)GFFloatToLongNear(v.Y * 32768.0f);
        ((short*)data)[2] = (short)GFFloatToLongNear(v.Z * 32768.0f);
        ((short*)data)[3] = (short)GFFloatToLongNear(v.W * 32768.0f);
        return;
    case VSDT_DEC3N: {
        long z = GFFloatToLongNear(v.Z * 511.0f);
        long y = GFFloatToLongNear(v.Y * 1023.0f);
        long x = GFFloatToLongNear(v.X * 1023.0f);
        *(unsigned long*)data = (z << 22) | ((y & 0x7FF) << 11) | (x & 0x7FF);
        return;
    }
    case VSDT_DEC3N_SCALED: {
        long z = GFFloatToLongNear((v.Z - params.PosBiasZ) / params.PosScaleZ * 511.0f);
        long y = GFFloatToLongNear((v.Y - params.PosBiasY) / params.PosScaleY * 1023.0f);
        long x = GFFloatToLongNear((v.X - params.PosBiasX) / params.PosScaleX * 1023.0f);
        *(unsigned long*)data = (z << 22) | ((y & 0x7FF) << 11) | (x & 0x7FF);
        return;
    }
    }
}

int main()
{
    CVertexStreamConverter c; CVertexCompressionParams p;
    bool ok = true;
    float f[4] = { 0, 0, 0, 0 };
    c.ConvertTo((char*)f, VSDT_FLOAT3, C4DVector(1.5f, -2.0f, 3.25f, 4.0f), p);
    ok = ok && f[0] == 1.5f && f[1] == -2.0f && f[2] == 3.25f && f[3] == 0.0f;
    unsigned char col[4] = { 9, 9, 9, 9 };
    c.ConvertTo((char*)col, VSDT_D3DCOLOR, C4DVector(1.0f, 0.2f, 0.0f, 0.4f), p);
    ok = ok && col[0] == 0 && col[1] == 51 && col[2] == 255 && col[3] == 102;
    short s[4] = { 0, 0, 0, 0 };
    c.ConvertTo((char*)s, VSDT_SHORT4, C4DVector(-3.4f, 7.6f, 2.5f, 3.5f), p);
    ok = ok && s[0] == -3 && s[1] == 8 && s[2] == 2 && s[3] == 4;          // ties to even
    c.ConvertTo((char*)s, VSDT_SHORT2N, C4DVector(0.5f, -0.25f, 0.0f, 0.0f), p);
    ok = ok && s[0] == 16384 && s[1] == -8192;
    c.ConvertTo((char*)s, VSDT_SHORT2_UV, C4DVector(0.0f, -8.0f, 0.0f, 0.0f), p);
    ok = ok && s[0] == 16384 && s[1] == 0;
    unsigned int packed = 0;
    c.ConvertTo((char*)&packed, VSDT_DEC3N, C4DVector(1.0f, -1.0f, 1.0f, 0.0f), p);
    ok = ok && packed == ((511u << 22) | ((0x7FFu & (unsigned int)-1023) << 11) | 1023u);
    p.PosScaleX = 2.0f; p.PosBiasX = 0.5f; p.PosScaleZ = 4.0f; p.PosBiasZ = -1.0f;
    c.ConvertTo((char*)&packed, VSDT_DEC3N_SCALED, C4DVector(2.5f, -1.0f, 3.0f, 0.0f), p);
    ok = ok && packed == ((511u << 22) | ((0x7FFu & (unsigned int)-1023) << 11) | 1023u);
    unsigned int keep = 0x12345678;
    c.ConvertTo((char*)&keep, (EVertexStreamDataType)20, C4DVector(1.0f, 1.0f, 1.0f, 1.0f), p);
    ok = ok && keep == 0x12345678;
    printf(ok ? "00ac4910_TEST PASS\n" : "00ac4910_TEST FAIL\n");
    return ok ? 0 : 1;
}