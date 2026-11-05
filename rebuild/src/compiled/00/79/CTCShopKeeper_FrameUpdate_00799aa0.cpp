// CTCShopKeeper::FrameUpdate() -- every 15th frame (staggered by the thing's unique id): take a new customer when one is needed, then repress or release the keeper's opinions as the village's hero-criminal state changes
// Leaf: FrameUpdate
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799AA0, 206 bytes (plain `ret`); CTCShopKeeper vtable 0x0126C154 slot 9.
// Callees: CWorld::GetWorldFrame (static) @ 0x0049D870, _aullrem @ 0x00BFEBB0 (compiler helper), CTCShopKeeper::GetShopTC
// @ 0x00798F50, RequiresNewCustomer @ 0x007990C0, AcquireNewCustomer @ 0x00799900, RepressOpinions(bool) @ 0x007996B0, the
// interface LowerBound @ 0x0040F020, CTCVillageMember::GetPVillage const @ 0x007EE540, CTCVillage::IsHeroCriminal @ 0x0065BFCE.
// Layout: CThing UniqueID (unsigned __int64) +0x18, interface bitset +0x20 (0x23 = bit 3 of the second word), map +0x44;
// CTCShopKeeper RepressedOpinions +0x42; CTCShop StockState +0x0c.
// Shape: the id is read into a local before the frame call (`frame + id`, unsigned 64-bit remainder); HasTC + one-ternary
// GetTC on the re-read ParentThing; the flag byte is loaded once for both arms of the criminal test.
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CWorld { public:
    static long __fastcall GetFrame();                        // 0x0049D870
};
class CTCVillage { public:
    bool IsHeroCriminal();                              // 0x0065BFCE
};
class CTCVillageMember { public:
    CTCVillage* GetPVillage() const;                    // 0x007EE540
};
class CTCShop { public:
    unsigned char _pad00[0x0c];
    long StockState;                                    // +0x0c ESTOCK_STATE
};
class CTCShopKeeper { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x42 - 0x08];
    bool RepressedOpinions;                             // +0x42
    CTCShop* GetShopTC() const;                         // 0x00798F50
    bool RequiresNewCustomer();                         // 0x007990C0
    void RepressOpinions(bool repress);                 // 0x007996B0
    void AcquireNewCustomer();                          // 0x00799900
    void FrameUpdate();
};

void CTCShopKeeper::FrameUpdate()
{
    unsigned __int64 id = ParentThing->GetUniqueID();
    if ((CWorld::GetFrame() + id) % 15 == 0) {
        if (GetShopTC() && RequiresNewCustomer())
            AcquireNewCustomer();
        if (ParentThing->HasTC(TCI_VILLAGE_MEMBER)) {
            CTCVillage* village = ((CTCVillageMember*)((CTCVillageMember*)&ParentThing->GetTC(TCI_VILLAGE_MEMBER)))->GetPVillage();
            if (village) {
                if (village->IsHeroCriminal()) {
                    if (RepressedOpinions)
                        RepressOpinions(false);
                } else if (!RepressedOpinions && GetShopTC() && GetShopTC()->StockState == 0)
                    RepressOpinions(true);
            }
        }
    }
}