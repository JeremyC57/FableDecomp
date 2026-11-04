// CTCShopKeeper::GetShopTC() const -- the shop component (interface 0x53) of the keeper's shop thing when it is alive, else of the keeper's own thing; null when that thing has none
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00798F50, 163 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem const @ 0x00A01B50 (three times on the Shop pointer), the interface LowerBound
// @ 0x0040F020 (once per arm).
// Layout (retail; Ego_r.pdb CTCShopKeeper with the 12-byte vector): Customer +0x0c, Shop (CIntelligentPointer<CThing>) +0x14;
// CThing interface bitset +0x20 (0x53 = bit 19 of the third word, `test [+28h],80000h`), map +0x44, dead flag bit 0 of +0x91.
// Shape: each arm of the alive test holds its own HasTC + one-ternary GetTC lookup assigned to the result (VC tail-duplicates
// the return into both hit paths); the null result is a zero-initialised local (ebx) returned through the shared exit.
class CThing;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (it == map->end() || interfaceType < it->first ? map->end() : it)->second;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCShop;
class CTCShopKeeper { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    CIntelligentPointer<CThing> Customer;                   // +0x0c CIntelligentPointer<CThingCreatureBase>
    CIntelligentPointer<CThing> Shop;                       // +0x14 CIntelligentPointer<CThing>
    CTCShop* GetShopTC() const;
};

CTCShop* CTCShopKeeper::GetShopTC() const
{
    CTCShop* shopTC = 0;
    if (Shop.GetPItem() && !(Shop.GetPItem()->m_flags91 & 1)) {
        CThing* shop = Shop.GetPItem();
        if (shop->HasTC(0x53))
            shopTC = (CTCShop*)shop->GetTC(0x53);
    } else {
        if (ParentThing->HasTC(0x53))
            shopTC = (CTCShop*)ParentThing->GetTC(0x53);
    }
    return shopTC;
}