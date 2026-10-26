#include <cstdio>
class C3DVector;
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();
};
class CScriptModuleMoveBetweenLookingAt : public CScriptModuleBase {
public:
    unsigned char _pad04[0x90 - 4];
    CScriptModuleMoveBetweenLookingAt(const C3DVector& lookAt, const C3DVector& from, const C3DVector& to, long frames, float f);   // 0x008524C0
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
extern long g_WorldFramesPerSecond;                  // 0x01375550
class CMainGameComponent {
public:
    static long ConvertSecondsToWF(float seconds) { return (long)(g_WorldFramesPerSecond * seconds); }
};
class CWorld {
public:
    unsigned char      _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;               // +0x34
};
class CGameScriptInterface {
public:
    virtual void CameraMoveBetweenLookingAt(const C3DVector& from, const C3DVector& to, const C3DVector& lookAt, float time, float f) const;
    CWorld* World;                                       // +0x04
};
void CGameScriptInterface::CameraMoveBetweenLookingAt(const C3DVector& from, const C3DVector& to, const C3DVector& lookAt, float time, float f) const
{
    if (f == -1.0f) f = 0.2f;
    CGameCameraManager* manager;
    World->PCurrentGameCamera->GetCameraManager(&manager);
    manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(
        new NCameraModes::CScriptModuleMoveBetweenLookingAt(lookAt, from, to, CMainGameComponent::ConvertSecondsToWF(time), f)));
}

void CAIGameCameraBase::v00() {} void CAIGameCameraBase::v01() {} void CAIGameCameraBase::v02() {} void CAIGameCameraBase::v03() {}
void CAIGameCameraBase::v04() {} void CAIGameCameraBase::v05() {} void CAIGameCameraBase::v06() {} void CAIGameCameraBase::v07() {}
void CAIGameCameraBase::v08() {}
static int g_ctors, g_dtors, g_sets; static void* g_obj; static CGameCameraManager* g_mgrThis; static long g_seenRef;
NCameraModes::CScriptModuleBase::~CScriptModuleBase() { ++g_dtors; }
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
    g_seenRef = mode.Info ? mode.Info->RefCount : -1;
}
static CGameCameraManager g_mgr;
bool CAIGameCameraBase::GetCameraManager(CGameCameraManager** m) { *m = &g_mgr; return true; }
long g_WorldFramesPerSecond = 30;
static const C3DVector *g_a, *g_b, *g_c; static long g_frames; static float g_f;
NCameraModes::CScriptModuleMoveBetweenLookingAt::CScriptModuleMoveBetweenLookingAt(const C3DVector& lookAt, const C3DVector& from, const C3DVector& to, long frames, float f)
{ ++g_ctors; g_a = &lookAt; g_b = &from; g_c = &to; g_frames = frames; g_f = f; g_obj = this; }
int main()
{
    CAIGameCameraBase cam; static unsigned char raw[sizeof(CWorld) + 8]; CWorld& w = *(CWorld*)raw; w.PCurrentGameCamera = &cam;
    CGameScriptInterface gsi; gsi.World = &w;
    const C3DVector& p1 = *(const C3DVector*)(raw + 1); const C3DVector& p2 = *(const C3DVector*)(raw + 2); const C3DVector& p3 = *(const C3DVector*)(raw + 3);
    gsi.CameraMoveBetweenLookingAt(p1, p2, p3, 2.0f, -1.0f);
    bool ok = g_ctors == 1 && g_a == &p3 && g_b == &p1 && g_c == &p2 && g_frames == 60 && g_f == 0.2f && g_sets == 1
           && g_mgrThis == &g_mgr && g_seenRef == 1 && g_dtors == 1;
    gsi.CameraMoveBetweenLookingAt(p1, p2, p3, 0.5f, 0.75f);
    ok = ok && g_ctors == 2 && g_frames == 15 && g_f == 0.75f && g_sets == 2 && g_dtors == 2;
    std::printf(ok ? "00892530_TEST PASS\n" : "00892530_TEST FAIL\n");
    return ok ? 0 : 1;
}