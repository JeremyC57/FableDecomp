#include <cstdio>
class CCharString {
public:
    void* PStringData;
    CCharString(const char* s, long length);             // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
};
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
class CTCHeroStats {                                    // interface 4
public:
    enum { INTERFACE_TYPE = 4 };
    unsigned char _pad000[0x170];
    CCharString   DeathRecoveryMarkerName;               // +0x170 (Ego_r.pdb +0x188)
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DD0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CGameScriptInterface {
public:
    virtual CCharString GetDeathRecoveryMarkerName() const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};
CCharString CGameScriptInterface::GetDeathRecoveryMarkerName() const
{
    const CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        const CTCHeroStats* stats;
        if (hero->PeekTC(&stats))
            return stats->DeathRecoveryMarkerName;
    }
    return CCharString("", -1);
}

static CInterfaceMap::SEntry g_entries[3];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
static int g_copies, g_literals; static long g_len;
CCharString::CCharString(const char* s, long length) { PStringData = (void*)s; g_len = length; ++g_literals; }
CCharString::CCharString(const CCharString& o) { PStringData = o.PStringData; ++g_copies; }
static CThingPlayerCreature g_hero; static CThingPlayerCreature* g_heroPtr;
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { return g_heroPtr; }
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
int main()
{
    static unsigned char raw[sizeof(CTCHeroStats)]; CTCHeroStats& stats = *(CTCHeroStats*)raw; stats.DeathRecoveryMarkerName.PStringData = (void*)"MK_RECOVER";
    for (int i = 0; i < 9; ++i) g_hero.UsingComponents[i] = 0;
    g_hero.Dead = 0;
    g_entries[0].type = 2; g_entries[0].component = 0;
    g_entries[1].type = 4; g_entries[1].component = (CTCBase*)&stats;
    g_hero.Components.first = g_entries; g_hero.Components.last = g_entries + 2;
    CPlayerManager pm; CGameScriptInterface gsi; gsi.PlayerManager = &pm;
    g_heroPtr = 0; CCharString a = gsi.GetDeathRecoveryMarkerName();
    bool ok = g_literals == 1 && g_len == -1 && *(const char*)a.PStringData == 0 && g_copies == 0;
    g_heroPtr = &g_hero; CCharString b = gsi.GetDeathRecoveryMarkerName();
    ok = ok && g_literals == 2 && g_copies == 0;                          // no interface 4
    g_hero.UsingComponents[0] = 1u << 4; g_hero.Dead = 1; CCharString c = gsi.GetDeathRecoveryMarkerName();
    ok = ok && g_literals == 3;                                           // dead
    g_hero.Dead = 0; CCharString d = gsi.GetDeathRecoveryMarkerName();
    ok = ok && g_literals == 3 && g_copies == 1 && d.PStringData == stats.DeathRecoveryMarkerName.PStringData;
    std::printf(ok ? "00896ee0_TEST PASS\n" : "00896ee0_TEST FAIL\n");
    return ok ? 0 : 1;
}