// GFSerialiseVectorBinaryIn<CBrainUpdateZone>(CMemoryDataInputStream& stream, vector<CBrainUpdateZone>& out) -- read the element count, resize the vector to it (new elements zeroed zones), then read every zone in place through CPersistTraits<CBrainUpdateZone>
// Leaf: ??$GFSerialiseVectorBinaryIn@VCBrainUpdateZone@@@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00466BEB, 163 bytes, __fastcall (stream in ecx, out in edx), plain `ret`; y-off + size TU
//         (EBP frame, `leave`), like the landed GFSerialiseVectorBinaryIn<NInventory::CItem> 0x005C2AD8.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the count read's slow path),
//          vector<CBrainUpdateZone>::resize(size_type, const CBrainUpdateZone&) @ 0x00466D9C (landed as CRetailVectorelem12_resize),
//          CPersistTraits<CBrainUpdateZone>::TransferBinaryIn @ 0x00466C8E (Ego_r name set
//          ?TransferBinaryIn@?$CPersistTraits@VCBrainUpdateZone@@@@...).
// Identity: Ego_r.pdb name set ??$GFSerialiseVectorBinaryIn@VCBrainUpdateZone@@@@YI...; PDB CBrainUpdateZone (12 bytes) =
//   { long Distance; long UpdateFrequency; long InterruptionFrequency; } and CPersistTraits<CBrainUpdateZone> = { DefaultVal };
//   the traits and the resize default are both three zero stores. The only caller is the binary transfer 0x00466B65 (call
//   site 0x00466B8A), whose save arm calls GFSerialiseVectorBinaryOut<CBrainUpdateZone> 0x00466B93. The catalog label is correct.
// Layout: engine/CMemoryDataInputStream.h (position +4, chunkBase +0xC = current chunk read pointer, chunkLength +0x14).
// Shape: the landed 0x005C2AD8 template: traits first (its inline zeroing interleaved with the count read), the inline
//   4-byte Read (`position += size`), STLport's inline one-argument resize (`resize(n, T())`, the zeroed temporary in
//   [ebp-14h]), then traits.TransferBinaryIn(stream, &out[i]) with the index strength-reduced to a byte offset and a
//   down-counter. The size TU keeps the one-argument resize out of line unless __forceinline.
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
    void resize(size_type __new_size, const _Tp& __x);                  // 0x00466D9C for CBrainUpdateZone
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

class CBrainUpdateZone {                                   // 12 bytes
public:
    long Distance;                                          // +0x00
    long UpdateFrequency;                                   // +0x04
    long InterruptionFrequency;                             // +0x08
    CBrainUpdateZone() : Distance(0), UpdateFrequency(0), InterruptionFrequency(0) {}
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CBrainUpdateZone> {
    CBrainUpdateZone DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, CBrainUpdateZone* value);  // 0x00466C8E
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

template void __fastcall GFSerialiseVectorBinaryIn<CBrainUpdateZone>(CMemoryDataInputStreamReader& stream, std::vector<CBrainUpdateZone>& out);