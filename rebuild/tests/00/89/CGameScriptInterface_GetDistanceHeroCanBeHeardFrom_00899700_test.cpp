// behaviour test for 0x00899700 CGameScriptInterface::GetDistanceHeroCanBeHeardFrom
#include <stdio.h>
#include <string.h>
struct CTCBase;
class CInterfaceMap {                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const long* type);          // 0x0040F020
    const SEntry* LowerBound(const long* type) const;    // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (T*)it->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (const T*)it->component;
        return true;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature*       GetPPlayerCharacter() const;       // 0x00487DC0
    const CThingPlayerCreature* PeekPPlayerCharacter() const;      // 0x00487DD0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                                       // 0x00449970
};
class CTCStealth {
public:
    enum { INTERFACE_TYPE = 0x36 };
    unsigned char _pad00[0x14];
    float         DistanceCanBeHeardFrom;                // +0x14
    float GetDistanceCanBeHeardFrom() const { return DistanceCanBeHeardFrom; }
};
class CGameScriptInterface {
public:
    virtual float GetDistanceHeroCanBeHeardFrom() const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

float CGameScriptInterface::GetDistanceHeroCanBeHeardFrom() const
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPPlayerCharacter();
    if (hero && !hero->IsDead())
    {
        const CTCStealth* tc;
        hero->PeekTC(&tc);
        return tc->GetDistanceCanBeHeardFrom();
    }
    return 0.0f;
}

static CThingPlayerCreature* g_hero;
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return g_hero; }
const CThingPlayerCreature* CPlayer::PeekPPlayerCharacter() const { return g_hero; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}

static unsigned char g_tcmem[0x1000];
static unsigned char g_thingmem[0x200];
static CInterfaceMap::SEntry g_entries[3];
int main()
{
    int ok = 1;
    CPlayerManager pm; CGameScriptInterface gsi; gsi.PlayerManager = &pm;
    const CGameScriptInterface* p = &gsi;
    CTCStealth* tc = (CTCStealth*)g_tcmem;
    CThingPlayerCreature* hero = (CThingPlayerCreature*)g_thingmem;
    g_entries[0].type = 1; g_entries[0].component = 0;
    g_entries[1].type = CTCStealth::INTERFACE_TYPE; g_entries[1].component = (CTCBase*)tc;
    g_entries[2].type = 0x100; g_entries[2].component = 0;
    hero->Components.first = g_entries; hero->Components.last = g_entries + 3;
    hero->UsingComponents[CTCStealth::INTERFACE_TYPE >> 5] |= 1UL << (CTCStealth::INTERFACE_TYPE & 31);
    g_hero = hero;
    tc->DistanceCanBeHeardFrom = 12.5f;
    ok &= (p->GetDistanceHeroCanBeHeardFrom() == 12.5f);
    g_hero = 0;
    ok &= (p->GetDistanceHeroCanBeHeardFrom() == 0.0f);
    g_hero = hero; hero->Dead = 1;
    ok &= (p->GetDistanceHeroCanBeHeardFrom() == 0.0f);

    printf(ok ? "00899700_TEST PASS\n" : "00899700_TEST FAIL\n");
    return ok ? 0 : 1;
}