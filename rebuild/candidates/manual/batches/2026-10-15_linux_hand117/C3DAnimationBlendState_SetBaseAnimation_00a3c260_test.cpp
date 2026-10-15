#include <cstdio>
static int g_deleted = 0;
struct CIVCountedPointeeBase {
    unsigned long IVRefCount;
    virtual ~CIVCountedPointeeBase() {}
    virtual void Delete() { ++g_deleted; delete this; }
    void Release() { if (--IVRefCount == 0) Delete(); }
};
struct C3DAnimationStateBase : CIVCountedPointeeBase { };
struct C3DAnimationState : C3DAnimationStateBase { long AnimationIndex; float AnimationTime; };
template <class T>
struct CIVCountedPointer {
    T* m_pObject;
    CIVCountedPointer& operator=(T* object)
    {
        if (m_pObject) {
            m_pObject->Release();
            m_pObject = 0;
        }
        m_pObject = object;
        return *this;
    }
};
struct CSharedData { CIVCountedPointer<C3DAnimationStateBase> BaseAnimation; };
struct C3DAnimationBlendState {
    CSharedData* Data;
    int m_copies;
    void MakeOwnCopyOfData() { ++m_copies; }
    void SetBaseAnimation(const C3DAnimationState& state)
    {
        MakeOwnCopyOfData();
        Data->BaseAnimation = new C3DAnimationState(state);
    }
};
int main()
{
    C3DAnimationState a; a.IVRefCount = 1; a.AnimationIndex = 5; a.AnimationTime = 0.25f;
    CSharedData d; d.BaseAnimation.m_pObject = 0;
    C3DAnimationBlendState s; s.Data = &d; s.m_copies = 0;
    s.SetBaseAnimation(a);
    C3DAnimationState* first = (C3DAnimationState*)d.BaseAnimation.m_pObject;
    int ok = first && first->AnimationIndex == 5 && first->AnimationTime == 0.25f && first->IVRefCount == 1 && g_deleted == 0;
    a.AnimationIndex = 6;
    s.SetBaseAnimation(a);   // the old copy's count drops to zero -> Delete
    C3DAnimationState* second = (C3DAnimationState*)d.BaseAnimation.m_pObject;
    ok = ok && g_deleted == 1 && second->AnimationIndex == 6 && s.m_copies == 2;
    delete second;
    if (!ok) { std::printf("00a3c260_TEST FAIL\n"); return 1; }
    std::printf("00a3c260_TEST PASS\n");
    return 0;
}
