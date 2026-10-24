// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<CMusicEntry>& out) -- read the element count, resize the music-entry vector, then read every entry in place through its traits
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046B4AB, 154 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the catalog's <CBrainUpdateZone> is propagated (CBrainUpdateZone is 12 bytes). The 0x10 stride, the {0, -1, 0.0f,
//   false} DefaultVal and the callees identify CMusicEntry (Ego_r.pdb ??$GFSerialiseVectorBinaryIn@VCMusicEntry@@...): load
//   twin of the writer 0x0046B422; 0x0046B545 is CPersistTraits<CMusicEntry>::TransferBinaryIn.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (slow path of the inlined count Read),
//   vector<CMusicEntry>::resize(n) @ 0x0046B65A (out of line; labelled _Umove<EWeaponClass*>),
//   CPersistTraits<CMusicEntry>::TransferBinaryIn @ 0x0046B545.
// Shape: the generic template as the landed 0x005C2AD8: the traits holds a DefaultVal built by the inline CMusicEntry()
//   (`and`/`or -1` stores, Gain through `fldz ; fstp`, the bool as an immediate byte store), inlined 4-byte count Read,
//   `out.resize(count)` out of line, then `traits.TransferBinaryIn(stream, &out[i])` with the byte offset in esi and the
//   count run down in edi. Size TU with a frame pointer: `optimize("y", off)` + `("s", on)`.
// Model: engine/CMemoryDataInputStream.h + a reader adapter carrying the retail member functions; CMusicEntry/CDefString are
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
struct CDefString {
    long TablePos;                                          // +0x00 def-string table index (-1 = unset)
    CDefString() : TablePos(-1) { }
};
struct CMusicEntry {
    unsigned long ID;                                       // +0x00
    CDefString    Filename;                                 // +0x04
    float         Gain;                                     // +0x08
    bool          CacheToHD;                                // +0x0c
    CMusicEntry() : ID(0), Gain(0.0f), CacheToHD(false) { }
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize);                      // 0x0046B65A
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x0046B545
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