// Behaviour test for CThing::OnLevelUnload @ 0x004C77D0: flags around the loop, live TCs notified with adds forbidden.
#include <cstdio>
class CThing;
class CTCBase { public:
    bool ToBeRemoved; int calls; bool addsAllowedDuringCall; CThing* owner;
    CTCBase(bool removed, CThing* o) : ToBeRemoved(removed), calls(0), addsAllowedDuringCall(true), owner(o) { }
    virtual void OnLevelUnload();
};
struct SComponentPair { long first; CTCBase* second; };
class CThing { public:
    SComponentPair* m_componentsFirst;
    SComponentPair* m_componentsLast;
    unsigned char   NotPausedDuringCutscenes : 1;
    unsigned char   DeathPersistent : 1;
    unsigned char   KillOnLevelUnload : 1;
    unsigned char   ActivatedScript : 1;
    unsigned char   LevelIsUnloading : 1;
    unsigned char   IsAffectedByBulletTimeOverride : 1;
    unsigned char   AddingTCsPermitted : 1;
    void OnLevelUnload()
    {
        LevelIsUnloading = true;
        AddingTCsPermitted = false;
        for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
            CTCBase* tc = it->second;
            if (!tc->ToBeRemoved)
                tc->OnLevelUnload();
        }
        AddingTCsPermitted = true;
    }
};
void CTCBase::OnLevelUnload() { ++calls; addsAllowedDuringCall = owner->AddingTCsPermitted != 0; }
int main()
{
    CThing t; t.NotPausedDuringCutscenes = 1; t.DeathPersistent = 0; t.KillOnLevelUnload = 1; t.ActivatedScript = 0;
    t.LevelIsUnloading = 0; t.IsAffectedByBulletTimeOverride = 1; t.AddingTCsPermitted = 1;
    CTCBase a(false, &t), b(true, &t);
    SComponentPair v[2] = { { 1, &a }, { 2, &b } };
    t.m_componentsFirst = v; t.m_componentsLast = v + 2;
    t.OnLevelUnload();
    if (a.calls != 1 || a.addsAllowedDuringCall || b.calls != 0 || !t.LevelIsUnloading || !t.AddingTCsPermitted ||
        !t.NotPausedDuringCutscenes || !t.KillOnLevelUnload || !t.IsAffectedByBulletTimeOverride) {
        std::printf("004c77d0_TEST FAIL\n"); return 1;
    }
    std::printf("004c77d0_TEST PASS\n");
    return 0;
}