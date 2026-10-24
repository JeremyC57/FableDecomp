// GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, vector<NInventory::CItem>& out) -- read the count, resize the vector, then read every item in place
// Leaf: GFSerialiseVectorBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2AD8, length 164 bytes (__fastcall stream, out).
// Callees: vector<CItem>::resize 0x005C2AA9, CItem traits TransferBinaryIn 0x005C19A3, CCountedPointer dtor 0x005C04BF (the
//   traits' DefaultVal), ReadWithSrcChunkOverflow 0x00993CA0.
// Model: engine/CMemoryDataInputStream.h + reader adapter. NInventory::CItem is a LOCAL model next to engine/CItem.h: the header
//   flattens the CCountedPointer<CPerItemData> member into two words, which cannot carry the pointer's ctor/dtor that retail runs.
// Shape: `#pragma optimize("y", off)` + `("s", on)`; the traits object holds a DefaultVal CItem (zeroed, destroyed at exit);
//   `out[i]` is `*(begin() + i)` and the loop is `out.resize(count); for (long i = 0; i < count; ++i) traits.TransferBinaryIn(...)`.
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
struct SCountBlock;
template <class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pBlock;
    CCountedPointer() : m_pObject(0), m_pBlock(0) { }
    ~CCountedPointer();                                     // 0x005C04BF
};
namespace NInventory {
struct CPerItemData;
struct CItem {
    long ItemType;                                          // +0x00
    long NumberOfItems;                                     // +0x04
    CCountedPointer<CPerItemData> PPerItemData;             // +0x08
    long AverageBoughtPrice;                                // +0x10
    CItem() : ItemType(0), NumberOfItems(0), AverageBoughtPrice(0) { }
};
}
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void resize(unsigned int newSize);                      // 0x005C2AA9
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x005C19A3
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
template void __fastcall GFSerialiseVectorBinaryIn<NInventory::CItem>(CMemoryDataInputStreamReader& stream, std::vector<NInventory::CItem>& out);