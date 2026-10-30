// CGameScriptInterface::CameraCircleAroundThing(const CScriptThing&, const C3DVector&, float) const -- hand the camera manager a scripted dolly mode circling the thing at an offset
// Leaf: ?CameraCircleAroundThing@CGameScriptInterface@@UBEXABVCScriptThing@@ABVC3DVector@@M@Z
// Retail: Fable.exe @ 0x00892B50, 197 bytes, `ret 0xc`; Ego_r.pdb same symbol.
// Body: World (+0x04)->PCurrentGameCamera (+0x34)->GetCameraManager(&manager) (slot 9); manager->SetScriptedCameraMode(
// CCountedPointer<CScriptModuleBase>(new CScriptModuleDollyThing(thing.GetThing(), offset, time))) -- operator new
// 0x00BFEA1A (0x60 bytes); the DollyThing ctor is inline: thing.GetThing() (CScriptThing slot 11) evaluated first, base
// ctor CScriptModuleBase() 0x005ED070, vptr 0x01278188 (RTTI NCameraModes::CScriptModuleDollyThing), RotThing,
// DollyVector, TimeFor360Turn; the CCountedPointer(T*) ctor inline as in the matched 0x00892410.
// Layout: Ego_r.pdb CScriptModuleDollyThing (0x60): RotThing +0x4c, DollyVector +0x50, TimeFor360Turn +0x5c;
// CScriptModuleBase is 0x4c bytes.
// Shape: /O2 (16-aligned entry); CCountedPointer(T*) __forceinline, handing the Object member to the info block. C3DVector
// needs its user copy ctor: the implicit one copies through `lea eax,[esi+50h]` (199v197).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CThing;
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
};
namespace NCameraModes {
class CScriptModuleBase {
public:
    unsigned char _pad04[0x4c - 4];
    CScriptModuleBase();                                 // 0x005ED070
    virtual ~CScriptModuleBase();
};
class CScriptModuleDollyThing : public CScriptModuleBase {     // vtable 0x01278188
public:
    CThing*   RotThing;                                  // +0x4c
    C3DVector DollyVector;                               // +0x50
    float     TimeFor360Turn;                            // +0x5c
    CScriptModuleDollyThing(CThing* rotThing, const C3DVector& dollyVector, float timeFor360Turn)
        : RotThing(rotThing), DollyVector(dollyVector), TimeFor360Turn(timeFor360Turn) {}
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::CameraCircleAroundThing(const CScriptThing& thing, const C3DVector& offset, float timeFor360Turn) const
{
    CGameCameraManager* manager;
    World->PCurrentGameCamera->GetCameraManager(&manager);
    manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(
        new NCameraModes::CScriptModuleDollyThing(thing.GetThing(), offset, timeFor360Turn)));
}