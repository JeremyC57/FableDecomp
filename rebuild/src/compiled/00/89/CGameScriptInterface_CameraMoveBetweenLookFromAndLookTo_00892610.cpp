// CGameScriptInterface::CameraMoveBetweenLookFromAndLookTo(const C3DVector& lookFrom1, const C3DVector& lookFrom2, const C3DVector& lookTo1, const C3DVector& lookTo2, float time) const -- hand the camera manager a scripted camera mode that moves both the eye and the target
// Leaf: ?CameraMoveBetweenLookFromAndLookTo@CGameScriptInterface@@UBEXABVC3DVector@@000M@Z
// Retail: Fable.exe @ 0x00892610, 184 bytes, `ret 0x14`; Ego_r.pdb same symbol.
// Body: World (+0x04)->PCurrentGameCamera (+0x34)->GetCameraManager(&manager) (slot 9);
// manager->SetScriptedCameraMode(CCountedPointer<CScriptModuleBase>(new <0x80-byte mode>(arg3, arg1, arg4, arg2,
// ConvertSecondsToWF(time)))) -- operator new 0x00BFEA1A, mode ctor 0x008525F0, SetScriptedCameraMode 0x00699E60.
// Layout: as the matched 0x00892410.
// Names: parameter names are descriptive (the ctor takes argument 3, 1, 4, 2); mode class name not recovered.
// Shape: 0x00892410 model (__forceinline CCountedPointer(T*)); ConvertSecondsToWF inline (long)(WFPS * seconds).
// Opt: /O2.
class C3DVector;
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();
};
class CScriptModuleMoveBetweenLookFromAndLookTo : public CScriptModuleBase {
public:
    unsigned char _pad04[0x80 - 4];
    CScriptModuleMoveBetweenLookFromAndLookTo(const C3DVector& lookTo1, const C3DVector& lookFrom1, const C3DVector& lookTo2,
        const C3DVector& lookFrom2, long frames);   // 0x008525F0
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::CameraMoveBetweenLookFromAndLookTo(const C3DVector& lookFrom1, const C3DVector& lookFrom2,
    const C3DVector& lookTo1, const C3DVector& lookTo2, float time) const
{
    CGameCameraManager* manager;
    World->PCurrentGameCamera->GetCameraManager(&manager);
    manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(
        new NCameraModes::CScriptModuleMoveBetweenLookFromAndLookTo(lookTo1, lookFrom1, lookTo2, lookFrom2,
            CMainGameComponent::ConvertSecondsToWF(time))));
}