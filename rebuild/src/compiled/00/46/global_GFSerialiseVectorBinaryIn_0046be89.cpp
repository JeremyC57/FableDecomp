// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<CReverbEnvironmentEntry>& out) -- read the element count, resize the reverb-environment vector, then read every entry in place through its traits
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046BE89, 147 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the catalog's <CBrainUpdateZone> is propagated. Load half of CPersistContext::TransferBinary @ 0x0046BE1F, the
//   ReverbEnvironmentEntries (+0x6c) transfer of the landed CSoundDef::Transfer 0x0046A9CE (save half 0x0046BE4D); Ego_r.pdb
//   ??$GFSerialiseVectorBinaryIn@VCReverbEnvironmentEntry@@...; CReverbEnvironmentEntry (0x54 bytes) = {float Params[21]}.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (slow path of the inlined count Read),
//   vector<CReverbEnvironmentEntry>::resize(n) @ 0x0046BF72 (out of line; chunk row "$E1"),
//   CPersistTraits<CReverbEnvironmentEntry>::TransferBinaryIn @ 0x0046BF1C (landed as the SRecord54 traits).
// Shape: the generic template as the landed 0x005C2AD8: the traits holds a value-initialised DefaultVal (`DefaultVal()` on a
//   21-float POD = `push 15h ; pop ecx ; rep stosd`), the inlined 4-byte count Read, `out.resize(count)` out of line, then
//   `traits.TransferBinaryIn(stream, &out[i])` with the byte offset in esi and the count run down in edi. Size TU with a frame
//   pointer: `optimize("y", off)` + `("s", on)`.
// Model: engine/CMemoryDataInputStream.h + a reader adapter carrying the retail member functions; CReverbEnvironmentEntry is
//   local (no engine header); the vector is STLport's three words.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
struct CReverbEnvironmentEntry {
    float Params[21];                                       // +0x00
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize);                      // 0x0046BF72
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x0046BF1C
};
__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}
template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, std::vector<T>& out)
{
    CPersistTraits<T> traits;
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}
template void __fastcall GFSerialiseVectorBinaryIn<CReverbEnvironmentEntry>(CMemoryDataInputStreamReader& stream, std::vector<CReverbEnvironmentEntry>& out);