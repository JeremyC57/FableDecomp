// Behaviour test for NCreatureMode::CModeGuildmasterPowerUpHeal::OnDeactivate 0x008D69A0: a type-1 (AI creature) parent gets
// SuspendAI(false, this); every parent then gets the stand-mode OnDeactivate
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CThing {
public:
    CThing(unsigned char type) : Type(type), Suspender(0), Suspended(true) {}
    unsigned long GetType() const { return Type; }
    unsigned char Type;
    void* Suspender;
    bool Suspended;
};
class CThingAICreature : public CThing {
public:
    CThingAICreature() : CThing(1) {}
    void SuspendAI(bool suspend, void* suspender) { Suspended = suspend; Suspender = suspender; g_order[g_count++] = 1; }
};
class CModeBase {
public:
    CModeBase(CThing* thing) : ParentThing(thing) {}
    virtual ~CModeBase() {}
    virtual void OnDeactivate() {}
    CThing* ParentThing;
};
class CModeStandBase : public CModeBase {
public:
    CModeStandBase(CThing* thing) : CModeBase(thing) {}
    virtual void OnDeactivate() { g_order[g_count++] = 2; }
};
class CModeGuildmasterPowerUpHeal : public CModeStandBase {
public:
    CModeGuildmasterPowerUpHeal(CThing* thing) : CModeStandBase(thing) {}
    virtual void OnDeactivate()
    {
        if (ParentThing->GetType() == 1)
            static_cast<CThingAICreature*>(ParentThing)->SuspendAI(false, this);
        CModeStandBase::OnDeactivate();
    }
};
int main()
{
    CThingAICreature creature;
    CModeGuildmasterPowerUpHeal mode(&creature);
    CModeBase* base = &mode;
    base->OnDeactivate();
    if (g_count != 2 || g_order[0] != 1 || g_order[1] != 2 || creature.Suspended || creature.Suspender != &mode) {
        std::printf("008d69a0_TEST FAIL\n");
        return 1;
    }
    CThing other(3);
    CModeGuildmasterPowerUpHeal otherMode(&other);
    otherMode.OnDeactivate();
    if (g_count != 3 || g_order[2] != 2 || !other.Suspended) { std::printf("008d69a0_TEST FAIL\n"); return 1; }
    std::printf("008d69a0_TEST PASS\n");
    return 0;
}