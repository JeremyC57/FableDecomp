// CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferOut(CPersistContext* context, CCharString* out, const CUIRegionInfo* value) -- text persist write of a region's shop/house counts as "(region,shops,barbers,tattoos,titles,forSale,housesForSale,owned,housesOwned,taverns)"
// Leaf: ?TransferOut@?$CPersistTraits@VCUIRegionInfo@CTCInventoryMap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00614290, 362 bytes, `ret 0Ch` (traits `this` unused).
// Ego_r.pdb: ?TransferOut@?$CPersistTraits@VCUIRegionInfo@CTCInventoryMap@@@@QAEXAAVCPersistContext@@AAVCCharString@@
//   ABVCUIRegionInfo@CTCInventoryMap@@@Z; CUIRegionInfo (0x28 bytes) = ten longs: Region, Shops, BarberShops, TitleShops,
//   TatooShops, ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns. TatooShops (+0x10) is written before
//   TitleShops (+0xC).
// Callees: CCharString::operator+=(const char*) @ 0x0099F100 ("(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020),
//   TransferOutLong @ 0x009E2560 (`__fastcall` context, out, &copy).
// Shape: the landed 0x00614080 / 0x0040FA20 text writers: each long through an inline CPersistTraits<long>::TransferOut
//   (copy into the dead `out` argument slot, no frame). Speed-optimised (default) TU.
class CPersistContext;
class CCharString {
public:
    class CCharStringData* PStringData;
    CCharString& operator+=(const char* text);         // 0x0099F100
};
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

class CTCInventoryMap {
public:
    class CUIRegionInfo {                              // 0x28 bytes
    public:
        long Region;                                   // +0x00
        long Shops;                                    // +0x04
        long BarberShops;                              // +0x08
        long TitleShops;                               // +0x0C
        long TatooShops;                               // +0x10
        long ShopsForSale;                             // +0x14
        long HousesForSale;                            // +0x18
        long ShopsOwned;                               // +0x1C
        long HousesOwned;                              // +0x20
        long Taverns;                                  // +0x24
    };
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferOut(CPersistContext* context, CCharString* out, const long* value) const
    {
        long copy = *value;
        TransferOutLong(context, out, &copy);
    }
};
template <> struct CPersistTraits<CTCInventoryMap::CUIRegionInfo> {
    void TransferOut(CPersistContext* context, CCharString* out, const CTCInventoryMap::CUIRegionInfo* value);
};

void CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferOut(CPersistContext* context, CCharString* out,
                                                                 const CTCInventoryMap::CUIRegionInfo* value)
{
    CPersistTraits<long> longTraits;
    *out += "(";
    longTraits.TransferOut(context, out, &value->Region);
    *out += ",";
    longTraits.TransferOut(context, out, &value->Shops);
    *out += ",";
    longTraits.TransferOut(context, out, &value->BarberShops);
    *out += ",";
    longTraits.TransferOut(context, out, &value->TatooShops);
    *out += ",";
    longTraits.TransferOut(context, out, &value->TitleShops);
    *out += ",";
    longTraits.TransferOut(context, out, &value->ShopsForSale);
    *out += ",";
    longTraits.TransferOut(context, out, &value->HousesForSale);
    *out += ",";
    longTraits.TransferOut(context, out, &value->ShopsOwned);
    *out += ",";
    longTraits.TransferOut(context, out, &value->HousesOwned);
    *out += ",";
    longTraits.TransferOut(context, out, &value->Taverns);
    *out += ")";
}