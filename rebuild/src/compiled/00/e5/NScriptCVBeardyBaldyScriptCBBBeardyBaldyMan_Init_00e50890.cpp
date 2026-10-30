// NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Init() -- Beardy Baldy has information and is persistent; both last random speech indices start at 10
// Leaf: ?Init@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E50890, 53 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012E5AEC, which the landed
//   NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Alloc 0x00E507E0 stores (site 0x00E50816); Ego_r.pdb has ?Init@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CBB_BeardyBaldyMan (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 360 SetThingHasInformation, 477 SetThingPersistent.
class CCharString;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BeardyBaldyScript {
public:
    class CBB_BeardyBaldyMan;
};
class CV_BeardyBaldyScript::CBB_BeardyBaldyMan : public CEntityScriptBase {
public:
    CV_BeardyBaldyScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long lastRandomSpeechIdx1;                          // +0x1c
    long lastRandomSpeechIdx2;                          // +0x20
private:
    virtual void Init();
};

void CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Init()
{
    Game->SetThingHasInformation(Me, true, true, false);
    Game->SetThingPersistent(Me, true);
    lastRandomSpeechIdx1 = 10;
    lastRandomSpeechIdx2 = 10;
}
}