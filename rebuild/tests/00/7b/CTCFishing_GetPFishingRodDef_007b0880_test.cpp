#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
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
    CDefPointer() : Object(0) {}                    // test only
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

// ---- test stubs for the retail callees ----
static int g_destroyed; static CThing* g_hero; static CThing* g_rod;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void CDefClassBase::v00() {}
void CDefClassBase::Destroy() const { ++g_destroyed; }
static CTCCarrying* g_carrying;
CThing* CTCCarrying::GetThingInPrimarySlot() const { return this == g_carrying ? g_rod : 0; }
static CPlayer g_player; static CPlayerManager g_pm;
CThing* CPlayer::GetHeroThing() const { return g_hero; }
const CPlayer& CPlayerManager::GetMainPlayer() const { return g_player; }
const CPlayerManager& CTCFishing::GetPlayerManager() const { return g_pm; }
void CTCFishing::v00() {}

int main()
{
    int ok = 1;
    CTCCarrying carrying; CTCInventoryItem item; CInventoryItemDef def; def.RefCount = 1;
    item.PDef.Object = &def;
    CInterfaceMap::SEntry he; he.type = 0x46; he.component = (CTCBase*)&carrying;
    CInterfaceMap::SEntry re; re.type = 0xea; re.component = (CTCBase*)&item;
    CThing hero, rod; std::memset(&hero, 0, sizeof(hero)); std::memset(&rod, 0, sizeof(rod));
    hero.m_interfaces.first = &he; hero.m_interfaces.last = &he + 1; hero.m_presentBits[2] = 1u << 6;
    rod.m_interfaces.first = &re; rod.m_interfaces.last = &re + 1; rod.m_presentBits[7] = 1u << 10;
    g_hero = &hero; g_rod = &rod; g_carrying = &carrying;
    CTCFishing fishing;
    if (fishing.GetPFishingRodDef() != (const CFishingRodDef*)&def || def.RefCount != 1 || g_destroyed) ok = 0;
    rod.m_presentBits[7] = 0;            // the carried thing is not an inventory item
    if (fishing.GetPFishingRodDef() != 0) ok = 0;
    rod.m_presentBits[7] = 1u << 10; rod.m_flags91 = 1;   // dead rod
    if (fishing.GetPFishingRodDef() != 0) ok = 0;
    rod.m_flags91 = 0; hero.m_flags91 = 1;                // dead hero
    if (fishing.GetPFishingRodDef() != 0) ok = 0;
    hero.m_flags91 = 0; hero.m_presentBits[2] = 0;        // no carrying component: null carrier, no rod
    if (fishing.GetPFishingRodDef() != 0) ok = 0;
    g_hero = 0;
    if (fishing.GetPFishingRodDef() != 0) ok = 0;
    if (!ok) { std::printf("007b0880_TEST FAIL\n"); return 1; }
    std::printf("007b0880_TEST PASS\n");
    return 0;
}