// CGameScriptInterface::CameraMoveBetweenLookingAt(const C3DVector& from, const C3DVector& to, const CScriptThing& thing, float time, float f) const -- hand the camera manager a scripted move-between camera mode looking at a valid thing
// Leaf: ?CameraMoveBetweenLookingAt@CGameScriptInterface@@UBEXABVC3DVector@@0ABVCScriptThing@@MM@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members Object->Data, Count->Info are the header CCountedPointer's (same offsets).
// Naming wave 18: the global stand-in g_WorldFramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x008926D0, 167 bytes, `ret 0x14`; Ego_r.pdb same symbol.
// Body: if thing.IsAlive() (slot 75): t = thing.GetPThing() (slot 11); GetCameraManager(&manager) (`manager` in the dead
// `thing` slot); f == -1.0f defaults to 0.2f; manager->SetScriptedCameraMode(CCountedPointer<CScriptModuleBase>(new
// <0x90-byte mode>(*t, from, to, ConvertSecondsToWF(time), f))) -- mode ctor 0x00852D90, CCountedPointer(T*) out of line
// at 0x005ED270 (folded label), SetScriptedCameraMode 0x00699E60.
// Layout: as the matched 0x008924B0.
// Names: mode class name not recovered; descriptive.
// Shape: 0x008924B0 model: out-of-line CCountedPointer(T*) plus a declared copy ctor (in-place argument construction).
// Opt: /O2.
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
class CScriptModuleTransitionBetweenPoints : public CScriptModuleBase {
public:
    unsigned char _pad04[0x90 - 4];
    CScriptModuleTransitionBetweenPoints(const CThing& lookAt, const C3DVector& from, const C3DVector& to, long frames, float f);   // 0x00852D90
};
}
#include "engine/CCountedPointer.h"
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
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
private:
public:
    static long ConvertSecondsToWF(float seconds) { return (long)(CMainGameComponent::GetConstantFPS() * seconds); }
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
void CGameScriptInterface::CameraMoveBetweenLookingAt(const C3DVector& from, const C3DVector& to, const CScriptThing& thing, float time, float f) const
{
    if (thing.IsAlive())
    {
        CThing* t = thing.GetPThing();
        CGameCameraManager* manager;
        World->PCurrentGameCamera->GetCameraManager(&manager);
        if (f == -1.0f) f = 0.2f;
        manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(
            new NCameraModes::CScriptModuleTransitionBetweenPoints(*t, from, to, CMainGameComponent::ConvertSecondsToWF(time), f)));
    }
}