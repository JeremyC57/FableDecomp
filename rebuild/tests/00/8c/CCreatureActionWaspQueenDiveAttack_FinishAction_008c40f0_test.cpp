// Behaviour test for CCreatureAction_WaspQueenDiveAttack::FinishAction (0x008C40F0): the hit-on-collision component (200) is
// removed; a dive that ran its full time turns the queen by 0.5 in XY and resets interpolation; the base FinishAction always runs.
#include <stdio.h>
static int g_removed = -1, g_resets = 0, g_baseFinish = 0;
class CTCPhysicsBase { public:
    float Angle;
    float GetFacingAngleXY() const { return Angle; }
    void SetFacingAngleXY(float a) { Angle = a; }
    void ResetInterpolation() { g_resets++; }
};
class CThingCreatureBase { public:
    CTCPhysicsBase* Physics;
    CTCPhysicsBase* GetPhysicsTC() const { return Physics; }
    void RemoveTC(int type) { g_removed = type; }
};
class CCombatActionBase { public:
    CThingCreatureBase* PCreature;
    long FramesRunning, TotalTime;
    void FinishAction() { g_baseFinish++; }
};
class CCreatureAction_WaspQueenDiveAttack : public CCombatActionBase { public:
    void FinishAction()
    {
        PCreature->RemoveTC(200);
        if (FramesRunning >= TotalTime) {
            float angle = PCreature->GetPhysicsTC()->GetFacingAngleXY();
            PCreature->GetPhysicsTC()->SetFacingAngleXY(angle + 0.5f);
            PCreature->GetPhysicsTC()->ResetInterpolation();
        }
        CCombatActionBase::FinishAction();
    }
};
int main()
{
    CTCPhysicsBase phys; phys.Angle = 1.0f;
    CThingCreatureBase queen; queen.Physics = &phys;
    CCreatureAction_WaspQueenDiveAttack dive; dive.PCreature = &queen; dive.FramesRunning = 10; dive.TotalTime = 10;
    dive.FinishAction();
    if (g_removed != 200 || phys.Angle != 1.5f || g_resets != 1 || g_baseFinish != 1) { printf("008c40f0_TEST FAIL\n"); return 1; }
    g_removed = -1; dive.FramesRunning = 9;            // cut short: no turn
    dive.FinishAction();
    if (g_removed != 200 || phys.Angle != 1.5f || g_resets != 1 || g_baseFinish != 2) { printf("008c40f0_TEST FAIL\n"); return 1; }
    printf("008c40f0_TEST PASS\n");
    return 0;
}