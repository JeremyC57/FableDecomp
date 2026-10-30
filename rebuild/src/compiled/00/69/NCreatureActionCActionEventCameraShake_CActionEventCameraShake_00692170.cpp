// NCreatureAction::CActionEventCameraShake::CActionEventCameraShake() -- an unfinished CActionEventCameraShake action event
// Leaf: ??0CActionEventCameraShake@NCreatureAction@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00692170, 22 bytes, thiscall, plain `ret`.
// Identity (proven): it stores vtable 0x0125C0FC (RTTI .?AVCActionEventCameraShake@NCreatureAction@@); FableWin.pdb
//   ??0CActionEventCameraShake@NCreatureAction@@QAE@XZ. The manifest's "CGameJoystickManager::CGameJoystickManager" is propagated.
// Callees: CBaseClass::CBaseClass() 0x0099A2D0.
// Shape (family M194, representative 0x00692170): the default constructor over the inline protected CActionEventBase()
//   (`Finished(false)`, its own out-of-line copy is 0x006920F0, family M193): base constructor call, the Finished store, then
//   this class's vtable (CActionEventBase's store is dead). Speed TU.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CThingCreatureBase;
class CGameCameraManager;
namespace NCreatureAction {
class CActionEventBase : public CBaseClass {
protected:
    CActionEventBase() : Finished(false) { }
public:
    virtual ~CActionEventBase();
    bool Finished;                                        // +0x04
    long ActivationFrame;                                 // +0x08
    CThingCreatureBase* PThing;                           // +0x0C
    CGameCameraManager* PCameraManager;                   // +0x10
};
class CActionEventCameraShake : public CActionEventBase {
public:
    CActionEventCameraShake();
    virtual ~CActionEventCameraShake();
};
CActionEventCameraShake::CActionEventCameraShake()
{
}
}