// GFSerialiseVectorBinaryIn<float>(CMemoryDataInputStream& stream, vector<float>& out) -- read the element count, resize the vector to it (new elements 0.0f), then read every float in place through CPersistTraits<float>
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the real CPersistTraits<float>::TransferBinaryIn body out of line in the TU (noinline) lets VC7.1 home the never-initialised traits in the dead out slot
// Leaf: ??$GFSerialiseVectorBinaryIn@M@@YIXAAUCMemoryDataInputStreamReader@@AAV?$vector@M@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00464D23, 146 bytes, __fastcall (stream in ecx, out in edx), plain `ret`; size TU with
//         an EBP frame (`leave`), y-off + s like the landed GFSerialiseVectorBinaryIn<NInventory::CItem> 0x005C2AD8.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the count read's slow path), vector<float>::resize(size_type,
//          const float&) @ 0x00442770 (COMDAT-folded with every 4-byte trivially assignable vector; catalogued as
//          vector<unsigned int>::resize), CPersistTraits<float>::TransferBinaryIn(CMemoryDataInputStream*, float*) @ 0x0040EFB0
//          (landed; thiscall on the empty-in-effect traits object).
// Identity: Ego_r.pdb name set ??$GFSerialiseVectorBinaryIn@M@@YIXAAVCMemoryDataInputStream@@AAV?$vector@M...; the resize
//   default is `fldz` (float()) and the per-element reader is the float traits'. Paired with GFSerialiseVectorBinaryOut<float>
//   0x00464CED (both called from 0x00464CDA/0x00464CE4 and 0x0056A013/0x0056A027). The catalog label <CRGBColour> is wrong.
// Layout: engine/CMemoryDataInputStream.h (position +4, chunkBase +0xC = current chunk read pointer, chunkLength +0x14); the
//   inline Read is the landed 0x005C2AD8 one (`position += size` reuses the `lea ecx,[eax+4]` sum for the store).
// Shape: the landed 0x005C2AD8 template with STLport's inline one-argument resize (`resize(n, T())`, the float() temporary
//   in [ebp-8]); CPersistTraits<float>'s DefaultVal is default-initialised (no store), so the traits object takes no frame
//   word of its own and VC places it in the dead slot of the homed `out` argument ([ebp-0Ch]).
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
    void resize(size_type __new_size, const _Tp& __x);                  // 0x00442770 for float
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<float> {
    float DefaultVal;                                                     // never initialised here
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, float* out);
};

// The real CPersistTraits<float>::TransferBinaryIn body, out of line in the TU (GOTCHAS: empty traits objects).
__declspec(noinline) void CPersistTraits<float>::TransferBinaryIn(CMemoryDataInputStreamReader* stream, float* out)
{
    stream->Read(out, sizeof(float));
}

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

template void __fastcall GFSerialiseVectorBinaryIn<float>(CMemoryDataInputStreamReader& stream, std::vector<float>& out);