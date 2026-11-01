// Test for 0x00BEBC50 CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase: Clear() on a non-null SimulationInfo,
// then MeshEffect and SimulationInfo released (Delete at zero, pointer zeroed), then the occlusion base dtor.
#include <cstdio>
static int g_cleared = 0, g_deletes = 0, g_baseDtors = 0;
class CIVCountedPointeeBase { public:
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const { ++g_deletes; }
    long Release() const { return --IVRefCount; }
    mutable unsigned int IVRefCount;
};
template <class T> class CIVCountedPointer { public:
    ~CIVCountedPointer() { Release(); }
    T* operator->() const { return static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) { if (object->Release() == 0) object->Delete(); Object = 0; }
    }
    CIVCountedPointeeBase* Object;
};
class CEnginePrimitiveSimulationInfo : public CIVCountedPointeeBase { public: void Clear() { ++g_cleared; } };
class CEngineMeshEffect : public CIVCountedPointeeBase { };
class CEngineInternalPrimitiveOcclusionBase { public: virtual ~CEngineInternalPrimitiveOcclusionBase() { ++g_baseDtors; } };
class CEngineInternalPrimitiveMeshBase : public CEngineInternalPrimitiveOcclusionBase { public:
    virtual ~CEngineInternalPrimitiveMeshBase();
    CIVCountedPointer<CEnginePrimitiveSimulationInfo> SimulationInfo;
    CIVCountedPointer<CEngineMeshEffect> MeshEffect;
};
CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase()
{
    if (!SimulationInfo.IsNull())
        SimulationInfo->Clear();
}
int main()
{
    CEnginePrimitiveSimulationInfo sim; sim.IVRefCount = 2;
    CEngineMeshEffect effect; effect.IVRefCount = 1;
    { CEngineInternalPrimitiveMeshBase m; m.SimulationInfo.Object = &sim; m.MeshEffect.Object = &effect; }
    if (g_cleared != 1 || g_deletes != 1 || sim.IVRefCount != 1 || effect.IVRefCount != 0 || g_baseDtors != 1) { std::printf("00bebc50_TEST FAIL\n"); return 1; }
    { CEngineInternalPrimitiveMeshBase m; m.SimulationInfo.Object = 0; m.MeshEffect.Object = 0; }
    if (g_cleared != 1 || g_deletes != 1 || g_baseDtors != 2) { std::printf("00bebc50_TEST FAIL\n"); return 1; }
    { CEngineInternalPrimitiveMeshBase m; m.SimulationInfo.Object = &sim; m.MeshEffect.Object = 0; }
    if (g_cleared != 2 || g_deletes != 2 || sim.IVRefCount != 0 || g_baseDtors != 3) { std::printf("00bebc50_TEST FAIL\n"); return 1; }
    std::printf("00bebc50_TEST PASS\n");
    return 0;
}