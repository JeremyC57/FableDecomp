// CTCFishing::GetPFishingRodDef() const -- the fishing-rod def of the item the live hero carries in his primary slot, or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B0880, length 200 bytes (+ int3 padding). __fastcall, `ret`.
//         Ego_r.pdb: `?GetPFishingRodDef@CTCFishing@@QBEPBVCFishingRodDef@@XZ`.
// Callees: CTCBase::GetPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960,
//          CPlayer::GetHeroThing @ 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0
//          (interface 0x46 CTCCarrying, word 2 bit 6; interface 0xEA CTCInventoryItem, word 7 bit 10),
//          CTCCarrying::GetThingInPrimarySlot() const @ 0x006A1440, the def's virtual slot 1 (destroy at the last release).
// Shape: the carrying component is `T* p = 0; if (HasTC(k)) p = GetTC(k);` (the preloaded `xor ecx,ecx`, called even
//        when absent); the inventory item is the out-parameter GetInterface (component in eax); the rod def is a CDefPointer copy of the item's PDef whose add-ref and release share one null test
//        (both count stores kept; AddRef/Release are pointee members, else the count goes through edx) and whose raw
//        object is the result.
// Layout: CTCInventoryItem::PDef +0x0C (PDB); retail CThing Dead flag +0x91 bit 0.
class CThing;
class CTCBase;
class CFishingRodDef;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    void AddRef() const { ++RefCount; }
    void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CInventoryItemDef : public CDefClassBase { };

template<class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    __forceinline ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
};

class CTCInventoryItem {                            // interface 0xEA
public:
    unsigned char _pad00[0x0c];
    CDefPointer<const CInventoryItemDef> PDef;      // +0x0C
};

class CTCCarrying {                                 // interface 0x46
public:
    CThing* GetThingInPrimarySlot() const;          // 0x006A1440
};

class CPlayer {
public:
    CThing* GetHeroThing() const;                   // 0x00487DC0
};
class CPlayerManager {
public:
    const CPlayer& GetMainPlayer() const;           // 0x00449960
};

class CTCFishing {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    const CPlayerManager& GetPlayerManager() const; // 0x00686D50 (CTCBase)
    const CFishingRodDef* GetPFishingRodDef() const;
};

const CFishingRodDef* CTCFishing::GetPFishingRodDef() const
{
    CThing* hero = GetPlayerManager().GetMainPlayer().GetHeroThing();
    if (hero && !(hero->m_flags91 & 1)) {
        CTCCarrying* carrying = 0;
        if (hero->HasTC(0x46))
            carrying = (CTCCarrying*)hero->GetTC(0x46);
        CThing* rod = carrying->GetThingInPrimarySlot();
        CTCInventoryItem* item;
        if (rod && !(rod->m_flags91 & 1) && rod->GetInterface(&item, 0xea)) {
            CDefPointer<const CInventoryItemDef> def(item->PDef);
            return (const CFishingRodDef*)def.Object;
        }
    }
    return 0;
}