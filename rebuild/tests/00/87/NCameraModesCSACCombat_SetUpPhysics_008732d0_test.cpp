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
static int g_deleted = 0;
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    static void DeleteData(void* data) { delete (T*)data; ++g_deleted; }
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
struct CBase {
    CGameCameraManager* CameraManager;
    CSubComponentBase* added; CCPPointerInfo* info; bool flag;
    void AddSubComponent(CCountedPointer<CSubComponentBase> component, bool f) { added = component.Data; info = component.Info; flag = f; }
};
struct CSACCombat : CBase {
    void SetUpPhysics()
    {
        AddSubComponent(CCountedPointer<CSubComponentBase>(new CRigidPhysicsLeaf(*this, *CameraManager)), false);
    }
};
int main()
{
    CGameCameraManager mgr; mgr.id = 3;
    CSACCombat m; m.CameraManager = &mgr; m.added = 0; m.info = 0; m.flag = true;
    m.SetUpPhysics();
    int bad = (!m.added || m.added->Owner != &m || m.added->CameraManager != &mgr || m.flag || !m.info || m.info->RefCount != 1 || m.info->Data != (void*)m.added);
    if (!bad) { m.info->DeleteFunc(m.info->Data); delete m.info; if (g_deleted != 1) bad = 1; }
    if (bad) { std::printf("008732d0_TEST FAIL\n"); return 1; }
    std::printf("008732d0_TEST PASS\n");
    return 0;
}