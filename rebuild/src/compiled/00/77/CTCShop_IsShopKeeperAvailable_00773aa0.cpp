// CTCShop::IsShopKeeperAvailable() const -- TRUE when the shop is open (StockState 0), its keeper is alive, has the CTCShopKeeper component (interface 0x90) and that component's RepressedOpinions flag is set
// Leaf: IsShopKeeperAvailable
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe 0x00773AA0, 118 bytes (plain `ret`, BOOL result).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (inline GetInterface, landed CAIStateGroupBase::CleanupMain 0x008FD050 form).
// Layout: retail CTCShop: StockState +0x0C, PShopKeeper (CIntelligentPointer<CThingCreatureBase>) +0x10; CThing
// presence bitset +0x20 (type 0x90 = bit 16 of +0x30), interface map +0x44, dead flag +0x91 bit 0; retail
// CTCShopKeeper (PDB less the 4 bytes of the debug vector): RepressedOpinions +0x42 (PDB 0x46).
// Shape: IS_THING_ALIVE inlined, the presence test on a third GetPItem, the type key in the `push ecx` slot.
struct CTCBaseAny;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
class CTCShopKeeper { public:
    unsigned char _pad00[0x42];
    bool RepressedOpinions;                         // +0x42
};
class CTCShop { public:
    virtual void v00();
    void*                   ParentThing;            // +0x04 (CTCBase)
    bool                    ToBeRemoved;            // +0x08
    bool                    IsInGlobalUpdateTCs;    // +0x09
    long                    StockState;             // +0x0c
    CIntelligentPointer<CThing> PShopKeeper;            // +0x10
    long IsShopKeeperAvailable() const;
};

long CTCShop::IsShopKeeperAvailable() const
{
    if (StockState == 0 && IS_THING_ALIVE(PShopKeeper)) {
        CThing* keeper = PShopKeeper.Get();
        CTCShopKeeper* shopKeeper;
        if (keeper->GetTC(TCI_SHOP_KEEPER, &shopKeeper)) {
            if (shopKeeper->RepressedOpinions)
                return 1;
        }
    }
    return 0;
}