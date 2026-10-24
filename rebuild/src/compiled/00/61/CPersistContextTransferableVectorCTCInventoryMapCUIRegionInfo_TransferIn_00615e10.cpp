// CPersistContext_TransferableVector<CTCInventoryMap::CUIRegionInfo>::TransferIn(CPersistContext* context, CStringParser* parser, long index) -- parses map region record `index` through a default CPersistTraits<CUIRegionInfo>
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00615E10, 85 bytes (+11 int3 padding), `ret 0Ch`.
// Callee: 0x00615E70 CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferIn(CPersistContext*, CStringParser*,
// CUIRegionInfo*) (thiscall, `ret 0Ch`).
// Persist READ hook (vtable 0x01257178 slot 3; slot 2 is TransferOut 0x00614230, slot 1 Resize 0x00619420, slot 0 the
// count 0x00614210) of CPersistContext_TransferableVector<CTCInventoryMap::CUIRegionInfo>. The manifest's `<long>` is
// propagated: the 40-byte stride (`lea edx,[eax+eax*4] ; lea ecx,[eax+edx*8]`) and the traits callee name the type
// (Ego_r.pdb CUIRegionInfo, 0x28 bytes: ten longs Region..Taverns). The traits object ({ CUIRegionInfo DefaultVal; },
// the `sub esp,28h` frame) is zeroed member by member through one register by the record's inline constructor.
struct CPersistContext;
struct CStringParser;
struct CCharString;

struct CUIRegionInfo {                             // CTCInventoryMap::CUIRegionInfo, 0x28 bytes
    long Region;                                   // +0x00
    long Shops;                                    // +0x04
    long BarberShops;                              // +0x08
    long TitleShops;                               // +0x0c
    long TatooShops;                               // +0x10
    long ShopsForSale;                             // +0x14
    long HousesForSale;                            // +0x18
    long ShopsOwned;                               // +0x1c
    long HousesOwned;                              // +0x20
    long Taverns;                                  // +0x24
    CUIRegionInfo()
        : Region(0), Shops(0), BarberShops(0), TitleShops(0), TatooShops(0), ShopsForSale(0), HousesForSale(0),
          ShopsOwned(0), HousesOwned(0), Taverns(0)
    {
    }
};

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return *(m_pFirst + index); }
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value);       // 0x00614290 for CUIRegionInfo
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);   // 0x00615E70 for CUIRegionInfo
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CUIRegionInfo>* TransVector;         // +0x4
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    CPersistTraits<CUIRegionInfo> traits;
    traits.TransferIn(context, parser, &(*TransVector)[index]);
}