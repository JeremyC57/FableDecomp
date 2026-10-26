#include <cstdio>
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags) { }
    virtual void Delete() { ++g_deleted; }
    unsigned long IVRefCount;
    static int g_deleted;
    CIVCountedPointeeBase() : IVRefCount(0) { }
};
int CIVCountedPointeeBase::g_deleted = 0;

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CIVCountedPointeeBase* old = Object;
        if (old != other.Object) {
            if (old) {
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};
class C3DAnimationTransitionState { public: virtual void v00() { } unsigned long IVRefCount; CIVCountedPointer StartState; CIVCountedPointer EndState; float Progress;
    void SetEndState(const CIVCountedPointer& s) { EndState = s; } };
int main()
{
    CIVCountedPointeeBase a, b; a.IVRefCount = 1; b.IVRefCount = 1;
    C3DAnimationTransitionState t; t.EndState.Object = &a; CIVCountedPointer p; p.Object = &b;
    t.SetEndState(p);
    if (t.EndState.Object != &b || b.IVRefCount != 2 || a.IVRefCount != 0 || CIVCountedPointeeBase::g_deleted != 1 || t.StartState.Object != 0) { std::printf("007105a0_TEST FAIL\n"); return 1; }
    std::printf("007105a0_TEST PASS\n"); return 0;
}