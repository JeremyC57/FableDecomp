// _global::GFSerialiseVectorBinaryIn<long>(CMemoryDataInputStream& stream, std::vector<long>& out) -- read the element count (inline checked 4-byte read), resize the vector with zeros, then read every element through CPersistTraits
// Leaf: ??$GFSerialiseVectorBinaryIn@J@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00619620, 140 bytes, `ret` (__fastcall: stream in ecx, vector in edx).
// Caller: the CPersistContext transfer 0x00619890 (reached twice from CTCInventoryMap's serialise 0x00606300: the
//   VisitedRegions / TeleportRegions vector<long> members); its binary-out arm 0x00614750 writes every element with
//   WriteSLONG 0x00993EB0, so the element is a 4-byte long.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (count slow path), vector<long>::resize(n, const T&)
//   0x00619000, CPersistTraits<long>::TransferBinaryIn 0x006148A0 (manifest label CPersistTraits<float>: the 4-byte copy
//   bodies are interchangeable).
// Label correction: the catalog `GFSerialiseVectorBinaryIn<CRGBColour>` is propagated; the instantiation is <long>.
// Shape: byte twin (up to the call targets) of the landed GFSerialiseVectorBinaryIn<unsigned long> 0x00412D20, whose model
//   this is: one frame word serves the inline count read's block-scoped value, the resize(n) default temporary `T()`
//   (stored 0 from the zero edi) and the block-scoped traits object. Speed TU, no pragma.
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
    void resize(unsigned int newSize, const T& value);   // 0x00619000 (out of line)
    void resize(unsigned int newSize) { resize(newSize, T()); }
};

template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x006148A0
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
template void __fastcall GFSerialiseVectorBinaryIn<long>(CMemoryDataInputStreamReader& stream, CRetailVector<long>& out);