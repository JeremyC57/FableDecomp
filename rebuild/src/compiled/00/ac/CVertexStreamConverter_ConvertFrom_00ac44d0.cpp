// CVertexStreamConverter::ConvertFrom(const char* data, EVertexStreamDataType type, C4DVector& v, const CVertexCompressionParams& params) -- decode one vertex-stream element into a C4DVector (missing components default to 0, W to 1)
// Leaf: ?ConvertFrom@CVertexStreamConverter@@AAEXPBDW4EVertexStreamDataType@@AAVC4DVector@@ABVCVertexCompressionParams@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC44D0, 1076 bytes (code + nop + the 14-entry jump table at 0x00AC48CC), `ret 10h`;
//   `this` is unused (ecx is overwritten before any read).
// Identity: Ego_r.pdb / FableWin.pdb ?ConvertFrom@CVertexStreamConverter@@AAEXPBDW4EVertexStreamDataType@@AAVC4DVector@@
//   ABVCVertexCompressionParams@@@Z (private); sole caller CVertexStreamConverter::ConvertStream (landed 0x00AC4D70), which
//   pairs it with ConvertTo 0x00AC4910 per element.
// Constants: 1/255 @ 0x01231724, 1/2048 @ 0x0129EF8C, 8.0 @ 0x0125916C, 1/32768 @ 0x0129EF88, 1/1023 @ 0x0129EF84,
//   1/511 @ 0x0129EF80. Jump table checked entry by entry against retail 0x00AC48CC (object offsets 0x58 .. 0x354 = retail).
// Shape: `v = C4DVector(0,0,0,1)` goes through the 16-byte frame temporary (each component stored then copied); the 11:11:10
//   packed formats sign-extend with `x |= (x & sign) ? mask : 0` (neg/sbb/and/or) and the scaled variant multiplies by the
//   compression scale before the unit constant and adds the bias.
// Enumerators: the PDB name sets carry EVertexStreamDataType but not its enumerator names; they are given here from what
//   each case reads (descriptive; the element sizes agree with CVertexStreamFormat::GetDataTypeSize 0x00AB3740).
#include "engine/C4DVector.h"
#include "engine/CVertexCompressionParams.h"
enum EVertexStreamDataType {
    VSDT_FLOAT1 = 0, VSDT_FLOAT2 = 1, VSDT_FLOAT3 = 2, VSDT_FLOAT4 = 3, VSDT_D3DCOLOR = 4, VSDT_SHORT2 = 5,
    VSDT_SHORT4 = 6, VSDT_SHORT2_UV = 7, VSDT_SHORT1N = 8, VSDT_SHORT2N = 9, VSDT_SHORT3N = 10, VSDT_SHORT4N = 11,
    VSDT_DEC3N = 12, VSDT_DEC3N_SCALED = 13
};
class CVertexStreamConverter {
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