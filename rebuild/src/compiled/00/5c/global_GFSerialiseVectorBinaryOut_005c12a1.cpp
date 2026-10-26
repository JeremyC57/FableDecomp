// GFSerialiseVectorBinaryOut(CDataOutputStream& stream, const vector<NInventory::CItem>& value) -- write the count, then every item through its traits
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C12A1, length 89 bytes (__fastcall stream, value).
// Callees: CDataOutputStream::WriteLong 0x00993EB0, CItem traits TransferBinaryOut 0x005C0A98.
// Model: engine/CDataOutputStream.h with a methods-only writer adapter; NInventory::CItem is a local model (see 005C2AD8:
//   engine/CItem.h flattens the counted pointer).
// Shape: `#pragma optimize("y", off)` + `("s", on)`; the count is `value.size()` (`idiv` by 0x14); the traits holds a DefaultVal.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"
struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteLong(long value);                             // 0x00993EB0
};
struct SCountBlock;
template <class T> class CCountedPointer { public:
    T* m_pObject;
    SCountBlock* m_pBlock;
    CCountedPointer() : m_pObject(0), m_pBlock(0) { }
    ~CCountedPointer();                                     // 0x005C04BF
};
namespace NInventory {
class CPerItemData;
class CItem { public:
    long ItemType;
    long NumberOfItems;
    CCountedPointer<CPerItemData> PPerItemData;
    long AverageBoughtPrice;
    CItem() : ItemType(0), NumberOfItems(0), AverageBoughtPrice(0) { }
};
}
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const T* value) const;   // 0x005C0A98
};
template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, std::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}
template void __fastcall GFSerialiseVectorBinaryOut<NInventory::CItem>(CDataOutputStreamWriter& stream, std::vector<NInventory::CItem>& value);