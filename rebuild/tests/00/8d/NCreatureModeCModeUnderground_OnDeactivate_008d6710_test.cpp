// Test: CModeUnderground::OnDeactivate() clears ImmuneToAttack on an AI creature, runs the stand-mode OnDeactivate, then restores the saved targeting flags.
#include <cstdio>
static int g_standDeactivate = 0, g_revert = 0;
static void* g_revertThing = 0;
static long g_revertFlags = 0;
class CThing { public:
    unsigned long ThingTypeFlags;
};
class CThingAICreature : public CThing { public:
    bool ImmuneToAttack;
    void SetAsImmuneToAttack(bool immune) { ImmuneToAttack = immune; }
};
static void RevertOldTargetFlags(CThing& thing, long flags) { ++g_revert; g_revertThing = &thing; g_revertFlags = flags; }
class CModeStandBase { public:
    virtual ~CModeStandBase() { }
    virtual void OnDeactivate() { ++g_standDeactivate; }
    CThing* ParentThing;
};
class CModeUnderground : public CModeStandBase { public:
    long OldTargetFlags;
    virtual void OnDeactivate();
};
void CModeUnderground::OnDeactivate()
{
    if (ParentThing->ThingTypeFlags & 0x10)
        static_cast<CThingAICreature*>(ParentThing)->SetAsImmuneToAttack(false);
    CModeStandBase::OnDeactivate();
    RevertOldTargetFlags(*ParentThing, OldTargetFlags);
}
int main()
{
    CThingAICreature creature;
    creature.ThingTypeFlags = 0x10;
    creature.ImmuneToAttack = true;
    CModeUnderground mode;
    mode.ParentThing = &creature;
    mode.OldTargetFlags = 0x55;
    mode.OnDeactivate();
    if (creature.ImmuneToAttack || g_standDeactivate != 1 || g_revert != 1 || g_revertThing != &creature || g_revertFlags != 0x55) {
        std::printf("008d6710_TEST FAIL\n"); return 1;
    }
    creature.ThingTypeFlags = 0x08;
    creature.ImmuneToAttack = true;
    mode.OnDeactivate();
    if (!creature.ImmuneToAttack || g_standDeactivate != 2 || g_revert != 2) { std::printf("008d6710_TEST FAIL\n"); return 1; }
    std::printf("008d6710_TEST PASS\n");
    return 0;
}