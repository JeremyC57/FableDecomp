#include <cstdio>
// Behaviour test for CCreatureAction_CastStrengthMultiStrike::FrameUpdate (0x008E6390): the base CCombatActionBase::FrameUpdate runs
// first, then FinishAction runs iff IsActionReadyForInterruption() is true. Standalone model of that control flow (the row's retail
// class needs the whole CCreatureActionBase vtable to link, so the test does not embed it).
struct ActionModel {
    int baseCalls, finishCalls, ready, order;
    ActionModel() : baseCalls(0), finishCalls(0), ready(0), order(0) { }
    void BaseFrameUpdate() { ++baseCalls; order = order * 10 + 1; }
    virtual bool IsActionReadyForInterruption() const { return ready != 0; }
    virtual void FinishAction() { ++finishCalls; order = order * 10 + 2; }
    void FrameUpdate()
    {
        BaseFrameUpdate();
        if (IsActionReadyForInterruption())
            FinishAction();
    }
};

int main() {
    ActionModel a;
    a.FrameUpdate();
    bool falseBranch = a.baseCalls == 1 && a.finishCalls == 0 && a.order == 1;

    ActionModel b;
    b.ready = 1;
    b.FrameUpdate();
    bool trueBranch = b.baseCalls == 1 && b.finishCalls == 1 && b.order == 12;

    if (falseBranch && trueBranch) {
        std::printf("MULTISTRIKE_FRAMEUPDATE_OK\n");
        return 0;
    }
    return 1;
}
