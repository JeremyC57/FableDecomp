// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<CObjectFamilyEntry>& out) -- read the element count, resize the object-family vector, then read every entry in place through its traits
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046C31C, 157 bytes, `ret` (__fastcall: the stream in ecx, the vector in edx).
// Identity: the catalog's <CRGBColour> is propagated. The stride-8 loop calls CPersistTraits<CObjectFamilyEntry>::TransferBinaryIn
//   @ 0x0046C3B9 (landed under that name); Ego_r.pdb CObjectFamilyEntry (8 bytes) = {long Object, long Probability}
//   (CObjectFamilyDef::Objects is a vector<CObjectFamilyEntry>).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (slow path of the inlined count Read),
//   vector<CObjectFamilyEntry>::resize(n, const CObjectFamilyEntry&) @ 0x0046C430 (catalog "CArray<pair>::push_back"),
//   CPersistTraits<CObjectFamilyEntry>::TransferBinaryIn @ 0x0046C3B9.
// Shape: the generic template (landed 0x005C2AD8). CObjectFamilyEntry is a POD with no constructor, so the inlined
//   one-argument `resize(n)` -> `resize(n, T())` value-initialises its temporary with `xor eax,eax ; stosd ; stosd`,
//   while the specialised traits constructor zeroes DefaultVal member by member (the two `and dword ptr [ebp-..],0`
//   stores hoisted above the register pushes). Size TU with a frame pointer: `optimize("y", off)` + `("s", on)`.
// Model: engine/CMemoryDataInputStream.h + a reader adapter carrying the retail member functions; CObjectFamilyEntry is
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
class CObjectFamilyEntry { public:
    long Object;                                            // +0x00 object def index
    long Probability;                                       // +0x04 relative weight
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize, const T& x);         // 0x0046C430
    __forceinline void resize(unsigned int newSize) { resize(newSize, T()); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CObjectFamilyEntry> {
    CObjectFamilyEntry DefaultVal;
    CPersistTraits() { DefaultVal.Object = 0; DefaultVal.Probability = 0; }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, CObjectFamilyEntry* out) const;   // 0x0046C3B9
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
template void __fastcall GFSerialiseVectorBinaryIn<CObjectFamilyEntry>(CMemoryDataInputStreamReader& stream, std::vector<CObjectFamilyEntry>& out);