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
struct CIVCountedPointerC { CIVCountedPointeeBase* Object; CIVCountedPointerC(CIVCountedPointeeBase* p) : Object(p) { }
    CIVCountedPointerC(const CIVCountedPointerC& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; } };
struct CStateBase : CIVCountedPointeeBase { CStateBase() { IVRefCount = 1; } };
class C3DAnimationTransitionState : public CStateBase { public: CIVCountedPointerC StartState; CIVCountedPointerC EndState; float Progress;
    C3DAnimationTransitionState(const CIVCountedPointerC& s, const CIVCountedPointerC& e, float p) : StartState(s), EndState(e), Progress(p) { } };
int main()
{
    CIVCountedPointeeBase a, b; a.IVRefCount = 1; b.IVRefCount = 1;
    CIVCountedPointerC ps(&a), pe(&b), pn(0);
    C3DAnimationTransitionState t(ps, pe, 0.25f);
    if (t.IVRefCount != 1 || t.StartState.Object != &a || t.EndState.Object != &b || a.IVRefCount != 2 || b.IVRefCount != 2 || t.Progress != 0.25f) { std::printf("00a3be70_TEST FAIL\n"); return 1; }
    C3DAnimationTransitionState u(pn, pn, 1.0f);
    if (u.StartState.Object != 0 || u.EndState.Object != 0 || u.Progress != 1.0f) { std::printf("00a3be70_TEST FAIL\n"); return 1; }
    std::printf("00a3be70_TEST PASS\n"); return 0;
}