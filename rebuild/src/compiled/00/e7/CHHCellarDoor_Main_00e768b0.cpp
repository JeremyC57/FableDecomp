// CHH_CellarDoor::Main() -- unless the script was aborted, mark the cellar door usable or not after the parent script's flag at +0x50
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E768B0, length 80 bytes.
// Callees: the abort test @ 0x00F35B30 (on `this`, bool; manifest "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated), the script interface's slot 447
// SetThingAsUsable(const CScriptThing&, bool). Ego_r.pdb CHH_CellarDoor (28 bytes): PQuest +4, the CScriptThing +8, ParentClass +0x14 (CV_ExposeMayorScript, flag byte +0x50 unnamed).
// Shape: the flag read first and both arms duplicated (abort test, `push 0` / `push 1`, the thing's address by `add esi,8`), `this` in ecx before the branch.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                             // 8 bytes at +8 of the script object
    unsigned char _body[8];
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CV_ExposeMayorScript {
    unsigned char _pad[0x50];
    bool          m_cellarDoorUsable;              // +0x50
};
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CHH_CellarDoor : public CEntityScriptBase {
    void*                   __vftable;             // +0x00
    CGameScriptInterface*   PQuest;                // +0x04
    CScriptThing            Thing;                 // +0x08
    unsigned char           _pad10[4];             // +0x10
    CV_ExposeMayorScript*   ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    void Main();
};

void CHH_CellarDoor::Main()
{
    if (!ParentClass->m_cellarDoorUsable) {
        if (!IsActiveThreadTerminating())
            PQuest->SetThingAsUsable(Thing, false);
    } else {
        if (!IsActiveThreadTerminating())
            PQuest->SetThingAsUsable(Thing, true);
    }
}