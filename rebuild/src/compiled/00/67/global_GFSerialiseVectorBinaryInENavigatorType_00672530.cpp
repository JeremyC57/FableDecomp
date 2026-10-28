// GFSerialiseVectorBinaryIn<ENavigatorType>(CMemoryDataInputStream& stream, vector<ENavigatorType>& out) -- read the element count, resize the vector to it (new elements zero), then read every navigator type in place through CPersistTraits<ENavigatorType>
// Leaf: ??$GFSerialiseVectorBinaryIn@W4ENavigatorType@@
//
// Retail 0x00672530, 148 bytes (+ padding to 160), __fastcall (stream in ecx, out in edx), plain `ret`; speed TU (no frame pointer, `sub esp, 8`).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the count read's slow path),
//          vector<ENavigatorType>::resize(size_type, const ENavigatorType&) @ 0x00672660,
//          CPersistTraits<ENavigatorType>::TransferBinaryIn @ 0x006725D0.
// Identity: the load arm of the binary transfer of CThingCreatureDef::NavigatorTypes (0x006724B0, from
//   CThingCreatureDef::Transfer 0x00671600; Ego_r.pdb vector<ENavigatorType> NavigatorTypes); its save arm is
//   GFSerialiseVectorBinaryOut<ENavigatorType> 0x006724F0.  The catalog's "<CRGBColour>" is a propagated label.
// Layout: engine/CMemoryDataInputStream.h (position +4, chunkBase +0xC = current chunk read pointer, chunkLength +0x14).
// Shape: the landed 0x00466BEB template without its y-off/size pragmas: traits.DefaultVal zeroed first, the inline 4-byte
//   Read (the count lands in ebx on the fast path, through the frame slot on the slow one), STLport's one-argument
//   resize forwarding `resize(n, T())` (the zero temporary reuses the count slot), then
//   traits.TransferBinaryIn(&stream, &out[i]) with `out._M_start` re-read every pass.
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

enum ENavigatorType { NAVIGATOR_TYPE_NONE = -1, NAVIGATOR_TYPE_0 = 0 };

namespace std {
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void resize(size_type __new_size, const _Tp& __x);                  // 0x00672660 for ENavigatorType
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<ENavigatorType> {
    ENavigatorType DefaultVal;
    CPersistTraits() : DefaultVal(ENavigatorType()) {}
    void TransferBinaryIn(CMemoryDataInputStreamReader* stream, ENavigatorType* value);  // 0x006725D0
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

template void __fastcall GFSerialiseVectorBinaryIn<ENavigatorType>(CMemoryDataInputStreamReader& stream, std::vector<ENavigatorType>& out);