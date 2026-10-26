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
struct CIVCountedPointerR { CIVCountedPointeeBase* Object;
    CIVCountedPointerR& operator=(CIVCountedPointeeBase* p) { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } Object = p; if (p) ++p->IVRefCount; return *this; } };
class C3DVector { public: float X, Y, Z; };
class C3DAnimationInfo { public: virtual void v00() { } void* Bank; unsigned long Index; CIVCountedPointerR Data; float Duration; float NonLoopingDuration; C3DVector MovementVector; float Rotation;
    void Reset() { Data = 0; Duration = 0.0f; NonLoopingDuration = 0.0f; MovementVector.X = MovementVector.Y = MovementVector.Z = 0.0f; } };
int main()
{
    CIVCountedPointeeBase a; a.IVRefCount = 1;
    C3DAnimationInfo i; i.Data.Object = &a; i.Duration = 3.0f; i.NonLoopingDuration = 2.0f; i.MovementVector.X = 1; i.MovementVector.Y = 2; i.MovementVector.Z = 3; i.Rotation = 7.0f;
    i.Reset();
    if (i.Data.Object != 0 || a.IVRefCount != 0 || CIVCountedPointeeBase::g_deleted != 1 || i.Duration != 0 || i.NonLoopingDuration != 0 || i.MovementVector.X != 0 || i.MovementVector.Y != 0 || i.MovementVector.Z != 0 || i.Rotation != 7.0f) { std::printf("00a24d40_TEST FAIL\n"); return 1; }
    i.Reset();   // null data: no double release
    if (CIVCountedPointeeBase::g_deleted != 1) { std::printf("00a24d40_TEST FAIL\n"); return 1; }
    std::printf("00a24d40_TEST PASS\n"); return 0;
}