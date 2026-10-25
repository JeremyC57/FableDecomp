// Behaviour test (0x00924720): CCreatureAction_KrakenLowChargeLoop::FinishAction runs the base FinishAction, then the next charge loop while the target lives and the kraken was not hurt in phase 2, the low attack once the def's loop count is reached, else the "out of" action
#include <cstdio>
enum { ACTION_NONE, ACTION_CHARGE_LOOP, ACTION_ATTACK_INTO, ACTION_ATTACK_OUT_OF };
struct CKrakenDef { int LowChargeLoops; };
struct CTCKraken { CKrakenDef* Def; int Phase; };
struct CCreature { float m_health; int NextAction; int NextLoop; void* NextTarget; };
struct CAction {
    CCreature* PCreature; void* PTarget; bool TargetAlive; CTCKraken* Kraken; int LoopNum; float StartHealth; int BaseCalls;
    void BaseFinishAction() { ++BaseCalls; }
    void FinishAction()
    {
        BaseFinishAction();
        CTCKraken* kraken = Kraken;
        if (TargetAlive && !(kraken->Phase == 2 && PCreature->m_health < StartHealth)) {
            ++LoopNum;
            if (LoopNum < kraken->Def->LowChargeLoops) {
                PCreature->NextAction = ACTION_CHARGE_LOOP; PCreature->NextTarget = PTarget; PCreature->NextLoop = LoopNum;
            } else {
                PCreature->NextAction = ACTION_ATTACK_INTO; PCreature->NextTarget = PTarget;
            }
        } else
            PCreature->NextAction = ACTION_ATTACK_OUT_OF;
    }
};
int main()
{
    CKrakenDef def; def.LowChargeLoops = 3;
    CTCKraken kraken; kraken.Def = &def; kraken.Phase = 1;
    CCreature creature; creature.m_health = 50.0f; creature.NextAction = ACTION_NONE; creature.NextLoop = -1; creature.NextTarget = 0;
    int target = 0;
    CAction a; a.PCreature = &creature; a.PTarget = &target; a.TargetAlive = true; a.Kraken = &kraken; a.LoopNum = 0; a.StartHealth = 100.0f; a.BaseCalls = 0;
    a.FinishAction();
    if (a.BaseCalls != 1 || a.LoopNum != 1 || creature.NextAction != ACTION_CHARGE_LOOP || creature.NextLoop != 1 || creature.NextTarget != &target) { std::printf("00924720_TEST FAIL\n"); return 1; }
    a.LoopNum = 2; a.FinishAction();
    if (creature.NextAction != ACTION_ATTACK_INTO || a.LoopNum != 3) { std::printf("00924720_TEST FAIL\n"); return 1; }
    kraken.Phase = 2; a.LoopNum = 0; a.FinishAction();                       // hurt during phase 2
    if (creature.NextAction != ACTION_ATTACK_OUT_OF || a.LoopNum != 0) { std::printf("00924720_TEST FAIL\n"); return 1; }
    creature.m_health = 100.0f; a.FinishAction();                            // phase 2 but not hurt
    if (creature.NextAction != ACTION_CHARGE_LOOP || a.LoopNum != 1) { std::printf("00924720_TEST FAIL\n"); return 1; }
    a.TargetAlive = false; a.FinishAction();
    if (creature.NextAction != ACTION_ATTACK_OUT_OF || a.LoopNum != 1 || a.BaseCalls != 5) { std::printf("00924720_TEST FAIL\n"); return 1; }
    std::printf("00924720_TEST PASS\n");
    return 0;
}