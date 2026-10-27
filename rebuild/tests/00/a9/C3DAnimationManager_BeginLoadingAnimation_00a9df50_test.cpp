// Behaviour test for 0x00A9DF50 C3DAnimationManager::BeginLoadingAnimation(const C3DAnimationStateBase&)
// Model: a transition starts loading both its start and end states (recursively); a plain state starts its animation (wait = false).
#include <cstdio>
class C3DAnimationInfo { public:
    int Calls; bool LastWait;
    void BeginLoading(bool wait) const { C3DAnimationInfo* self = const_cast<C3DAnimationInfo*>(this); ++self->Calls; self->LastWait = wait; }
};
class CMeshDataBank { public: C3DAnimationInfo Infos[3]; const C3DAnimationInfo* PeekAnimation(unsigned long i) const { return &Infos[i]; } };
class C3DAnimationStateBase {
public:
    enum EType { STATE_ANIMATION = 0, STATE_ALPHA = 1, STATE_TRANSITION = 2 };
    virtual ~C3DAnimationStateBase() { }
    virtual EType GetType() const = 0;
};
class C3DAnimationState : public C3DAnimationStateBase {
public:
    long AnimationIndex;
    virtual EType GetType() const { return STATE_ANIMATION; }
};
class C3DAnimationTransitionState : public C3DAnimationStateBase {
public:
    C3DAnimationStateBase* StartState;
    C3DAnimationStateBase* EndState;
    virtual EType GetType() const { return STATE_TRANSITION; }
};
class C3DAnimationManager { public:
    CMeshDataBank* MeshBank;
    void BeginLoadingAnimation(const C3DAnimationStateBase& state)
    {
        if (state.GetType() == C3DAnimationStateBase::STATE_TRANSITION) {
            const C3DAnimationTransitionState& t = static_cast<const C3DAnimationTransitionState&>(state);
            BeginLoadingAnimation(*t.StartState);
            BeginLoadingAnimation(*t.EndState);
        }
        else {
            const C3DAnimationState& a = static_cast<const C3DAnimationState&>(state);
            MeshBank->PeekAnimation(a.AnimationIndex)->BeginLoading(false);
        }
    }
};
int main()
{
    CMeshDataBank bank;
    for (int i = 0; i < 3; ++i) { bank.Infos[i].Calls = 0; bank.Infos[i].LastWait = true; }
    C3DAnimationManager m; m.MeshBank = &bank;
    C3DAnimationState s0, s2; s0.AnimationIndex = 0; s2.AnimationIndex = 2;
    C3DAnimationTransitionState t; t.StartState = &s0; t.EndState = &s2;
    C3DAnimationTransitionState u; u.StartState = &t; u.EndState = &s0;
    m.BeginLoadingAnimation(u);
    if (bank.Infos[0].Calls != 2 || bank.Infos[1].Calls != 0 || bank.Infos[2].Calls != 1) { std::printf("00a9df50_TEST FAIL\n"); return 1; }
    if (bank.Infos[0].LastWait || bank.Infos[2].LastWait) { std::printf("00a9df50_TEST FAIL\n"); return 1; }
    std::printf("00a9df50_TEST PASS\n");
    return 0;
}