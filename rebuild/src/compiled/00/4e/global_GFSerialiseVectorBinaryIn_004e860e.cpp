// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<EVillageTaskType>& out) -- read the count, resize, read every element
// Leaf: ??$GFSerialiseVectorBinaryIn@W4EVillageTaskType@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@W4EVillageTaskType@@V?$allocator@W4EVillageTaskType@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E860E, 148 bytes, `__fastcall` (stream in ecx, vector in edx), plain `ret`.
// Identity (proven): its only caller is CPersistContext::TransferBinary 0x004E85AE, which CVillageDef::Transfer 0x004E82C7 applies to +0x28: Ego_r.pdb CVillageDef::Tasks (+40) is vector<EVillageTaskType>.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the count read's slow path), vector<EVillageTaskType>::resize(n, const
//   EVillageTaskType&) 0x004E8705, CPersistTraits<EVillageTaskType>::TransferBinaryIn 0x004E86A2.
// Shape (family M048, representative 0x00463F9C): the landed size-TU template (0x00469CB2) with an enum traits whose
//   DefaultVal is zero (`and dword ptr [ebp-10h],0`) and STLport's `resize(n, T())` temporary zeroed in its frame word.
// Model: engine/CMemoryDataInputStream.h + the reader adapter of the landed template.
enum EVillageTaskType { EVILLAGETASKTYPE_NONE = 0 };
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
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class T, class _Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int newSize, const T& x);         // 0x004E8705
    __forceinline void resize(unsigned int newSize) { resize(newSize, T()); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<EVillageTaskType> {
    EVillageTaskType DefaultVal;
    CPersistTraits() : DefaultVal(EVillageTaskType(0)) { }
    void TransferBinaryIn(CMemoryDataInputStream& stream, EVillageTaskType* out) const;   // 0x004E86A2
};
__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}
template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<T>& out)
{
    CPersistTraits<T> traits;
    long count;
    ReadCount(static_cast<CMemoryDataInputStreamReader&>(stream), &count);
    out.resize(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}
template void __fastcall GFSerialiseVectorBinaryIn<EVillageTaskType>(CMemoryDataInputStream& stream, std::vector<EVillageTaskType>& out);