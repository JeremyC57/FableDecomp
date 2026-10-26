#include <cstdio>
class C3DVector;
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();
};
class CScriptModuleGoToLookAt : public CScriptModuleBase {
public:
    unsigned char _pad04[0x6c - 4];
    CScriptModuleGoToLookAt(const C3DVector& camPos, const C3DVector& lookAt);   // 0x007187C0
};
}
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(void (__fastcall* deleteFunc)(void*), void* data) : RefCount(1), DeleteFunc(deleteFunc), Data(data) {}
};
template <class T> class CCountedPointer {
public:
    T*              Object;
    CCPPointerInfo* Info;
    static void __fastcall DeleteObject(void* data);     // 0x005ECF50 (`delete (T*)data`, folded)
    __forceinline CCountedPointer(T* object) : Object(object)
    {
        if (object)
            Info = new CCPPointerInfo(DeleteObject, Object);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer();
};
class CGameCameraManager {
public:
    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> mode);   // 0x00699E60
};
class CAIGameCameraBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** manager);   // slot 9 (+0x24)
};
class CWorld {
public:
    unsigned char      _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;               // +0x34
};
class CGameScriptInterface {
public:
    virtual void CameraMoveToPosAndLookAtPos(const C3DVector& camPos, const C3DVector& lookAt, float time) const;
    CWorld* World;                                       // +0x04
};
void CGameScriptInterface::CameraMoveToPosAndLookAtPos(const C3DVector& camPos, const C3DVector& lookAt, float time) const
{
    CGameCameraManager* manager;
    World->PCurrentGameCamera->GetCameraManager(&manager);
    manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(new NCameraModes::CScriptModuleGoToLookAt(camPos, lookAt)));
}

static int g_ctors, g_dtors, g_sets; static const C3DVector* g_pos; static const C3DVector* g_look; static void* g_obj;
static CGameCameraManager* g_mgrThis; static long g_seenRef; static void* g_seenFunc;
NCameraModes::CScriptModuleBase::~CScriptModuleBase() { ++g_dtors; }
NCameraModes::CScriptModuleGoToLookAt::CScriptModuleGoToLookAt(const C3DVector& camPos, const C3DVector& lookAt) { ++g_ctors; g_pos = &camPos; g_look = &lookAt; g_obj = this; }
template <class T> void __fastcall CCountedPointer<T>::DeleteObject(void* data) { delete (T*)data; }
template <class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer& o) { Object = o.Object; Info = o.Info; if (Info) ++Info->RefCount; }
template <class T> CCountedPointer<T>::~CCountedPointer()
{
    if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
    Object = 0; Info = 0;
}
void CGameCameraManager::SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> mode)
{
    ++g_sets; g_mgrThis = this; if (mode.Object != g_obj) g_sets += 100;
    g_seenRef = mode.Info ? mode.Info->RefCount : -1; g_seenFunc = mode.Info ? (void*)mode.Info->DeleteFunc : 0;
    if (mode.Info && mode.Info->Data != g_obj) g_sets += 1000;
}
static CGameCameraManager g_mgr;
void CAIGameCameraBase::v00() {} void CAIGameCameraBase::v01() {} void CAIGameCameraBase::v02() {} void CAIGameCameraBase::v03() {}
void CAIGameCameraBase::v04() {} void CAIGameCameraBase::v05() {} void CAIGameCameraBase::v06() {} void CAIGameCameraBase::v07() {}
void CAIGameCameraBase::v08() {}
bool CAIGameCameraBase::GetCameraManager(CGameCameraManager** m) { *m = &g_mgr; return true; }
int main()
{
    CAIGameCameraBase cam; static unsigned char raw[sizeof(CWorld)]; CWorld& w = *(CWorld*)raw; w.PCurrentGameCamera = &cam;
    CGameScriptInterface gsi; gsi.World = &w;
    const C3DVector& camPos = *(const C3DVector*)(raw + 1); const C3DVector& look = *(const C3DVector*)(raw + 2);
    gsi.CameraMoveToPosAndLookAtPos(camPos, look, 1.0f);
    bool ok = g_ctors == 1 && g_pos == &camPos && g_look == &look && g_sets == 1 && g_mgrThis == &g_mgr && g_seenRef == 1
           && g_seenFunc == (void*)&CCountedPointer<NCameraModes::CScriptModuleBase>::DeleteObject && g_dtors == 1;
    std::printf(ok ? "00892410_TEST PASS\n" : "00892410_TEST FAIL\n");
    return ok ? 0 : 1;
}