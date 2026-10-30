// CGameScriptInterface::CameraMoveToPosAndLookAtThing(const C3DVector& pos, const CScriptThing& thing, float) const -- hand the camera manager a scripted go-to-and-look-at camera mode for a valid thing
// Leaf: ?CameraMoveToPosAndLookAtThing@CGameScriptInterface@@UBEXABVC3DVector@@ABVCScriptThing@@M@Z
// Retail: Fable.exe @ 0x008924B0, 116 bytes, `ret 0xc`; Ego_r.pdb same symbol. The float argument is unused.
// Body: if thing.IsAlive() (slot 75) and bit 2 of the thing's ThingTypeFlags (+0x6c): t = thing.GetPThing() (slot 11, called
// again); World (+0x04)->PCurrentGameCamera (+0x34)->GetCameraManager(&manager) (slot 9, +0x24, Ego_r.pdb
// ?GetCameraManager@CAIGameCameraBase@@UAE_NPAPAVCGameCameraManager@@@Z; `manager` lives in the dead `thing` slot);
// manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(new NCameraModes::CScriptModuleGoToLookAt(
// pos, *t))) -- operator new 0x00BFEA1A (0x6c bytes), ctor 0x00852D10, CCountedPointer(T*) 0x005ED270 (folded label),
// SetScriptedCameraMode 0x00699E60 (by-value CCountedPointer).
// Layout: CThing::ThingTypeFlags +0x6c (Ego_r.pdb +0x7c under the retail -0x10 shift); CWorld::PCurrentGameCamera +0x34
// (Ego_r.pdb). CCountedPointer is 8 bytes (object, count). The meaning of type-flag bit 2 is not recovered.
// Shape: /O2 (16-aligned entry). CCountedPointer needs its declared copy constructor: without it VC7.1 builds the temporary
// in the frame and copies it to the argument area (137v116); with it the pointer is constructed in place (`sub esp,8; mov ecx,esp`).
class C3DVector;
class CThing {
public:
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                        // +0x6c
};
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();
};
class CScriptModuleGoToLookAt : public CScriptModuleBase {
public:
    unsigned char _pad04[0x6c - 4];
    CScriptModuleGoToLookAt(const C3DVector& camPos, const CThing& target);   // 0x00852D10
};
}
template <class T> class CCountedPointer {
public:
    T*    Object;
    long* Count;
    CCountedPointer(T* object);                          // 0x005ED270 (instance shared by folding)
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::CameraMoveToPosAndLookAtThing(const C3DVector& camPos, const CScriptThing& targetThing, float time) const
{
    if (targetThing.IsAlive() && (targetThing.GetPThing()->ThingTypeFlags & 4))
    {
        CThing* t = targetThing.GetPThing();
        CGameCameraManager* manager;
        World->PCurrentGameCamera->GetCameraManager(&manager);
        manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(new NCameraModes::CScriptModuleGoToLookAt(camPos, *t)));
    }
}