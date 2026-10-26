// CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferBinaryOut(CDataOutputStream*, const CUIRegionInfo*) -- write the ten region counters
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FA610, 124 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0 (x10).
// Layout (PDB CTCInventoryMap::CUIRegionInfo, 0x28 bytes, ten longs): Region, Shops, BarberShops, TitleShops, TatooShops,
//   ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns. Retail writes TatooShops (+0x10) before TitleShops
//   (+0xC): the persist order differs from the declaration order. Speed-optimised (default) TU.
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteSLONG(long);   // 0x00993EB0
};

struct CDataOutputStream_Methods : CDataOutputStream {
};

class CTCInventoryMap { public:
    class CUIRegionInfo { public:
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

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CTCInventoryMap::CUIRegionInfo> {
    void TransferBinaryOut(CDataOutputStream* stream, const CTCInventoryMap::CUIRegionInfo* value);
};

void CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferBinaryOut(CDataOutputStream* stream, const CTCInventoryMap::CUIRegionInfo* value)
{
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)stream;
    out->WriteSLONG(value->Region);
    out->WriteSLONG(value->Shops);
    out->WriteSLONG(value->BarberShops);
    out->WriteSLONG(value->TatooShops);
    out->WriteSLONG(value->TitleShops);
    out->WriteSLONG(value->ShopsForSale);
    out->WriteSLONG(value->HousesForSale);
    out->WriteSLONG(value->ShopsOwned);
    out->WriteSLONG(value->HousesOwned);
    out->WriteSLONG(value->Taverns);
}