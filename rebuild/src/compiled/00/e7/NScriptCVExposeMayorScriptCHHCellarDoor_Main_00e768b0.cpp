// NScript::CV_ExposeMayorScript::CHH_CellarDoor::Main() -- unless the script was aborted, mark the cellar door usable or not after the parent script's flag at +0x50
// Leaf: ?Main@CHH_CellarDoor@CV_ExposeMayorScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E768B0, length 80 bytes.
// Callees: the abort test @ 0x00F35B30 (on `this`, bool; manifest "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated), the script interface's slot 447
// SetThingAsUsable(const CScriptThing&, bool). Ego_r.pdb CHH_CellarDoor (28 bytes): PQuest +4, the CScriptThing +8, ParentClass +0x14 (CV_ExposeMayorScript, flag byte +0x50 unnamed).
// Shape: the flag read first and both arms duplicated (abort test, `push 0` / `push 1`, the thing's address by `add esi,8`), `this` in ecx before the branch.
// Naming wave 35: identity reg 00e75d84 "HH_CellarDoor" in 00e75ba0; Alloc 00e76800; vtable 012e9908 slot 1; PDB ?Main@CHH_CellarDoor@CV_ExposeMayorScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CHH_CellarDoor::Main (wave3/labels_alloc.tsv); PDB ?Main@CHH_CellarDoor@CV_ExposeMayorScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CHH_CellarDoor@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); outer stand-in CV_ExposeMayorScript moved into NScript); same bytes.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                             // 8 bytes at +8 of the script object
    unsigned char _body[8];
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CV_ExposeMayorScript {
public:
    unsigned char _pad[0x50];
    bool          m_cellarDoorUsable;              // +0x50
public:
    class CHH_CellarDoor;            // the entity class below (naming wave 35)
};
}
using NScript::CV_ExposeMayorScript;
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CV_ExposeMayorScript::CHH_CellarDoor : public CEntityScriptBase {
public:
        CGameScriptInterface*   PQuest;                // +0x04
    CScriptThing            Thing;                 // +0x08
    unsigned char           _pad10[4];             // +0x10
    CV_ExposeMayorScript*   ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
private:
    virtual void Main();                          // PDB ?Main@CHH_CellarDoor@CV_ExposeMayorScript@NScript@@EAEXXZ
};
}

void NScript::CV_ExposeMayorScript::CHH_CellarDoor::Main()
{
    if (!ParentClass->m_cellarDoorUsable) {
        if (!IsActiveThreadTerminating())
            PQuest->SetThingAsUsable(Thing, false);
    } else {
        if (!IsActiveThreadTerminating())
            PQuest->SetThingAsUsable(Thing, true);
    }
}