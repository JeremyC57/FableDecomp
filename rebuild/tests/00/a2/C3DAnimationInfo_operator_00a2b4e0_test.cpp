// Behaviour test for C3DAnimationInfo::operator= (0x00A2B4E0): memberwise copy with counted-pointer
// release/add-ref; self-identical data pointers are left alone.
#include <cstdio>

static int g_deleted = 0;
class CIVCountedPointeeBase { public: unsigned long IVRefCount; virtual ~CIVCountedPointeeBase() {} virtual void Delete() { ++g_deleted; } };
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) { CIVCountedPointeeBase* old = Object; if (--old->IVRefCount == 0) old->Delete(); Object = 0; }
            Object = other.Object;
            if (Object) ++Object->IVRefCount;
        }
        return *this;
    }
};
class C3DVector { public: float X, Y, Z; };
class C3DAnimationInfo { public:
    void* Bank; unsigned long Index; CIVCountedPointer Data; float Duration, NonLoopingDuration; C3DVector MovementVector; float Rotation;
    C3DAnimationInfo& operator=(const C3DAnimationInfo& o)
    {
        Bank = o.Bank; Index = o.Index; Data = o.Data; Duration = o.Duration; NonLoopingDuration = o.NonLoopingDuration;
        MovementVector = o.MovementVector; Rotation = o.Rotation; return *this;
    }
};

int main()
{
    CIVCountedPointeeBase oldData, newData; oldData.IVRefCount = 1; newData.IVRefCount = 1;
    C3DAnimationInfo a = { 0, 1, { &oldData }, 1.0f, 2.0f, { 0, 0, 0 }, 0 };
    C3DAnimationInfo b = { &a, 7, { &newData }, 3.0f, 4.0f, { 1.0f, 2.0f, 3.0f }, 0.5f };
    C3DAnimationInfo* r = &(a = b);
    if (r != &a || a.Bank != &a || a.Index != 7 || a.Data.Object != &newData || newData.IVRefCount != 2 || g_deleted != 1
        || a.Duration != 3.0f || a.NonLoopingDuration != 4.0f || a.MovementVector.Z != 3.0f || a.Rotation != 0.5f) {
        std::printf("00a2b4e0_TEST FAIL\n"); return 1;
    }
    a = b;
    if (newData.IVRefCount != 2) { std::printf("00a2b4e0_TEST FAIL\n"); return 1; }
    std::printf("00a2b4e0_TEST PASS\n");
    return 0;
}