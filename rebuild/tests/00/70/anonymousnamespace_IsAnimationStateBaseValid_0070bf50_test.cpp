// Behaviour test for `anonymous namespace'::IsAnimationStateBaseValid 0x0070BF50 (model copy of the source logic).
#include <cstdio>
struct Info { bool loaded; bool IsLoaded() const { return loaded; } };
struct Bank {
    Info infos[4];
    const Info* PeekAnimation(unsigned long index) const { return &infos[index]; }
};
struct StateBase {
    virtual ~StateBase() {}
    virtual int GetType() const = 0;
};
struct AnimState : StateBase { long AnimationIndex; int GetType() const { return 0; } };
struct AlphaState : AnimState { float AlphaWeight; int GetType() const { return 1; } };
struct TransitionState : StateBase {
    StateBase* StartState; StateBase* EndState;
    int GetType() const { return 2; }
};
struct OtherState : StateBase { int GetType() const { return 7; } };

static bool IsAnimationStateBaseValid(const Bank& bank, const StateBase& state)
{
    if (state.GetType() == 0) {
        const unsigned long index = static_cast<const AnimState&>(state).AnimationIndex;
        if (!bank.PeekAnimation(index)->IsLoaded())
            return false;
    }
    else if (state.GetType() == 1) {
        const unsigned long index = static_cast<const AlphaState&>(state).AnimationIndex;
        if (!bank.PeekAnimation(index)->IsLoaded())
            return false;
    }
    else if (state.GetType() == 2) {
        const TransitionState* transition = static_cast<const TransitionState*>(&state);
        if (!IsAnimationStateBaseValid(bank, *transition->StartState))
            return false;
        if (!IsAnimationStateBaseValid(bank, *transition->EndState))
            return false;
    }
    return true;
}

int main()
{
    Bank bank;
    bank.infos[0].loaded = true; bank.infos[1].loaded = false; bank.infos[2].loaded = true; bank.infos[3].loaded = false;
    AnimState a0; a0.AnimationIndex = 0;
    AnimState a1; a1.AnimationIndex = 1;
    AlphaState b2; b2.AnimationIndex = 2; b2.AlphaWeight = 0.5f;
    AlphaState b3; b3.AnimationIndex = 3; b3.AlphaWeight = 0.5f;
    OtherState other;
    TransitionState t1; t1.StartState = &a0; t1.EndState = &b2;
    TransitionState t2; t2.StartState = &a0; t2.EndState = &b3;
    TransitionState t3; t3.StartState = &a1; t3.EndState = &b2;
    TransitionState t4; t4.StartState = &t1; t4.EndState = &other;
    TransitionState t5; t5.StartState = &t1; t5.EndState = &t2;
    bool ok = IsAnimationStateBaseValid(bank, a0) && !IsAnimationStateBaseValid(bank, a1)
           && IsAnimationStateBaseValid(bank, b2) && !IsAnimationStateBaseValid(bank, b3)
           && IsAnimationStateBaseValid(bank, other)
           && IsAnimationStateBaseValid(bank, t1) && !IsAnimationStateBaseValid(bank, t2) && !IsAnimationStateBaseValid(bank, t3)
           && IsAnimationStateBaseValid(bank, t4) && !IsAnimationStateBaseValid(bank, t5);
    if (!ok) { std::printf("0070bf50_TEST FAIL\n"); return 1; }
    std::printf("0070bf50_TEST PASS\n");
    return 0;
}