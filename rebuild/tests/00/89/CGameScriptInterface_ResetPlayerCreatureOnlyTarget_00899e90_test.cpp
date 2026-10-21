#include <cstdio>
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
class CTCTargetingBase {                                // interface 8 (class not confirmed)
public:
    enum { INTERFACE_TYPE = 8 };
    void SetOnlyTarget(CThing* target);                  // 0x008ABC90 (name not confirmed)
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPPlayerCharacter() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CMainGameComponent {
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                      // +0x1c (scoped_ptr)
};
class CGameScriptInterface {
public:
    virtual void ResetPlayerCreatureOnlyTarget() const;
    void*               World;                           // +0x04
    CMainGameComponent* Component;                       // +0x08
};
void CGameScriptInterface::ResetPlayerCreatureOnlyTarget() const
{
    CThing* hero = Component->PPlayerManager->GetMainPlayer().GetPPlayerCharacter();
    if (hero && !hero->IsDead())
    {
        CTCTargetingBase* tc;
        if (hero->GetTC(&tc))
            tc->SetOnlyTarget(0);
    }
}

static CInterfaceMap::SEntry g_entries[3];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
class CTCImpl { public: int pad; };
static int g_calls; static CThing* g_target = (CThing*)1; static CTCTargetingBase* g_who;
void CTCTargetingBase::SetOnlyTarget(CThing* t) { ++g_calls; g_target = t; g_who = this; }
static CThingPlayerCreature g_hero; static CThingPlayerCreature* g_heroPtr;
CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return g_heroPtr; }
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
int main()
{
    static CTCImpl impl; CTCTargetingBase* tc = (CTCTargetingBase*)&impl;
    for (int i = 0; i < 9; ++i) g_hero.UsingComponents[i] = 0;
    g_hero.Dead = 0;
    g_entries[0].type = 3; g_entries[0].component = 0;
    g_entries[1].type = 8; g_entries[1].component = (CTCBase*)tc;
    g_hero.Components.first = g_entries; g_hero.Components.last = g_entries + 2;
    CPlayerManager pm; CMainGameComponent mgc; mgc.PPlayerManager = &pm;
    CGameScriptInterface gsi; gsi.Component = &mgc;
    g_heroPtr = 0; gsi.ResetPlayerCreatureOnlyTarget();
    bool ok = g_calls == 0;
    g_heroPtr = &g_hero; gsi.ResetPlayerCreatureOnlyTarget();
    ok = ok && g_calls == 0;                                     // no interface 8
    g_hero.UsingComponents[0] = 1u << 8; g_hero.Dead = 1; gsi.ResetPlayerCreatureOnlyTarget();
    ok = ok && g_calls == 0;                                     // dead
    g_hero.Dead = 0; gsi.ResetPlayerCreatureOnlyTarget();
    ok = ok && g_calls == 1 && g_target == 0 && g_who == tc;
    std::printf(ok ? "00899e90_TEST PASS\n" : "00899e90_TEST FAIL\n");
    return ok ? 0 : 1;
}