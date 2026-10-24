// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<CMusicSetEntry>& out) -- read the element count, resize the music-set vector, then read every entry in place through its traits
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046B9B0, 155 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the catalog's <CBrainUpdateZone> is propagated. Load half of CPersistContext::TransferBinary @ 0x0046B92A, the
//   MusicSetEntries (+0x60) transfer of the landed CSoundDef::Transfer 0x0046A9CE (save half 0x0046B958); Ego_r.pdb
//   ??$GFSerialiseVectorBinaryIn@VCMusicSetEntry@@...; CMusicSetEntry (12 bytes) = {NormalMusicID, DangerMusicID, LoopCount}.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (slow path of the inlined count Read),
//   vector<CMusicSetEntry>::resize(n, const CMusicSetEntry&) @ 0x0046BB59, CPersistTraits<CMusicSetEntry>::TransferBinaryIn
//   @ 0x0046BA4B (labelled for CBrainUpdateZone).
// Shape: the generic template (landed 0x005C2AD8): the traits DefaultVal and STLport's inlined `resize(n, T())` temporary are
//   both built by the inline CMusicSetEntry(), which zeroes only the two music IDs (the frame words of LoopCount are never
//   written: retail stores two words for each 12-byte object); the DefaultVal stores are `and dword ptr [..],0`, the
//   temporary's share esi's zero with the loop offset. The one-argument resize is `__forceinline`; the traits reader stays out
//   of line. Size TU with a frame pointer: `optimize("y", off)` + `("s", on)`.
// Model: engine/CMemoryDataInputStream.h + a reader adapter carrying the retail member functions; CMusicSetEntry is local (no
//   engine header); the vector is STLport's three words.
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
struct CMusicSetEntry {
    long NormalMusicID;                                     // +0x00
    long DangerMusicID;                                     // +0x04
    long LoopCount;                                         // +0x08 (left unset by the constructor)
    CMusicSetEntry() : NormalMusicID(0), DangerMusicID(0) { }
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize, const T& x);         // 0x0046BB59
    __forceinline void resize(unsigned int newSize) { resize(newSize, T()); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x0046BA4B
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
template void __fastcall GFSerialiseVectorBinaryIn<CMusicSetEntry>(CMemoryDataInputStreamReader& stream, std::vector<CMusicSetEntry>& out);