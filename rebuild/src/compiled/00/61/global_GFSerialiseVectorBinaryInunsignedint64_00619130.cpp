// _global::GFSerialiseVectorBinaryIn<unsigned __int64>(CMemoryDataInputStream& stream, std::vector<unsigned __int64>& out) -- read the element count (inline checked 4-byte read), resize the vector with zeros, then read every 64-bit element through CPersistTraits
// Leaf: ??$GFSerialiseVectorBinaryIn@_K@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00619130, 148 bytes (+12 int3), `ret` (__fastcall: stream in ecx, vector in edx).
// Caller: the CPersistContext transfer 0x006192F0, whose binary-out arm is GFSerialiseVectorBinaryOut<unsigned __int64>
//   0x00612D30 (this chunk) over the same vector argument.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (count slow path), vector<unsigned __int64>::resize(n,
//   const T&) 0x00618EF0, CPersistTraits<unsigned __int64>::TransferBinaryIn 0x0044B890 (its manifest label).
// Label correction: the catalog `GFSerialiseVectorBinaryIn<CRGBColour>` is propagated; the element is 8 bytes
//   (`lea eax,[edx+edi*8]`, two zero words for `T()`) and the traits callee names unsigned __int64.
// Shape: the landed GFSerialiseVectorBinaryIn<unsigned long> 0x00412D20 template with an 8-byte element: the 8-byte frame
//   holds the count read (first word), the zeroed `T()` resize temporary (both words, `mov [esp+18h],edi ;
//   mov [esp+1Ch],edi`) and the block-scoped traits object. Speed TU, no pragma.
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (CDataInputStream)
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

template <class T>
class CRetailVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
    void resize(unsigned int newSize, const T& value);   // 0x00618EF0 (out of line)
    void resize(unsigned int newSize) { resize(newSize, T()); }
};

template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x0044B890
};

__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, CRetailVector<T>& out)
{
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    {
        CPersistTraits<T> traits;
        for (long i = 0; i < count; ++i)
            traits.TransferBinaryIn(stream, &out[i]);
    }
}
template void __fastcall GFSerialiseVectorBinaryIn<unsigned __int64>(CMemoryDataInputStreamReader& stream, CRetailVector<unsigned __int64>& out);