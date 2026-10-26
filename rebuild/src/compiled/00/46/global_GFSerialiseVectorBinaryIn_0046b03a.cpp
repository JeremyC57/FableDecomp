// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<CAtmosBankEntry>& out) -- read the element count, resize the atmos-bank vector, then read every entry in place through its traits
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046B03A, 156 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the catalog's <CRGBColour> is propagated. Load half of CPersistContext::TransferBinary @ 0x0046AFC6 (save half
//   0x0046AFF4 writes ID through WriteULONG and Filename through WriteSLONG); Ego_r.pdb
//   ??$GFSerialiseVectorBinaryIn@VCAtmosBankEntry@@@@YIXAAVCMemoryDataInputStream@@...; CAtmosBankEntry (8 bytes) =
//   {unsigned long ID, CDefString Filename}, default {0, -1}.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (slow path of the inlined count Read),
//   vector<CAtmosBankEntry>::resize(n, const CAtmosBankEntry&) @ 0x0046B144, CPersistTraits<CAtmosBankEntry>::TransferBinaryIn
//   @ 0x0046B0D6 (landed as the pair<CDefIndex, CDefString> traits: it reads ID inline, then CDefString::TransferBinaryIn).
// Shape: the generic template (landed 0x005C2AD8): the traits holds a DefaultVal built by CAtmosBankEntry() (`ID(0)` and
//   CDefString's `TablePos(-1)`, inlined; the zero shares the register of the inlined Read's 64-bit range check), STLport's
//   one-argument `resize(n)` is inlined to `resize(n, T())` (a second {0, -1} temporary) and must be `__forceinline`, then
//   `traits.TransferBinaryIn(stream, &out[i])` out of line. Size TU with a frame pointer: `optimize("y", off)` + `("s", on)`.
// Model: engine/CMemoryDataInputStream.h + a reader adapter carrying the retail member functions; CAtmosBankEntry/CDefString
//   are local (no engine header); the vector is STLport's three words.
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
class CDefString { public:
    long TablePos;                                          // +0x00 def-string table index (-1 = unset)
    CDefString() : TablePos(-1) { }
};
class CAtmosBankEntry { public:
    unsigned long ID;                                       // +0x00
    CDefString    Filename;                                 // +0x04
    CAtmosBankEntry() : ID(0) { }
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize, const T& x);         // 0x0046B144
    __forceinline void resize(unsigned int newSize) { resize(newSize, T()); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x0046B0D6
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
template void __fastcall GFSerialiseVectorBinaryIn<CAtmosBankEntry>(CMemoryDataInputStreamReader& stream, std::vector<CAtmosBankEntry>& out);