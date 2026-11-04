// CVertexStreamConverter::ConvertTo(char* data, EVertexStreamDataType type, const C4DVector& v, const CVertexCompressionParams& params) -- encode a C4DVector as one vertex-stream element (rounded to nearest)
// Leaf: ?ConvertTo@CVertexStreamConverter@@AAEXPADW4EVertexStreamDataType@@ABVC4DVector@@ABVCVertexCompressionParams@@@Z
// Flags: none
// Retail-asm: 0x00AC4999 `fld dword ptr [esp+10h]` + 0x00AC499D `fistp dword ptr [esp+0Ch]` (and every other fistp site of the
//   row, e.g. 0x00AC4A11, 0x00AC4AC1, 0x00AC4C62, 0x00AC4D0B) -- the engine's inline-assembly float-to-long helper
//   GFFloatToLongNear: the scaled value is stored to the helper's by-value parameter copy (the dead `v` argument slot),
//   reloaded and stored with a bare dword fistp. VC7.1 C++ `(long)x` / `(int)x` / `(short)x` call __ftol2, /QIfist emits
//   `fistp qword`, `_mm_cvtss_si32` emits cvtss2si (attempts table in NOTES.md), so the dword fistp is the original's asm.
// Helper: engine/GFFloatToLongNear.h (byte-matching asm path + FABLE_PORTABLE C++ twin); the row itself carries no asm.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC4910, 1120 bytes (code + the 14-entry jump table at 0x00AC4D38), `ret 10h`;
//   `this` unused.
// Identity: Ego_r.pdb / FableWin.pdb ?ConvertTo@CVertexStreamConverter@@AAEXPADW4EVertexStreamDataType@@ABVC4DVector@@
//   ABVCVertexCompressionParams@@@Z (private); the inverse of ConvertFrom 0x00AC44D0, called from ConvertStream 0x00AC4D70.
// Constants: 255.0 @ 0x01230014, 8.0 @ 0x0125916C, 2048.0 @ 0x012724C0, 32768.0 @ 0x0129EF98, 1023.0 @ 0x0129EF90,
//   511.0 @ 0x0129EF94. Jump table checked entry by entry against retail 0x00AC4D38 (offsets 0x15 .. 0x3A8 = retail).
// Shape: the 11:11:10 formats round Z, Y, X in that order, then pack `(z << 22) | ((y & 0x7FF) << 11) | (x & 0x7FF)`
//   (VC7.1 reassociates it to retail's `((z << 11 | y & 0x7FF) << 11) | x & 0x7FF`); the scaled variant first removes the
//   compression bias and divides by the scale.
// Enumerators: the PDB name sets carry EVertexStreamDataType but not its enumerator names; they are given here from what
//   each case reads (descriptive; the element sizes agree with CVertexStreamFormat::GetDataTypeSize 0x00AB3740).
#include "engine/C4DVector.h"
#include "engine/CVertexCompressionParams.h"
#include "engine/GFFloatToLongNear.h"
enum EVertexStreamDataType {
    VSDT_FLOAT1 = 0, VSDT_FLOAT2 = 1, VSDT_FLOAT3 = 2, VSDT_FLOAT4 = 3, VSDT_D3DCOLOR = 4, VSDT_SHORT2 = 5,
    VSDT_SHORT4 = 6, VSDT_SHORT2_UV = 7, VSDT_SHORT1N = 8, VSDT_SHORT2N = 9, VSDT_SHORT3N = 10, VSDT_SHORT4N = 11,
    VSDT_DEC3N = 12, VSDT_DEC3N_SCALED = 13
};
class CVertexStreamConverter {
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