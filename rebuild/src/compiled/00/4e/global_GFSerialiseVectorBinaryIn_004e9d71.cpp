// _global::GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<CMusicEntry>& out) -- read the element count, resize the music entry vector to it, then read every entry in place through its persist traits
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E9D71, 154 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the load arm of the CPersistContext vector transfer at 0x004E9CDF, whose save arm calls
//   GFSerialiseVectorBinaryOut<CMusicEntry> @ 0x004E9CE8 (this chunk); the 16-byte stride, the inline CMusicEntry DefaultVal
//   and the traits reader 0x004E9E0B (manifest CPersistTraits<CMusicEntry>::TransferBinaryIn) agree.
// Label correction: the catalog's <CBrainUpdateZone> is propagated (CBrainUpdateZone is 12 bytes, the stride here is 16).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined count read's slow path),
//   vector<CMusicEntry>::resize(size_type) @ 0x004E9F20 (STLport's one-argument resize, out of line: it builds the
//   CMusicEntry() temporary itself), CPersistTraits<CMusicEntry>::TransferBinaryIn @ 0x004E9E0B.
// Shape: the generic template of the landed 0x00466BEB with the one-argument resize NOT forced inline: the traits' DefaultVal
//   default-constructed first (Ego_r.pdb CMusicEntry {ulong ID, CDefString Filename, float Gain, bool CacheToHD}: `and ..,0`,
//   `or ..,-1`, `fldz ; fstp`, a zero byte) interleaved with the inline 4-byte count Read, `out.resize(count)`, then
//   `traits.TransferBinaryIn(stream, &out[i])` with the index folded into a byte offset and a down-counter; the vector
//   reference is spilled. Size TU with a frame pointer: `optimize("y", off)` + `("s", on)`.
// Model: engine/CMemoryDataInputStream.h + a methods-only reader adapter; CMusicEntry is the PDB's 16-byte record (no engine
//   header); the vector is STLport's three words.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend struct CMemoryDataInputStreamReader;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
class CDefString {
public:
    long TablePos;                                          // +0x00 def-string table index
    CDefString() : TablePos(-1) { }
};
class CMusicEntry {
public:
    unsigned long ID;                                       // +0x00
    CDefString    Filename;                                 // +0x04
    float         Gain;                                     // +0x08
    bool          CacheToHD;                                // +0x0c
    CMusicEntry() : ID(0), Gain(0.0f), CacheToHD(false) { }
};
namespace std {
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void resize(size_type __new_size);                      // 0x004E9F20 (builds the _Tp() temporary itself)
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* value) const;   // 0x004E9E0B
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
template void __fastcall GFSerialiseVectorBinaryIn<CMusicEntry>(CMemoryDataInputStreamReader& stream, std::vector<CMusicEntry>& out);