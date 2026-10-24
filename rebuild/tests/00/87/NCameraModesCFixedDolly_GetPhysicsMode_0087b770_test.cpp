#include <cstdio>
struct CGameCameraManager { int id; };
struct CCPPointerInfo {
    long RefCount; void (*DeleteFunc)(void* data); void* Data;
    CCPPointerInfo(void (*deleteFunc)(void*), void* data) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};
struct CBase;
struct CSubComponentBase {
    CBase* Owner; const CGameCameraManager* CameraManager;
    CSubComponentBase(CBase* o, const CGameCameraManager* m) : Owner(o), CameraManager(m) { }
    virtual ~CSubComponentBase() { }
};
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    static void DeleteData(void* data) { delete (T*)data; }
    CCountedPointer(T* data) : Data(data)
    {
        if (Data)
            Info = new CCPPointerInfo(DeleteData, Data);
        else
            Info = 0;
    }
};
struct CRigidPhysicsLeaf : CSubComponentBase {
    CRigidPhysicsLeaf(CBase& owner, const CGameCameraManager& manager) : CSubComponentBase(&owner, &manager) { }
};
struct CBase { CGameCameraManager* CameraManager; };
struct CFixedDolly : CBase {
    CCountedPointer<CSubComponentBase> GetPhysicsMode()
    {
        return CCountedPointer<CSubComponentBase>(new CRigidPhysicsLeaf(*this, *CameraManager));
    }
};
int main()
{
    CGameCameraManager mgr; mgr.id = 9;
    CFixedDolly d; d.CameraManager = &mgr;
    CCountedPointer<CSubComponentBase> p = d.GetPhysicsMode();
    int bad = (!p.Data || p.Data->Owner != &d || p.Data->CameraManager != &mgr || !p.Info || p.Info->RefCount != 1 || p.Info->Data != (void*)p.Data);
    if (!bad) { p.Info->DeleteFunc(p.Info->Data); delete p.Info; }
    if (bad) { std::printf("0087b770_TEST FAIL\n"); return 1; }
    std::printf("0087b770_TEST PASS\n");
    return 0;
}