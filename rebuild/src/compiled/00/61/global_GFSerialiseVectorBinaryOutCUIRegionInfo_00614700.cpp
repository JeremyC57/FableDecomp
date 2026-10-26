// _global::GFSerialiseVectorBinaryOut<CUIRegionInfo>(CDataOutputStream& stream, vector<CTCInventoryMap::CUIRegionInfo>& value) -- write the element count, then every region-info record through its persist traits
// Leaf: ??$GFSerialiseVectorBinaryOut@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00614700, 78 bytes (+2 int3), `ret` (__fastcall: the stream in ecx, the
//   vector in edx). Caller: the CPersistContext transfer 0x006197D0 (CTCInventoryMap::UIRegionsInfo), whose binary-in
//   arm is GFSerialiseVectorBinaryIn<CUIRegionInfo> 0x00619560 (this chunk).
// Callees: CDataOutputStream::WriteSLONG 0x00993EB0 (the count), CPersistTraits<CTCInventoryMap::CUIRegionInfo>::
//   TransferBinaryOut 0x005FA610 (landed; `ret 8`, the stream and the record pointer).
// Label correction: the catalog `GFSerialiseVectorBinaryOut<CHeroMarriageInfo>` is propagated; the element is the
//   0x28-byte CTCInventoryMap::CUIRegionInfo (`imul 66666667h ; sar edx,4` = divide by 40) and the traits callee names it.
// Shape: the generic template of the landed 0x005C12A1 / 0x0046AFF4: the traits local is 0x28 bytes and never written,
//   i.e. CPersistTraits<T> holds an uninitialised `T DefaultVal` (CUIRegionInfo is a POD); the loop is strength-reduced
//   (byte offset in ebx, opened inside the count guard; count down in esi). Speed TU, no pragma.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; CUIRegionInfo as the landed traits 0x005FA610.
#include "engine/CDataOutputStream.h"

struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteSLONG(long value);                             // 0x00993EB0
};
class CTCInventoryMap { public:
    class CUIRegionInfo { public:                                  // 0x28 bytes (Ego_r.pdb)
        long Region;            // +0x00
        long Shops;             // +0x04
        long BarberShops;       // +0x08
        long TitleShops;        // +0x0C
        long TatooShops;        // +0x10
        long ShopsForSale;      // +0x14
        long HousesForSale;     // +0x18
        long ShopsOwned;        // +0x1C
        long HousesOwned;       // +0x20
        long Taverns;           // +0x24
    };
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const T* value) const;   // 0x005FA610
};
template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, std::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}
template void __fastcall GFSerialiseVectorBinaryOut<CTCInventoryMap::CUIRegionInfo>(CDataOutputStreamWriter& stream,
                                                                                    std::vector<CTCInventoryMap::CUIRegionInfo>& value);