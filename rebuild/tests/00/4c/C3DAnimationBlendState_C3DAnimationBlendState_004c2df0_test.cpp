// Behaviour test for the implicit C3DAnimationBlendState::~C3DAnimationBlendState 0x004C2DF0: drop one reference to the shared
// blend data (Delete only at zero, pointer nulled), then the CBaseClass base destructor
#include <cstdio>
static int g_deletes = 0, g_baseDtors = 0;
class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase() : IVRefCount(0) {}
    virtual ~CIVCountedPointeeBase() {}
    virtual void Delete() const { ++g_deletes; }
    long Release() const { return --IVRefCount; }
    mutable unsigned long IVRefCount;
};
class CBaseClass {
public:
    virtual ~CBaseClass() { ++g_baseDtors; }
};
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) {}
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* Object;
};
class C3DAnimationBlendState : public CBaseClass {
public:
    class CSharedData;
    CIVCountedPointer<CSharedData> Data;
};
int main()
{
    CIVCountedPointeeBase shared;
    shared.IVRefCount = 2;
    {
        C3DAnimationBlendState a;
        a.Data.Object = &shared;
    }
    if (shared.IVRefCount != 1 || g_deletes != 0 || g_baseDtors != 1) { std::printf("004c2df0_TEST FAIL\n"); return 1; }
    {
        C3DAnimationBlendState b;
        b.Data.Object = &shared;
    }
    if (shared.IVRefCount != 0 || g_deletes != 1 || g_baseDtors != 2) { std::printf("004c2df0_TEST FAIL\n"); return 1; }
    {
        C3DAnimationBlendState empty;
    }
    if (g_deletes != 1 || g_baseDtors != 3) { std::printf("004c2df0_TEST FAIL\n"); return 1; }
    CBaseClass* p = new C3DAnimationBlendState;
    delete p;
    if (g_baseDtors != 4) { std::printf("004c2df0_TEST FAIL\n"); return 1; }
    std::printf("004c2df0_TEST PASS\n");
    return 0;
}