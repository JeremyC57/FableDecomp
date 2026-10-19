// CGameScriptInterface::SetHeroWillAsUsable(bool usable) const -- enable or disable the hero's will (CTCHero, TC 0x29); when
// Grade: RELOCATION_MATCH -- the current region is bound to a named `CRegion&` local before the def counted pointer is built from its Data: the def then loads straight into ecx (inc, flag read, dec, virtual release) like retail; building it inside one expression held it in eax
// enabling, the will is only usable in the current region if the region definition does not forbid it
// Leaf: SetHeroWillAsUsable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00898340, length 208 bytes, `ret 4`.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, the TC map lower bound
// @ 0x0040F020, CWorld virtual slot 12 GetWorldMap (+0x30), CWorldMap::GetCurrentRegionNumber @ 0x004FB150,
// CWorldMap::GetRegionEntry(long) @ 0x004FB490 (0x58-byte region records), the region def's virtual slot 1 (destroy).
// Layout: CGameScriptInterface World +0x04, PlayerManager +0x14; CThing dead bit +0x91 bit 0, TC 0x29 present bit
// +0x24 bit 9, TC map +0x44; CTCHero: CTCHero WillUsable +0x0D, WillUseOverriddenByScripts +0x0F; region record def
// pointer +0x24 (counted, count at def+4), CRegionDef::PreventHeroWillUse +0x71 (PDB +0x7D).
// Shape: the will TC comes from an unchecked inline lookup (`T* tc; GetInterface(&tc, k); return tc;` -- the absent
// path reads the never-written local, as retail does); World is read once into a local, so both GetWorldMap calls go
// through esi; the region's def pointer is a by-value CCountedPointer copy whose copy ctor and dtor share the null
// test.

struct CTCBase;
struct CInterfaceMap_Retail {                    // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

struct CThing {
    virtual void v000();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];       // +0x20 std::bitset of the interface types present
    CInterfaceMap_Retail m_interfaces;           // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_dead : 1;             // +0x91 bit 0
    unsigned char m_91_1 : 7;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* tc;
        GetInterface(&tc, interfaceType);
        return tc;
    }
};

struct CTCHero {                          // TC 0x29 (PDB layout agrees)
    unsigned char _pad[0xc];
    bool WeaponsUsable;                   // +0x0C
    bool WillUsable;                      // +0x0D
    bool WeaponUseOverriddenByScripts;    // +0x0E
    bool WillUseOverriddenByScripts;      // +0x0F
};

class CPlayer {
public:
    CThing* GetHeroThing();               // 0x00487DC0
};

struct CPlayerManager {
    CPlayer& GetMainPlayer();             // 0x00449970 (the non-const overload; 0x00449960 is the const one)
};

struct CRegionDef {
    virtual void v000();
    virtual void DeleteThis();            // slot 1 (+0x4)
    long m_refCount;                      // +0x04
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) DeleteThis(); }
    unsigned char _pad08[0x69];
    bool PreventHeroWillUse;                 // +0x71
};

template<class T> struct CCountedPointer {
    T* Data;
    CCountedPointer(T* data) : Data(data) { if (Data) Data->AddRef(); }
    ~CCountedPointer() { if (Data) Data->Release(); }
    T* operator->() const { return Data; }
    bool IsNull() const { return Data == 0; }
};

struct CRegion {                          // 0x58-byte world-map region record
    unsigned char _pad00[0x24];
    CCountedPointer<CRegionDef> m_pDef;   // +0x24
    unsigned char _pad28[0x30];
};

struct CWorldMap {
    long GetCurrentRegionNumber();        // 0x004FB150
    CRegion& GetRegionEntry(long region); // 0x004FB490
};

struct CWorld {
    virtual void w000(); virtual void w001(); virtual void w002(); virtual void w003(); virtual void w004(); virtual void w005();
    virtual void w006(); virtual void w007(); virtual void w008(); virtual void w009(); virtual void w010(); virtual void w011();
    virtual CWorldMap* GetWorldMap();     // slot 12 (+0x30)
};

struct CGameScriptInterface {
    virtual void v000();
    CWorld* World;                        // +0x04
    void* Component;                      // +0x08
    void* DisplayEngine;                  // +0x0c
    void* DefinitionManager;              // +0x10
    CPlayerManager* PlayerManager;        // +0x14
    void SetHeroWillAsUsable(bool usable) const;
};

void CGameScriptInterface::SetHeroWillAsUsable(bool usable) const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetHeroThing();
    if (hero && !hero->m_dead) {
        CTCHero* will = hero->GetTC<CTCHero>(0x29);
        will->WillUseOverriddenByScripts = !usable;
        if (usable) {
            CRegion& region = World->GetWorldMap()->GetRegionEntry(World->GetWorldMap()->GetCurrentRegionNumber());
            CCountedPointer<CRegionDef> def(region.m_pDef.Data);
            if (!def.IsNull()) {
                if (!def->PreventHeroWillUse)
                    will->WillUsable = true;
            }
        } else {
            will->WillUsable = false;
        }
    }
}