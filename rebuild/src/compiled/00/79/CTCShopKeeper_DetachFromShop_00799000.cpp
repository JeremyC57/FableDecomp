// CTCShopKeeper::DetachFromShop() -- tell the alive shop's shop component it has no keeper, clear the keeper's shop pointer, and refresh the player GUI when a shop was told
// Leaf: ?DetachFromShop@CTCShopKeeper@@QAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799000, 181 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem const @ 0x00A01B50, the interface LowerBound @ 0x0040F020, CTCShop::SetShopKeeper
// (CThingCreatureBase*) @ 0x007751A0, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (the clearing temporary; CIntelligentPointer vtable 0x01238C6C stored inline), CPlayerGui::UpdateMiniMapMarkers(bool) @ 0x004378C0 on
// (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790.
// Layout (retail; Ego_r.pdb CTCShopKeeper): Shop (CIntelligentPointer<CThing>) +0x14; CThing interface bitset +0x20 (0x53 =
// bit 19 of the third word), map +0x44, dead flag bit 0 of +0x91.
// Shape: nested alive / HasTC tests (the third GetPItem in a named local serves HasTC and GetTC) with the one-ternary GetTC (component straight into ecx); the flag rides in bl across the
// clearing block (`cleared(0); item = cleared.GetPItem(); Shop.SetPItem(item);`, the temporary in its own scope).
class CThing;
class CBaseObject;
class CThingCreatureBase;
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
class CTCShop { public:
    void SetShopKeeper(CThingCreatureBase* shopKeeper); // 0x007751A0
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool flag);               // 0x004378C0
};
class CTCShopKeeper { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    CBaseIntelligentPointer Customer;                   // +0x0c CIntelligentPointer<CThingCreatureBase>
    CBaseIntelligentPointer Shop;                       // +0x14 CIntelligentPointer<CThing>
    void DetachFromShop();
};

void CTCShopKeeper::DetachFromShop()
{
    bool detached = false;
    if (((CThing*)Shop.GetPItem()) && !((CThing*)Shop.GetPItem())->IsDead()) {
        CThing* shop = ((CThing*)Shop.GetPItem());
        if (shop->HasTC(TCI_SHOP)) {
            ((CTCShop*)((CTCShop*)&shop->GetTC(TCI_SHOP)))->SetShopKeeper(0);
            detached = true;
        }
    }
    {
        CIntelligentPointer<CThing> cleared(0);
        CThing* item = ((CThing*)cleared.GetPItem());
        Shop.SetPItem((CBaseObject*)item);
    }
    if (detached)
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
}