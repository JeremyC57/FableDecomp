// Behaviour test for 0x00A9DEF0 C3DAnimationManager::IsAnimationLoaded(const C3DAnimationStateBase&) const
// Model: a transition state is loaded when both its start and end states are; a plain state asks the mesh bank's animation info.
#include <cstdio>
class C3DAnimationInfo { public: bool Loaded; bool IsLoaded() const { return Loaded; } };
class CMeshDataBank { public: C3DAnimationInfo Infos[4]; const C3DAnimationInfo* PeekAnimation(unsigned long i) const { return &Infos[i]; } };
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
    bool IsAnimationLoaded(const C3DAnimationStateBase& state) const
    {
        if (state.GetType() == C3DAnimationStateBase::STATE_TRANSITION) {
            const C3DAnimationTransitionState& t = static_cast<const C3DAnimationTransitionState&>(state);
            return IsAnimationLoaded(*t.StartState) && IsAnimationLoaded(*t.EndState);
        }
        const C3DAnimationState& a = static_cast<const C3DAnimationState&>(state);
        return MeshBank->PeekAnimation(a.AnimationIndex)->IsLoaded();
    }
};
int main()
{
    CMeshDataBank bank;
    bank.Infos[0].Loaded = true; bank.Infos[1].Loaded = false; bank.Infos[2].Loaded = true; bank.Infos[3].Loaded = true;
    C3DAnimationManager m; m.MeshBank = &bank;
    C3DAnimationState s0, s1, s2; s0.AnimationIndex = 0; s1.AnimationIndex = 1; s2.AnimationIndex = 2;
    C3DAnimationTransitionState t; t.StartState = &s0; t.EndState = &s2;
    C3DAnimationTransitionState u; u.StartState = &t; u.EndState = &s1;
    if (!m.IsAnimationLoaded(s0) || m.IsAnimationLoaded(s1)) { std::printf("00a9def0_TEST FAIL\n"); return 1; }
    if (!m.IsAnimationLoaded(t) || m.IsAnimationLoaded(u)) { std::printf("00a9def0_TEST FAIL\n"); return 1; }
    std::printf("00a9def0_TEST PASS\n");
    return 0;
}