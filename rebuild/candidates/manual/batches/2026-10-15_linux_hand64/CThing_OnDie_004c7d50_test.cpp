#include <cstdio>
// Behaviour of CThing::OnDie (0x004C7D50): AddingTCsPermitted is cleared while every component's OnDie runs
// (for_each over the component map), then set again; the neighbouring flag bits are untouched.
struct CTCBase {
    int m_died; bool m_addingAllowedWhenCalled;
};
struct SComponentPair { long first; CTCBase* second; };
struct CThing;
static CThing* g_thing = 0;

struct CThing {
    unsigned char   m_pad00[0x44];
    SComponentPair* m_componentsFirst;
    SComponentPair* m_componentsLast;
    SComponentPair* m_componentsEnd;
    unsigned char   m_pad50[0x43];
    unsigned char   NotPausedDuringCutscenes : 1;
    unsigned char   DeathPersistent : 1;
    unsigned char   KillOnLevelUnload : 1;
    unsigned char   ActivatedScript : 1;
    unsigned char   LevelIsUnloading : 1;
    unsigned char   IsAffectedByBulletTimeOverride : 1;
    unsigned char   AddingTCsPermitted : 1;
    void OnDie();
};

struct CCallOnDie {
    void operator()(SComponentPair& p) const { p.second->m_died++; p.second->m_addingAllowedWhenCalled = g_thing->AddingTCsPermitted != 0; }
};

static CCallOnDie for_each(SComponentPair* first, SComponentPair* last, CCallOnDie f)
{
    for (; first != last; ++first) f(*first);
    return f;
}

void CThing::OnDie()
{
    AddingTCsPermitted = false;
    for_each(m_componentsFirst, m_componentsLast, CCallOnDie());
    AddingTCsPermitted = true;
}

int main()
{
    CTCBase tcs[2]; SComponentPair pairs[2];
    for (int i = 0; i < 2; ++i) { tcs[i].m_died = 0; tcs[i].m_addingAllowedWhenCalled = true; pairs[i].first = i; pairs[i].second = &tcs[i]; }
    CThing t; g_thing = &t;
    t.m_componentsFirst = pairs; t.m_componentsLast = pairs + 2; t.m_componentsEnd = pairs + 2;
    t.NotPausedDuringCutscenes = 1; t.DeathPersistent = 0; t.KillOnLevelUnload = 1; t.ActivatedScript = 0;
    t.LevelIsUnloading = 1; t.IsAffectedByBulletTimeOverride = 0; t.AddingTCsPermitted = 1;
    t.OnDie();
    if (tcs[0].m_died != 1 || tcs[1].m_died != 1 || tcs[0].m_addingAllowedWhenCalled || tcs[1].m_addingAllowedWhenCalled) { std::printf("004c7d50_TEST FAIL\n"); return 1; }
    if (!t.AddingTCsPermitted || !t.NotPausedDuringCutscenes || t.DeathPersistent || !t.KillOnLevelUnload || !t.LevelIsUnloading) { std::printf("004c7d50_TEST FAIL\n"); return 1; }
    std::printf("004c7d50_TEST PASS\n");
    return 0;
}
