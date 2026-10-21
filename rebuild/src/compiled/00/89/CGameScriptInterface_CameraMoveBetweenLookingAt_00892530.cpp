// CGameScriptInterface::CameraMoveBetweenLookingAt(const C3DVector& from, const C3DVector& to, const C3DVector& lookAt, float time, float f) const -- hand the camera manager a scripted move-between camera mode looking at a fixed position
// Leaf: ?CameraMoveBetweenLookingAt@CGameScriptInterface@@UBEXABVC3DVector@@00MM@Z
// Retail: Fable.exe @ 0x00892530, 211 bytes, `ret 0x14`; Ego_r.pdb same symbol.
// Body: f == -1.0f (float 0x0122DEE0) defaults to 0.2f; World (+0x04)->PCurrentGameCamera (+0x34)->GetCameraManager(&manager)
// (slot 9); manager->SetScriptedCameraMode(CCountedPointer<CScriptModuleBase>(new <0x90-byte mode>(lookAt, from, to,
// ConvertSecondsToWF(time), f))) -- operator new 0x00BFEA1A, mode ctor 0x008524C0, SetScriptedCameraMode 0x00699E60.
// ConvertSecondsToWF (Ego_r ?ConvertSecondsToWF@CMainGameComponent@@SIJM@Z) inlines as (long)(WFPS * seconds):
// fild 0x01375550; fmul; _ftol.
// Layout: as the matched 0x00892410 (CWorld::PCurrentGameCamera +0x34; CCountedPointer 8 bytes object/info).
// Names: the mode class name is not recovered (RTTI not read); CScriptModuleMoveBetweenLookingAt is descriptive.
// Shape: 0x00892410 model: __forceinline CCountedPointer(T*) handing the info block the Object member.
// Opt: /O2.
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
struct CCPPointerInfo {
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