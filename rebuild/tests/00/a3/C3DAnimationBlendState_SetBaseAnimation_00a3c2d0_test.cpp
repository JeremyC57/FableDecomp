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
struct CSharedData : CIVCountedPointeeBase { CIVCountedPointer BaseAnimation; unsigned char CombinationBlends[0xc]; void* SkeletalMorph[2]; };
static int g_copies = 0;
class C3DAnimationBlendState { public: virtual void v00() { } CSharedData* Data;
    void MakeOwnCopyOfData() { ++g_copies; }
    void SetBaseAnimation(const CIVCountedPointer& a) { MakeOwnCopyOfData(); Data->BaseAnimation = a; } };
int main()
{
    CIVCountedPointeeBase a, b; a.IVRefCount = 1; b.IVRefCount = 1;
    CSharedData d; d.BaseAnimation.Object = &a; C3DAnimationBlendState s; s.Data = &d;
    CIVCountedPointer p; p.Object = &b; s.SetBaseAnimation(p);
    if (g_copies != 1 || d.BaseAnimation.Object != &b || b.IVRefCount != 2 || a.IVRefCount != 0 || CIVCountedPointeeBase::g_deleted != 1) { std::printf("00a3c2d0_TEST FAIL\n"); return 1; }
    std::printf("00a3c2d0_TEST PASS\n"); return 0;
}