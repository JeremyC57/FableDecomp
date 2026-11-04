// Behaviour test for C3DAnimationTransitionState::FindFirstAnimation 0x00710530: a transition's first animation is its start state's
// first animation (virtual, so a nested transition recurses to its own start state).
#include <cstdio>
struct C3DAnimationState;
struct C3DAnimationStateBase { virtual ~C3DAnimationStateBase() {} virtual const C3DAnimationState& FindFirstAnimation() const = 0; };
struct C3DAnimationState : C3DAnimationStateBase { long AnimationIndex; const C3DAnimationState& FindFirstAnimation() const { return *this; } };
struct C3DAnimationTransitionState : C3DAnimationStateBase {
    C3DAnimationStateBase* StartState; C3DAnimationStateBase* EndState;
    const C3DAnimationState& FindFirstAnimation() const { return StartState->FindFirstAnimation(); }
};
int main()
{
    C3DAnimationState a1, a2, a3; a1.AnimationIndex = 1; a2.AnimationIndex = 2; a3.AnimationIndex = 3;
    C3DAnimationTransitionState inner; inner.StartState = &a1; inner.EndState = &a2;
    C3DAnimationTransitionState outer; outer.StartState = &inner; outer.EndState = &a3;
    if (&inner.FindFirstAnimation() != &a1 || &outer.FindFirstAnimation() != &a1) { std::printf("00710530_TEST FAIL\n"); return 1; }
    std::printf("00710530_TEST PASS\n");
    return 0;
}