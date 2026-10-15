#include <cstdio>
static int g_deleted = 0;
struct CIVCountedPointeeBase {
    unsigned long IVRefCount;
    virtual ~CIVCountedPointeeBase() {}
    virtual void Delete() { ++g_deleted; }
    void AddRef() { ++IVRefCount; }
    void Release() { if (--IVRefCount == 0) Delete(); }
};
template <class T>
struct CIVCountedPointer {
    T* m_pObject;
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            if (m_pObject) {
                m_pObject->Release();
                m_pObject = 0;
            }
            m_pObject = other.m_pObject;
            if (m_pObject)
                m_pObject->AddRef();
        }
        return *this;
    }
};
struct CSharedData : CIVCountedPointeeBase { };
struct C3DAnimationBlendState { virtual ~C3DAnimationBlendState() {} CIVCountedPointer<CSharedData> Data; };
struct C3DAnimationInterpolator {
    C3DAnimationBlendState StartState;
    C3DAnimationBlendState EndState;
    C3DAnimationInterpolator& operator=(const C3DAnimationInterpolator& other)
    {
        StartState = other.StartState;
        EndState = other.EndState;
        return *this;
    }
};
int main()
{
    CSharedData a, b, c; a.IVRefCount = 1; b.IVRefCount = 1; c.IVRefCount = 1;
    C3DAnimationInterpolator x, y;
    x.StartState.Data.m_pObject = &a; x.EndState.Data.m_pObject = 0;
    y.StartState.Data.m_pObject = &b; y.EndState.Data.m_pObject = &c;
    C3DAnimationInterpolator& r = (x = y);
    int ok = &r == &x && x.StartState.Data.m_pObject == &b && x.EndState.Data.m_pObject == &c &&
             a.IVRefCount == 0 && g_deleted == 1 && b.IVRefCount == 2 && c.IVRefCount == 2;
    x = y;   // same pointers: untouched
    ok = ok && b.IVRefCount == 2 && c.IVRefCount == 2 && g_deleted == 1;
    if (!ok) { std::printf("00bd85f0_TEST FAIL\n"); return 1; }
    std::printf("00bd85f0_TEST PASS\n");
    return 0;
}