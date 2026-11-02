// Test: CModeDragonFireBeam::OnDeactivate() kills the beam, resumes the AI of an AI creature (SuspendAI(false, this)), then the stand-mode OnDeactivate.
#include <cstdio>
static int g_kill = 0, g_resume = 0, g_stand = 0;
static void* g_suspender = 0;
class CThing { public:
    unsigned char Type;
    unsigned long GetType() const { return Type; }
};
class CThingAICreature : public CThing { public:
    void SuspendAI(bool suspend, void* suspender) { if (!suspend) ++g_resume; g_suspender = suspender; }
};
class CModeStandBase { public:
    virtual ~CModeStandBase() { }
    virtual void OnDeactivate() { ++g_stand; }
    CThing* ParentThing;
};
class CModeDragonFireBeam : public CModeStandBase { public:
    virtual void OnDeactivate();
private:
    void KillBeamEffect() { ++g_kill; }
};
void CModeDragonFireBeam::OnDeactivate()
{
    KillBeamEffect();
    if (ParentThing->GetType() == 1)
        static_cast<CThingAICreature*>(ParentThing)->SuspendAI(false, this);
    CModeStandBase::OnDeactivate();
}
int main()
{
    CThingAICreature dragon;
    dragon.Type = 1;
    CModeDragonFireBeam mode;
    mode.ParentThing = &dragon;
    mode.OnDeactivate();
    if (g_kill != 1 || g_resume != 1 || g_suspender != &mode || g_stand != 1) { std::printf("008dbb00_TEST FAIL\n"); return 1; }
    dragon.Type = 2;
    mode.OnDeactivate();
    if (g_kill != 2 || g_resume != 1 || g_stand != 2) { std::printf("008dbb00_TEST FAIL\n"); return 1; }
    std::printf("008dbb00_TEST PASS\n");
    return 0;
}