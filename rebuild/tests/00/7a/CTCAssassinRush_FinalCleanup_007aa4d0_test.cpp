// Behaviour test for 0x007AA4D0 CTCAssassinRush::FinalCleanup.
#include <cstdio>

struct CTCBase {};
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
struct CThing {
    unsigned char State;
    unsigned int m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_stateFlags;
    int killed, suspendCalls;
    bool IsDead() const { return (m_stateFlags & 1) != 0; }
    void Kill(bool) { ++killed; }
    void SetSuspendAnimation(bool suspend, long reason) { if (!suspend && reason == 0) ++suspendCalls; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1U << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
static bool IS_THING_ALIVE(const CBaseIntelligentPointer& p) { if (p.GetPItem() != 0) { if (!p.GetPItem()->IsDead()) return true; } return false; }
static bool IS_THING_ALIVE(const CThing* t) { if (t != 0) { if (!t->IsDead()) return true; } return false; }
struct CTCSpecialAbilities : CTCBase { bool AbilityMutexLock; long CurrentAbility; };
static unsigned long g_removedKey = 0;
struct CBulletTimeManager { void RemoveBulletTimeSlowdown(unsigned long key) { g_removedKey = key; } };
struct CWorld { CBulletTimeManager* PBulletTimeManager; void RemoveBulletTimeSlowdown(unsigned long k) { PBulletTimeManager->RemoveBulletTimeSlowdown(k); } };
static CBulletTimeManager g_btm;
static CWorld g_world = { &g_btm };
struct CTCAssassinRush {
    CThing* ParentThing;
    CBaseIntelligentPointer AssassinTrail;
    CBaseIntelligentPointer AssassinDisplacementTrail;
    bool CompletedExitModes;
    CWorld* GetWorld() { return &g_world; }
    void FinalCleanup()
    {
        if (IS_THING_ALIVE(ParentThing)) {
            CTCSpecialAbilities* abilities;
            if (ParentThing->GetInterface(&abilities, 0xa7)) {
                if (abilities->CurrentAbility == 13) {
                    abilities->AbilityMutexLock = false;
                    abilities->CurrentAbility = 0;
                }
            }
            if (!CompletedExitModes) {
                if (ParentThing->State == 1)
                    ParentThing->SetSuspendAnimation(false, 0);
                else
                    GetWorld()->RemoveBulletTimeSlowdown((unsigned long)this);
            }
        }
        if (IS_THING_ALIVE(AssassinTrail))
            AssassinTrail.GetPItem()->Kill(true);
        if (IS_THING_ALIVE(AssassinDisplacementTrail))
            AssassinDisplacementTrail.GetPItem()->Kill(true);
    }
};

static void Reset(CThing& t) { t.State = 0; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_stateFlags = 0; t.killed = 0; t.suspendCalls = 0; t.m_interfaces.first = t.m_interfaces.last = 0; }

int main()
{
    CThing parent, trail, disp;
    Reset(parent); Reset(trail); Reset(disp);
    CTCSpecialAbilities sa; sa.AbilityMutexLock = true; sa.CurrentAbility = 13;
    CInterfaceMap::SEntry e[2]; e[0].type = 0xa7; e[0].component = &sa; e[1].type = 0; e[1].component = 0;
    parent.m_interfaces.first = e; parent.m_interfaces.last = e + 1; parent.m_presentBits[5] = 0x80;
    CTCAssassinRush r; r.ParentThing = &parent; r.AssassinTrail.m_pItem = &trail; r.AssassinDisplacementTrail.m_pItem = &disp; r.CompletedExitModes = false;
    // player-controlled parent: ability released, animation resumed, both trails killed
    parent.State = 1;
    r.FinalCleanup();
    if (sa.AbilityMutexLock || sa.CurrentAbility != 0 || parent.suspendCalls != 1 || g_removedKey != 0 || trail.killed != 1 || disp.killed != 1) { std::printf("007aa4d0_TEST FAIL\n"); return 1; }
    // other parent: bullet-time slowdown removed with this as key; dead trail not killed
    parent.State = 2; sa.AbilityMutexLock = true; sa.CurrentAbility = 4; disp.m_stateFlags = 1;
    r.FinalCleanup();
    if (!sa.AbilityMutexLock || sa.CurrentAbility != 4 || g_removedKey != (unsigned long)&r || trail.killed != 2 || disp.killed != 1) { std::printf("007aa4d0_TEST FAIL\n"); return 1; }
    // exit modes done and dead parent: nothing but the trails
    g_removedKey = 0; r.CompletedExitModes = true; r.AssassinTrail.m_pItem = 0;
    r.FinalCleanup();
    if (g_removedKey != 0 || parent.suspendCalls != 1 || trail.killed != 2) { std::printf("007aa4d0_TEST FAIL\n"); return 1; }
    parent.m_stateFlags = 1; r.CompletedExitModes = false; sa.CurrentAbility = 13;
    r.FinalCleanup();
    if (g_removedKey != 0 || sa.CurrentAbility != 13) { std::printf("007aa4d0_TEST FAIL\n"); return 1; }
    std::printf("007aa4d0_TEST PASS\n");
    return 0;
}