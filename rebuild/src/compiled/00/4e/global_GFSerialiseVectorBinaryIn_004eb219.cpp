// _global::GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<CExplosionRing>& out) -- read the element count, resize the explosion ring vector to it (new rings default), then read every ring in place through its own TransferBinaryIn
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EB219, 159 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the load arm of the CPersistContext vector transfer at 0x004EB19F, whose save arm calls
//   GFSerialiseVectorBinaryOut<CExplosionRing> @ 0x004EB1A8 (this chunk); the element reader 0x004EB2B8 is the manifest's
//   CExplosionRing::TransferBinaryIn.
// Label correction: the catalog's <CBrainUpdateZone> is propagated (CBrainUpdateZone is 12 bytes; the stride here is 16 and
//   the resize default is CExplosionRing's {1.0, 1, 0.0, 1.0}).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined count read's slow path),
//   vector<CExplosionRing>::resize(size_type, const CExplosionRing&) @ 0x004EB41F, CExplosionRing::TransferBinaryIn
//   (CDataInputStream&) @ 0x004EB2B8 (thiscall on the element).
// Shape: the generic template (landed 0x00466BEB): the inline 4-byte count Read, STLport's one-argument resize forced inline
//   (`resize(n, T())`: the CExplosionRing() temporary at [ebp-18h] -- Ego_r.pdb {float RingRadius, long NumExplosions,
//   float AngleOffset, float SecondsBeforeSpawnNewRing}, defaults `fld1`, 1, `fldz`, `fld1`), then
//   `traits.TransferBinaryIn(stream, &out[i])` through an empty traits specialization forwarding to the element's member;
//   the index folds into a byte offset, the count runs down, the vector reference is spilled. Size TU with a frame
//   pointer: `optimize("y", off)` + `("s", on)`.
// Model: engine/CMemoryDataInputStream.h + a methods-only reader adapter; CExplosionRing is the PDB's 16-byte record (no
//   engine header); the vector is STLport's three words.
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
class CExplosionRing {
public:
    float RingRadius;                                       // +0x00
    long  NumExplosions;                                    // +0x04
    float AngleOffset;                                      // +0x08
    float SecondsBeforeSpawnNewRing;                        // +0x0c
    CExplosionRing() : RingRadius(1.0f), NumExplosions(1), AngleOffset(0.0f), SecondsBeforeSpawnNewRing(1.0f) { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream);   // 0x004EB2B8
};
namespace std {
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void resize(size_type __new_size, const _Tp& __x);      // 0x004EB41F
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    _Tp* begin() { return _M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CExplosionRing> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStreamReader& stream, CExplosionRing* value) const
    {
        value->TransferBinaryIn(stream);
    }
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
template void __fastcall GFSerialiseVectorBinaryIn<CExplosionRing>(CMemoryDataInputStreamReader& stream, std::vector<CExplosionRing>& out);