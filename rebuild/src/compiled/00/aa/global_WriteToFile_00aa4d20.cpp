// WriteToFile(const CVertexCompressionParams& params, CDataOutputStream& stream) -- serialise the eight vertex-compression floats (position scale XYZW, position bias XYZW) to a data output stream
// Leaf: ?WriteToFile@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA4D20, 96 bytes, `ret` (free `__fastcall`: params in ecx -> edi,
//   stream in edx -> esi).
// Identity: Ego_r.pdb `?WriteToFile@@YIXABVCVertexCompressionParams@@AAVCDataOutputStream@@@Z` (BSim similarity 1.0);
//   one caller at 0x00AA5903.
// Callee: CDataOutputStream::WriteFloat(float) @ 0x00993EE0 (x8, `push 4 ; lea ; call [vt+10h]` = a 4-byte Write).
// Layout: CVertexCompressionParams is 32 bytes of floats (Ego_r.pdb: PosScaleX..W +0..+0xC, PosBiasX..W +0x10..+0x1C).
// Shape: each float is pushed as a dword (`mov reg,[edi+n] ; push reg`), the stream reloaded into ecx for every call.
class CDataOutputStream {
public:
    void WriteFloat(float value);                  // 0x00993EE0
};

class CVertexCompressionParams {
public:
    float PosScaleX;                               // +0x00
    float PosScaleY;                               // +0x04
    float PosScaleZ;                               // +0x08
    float PosScaleW;                               // +0x0c
    float PosBiasX;                                // +0x10
    float PosBiasY;                                // +0x14
    float PosBiasZ;                                // +0x18
    float PosBiasW;                                // +0x1c
};

void __fastcall WriteToFile(const CVertexCompressionParams& params, CDataOutputStream& stream)
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