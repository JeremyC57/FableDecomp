// GFSerialiseVectorBinaryIn<CHeroMarriageInfo>(CMemoryDataInputStream& stream, vector<CHeroMarriageInfo>& out) -- read the count, resize the marriage vector, then read every record in place
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x00581404, 137 bytes, plain `ret` (`__fastcall` stream, out).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the slow arm of the inline 4-byte read),
// vector<CHeroMarriageInfo>::resize(n) @ 0x00581284 (builds the zeroed T() and calls resize(n, x) 0x005811F1),
// CPersistTraits<CHeroMarriageInfo>::TransferBinaryIn(stream, T*) @ 0x0057F9E9 (24-byte records, `add esi,18h`).
// Only caller 0x005816A0 (the CPersistContext vector transfer of the CTCHeroStats TU).
// Label: the catalog "<CBrainUpdateZone>" instantiation is propagated: the element size (0x18), the traits callee and
// the resize callee are the CHeroMarriageInfo ones of this TU.
// Shape: the landed GFSerialiseVectorBinaryIn<NInventory::CItem> 0x005C2AD8 template: the inline stream Read with the
// 64-bit position guard (`newPosition >= 0 && newPosition <= 0x7fffffff`, the high word a constant zero), the count
// copied with `movsd` into its frame word, `out` spilled to [ebp-8], the traits object (a 24-byte DefaultVal that a
// POD record leaves unconstructed) in the frame, `out[i]` as `*(begin() + i)` (a byte-offset induction register).
// Frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CMemoryDataInputStream.h"
#include "engine/CHeroMarriageInfo.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend struct CMemoryDataInputStreamReader;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
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
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void resize(unsigned int newSize);                      // 0x00581284
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x0057F9E9
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
template void __fastcall GFSerialiseVectorBinaryIn<CHeroMarriageInfo>(CMemoryDataInputStreamReader& stream, std::vector<CHeroMarriageInfo>& out);