// Behaviour test for CAnimSet::GetLastMeshAnim (0x0070CAD0): the counted-pointer / animation-state model and the body
// verbatim from final.cpp; the out-of-line C3DAnimationTransitionState constructor and the Delete virtuals are stubbed so
// the ownership transfer is observable. Checks the one-anim case (a C3DAnimationState with the set's LastPercentageComplete),
// the two-anim case (a transition holding both states and InternalAlphas[1].LastAlpha), the reference counts after the
// temporaries and locals die, and that dropping the returned pointer deletes every state exactly once.
#include <stdio.h>

class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);       // slot 0
    virtual void Delete();                 // slot 1
    unsigned long IVRefCount;              // +4
    CIVCountedPointeeBase() : IVRefCount(1) { }
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    template<class T> CIVCountedPointer(const T& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

template<class T> struct CIVTypedPointer {
    T* Object;
    CIVTypedPointer(T* object) : Object(object) { }
    template<class U> CIVTypedPointer(const CIVTypedPointer<U>& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVTypedPointer()
    {
        if (Object && --Object->IVRefCount == 0)
            Object->Delete();
    }
};

class C3DAnimationStateBase : public CIVCountedPointeeBase { public:
    C3DAnimationStateBase() { }
};

class C3DAnimationState : public C3DAnimationStateBase { public:     // vtable 0x01249F50
    long  AnimationIndex;                  // +8
    float AnimationTime;                   // +0xc
    C3DAnimationState(long index, float time) : AnimationIndex(index), AnimationTime(time) { }
    virtual void Destroy(int flags);
    virtual void Delete();
};

class C3DAnimationTransitionState : public C3DAnimationStateBase { public:
    CIVCountedPointer StartState;
    CIVCountedPointer EndState;
    float Progress;
    C3DAnimationTransitionState(const CIVCountedPointer& start, const CIVCountedPointer& end, float progress);   // 0x00A3BE70
};

struct CShortVector {
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
};
class CTransitionBlendAlpha { public: float CurrentAlpha; float LastAlpha; };
struct CAlphaVector {
    CTransitionBlendAlpha* _M_start; CTransitionBlendAlpha* _M_finish; CTransitionBlendAlpha* _M_end_of_storage;
    const CTransitionBlendAlpha* begin() const { return _M_start; }
    const CTransitionBlendAlpha& operator[](unsigned long n) const { return begin()[n]; }
};

class CAnimSet
{
public:
    long UpdateMode;
    CShortVector Anims;
    CAlphaVector InternalAlphas;
    float CurrentPercentageComplete;
    float LastPercentageComplete;
    CIVTypedPointer<C3DAnimationStateBase> GetLastMeshAnim() const;
};

CIVTypedPointer<C3DAnimationStateBase> CAnimSet::GetLastMeshAnim() const
{
    CIVTypedPointer<C3DAnimationState> start(new C3DAnimationState(Anims._M_start[0], LastPercentageComplete));
    if (Anims.size() == 1)
        return start;
    CIVTypedPointer<C3DAnimationState> end(new C3DAnimationState(Anims._M_start[1], LastPercentageComplete));
    CIVTypedPointer<C3DAnimationTransitionState> transition(new C3DAnimationTransitionState(start, end, InternalAlphas[1].LastAlpha));
    return transition;
}

// ---- stubs ----
static int g_stateDeletes = 0;
static int g_transitionDeletes = 0;
static int g_transitionCtors = 0;
void C3DAnimationState::Destroy(int) { }
void C3DAnimationState::Delete() { g_stateDeletes++; delete this; }
void CIVCountedPointeeBase::Destroy(int) { }
void CIVCountedPointeeBase::Delete() { g_transitionDeletes++; delete (C3DAnimationTransitionState*)this; }   // only transitions reach the base Delete here
C3DAnimationTransitionState::C3DAnimationTransitionState(const CIVCountedPointer& start, const CIVCountedPointer& end, float progress)
    : StartState(start), EndState(end), Progress(progress)
{
    // The landed 0x00A3BE70 takes its own reference on both states (the members are copied by the implicit copy ctor).
    if (StartState.Object) ++StartState.Object->IVRefCount;
    if (EndState.Object) ++EndState.Object->IVRefCount;
    g_transitionCtors++;
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

int main()
{
    short anims[2] = { 7, 9 };
    CTransitionBlendAlpha alphas[2];
    alphas[0].CurrentAlpha = 0.1f; alphas[0].LastAlpha = 0.2f;
    alphas[1].CurrentAlpha = 0.3f; alphas[1].LastAlpha = 0.4f;
    CAnimSet set;
    set.UpdateMode = 0;
    set.Anims._M_start = anims; set.Anims._M_finish = anims + 1; set.Anims._M_end_of_storage = anims + 2;
    set.InternalAlphas._M_start = alphas; set.InternalAlphas._M_finish = alphas + 2; set.InternalAlphas._M_end_of_storage = alphas + 2;
    set.CurrentPercentageComplete = 0.5f;
    set.LastPercentageComplete = 0.75f;

    // One anim: a plain animation state at the set's LastPercentageComplete, owned only by the returned pointer.
    {
        CIVTypedPointer<C3DAnimationStateBase> result = set.GetLastMeshAnim();
        C3DAnimationState* state = (C3DAnimationState*)result.Object;
        CHECK(state != 0 && state->AnimationIndex == 7 && state->AnimationTime == set.LastPercentageComplete);
        CHECK(state->GetRefCount() == 1 && g_transitionCtors == 0);
    }
    CHECK(g_stateDeletes == 1 && g_transitionDeletes == 0);

    // Two anims: a transition of both states at InternalAlphas[1].LastAlpha; each state is held once by the transition.
    set.Anims._M_finish = anims + 2;
    {
        CIVTypedPointer<C3DAnimationStateBase> result = set.GetLastMeshAnim();
        C3DAnimationTransitionState* transition = (C3DAnimationTransitionState*)result.Object;
        CHECK(transition != 0 && g_transitionCtors == 1 && transition->GetRefCount() == 1);
        CHECK(transition->Progress == alphas[1].LastAlpha);
        C3DAnimationState* start = (C3DAnimationState*)transition->StartState.Object;
        C3DAnimationState* end = (C3DAnimationState*)transition->EndState.Object;
        CHECK(start != 0 && start->AnimationIndex == 7 && start->AnimationTime == set.LastPercentageComplete && start->GetRefCount() == 1);
        CHECK(end != 0 && end->AnimationIndex == 9 && end->AnimationTime == set.LastPercentageComplete && end->GetRefCount() == 1);
        CHECK(g_stateDeletes == 1);
    }
    CHECK(g_transitionDeletes == 1 && g_stateDeletes == 3);

    printf("0070cad0_TEST %s\n", g_fail ? "FAIL" : "PASS");
    return g_fail ? 1 : 0;
}