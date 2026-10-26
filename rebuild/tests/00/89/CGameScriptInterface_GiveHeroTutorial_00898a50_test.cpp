#include <cstdio>
class CTCBase;
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
        *out = (T*)((it == map->last || T::INTERFACE_TYPE < it->type) ? map->last : it)->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        *out = (const T*)((it == map->last || T::INTERFACE_TYPE < it->type) ? map->last : it)->component;
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
enum ETutorialCategory { TUTORIAL_CATEGORY_FORCE_DWORD = 0x7fffffff };
class CTCHero {
public:
    enum { INTERFACE_TYPE = 0x29 };
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x006C9FA0
};
class CGameScriptInterface {
public:
    virtual bool GiveHeroTutorial(ETutorialCategory category) const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

bool CGameScriptInterface::GiveHeroTutorial(ETutorialCategory category) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPPlayerCharacter();
    CTCHero* tc;
    if (hero && !hero->IsDead() && hero->GetTC(&tc) && tc)
        return tc->GiveTutorialForCategory(category, false);
    return false;
}

static CInterfaceMap::SEntry g_entries[3];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
static CThingPlayerCreature g_hero; static CThingPlayerCreature* g_heroPtr;
CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return g_heroPtr; }
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
static CTCHero* g_calledOn; static long g_cat; static int g_force, g_calls;
bool CTCHero::GiveTutorialForCategory(ETutorialCategory c, bool force) { g_calledOn = this; g_cat = c; g_force = force; ++g_calls; return c == 7; }
int main()
{
    static unsigned char raw[4]; CTCHero* hero_tc = (CTCHero*)raw;
    for (int i = 0; i < 9; ++i) g_hero.UsingComponents[i] = 0;
    g_hero.UsingComponents[1] = 1u << (0x29 & 31); g_hero.Dead = 0;
    g_entries[0].type = 4; g_entries[0].component = 0;
    g_entries[1].type = 0x29; g_entries[1].component = (CTCBase*)hero_tc;
    g_hero.Components.first = g_entries; g_hero.Components.last = g_entries + 2;
    CPlayerManager pm; CGameScriptInterface gsi; gsi.PlayerManager = &pm;
    g_heroPtr = &g_hero;
    bool ok = gsi.GiveHeroTutorial((ETutorialCategory)7) && g_calledOn == hero_tc && g_cat == 7 && g_force == 0 && g_calls == 1;
    ok = ok && !gsi.GiveHeroTutorial((ETutorialCategory)3) && g_calls == 2;
    g_hero.Dead = 1; ok = ok && !gsi.GiveHeroTutorial((ETutorialCategory)7) && g_calls == 2; g_hero.Dead = 0;
    g_hero.UsingComponents[1] = 0; ok = ok && !gsi.GiveHeroTutorial((ETutorialCategory)7) && g_calls == 2; g_hero.UsingComponents[1] = 1u << (0x29 & 31);
    g_entries[1].component = 0; ok = ok && !gsi.GiveHeroTutorial((ETutorialCategory)7) && g_calls == 2; g_entries[1].component = (CTCBase*)hero_tc;
    g_heroPtr = 0; ok = ok && !gsi.GiveHeroTutorial((ETutorialCategory)7) && g_calls == 2;
    std::printf(ok ? "00898a50_TEST PASS\n" : "00898a50_TEST FAIL\n");
    return ok ? 0 : 1;
}