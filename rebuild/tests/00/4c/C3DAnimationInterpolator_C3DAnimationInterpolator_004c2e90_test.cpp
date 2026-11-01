// Test for 0x004C2E90 C3DAnimationInterpolator::~C3DAnimationInterpolator (implicit): EndState then StartState release their shared
// data (Delete at zero, pointer zeroed), each running ~CBaseClass, then the interpolator's own ~CBaseClass.
#include <cstdio>
static int g_deletes = 0, g_baseDtors = 0, g_order[4], g_n = 0;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const { ++g_deletes; }
    long Release() const { return --IVRefCount; }
    mutable unsigned int IVRefCount;
    int Id;
};
template <class T> class CIVCountedPointer { public:
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) { if (g_n < 4) g_order[g_n++] = object->Id; if (object->Release() == 0) object->Delete(); Object = 0; }
    }
    CIVCountedPointeeBase* Object;
};
class CBaseClass { public: virtual ~CBaseClass() { ++g_baseDtors; } };
class C3DAnimationBlendState : public CBaseClass { public:
    class CSharedData;
    CIVCountedPointer<CSharedData> Data;
};
class C3DAnimationInterpolator : public CBaseClass { public:
    C3DAnimationBlendState StartState;
    C3DAnimationBlendState EndState;
};
int main()
{
    CIVCountedPointeeBase a, b; a.IVRefCount = 1; a.Id = 1; b.IVRefCount = 2; b.Id = 2;
    { C3DAnimationInterpolator i; i.StartState.Data.Object = &a; i.EndState.Data.Object = &b; }
    if (g_deletes != 1 || a.IVRefCount != 0 || b.IVRefCount != 1 || g_baseDtors != 3 || g_n != 2 || g_order[0] != 2 || g_order[1] != 1) { std::printf("004c2e90_TEST FAIL\n"); return 1; }
    { C3DAnimationInterpolator i; i.StartState.Data.Object = 0; i.EndState.Data.Object = &b; }
    if (g_deletes != 2 || b.IVRefCount != 0 || g_baseDtors != 6) { std::printf("004c2e90_TEST FAIL\n"); return 1; }
    std::printf("004c2e90_TEST PASS\n");
    return 0;
}