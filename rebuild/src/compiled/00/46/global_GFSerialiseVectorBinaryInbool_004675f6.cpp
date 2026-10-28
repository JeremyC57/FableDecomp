// GFSerialiseVectorBinaryIn<bool>(CMemoryDataInputStream& stream, vector<bool>& out) -- read the element count, resize the vector to it (new elements false), then read every flag in place through CPersistTraits<bool>
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the real CPersistTraits<bool>::TransferBinaryIn body out of line in the TU (noinline) lets VC7.1 home the never-initialised traits in the dead out slot
// Leaf: ??$GFSerialiseVectorBinaryIn@_N@@YIXAAUCMemoryDataInputStreamReader@@AAV?$vector@_N@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004675F6, 144 bytes, __fastcall (stream in ecx, out in edx), plain `ret`; size TU with
//         an EBP frame (`leave`), y-off + s like the landed GFSerialiseVectorBinaryIn<NInventory::CItem> 0x005C2AD8.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the count read's slow path), vector<bool>::resize(size_type,
//          const bool&) @ 0x00467686 (retail vector<bool> is a plain byte vector; catalogued as vector<unsigned char>::resize),
//          CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream*, bool*) @ 0x00403EB0 (landed; thiscall on the traits).
// Identity: Ego_r.pdb name set ??$GFSerialiseVectorBinaryIn@_N@@YIXAAVCMemoryDataInputStream@@AAV?$vector@_N...; the resize
//   default is `mov byte [ebp-1],0` (bool()) and the per-element reader is the bool traits' (1-byte stride). Paired with
//   GFSerialiseVectorBinaryOut<bool> 0x004675C5 (callers 0x004675BC and 0x00CDCFCE). The catalog label <CRGBColour> is wrong.
// Layout: engine/CMemoryDataInputStream.h (position +4, chunkBase +0xC = current chunk read pointer, chunkLength +0x14); the
//   inline Read is the landed 0x005C2AD8 one (`position += size` reuses the `lea ecx,[eax+4]` sum for the store).
// Shape: the landed 0x005C2AD8 template with STLport's inline one-argument resize (`resize(n, T())`, the bool() temporary
//   in [ebp-1]); retail gives the never-initialised traits object no frame word of its own: it shares [ebp-0Ch] with the
//   homed `out` argument.
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
    void resize(size_type __new_size, const _Tp& __x);                  // 0x00467686 for bool
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<bool> {
    bool DefaultVal;                                                     // never initialised here
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, bool* out);
};

// The real CPersistTraits<bool>::TransferBinaryIn body, out of line in the TU (GOTCHAS: empty traits objects).
__declspec(noinline) void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStreamReader* stream, bool* out)
{
    stream->Read(out, sizeof(bool));
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

template void __fastcall GFSerialiseVectorBinaryIn<bool>(CMemoryDataInputStreamReader& stream, std::vector<bool>& out);