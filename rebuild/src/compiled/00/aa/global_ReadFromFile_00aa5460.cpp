// ReadFromFile(CVertexCompressionParams& params, CDataInputStream& in) -- read the eight vertex compression floats (position scale XYZW, position bias XYZW) from the stream in declaration order
// Leaf: ?ReadFromFile@@YIXAAVCVertexCompressionParams@@AAVCDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA5460, 700 bytes, `__fastcall` (params in ecx, stream in edx), one frame dword (the read temporary).
// Identity (proven): (1) Ego_r.pdb names the free function ?ReadFromFile@@YIXAAVCVertexCompressionParams@@AAVCDataInputStream@@@Z
//   (donor 0x00C40310, beside its WriteToFile twin 0x00C3FC50); (2) the sole caller is the landed
//   C3DMeshFileXVertexDataChunk::ReadFromFile 0x00AA66B0, which calls it as ::ReadFromFile(VBCParams..., input) for version >= 2 and
//   binds this address under that spelling; (3) the body stores eight dwords at +0..+0x1C of the first argument, the Ego_r.pdb
//   CVertexCompressionParams layout (PosScaleX/Y/Z/W, PosBiasX/Y/Z/W, 32 bytes). The catalog label "ReadFromFile" lacks the signature.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inlined Read's slow path, x8).
// Shape: the landed caller's inline CDataInputStream::Read model (the IsValidPosition range test on the 64-bit new position, the
//   in-chunk memcpy fast path, the overflow call) through a by-value ReadFloat helper; the helper's temporary takes the one frame
//   slot and each result is copied to its member through an integer register (a float copy with no arithmetic).
#include "fable_compiler.h"
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CInputStreamAccess   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

class CVertexCompressionParams {                    // Ego_r.pdb, 32 bytes
public:
    float PosScaleX;                                // +0x00
    float PosScaleY;                                // +0x04
    float PosScaleZ;                                // +0x08
    float PosScaleW;                                // +0x0C
    float PosBiasX;                                 // +0x10
    float PosBiasY;                                 // +0x14
    float PosBiasZ;                                 // +0x18
    float PosBiasW;                                 // +0x1C
};

struct CInputStreamAccess : CDataInputStream {
    static FABLE_FORCEINLINE bool IsValidPosition(__int64 pos) { return pos >= 0 && pos <= 0x7fffffff; }
    FABLE_FORCEINLINE void Read(void* buffer, long size)
    {
        if (size <= 0) return;
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition)) return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    FABLE_FORCEINLINE float ReadFloat() { float value; Read(&value, 4); return value; }
};

void __fastcall ReadFromFile(CVertexCompressionParams& params, CDataInputStream& input)
{
    CInputStreamAccess& in = (CInputStreamAccess&)input;
    params.PosScaleX = in.ReadFloat();
    params.PosScaleY = in.ReadFloat();
    params.PosScaleZ = in.ReadFloat();
    params.PosScaleW = in.ReadFloat();
    params.PosBiasX = in.ReadFloat();
    params.PosBiasY = in.ReadFloat();
    params.PosBiasZ = in.ReadFloat();
    params.PosBiasW = in.ReadFloat();
}