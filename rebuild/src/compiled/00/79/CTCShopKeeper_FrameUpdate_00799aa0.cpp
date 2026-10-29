// CTCShopKeeper::FrameUpdate() -- every 15th frame (staggered by the thing's unique id): take a new customer when one is needed, then repress or release the keeper's opinions as the village's hero-criminal state changes
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799AA0, 206 bytes (plain `ret`); CTCShopKeeper vtable 0x0126C154 slot 9.
// Callees: CWorld::GetWorldFrame (static) @ 0x0049D870, _aullrem @ 0x00BFEBB0 (compiler helper), CTCShopKeeper::GetShopTC
// @ 0x00798F50, RequiresNewCustomer @ 0x007990C0, AcquireNewCustomer @ 0x00799900, RepressOpinions(bool) @ 0x007996B0, the
// interface LowerBound @ 0x0040F020, CTCVillageMember::GetPVillage const @ 0x007EE540, CTCVillage::IsHeroCriminal @ 0x0065BFCE.
// Layout: CThing UniqueID (unsigned __int64) +0x18, interface bitset +0x20 (0x23 = bit 3 of the second word), map +0x44;
// CTCShopKeeper RepressedOpinions +0x42; CTCShop StockState +0x0c.
// Shape: the id is read into a local before the frame call (`frame + id`, unsigned 64-bit remainder); HasTC + one-ternary
// GetTC on the re-read ParentThing; the flag byte is loaded once for both arms of the criminal test.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x18];
    unsigned __int64 UniqueID;                          // +0x18
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->first ? map->last : it)->second;
    }
};
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
    unsigned __int64 id = ParentThing->UniqueID;
    if ((CWorld::GetFrame() + id) % 15 == 0) {
        if (GetShopTC() && RequiresNewCustomer())
            AcquireNewCustomer();
        if (ParentThing->HasTC(0x23)) {
            CTCVillage* village = ((CTCVillageMember*)ParentThing->GetTC(0x23))->GetPVillage();
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