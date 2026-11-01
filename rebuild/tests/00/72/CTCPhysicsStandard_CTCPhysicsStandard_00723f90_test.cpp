// Test for 0x00723F90 CTCPhysicsStandard::~CTCPhysicsStandard: the owned mesh info is deleted when set, the PhysicsDef reference
// is released (Delete at zero) and cleared, then ~CTCBase.
#include <cstdio>
static int g_deletes = 0, g_baseDtors = 0, g_infoDeletes = 0;
class CDefPointeeBase { public:
    virtual ~CDefPointeeBase() { }
    virtual void Delete() const { ++g_deletes; }
    long Release() const { return --IVRefCount; }
    mutable unsigned int IVRefCount;
};
template <class T> class CDefPointer { public:
    ~CDefPointer() { Release(); }
    void Release()
    {
        if (Object) { CDefPointeeBase* object = Object; if (object->Release() == 0) object->Delete(); Object = 0; }
    }
    CDefPointeeBase* Object;
};
class CThingPhysicsMeshInfo { public:
    unsigned short BankIndex; float RenderSizeX;
    static void operator delete(void* p) { ++g_infoDeletes; ::operator delete(p); }
};
class CPhysicsDef;
class CTCBase { public: virtual ~CTCBase() { ++g_baseDtors; } void* ParentThing; };
class CTCPhysicsBase : public CTCBase { public: float Radius; CDefPointer<const CPhysicsDef> PhysicsDef; };
class CTCPhysicsStandard : public CTCPhysicsBase { public:
    virtual ~CTCPhysicsStandard();
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;
};
CTCPhysicsStandard::~CTCPhysicsStandard()
{
    if (PPhysicsMeshInfo)
        delete PPhysicsMeshInfo;
}
int main()
{
    CDefPointeeBase def; def.IVRefCount = 2;
    { CTCPhysicsStandard p; p.PhysicsDef.Object = &def; p.PPhysicsMeshInfo = new CThingPhysicsMeshInfo; }
    if (g_infoDeletes != 1 || def.IVRefCount != 1 || g_deletes != 0 || g_baseDtors != 1) { std::printf("00723f90_TEST FAIL\n"); return 1; }
    { CTCPhysicsStandard p; p.PhysicsDef.Object = &def; p.PPhysicsMeshInfo = 0; }
    if (g_infoDeletes != 1 || def.IVRefCount != 0 || g_deletes != 1 || g_baseDtors != 2) { std::printf("00723f90_TEST FAIL\n"); return 1; }
    { CTCPhysicsStandard p; p.PhysicsDef.Object = 0; p.PPhysicsMeshInfo = 0; }
    if (g_deletes != 1 || g_baseDtors != 3) { std::printf("00723f90_TEST FAIL\n"); return 1; }
    std::printf("00723f90_TEST PASS\n");
    return 0;
}