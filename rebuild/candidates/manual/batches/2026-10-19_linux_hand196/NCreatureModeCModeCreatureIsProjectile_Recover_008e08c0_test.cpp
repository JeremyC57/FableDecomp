// behaviour copy of NCreatureMode::CModeCreatureIsProjectile::Recover
#include <cstdio>
static int g_suspend = -1, g_enable = -1, g_wake = 0, g_actions = 0, g_removed = -1, g_dtor = 0; static bool g_flip = true;
struct CThingCreatureBase;
struct CCombatActionBase { CThingCreatureBase* owner; ~CCombatActionBase() { ++g_dtor; } };
struct CRecover : CCombatActionBase { bool CanBeHit, Flip;
    CRecover(CThingCreatureBase* o, bool flip) { owner = o; CanBeHit = false; Flip = flip; } };
struct CPhysics { void SetEnabled(bool e) { g_enable = e; } void Wake() { ++g_wake; } };
struct CTCCreatureModeManager { void RemoveMode(unsigned long m) { g_removed = (int)m; } };
struct CThingCreatureBase {
    unsigned char m_state; CPhysics* PPhysics; CTCCreatureModeManager modes;
    void SetSuspendAnimation(bool s, long) { g_suspend = s; }
    bool SetCurrentAction(const CCombatActionBase& a) { ++g_actions; g_flip = static_cast<const CRecover&>(a).Flip; return a.owner == this; }
    bool GetInterface(CTCCreatureModeManager** out, long type) { if (type == 0x31) { *out = &modes; return true; } return false; }
};
struct CMode {
    CThingCreatureBase* PCreature;
    void Recover()
    {
        CThingCreatureBase* creature = PCreature;
        if (creature->m_state == 1) {
            creature->SetSuspendAnimation(false, 0);
            creature->PPhysics->SetEnabled(true);
            creature->PPhysics->Wake();
        }
        PCreature->SetCurrentAction(CRecover(PCreature, false));
        CTCCreatureModeManager* modes;
        PCreature->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x39);
    }
};
int main()
{
    CPhysics p; CThingCreatureBase c; c.m_state = 1; c.PPhysics = &p;
    CMode m; m.PCreature = &c; m.Recover();
    if (g_suspend != 0 || g_enable != 1 || g_wake != 1 || g_actions != 1 || g_flip || g_dtor != 1 || g_removed != 0x39)
    { std::printf("008e08c0_TEST FAIL\n"); return 1; }
    std::printf("008e08c0_TEST PASS\n");
    return 0;
}
