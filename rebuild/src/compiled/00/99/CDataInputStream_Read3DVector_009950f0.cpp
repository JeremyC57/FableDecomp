// CDataInputStream::Read3DVector() -- read X, Y then Z and return the 3D vector by value
// Leaf: Read3DVector
// Retail: Fable.exe (TLC, VC7.1) @ 0x009950F0, 288 bytes (+ int3 padding), `ret 4` (thiscall on the stream, hidden result pointer).
// Identity: Ego_r.pdb name set ?Read3DVector@CDataInputStream@@QAE?AVC3DVector@@XZ. The manifest's
//   "CPersistTraits<CBrainUpdateZone>::TransferBinaryIn" is propagated: the 23 callers (the landed particle readers
//   CPSCAttractor::ReadBinary 0x00B18390, CParticleSystem::ReadBinary 0x00AF4660, CPSCSplineBase::ReadBinary 0x00B22B40,
//   CPersistContext::Transfer<C3DVector> 0x004859AD) all call it as CDataInputStream::Read3DVector.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x3, the slow path of the inlined Read).
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14).
// Shape: the landed Read2DVector 0x00995030 with a third coordinate: three inlined Read calls into three float frame
//   locals, then the result is built from them (X via fld/fstp, Y and Z through GPRs) and its address returned.
//   Speed-optimised (default) TU, the same as the other 0x0099xxxx readers.
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct CDataInputStreamReader : CDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
    C3DVector Read3DVector();
};

C3DVector CDataInputStreamReader::Read3DVector()
{
    float x = ReadFloat();
    float y = ReadFloat();
    float z = ReadFloat();
    return C3DVector(x, y, z);
}