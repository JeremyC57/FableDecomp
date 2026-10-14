#include <cstdio>
struct CIVCountedPointeeBase {
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
struct CMeshDataBankEntry { virtual void v00() { } void* Bank; unsigned long Index; };
struct CIVCountedPointerC { CIVCountedPointeeBase* Object; CIVCountedPointerC(CIVCountedPointeeBase* p) : Object(p) { }
    CIVCountedPointerC(const CIVCountedPointerC& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; } };
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct C3DAnimationInfo : CMeshDataBankEntry { CIVCountedPointerC Data; float Duration; float NonLoopingDuration; C3DVector MovementVector; float Rotation;
    C3DAnimationInfo(CIVCountedPointeeBase* d) : Data(d), Duration(1), NonLoopingDuration(2), MovementVector(3, 4, 5), Rotation(6) { Bank = (void*)0x1234; Index = 9; }
    C3DAnimationInfo(const C3DAnimationInfo& o) : CMeshDataBankEntry(o), Data(o.Data), Duration(o.Duration), NonLoopingDuration(o.NonLoopingDuration), MovementVector(o.MovementVector), Rotation(o.Rotation) { } };
int main()
{
    CIVCountedPointeeBase a; a.IVRefCount = 1;
    C3DAnimationInfo src(&a); C3DAnimationInfo dst(src);
    if (dst.Bank != (void*)0x1234 || dst.Index != 9 || dst.Data.Object != &a || a.IVRefCount != 2 || dst.Duration != 1 || dst.NonLoopingDuration != 2 || dst.MovementVector.X != 3 || dst.MovementVector.Y != 4 || dst.MovementVector.Z != 5 || dst.Rotation != 6) { std::printf("00a2a4f0_TEST FAIL\n"); return 1; }
    C3DAnimationInfo n(0); C3DAnimationInfo m(n); if (m.Data.Object != 0) { std::printf("00a2a4f0_TEST FAIL\n"); return 1; }
    std::printf("00a2a4f0_TEST PASS\n"); return 0;
}
