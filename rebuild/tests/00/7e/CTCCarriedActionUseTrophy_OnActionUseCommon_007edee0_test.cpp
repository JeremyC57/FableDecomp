// Behaviour test for CTCCarriedActionUseTrophy::OnActionUseCommon (0x007EDEE0): a player targeting a live thing with
// interface 0xda while no minigame runs goes to the wall-mount action; every other case goes to the trophy.
#include <cstdio>
#include <cstring>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};
class CThing { public:
    virtual void v000() {}
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_dead : 1;
    unsigned char m_91_1 : 7;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    CTCBase* GetTCSelect(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
    void Init() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_interfaces.last = 0; m_dead = 0; m_91_1 = 0; }
    void Add(CInterfaceMap::SEntry* e, int n) { m_interfaces.first = e; m_interfaces.last = e + n; for (int i = 0; i < n; ++i) m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31); }
};
class CThingCreatureBase : public CThing { public:
    bool Player;
    virtual bool IsPlayer() const { return Player; }
};
struct CTCTargetSource {
    CThing* Target;
    virtual CThing* GetTargetThing() { return Target; }
};
static int g_wallMount = 0, g_trophy = 0;
struct CTCCarriedActionUseWallMount {
    virtual void OnActionUse(CThingCreatureBase* user) { ++g_wallMount; }
};
class CTCTrophy { public:
    bool MinigameInProgress;
    void OnTrophyUseAction(CThingCreatureBase* user) { ++g_trophy; }
};
struct CTCCarriedActionUseTrophy {
    virtual void v00() {}
    CThing* ParentThing;
    CTCCarriedActionUseWallMount PTCCarriedActionUseWallMount;
    void OnActionUseCommon(CThingCreatureBase* user);
};

void CTCCarriedActionUseTrophy::OnActionUseCommon(CThingCreatureBase* user)
{
    CTCTrophy* trophy;
    ParentThing->GetInterface(&trophy, 0x39);
    if (!trophy->MinigameInProgress && user->IsPlayer() && user->HasTC(8)) {
        CThing* target = ((CTCTargetSource*)user->GetTCSelect(8))->GetTargetThing();
        if (target && !target->m_dead && target->HasTC(0xda)) {
            PTCCarriedActionUseWallMount.OnActionUse(user);
            return;
        }
    }
    trophy->OnTrophyUseAction(user);
}

static int Run(CTCCarriedActionUseTrophy& a, CThingCreatureBase& u)
{
    int w = g_wallMount, t = g_trophy;
    a.OnActionUseCommon(&u);
    return (g_wallMount - w) * 10 + (g_trophy - t);
}
int main()
{
    int bad = 0;
    CTCTrophy trophy; trophy.MinigameInProgress = false;
    CThing owner; owner.Init();
    CInterfaceMap::SEntry oe[2] = { { 0x10, 0 }, { 0x39, (CTCBase*)&trophy } };
    owner.Add(oe, 2);
    CThing mount; mount.Init();
    CInterfaceMap::SEntry me[1] = { { 0xda, 0 } };
    mount.Add(me, 1);
    CTCTargetSource targeting; targeting.Target = &mount;
    CThingCreatureBase user; user.Init(); user.Player = true;
    CInterfaceMap::SEntry ue[2] = { { 8, (CTCBase*)&targeting }, { 0x20, 0 } };
    user.Add(ue, 2);
    CTCCarriedActionUseTrophy action; action.ParentThing = &owner;
    if (Run(action, user) != 10) bad |= 1;           // wall mount
    mount.m_dead = 1;
    if (Run(action, user) != 1) bad |= 2;            // dead target -> trophy
    mount.m_dead = 0; mount.m_presentBits[6] = 0;
    if (Run(action, user) != 1) bad |= 4;            // target without 0xda -> trophy
    mount.m_presentBits[6] = 1UL << 26; targeting.Target = 0;
    if (Run(action, user) != 1) bad |= 8;            // no target
    targeting.Target = &mount; user.Player = false;
    if (Run(action, user) != 1) bad |= 16;           // not a player
    user.Player = true; trophy.MinigameInProgress = true;
    if (Run(action, user) != 1) bad |= 32;           // minigame running
    trophy.MinigameInProgress = false; user.m_presentBits[0] = 0;
    if (Run(action, user) != 1) bad |= 64;           // no interface 8
    if (bad) { std::printf("007edee0_TEST FAIL\n"); return 1; }
    std::printf("007edee0_TEST PASS\n");
    return 0;
}