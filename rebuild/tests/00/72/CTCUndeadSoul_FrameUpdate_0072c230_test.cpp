// behaviour test for 0072c230 CTCUndeadSoul::FrameUpdate: the soul becomes drawable and targetable while the controlled
// hero is alive and can use ability 7, and stops otherwise; a player-type soul toggles its animation suspension
#include <cstdio>
class CThing;
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
    const SEntry* LowerBound(const long* type) const { const SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
static int g_suspendCalls = 0; static bool g_suspend = false;
class CThing {
public:
    virtual bool DrawIsDrawable() { return m_drawable; }
    virtual void SetAsDrawable(bool drawable) { m_drawable = drawable; }
    bool          m_drawable;
    unsigned char m_type;
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    void SetSuspendAnimation(bool suspend, long reason) { ++g_suspendCalls; g_suspend = suspend; (void)reason; }
};
class CTCInventoryAbilities { public: bool able; bool IsAbleToUseAbility(long ability, bool checkLevel) const { return ability == 7 && checkLevel && able; } };
class CTCTargeted { public: bool Targetable; };
static CThing* g_hero = 0;
class CPlayer { public: CThing* GetControlledCreature() { return g_hero; } };
class CPlayerManager { public: CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
static CPlayerManager g_players;
struct CTCBaseModel {
    virtual void v00() {}
    CThing* ParentThing;
    CPlayerManager* GetPlayerManager() { return &g_players; }
};
struct CTCUndeadSoul : CTCBaseModel {
    virtual void FrameUpdate();
};
void CTCUndeadSoul::FrameUpdate()
{
    const CThing* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
    if (hero && !(hero->m_flags & 1) && hero->HasTC(0x6f)
        && ((const CTCInventoryAbilities*)hero->GetTC(0x6f))->IsAbleToUseAbility(7, true)) {
        if (!ParentThing->DrawIsDrawable()) {
            ParentThing->SetAsDrawable(true);
            CTCTargeted* targeted = 0;
            if (ParentThing->HasTC(9))
                targeted = (CTCTargeted*)ParentThing->GetTC(9);
            targeted->Targetable = true;
            if (ParentThing->m_type == 1)
                ParentThing->SetSuspendAnimation(false, 0);
        }
    } else if (ParentThing->DrawIsDrawable()) {
        ParentThing->SetAsDrawable(false);
        CTCTargeted* targeted = 0;
        if (ParentThing->HasTC(9))
            targeted = (CTCTargeted*)ParentThing->GetTC(9);
        targeted->Targetable = false;
        if (ParentThing->m_type == 1)
            ParentThing->SetSuspendAnimation(true, 0);
    }
}
static void Clear(CThing& t) { for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_flags = 0; }
int main()
{
    CTCInventoryAbilities abilities; abilities.able = true;
    CTCTargeted targeted; targeted.Targetable = false;
    CInterfaceMap::SEntry heroEntries[1] = { { 0x6f, (CTCBase*)&abilities } };
    CInterfaceMap::SEntry soulEntries[1] = { { 9, (CTCBase*)&targeted } };
    CThing hero; Clear(hero); hero.m_presentBits[3] = 1UL << 15; hero.m_interfaces.first = heroEntries; hero.m_interfaces.last = heroEntries + 1;
    CThing soul; Clear(soul); soul.m_presentBits[0] = 1UL << 9; soul.m_interfaces.first = soulEntries; soul.m_interfaces.last = soulEntries + 1;
    soul.m_drawable = false; soul.m_type = 1;
    CTCUndeadSoul tc; tc.ParentThing = &soul; g_hero = &hero;
    tc.FrameUpdate();
    bool ok = soul.m_drawable && targeted.Targetable && g_suspendCalls == 1 && !g_suspend;
    tc.FrameUpdate();                          // already drawable: unchanged
    ok = ok && g_suspendCalls == 1;
    abilities.able = false; tc.FrameUpdate();
    ok = ok && !soul.m_drawable && !targeted.Targetable && g_suspendCalls == 2 && g_suspend;
    abilities.able = true; g_hero = 0; soul.m_drawable = true; tc.FrameUpdate();
    ok = ok && !soul.m_drawable && g_suspendCalls == 3;
    if (!ok) { std::printf("0072c230_TEST FAIL\n"); return 1; }
    std::printf("0072c230_TEST PASS\n");
    return 0;
}