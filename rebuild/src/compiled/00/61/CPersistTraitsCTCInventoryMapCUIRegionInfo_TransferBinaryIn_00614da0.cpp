// CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferBinaryIn(CMemoryDataInputStream* stream, CUIRegionInfo* value) -- binary read of one region's shop/house counts: reset to a zeroed record, then the ten longs (TatooShops before TitleShops, as written)
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCUIRegionInfo@CTCInventoryMap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00614DA0, 990 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCUIRegionInfo@CTCInventoryMap@@@@QAEXAAVCMemoryDataInputStream@@
//   AAVCUIRegionInfo@CTCInventoryMap@@@Z and the default constructor ??0CUIRegionInfo@CTCInventoryMap@@QAE@XZ;
//   CUIRegionInfo (0x28 bytes) = ten longs: Region, Shops, BarberShops, TitleShops, TatooShops, ShopsForSale,
//   HousesForSale, ShopsOwned, HousesOwned, Taverns. The read order matches the landed text writer 0x00614290.
// Callee: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) when the chunk runs out.
// Shape: `*value = CUIRegionInfo()` (ten zero stores through ebp, then `mov ecx,0Ah ; rep movsd`), then
//   CMemoryDataInputStream::Read inlined ten times (speed TU, `position += size`) through an inline
//   CPersistTraits<long>::TransferBinaryIn whose local takes the dead stream-argument slot; the last read keeps four
//   duplicated `store ; pop ; ret 8` tails. No frame pointer.
#include "engine/CMemoryDataInputStream.h"

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

// Method view over the shared engine header (Ego_r.pdb layout: position +4, chunkBase +0xC = current chunk read
// pointer, chunkLength +0x14 = bytes left in the chunk); the overflow reader is CDataInputStream's.
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
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
        CUIRegionInfo()
            : Region(0), Shops(0), BarberShops(0), TitleShops(0), TatooShops(0), ShopsForSale(0), HousesForSale(0),
              ShopsOwned(0), HousesOwned(0), Taverns(0) {}
    };
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream* stream, long* value)
    {
        long number;
        static_cast<CMemoryDataInputStreamReader*>(stream)->Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<CTCInventoryMap::CUIRegionInfo> {
    void TransferBinaryIn(CMemoryDataInputStream* stream, CTCInventoryMap::CUIRegionInfo* value);
};

void CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferBinaryIn(CMemoryDataInputStream* stream,
                                                                      CTCInventoryMap::CUIRegionInfo* value)
{
    *value = CTCInventoryMap::CUIRegionInfo();
    CPersistTraits<long> longTraits;
    longTraits.TransferBinaryIn(stream, &value->Region);
    longTraits.TransferBinaryIn(stream, &value->Shops);
    longTraits.TransferBinaryIn(stream, &value->BarberShops);
    longTraits.TransferBinaryIn(stream, &value->TatooShops);
    longTraits.TransferBinaryIn(stream, &value->TitleShops);
    longTraits.TransferBinaryIn(stream, &value->ShopsForSale);
    longTraits.TransferBinaryIn(stream, &value->HousesForSale);
    longTraits.TransferBinaryIn(stream, &value->ShopsOwned);
    longTraits.TransferBinaryIn(stream, &value->HousesOwned);
    longTraits.TransferBinaryIn(stream, &value->Taverns);
}