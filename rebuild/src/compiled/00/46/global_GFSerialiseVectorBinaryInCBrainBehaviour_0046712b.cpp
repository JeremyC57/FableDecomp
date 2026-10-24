// GFSerialiseVectorBinaryIn<CBrainBehaviour>(CMemoryDataInputStream& stream, vector<CBrainBehaviour>& out) -- read the element count, resize the vector to it (new elements default behaviours), then read every behaviour in place through CPersistTraits<CBrainBehaviour>
// Leaf: ??$GFSerialiseVectorBinaryIn@VCBrainBehaviour@@@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046712B, 159 bytes, __fastcall (stream in ecx, out in edx), plain `ret`; y-off + size TU
//         (EBP frame, `leave`), like the landed GFSerialiseVectorBinaryIn<NInventory::CItem> 0x005C2AD8.
// Callees: CPersistTraits<CBrainBehaviour>::CPersistTraits() @ 0x004670F5 (landed), CDataInputStream::ReadWithSrcChunkOverflow
//          @ 0x00993CA0 (the count read's slow path), vector<CBrainBehaviour>::resize(size_type, const CBrainBehaviour&) @ 0x0046728D
//          (landed as CRetailVectorelem12_resize), CPersistTraits<CBrainBehaviour>::TransferBinaryIn @ 0x004671CA (landed).
// Identity: Ego_r.pdb name set ??$GFSerialiseVectorBinaryIn@VCBrainBehaviour@@@@YI...; PDB CBrainBehaviour (12 bytes) =
//   { CDefString Name; long Priority; long ScriptAIPriority; }, the default element is {-1, 0, 0} (CDefString's empty index
//   is -1, as the landed 0x0044FC00 notes). The only caller is the landed binary transfer 0x00467062 (call site 0x00467087).
//   The catalog label is correct.
// Layout: engine/CMemoryDataInputStream.h (position +4, chunkBase +0xC = current chunk read pointer, chunkLength +0x14).
// Shape: the landed 0x005C2AD8 template: traits first (its out-of-line constructor runs before the count read), the inline
//   4-byte Read (`position += size`), STLport's inline one-argument resize (`resize(n, T())`, the {-1, 0, 0} temporary in
//   [ebp-14h]), then traits.TransferBinaryIn(stream, &out[i]) with the index strength-reduced to a byte offset and a
//   down-counter. The size TU keeps the one-argument resize out of line unless __forceinline.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
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
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

namespace std {
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void resize(size_type __new_size, const _Tp& __x);                  // 0x0046728D for CBrainBehaviour
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

class CDefString {
public:
    long TablePos;                                          // +0x00
    CDefString() : TablePos(-1) {}
};

class CBrainBehaviour {                                    // 12 bytes
public:
    CDefString Name;                                        // +0x00
    long       Priority;                                    // +0x04
    long       ScriptAIPriority;                            // +0x08
    CBrainBehaviour() : Priority(0), ScriptAIPriority(0) {}
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CBrainBehaviour> {
    CBrainBehaviour DefaultVal;
    CPersistTraits();                                                                       // 0x004670F5
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, CBrainBehaviour* value);   // 0x004671CA
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

template void __fastcall GFSerialiseVectorBinaryIn<CBrainBehaviour>(CMemoryDataInputStreamReader& stream, std::vector<CBrainBehaviour>& out);