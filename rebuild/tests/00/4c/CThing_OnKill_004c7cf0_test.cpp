// Behaviour test for CThing::OnKill @ 0x004C7CF0: quest manager told (with the unloading flag), components killed with adds forbidden, manager notified.
#include <cstdio>
struct CThing;
static int g_order = 0;
struct CTCBase { int killedAt; bool addsAllowed; CThing* owner; void OnKill(); };
struct SComponentPair { long first; CTCBase* second; };
struct CCallOnKill { void operator()(SComponentPair& p) const { p.second->OnKill(); } };
static CCallOnKill for_each(SComponentPair* first, SComponentPair* last, CCallOnKill f)
{
    for (; first != last; ++first) f(*first);
    return f;
}
struct CQuestManager { int at; CThing* thing; bool unloading; void OnDeactivateEntity(CThing* t, bool u) { at = ++g_order; thing = t; unloading = u; } };
struct CThingManager { int at; CThing* thing; void OnThingKill(CThing* t) { at = ++g_order; thing = t; } };
static CQuestManager g_quests; static CThingManager g_things;
static CQuestManager* g_pQuestManager = &g_quests;
static CThingManager* g_pThingManager = &g_things;
struct CThing {
    SComponentPair* m_componentsFirst;
    SComponentPair* m_componentsLast;
    unsigned char   low : 4;
    unsigned char   LevelIsUnloading : 1;
    unsigned char   IsAffectedByBulletTimeOverride : 1;
    unsigned char   AddingTCsPermitted : 1;
    void OnKill()
    {
        g_pQuestManager->OnDeactivateEntity(this, LevelIsUnloading);
        AddingTCsPermitted = false;
        for_each(m_componentsFirst, m_componentsLast, CCallOnKill());
        AddingTCsPermitted = true;
        g_pThingManager->OnThingKill(this);
    }
};
void CTCBase::OnKill() { killedAt = ++g_order; addsAllowed = owner->AddingTCsPermitted != 0; }
int main()
{
    CThing t; t.low = 3; t.LevelIsUnloading = 1; t.IsAffectedByBulletTimeOverride = 0; t.AddingTCsPermitted = 1;
    CTCBase a = { 0, true, &t }, b = { 0, true, &t };
    SComponentPair v[2] = { { 1, &a }, { 2, &b } };
    t.m_componentsFirst = v; t.m_componentsLast = v + 2;
    t.OnKill();
    if (g_quests.at != 1 || g_quests.thing != &t || !g_quests.unloading || a.killedAt != 2 || b.killedAt != 3 ||
        a.addsAllowed || b.addsAllowed || g_things.at != 4 || g_things.thing != &t || !t.AddingTCsPermitted || t.low != 3) {
        std::printf("004c7cf0_TEST FAIL\n"); return 1;
    }
    std::printf("004c7cf0_TEST PASS\n");
    return 0;
}