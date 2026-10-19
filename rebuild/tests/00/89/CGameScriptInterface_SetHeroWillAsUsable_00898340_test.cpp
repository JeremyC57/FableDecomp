#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
#include <string.h>
// Behaviour test for CGameScriptInterface::SetHeroWillAsUsable (0x00898340).
struct CTCBase;
struct CInterfaceMap_Retail {                    // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};

struct CThing {
    virtual void v000() {}
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
    CThing* hero;
    CThing* GetHeroThing() { return hero; }
};

struct CPlayerManager {
    CPlayer player;
    CPlayer& GetMainPlayer() { return player; }
};

struct CRegionDef {
    virtual void v000() {}
    int deleted;
    virtual void DeleteThis() { ++deleted; }
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

struct CRegion {
    CRegion() : m_pDef(0) {}
    unsigned char _pad00[0x24];
    CCountedPointer<CRegionDef> m_pDef;   // +0x24
    unsigned char _pad28[0x30];
};

struct CWorldMap {
    CRegion regions[3]; long current;
    long GetCurrentRegionNumber() { return current; }
    CRegion& GetRegionEntry(long region) { return regions[region]; }
};

struct CWorld {
    virtual void w000() {} virtual void w001() {} virtual void w002() {} virtual void w003() {} virtual void w004() {} virtual void w005() {}
    virtual void w006() {} virtual void w007() {} virtual void w008() {} virtual void w009() {} virtual void w010() {} virtual void w011() {}
    CWorldMap* map;
    virtual CWorldMap* GetWorldMap() { return map; }
};

struct CGameScriptInterface {
    virtual void v000() {}
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

int main()
{
    int ok = 1;
    CThing hero; memset((char*)&hero + 4, 0, sizeof(hero) - 4);
    CTCHero tc; memset(&tc, 0, sizeof(tc));
    CInterfaceMap_Retail::SEntry e[1] = { { 0x29, (CTCBase*)&tc } }; hero.m_interfaces.first = e; hero.m_interfaces.last = e + 1;
    hero.m_presentBits[1] |= 1UL << (0x29 & 31);
    CPlayerManager pm; pm.player.hero = &hero;
    CWorldMap wm; wm.current = 1; CWorld w; w.map = &wm;
    CRegionDef def; def.m_refCount = 1; def.deleted = 0; def.PreventHeroWillUse = false;
    wm.regions[1].m_pDef.Data = &def;
    CGameScriptInterface gsi; gsi.World = &w; gsi.PlayerManager = &pm;
    gsi.SetHeroWillAsUsable(true);
    if (!tc.WillUsable || tc.WillUseOverriddenByScripts || def.m_refCount != 1 || def.deleted) ok = 0;
    gsi.SetHeroWillAsUsable(false);
    if (tc.WillUsable || !tc.WillUseOverriddenByScripts) ok = 0;
    def.PreventHeroWillUse = true; gsi.SetHeroWillAsUsable(true);
    if (tc.WillUsable || tc.WillUseOverriddenByScripts) ok = 0;
    wm.regions[1].m_pDef.Data = 0; gsi.SetHeroWillAsUsable(true);
    if (tc.WillUsable) ok = 0;
    hero.m_dead = 1; tc.WillUseOverriddenByScripts = true; gsi.SetHeroWillAsUsable(true);
    if (!tc.WillUseOverriddenByScripts) ok = 0;
    wm.regions[1].m_pDef.Data = 0;
    if (ok) { printf("00898340_TEST PASS\n"); return 0; }
    printf("00898340_TEST FAIL\n"); return 1;
}