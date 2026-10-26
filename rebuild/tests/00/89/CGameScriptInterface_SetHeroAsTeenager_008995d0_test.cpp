#include <cstdio>
#include "engine/CPlayer.h"
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
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 = a creature)
    unsigned char _pad11[0x20 - 0x11];
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
class CTCHeroMorph {                                    // interface 3
public:
    enum { INTERFACE_TYPE = 3 };
    void SetAsTeenager(bool teenager);                   // 0x0071BA40 (Ego_r.pdb ?SetAsTeenager@CTCHeroMorph@@QAEX_N@Z)
};
class CTCHeroStats {                                    // interface 4
public:
    enum { INTERFACE_TYPE = 4 };
    void SetAge_0057b26f(float age);                              // 0x0057B26F (clamped to [0, 1]; name not confirmed)
    void ResetTrainingStatus_0057a5be();                          // 0x0057A5BE (name not confirmed)
};
class CThingPlayerCreature : public CThing { };
struct CPlayer_Methods : CPlayer {
    CThingPlayerCreature* GetPControlledCreature() const; // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer_Methods& GetMainPlayer();                    // 0x00449970
};
class CGameScriptInterface {
public:
    virtual void SetHeroAsTeenager(bool teenager) const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};
void CGameScriptInterface::SetHeroAsTeenager(bool teenager) const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        CTCHeroMorph* morph;
        hero->GetTC(&morph);
        morph->SetAsTeenager(teenager);
        CTCHeroStats* stats;
        if (hero->GetTC(&stats))
        {
            stats->SetAge_0057b26f(1.0f);
            stats->ResetTrainingStatus_0057a5be();
        }
    }
}

static CInterfaceMap::SEntry g_entries[3];
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
static CThingPlayerCreature g_hero; static CThingPlayerCreature* g_heroPtr;
CThingPlayerCreature* CPlayer_Methods::GetPControlledCreature() const { return g_heroPtr; }
static unsigned char g_playerRaw[sizeof(CPlayer_Methods)];
CPlayer_Methods& CPlayerManager::GetMainPlayer() { return *(CPlayer_Methods*)g_playerRaw; }
static int g_teenCalls, g_ageCalls, g_resetCalls; static bool g_teen; static float g_age; static void* g_morphThis; static void* g_statsThis;
void CTCHeroMorph::SetAsTeenager(bool t) { ++g_teenCalls; g_teen = t; g_morphThis = this; }
void CTCHeroStats::SetAge_0057b26f(float a) { ++g_ageCalls; g_age = a; g_statsThis = this; }
void CTCHeroStats::ResetTrainingStatus_0057a5be() { ++g_resetCalls; if (this != g_statsThis) g_resetCalls += 100; }
int main()
{
    static char morph, stats;
    for (int i = 0; i < 9; ++i) g_hero.UsingComponents[i] = 0;
    g_hero.UsingComponents[0] = 1u << 3; g_hero.Dead = 0;
    g_entries[0].type = 3; g_entries[0].component = (CTCBase*)&morph;
    g_entries[1].type = 4; g_entries[1].component = (CTCBase*)&stats;
    g_hero.Components.first = g_entries; g_hero.Components.last = g_entries + 2;
    CPlayerManager pm; CGameScriptInterface gsi; gsi.PlayerManager = &pm;
    g_heroPtr = 0; gsi.SetHeroAsTeenager(true); bool ok = g_teenCalls == 0;
    g_heroPtr = &g_hero; g_hero.Dead = 1; gsi.SetHeroAsTeenager(true); ok = ok && g_teenCalls == 0; g_hero.Dead = 0;
    gsi.SetHeroAsTeenager(true);
    ok = ok && g_teenCalls == 1 && g_teen && g_morphThis == &morph && g_ageCalls == 0 && g_resetCalls == 0;
    g_hero.UsingComponents[0] = (1u << 3) | (1u << 4);
    gsi.SetHeroAsTeenager(false);
    ok = ok && g_teenCalls == 2 && !g_teen && g_ageCalls == 1 && g_age == 1.0f && g_statsThis == &stats && g_resetCalls == 1;
    std::printf(ok ? "008995d0_TEST PASS\n" : "008995d0_TEST FAIL\n");
    return ok ? 0 : 1;
}