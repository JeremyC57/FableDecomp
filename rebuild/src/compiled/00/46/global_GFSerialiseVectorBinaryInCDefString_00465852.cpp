// GFSerialiseVectorBinaryIn<CDefString>(CMemoryDataInputStream& stream, vector<CDefString>& out) -- read the element count, resize the vector to it (new elements empty def strings), then read every def string's table position in place
// Leaf: ??$GFSerialiseVectorBinaryIn@VCDefString@@@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00465852, 141 bytes, __fastcall (stream in ecx, out in edx), plain `ret`; y-off + size TU
//         (EBP frame, `leave`), like the landed GFSerialiseVectorBinaryIn<NInventory::CItem> 0x005C2AD8.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the count read's slow path),
//          vector<CDefString>::resize(size_type, const CDefString&) @ 0x004658DF (catalogued as a propagated CArray push_back),
//          CDefString::TransferBinaryIn(CDataInputStream&) @ 0x0044FC00 (landed; thiscall on the element).
// Identity: Ego_r.pdb name set ??$GFSerialiseVectorBinaryIn@VCDefString@@@@YI...; PDB CDefString = { long TablePos }, the
//   default element is -1 (`or [ebp-8],-1`) and each element reads itself through CDefString::TransferBinaryIn. Six callers
//   (0x00465817, 0x004E97F8, 0x004E9802, 0x006752F8, 0x00675F5B, 0x00F333FE). The catalog label is correct.
// Layout: engine/CMemoryDataInputStream.h (position +4, chunkBase +0xC = current chunk read pointer, chunkLength +0x14).
// Shape: the landed 0x005C2AD8 template: the inline 4-byte Read (`position += size`), STLport's inline one-argument resize
//   (`resize(n, T())`, the -1 temporary in [ebp-8]), then CPersistTraits<CDefString>'s inline transfer
//   (`value->TransferBinaryIn(stream)`) per element; the traits object (its DefaultVal store dead) folds away. The size TU
//   keeps the one-argument resize and the traits transfer out of line unless they are __forceinline.
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

namespace std {
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void resize(size_type __new_size, const _Tp& __x);                  // 0x004658DF for CDefString
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

class CDefString {
public:
    long TablePos;                                          // +0x00
    CDefString() : TablePos(-1) {}
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream);   // 0x0044FC00
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CDefString> {
    CDefString DefaultVal;
    __forceinline void TransferBinaryIn(CMemoryDataInputStreamReader* stream, CDefString* value) { value->TransferBinaryIn(*stream); }
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
        traits.TransferBinaryIn(&stream, &out[i]);
}

template void __fastcall GFSerialiseVectorBinaryIn<CDefString>(CMemoryDataInputStreamReader& stream, std::vector<CDefString>& out);