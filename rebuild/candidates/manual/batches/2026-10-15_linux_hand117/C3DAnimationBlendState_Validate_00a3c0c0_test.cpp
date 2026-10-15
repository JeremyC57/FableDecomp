#include <cstdio>
static int g_calls = 0;
struct C3DAnimationStateBase {
    int m_validated;
    C3DAnimationStateBase() : m_validated(0) {}
    virtual ~C3DAnimationStateBase() {}
    virtual void Validate() { ++m_validated; ++g_calls; }
};
struct C3DAnimationStateAlpha : C3DAnimationStateBase { float AlphaWeight; };
template <class T> struct CArray {
    T* m_pBegin; T* m_pEnd; T* m_pEndOfStorage;
    unsigned long Size() const { return (unsigned long)(m_pEnd - m_pBegin); }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};
template <class T> struct CIVCountedPointer { T* m_pObject; T* operator->() const { return m_pObject; } };
struct CSharedData {
    CIVCountedPointer<C3DAnimationStateBase> BaseAnimation;
    CArray<C3DAnimationStateAlpha> CombinationBlends;
};
struct C3DAnimationBlendState {
    CSharedData* Data;
    void Validate() const
    {
        if (Data) {
            Data->BaseAnimation->Validate();
            for (unsigned long i = 0; i < Data->CombinationBlends.Size(); ++i)
                Data->CombinationBlends[i].Validate();
        }
    }
};
int main()
{
    C3DAnimationStateBase base;
    C3DAnimationStateAlpha blends[3];
    CSharedData d; d.BaseAnimation.m_pObject = &base;
    d.CombinationBlends.m_pBegin = blends; d.CombinationBlends.m_pEnd = blends + 3; d.CombinationBlends.m_pEndOfStorage = blends + 3;
    C3DAnimationBlendState s; s.Data = &d;
    s.Validate();
    C3DAnimationBlendState empty; empty.Data = 0;
    empty.Validate();
    if (g_calls != 4 || base.m_validated != 1 || blends[0].m_validated != 1 || blends[2].m_validated != 1) {
        std::printf("00a3c0c0_TEST FAIL\n"); return 1;
    }
    std::printf("00a3c0c0_TEST PASS\n");
    return 0;
}
