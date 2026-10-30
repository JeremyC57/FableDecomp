// NScript::CQ_PrisonEscapeRescueMotherScript::CCellDoor::Main() -- the prison cell door: untargetable until the hero has stolen the prison key, then targetable; opens once the hero uses it
// Leaf: ?Main@CCellDoor@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD8FD0, 207 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 1).
// Identity (proven): slot 1 (Main) of the RTTI-less entity vtable 0x012DC528, which the landed
//   NScript::CQ_PrisonEscapeRescueMotherScript::CCellDoor::Alloc 0x00DD8F10 stores (site 0x00DD8F46); Ego_r.pdb has ?Main@CCellDoor@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CCellDoor (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 469 EntitySetTargetable, 7 NewScriptFrame, 425 OpenDoor; CScriptThing slot 20 MsgIsUsedBy;
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; IsActiveThreadTerminating 0x00F35B30.
//   PMasterData->PrisonKeyStolenByHero (+0xee) is the flag the landed CPrisonKeyDoor::Main 0x00DD9180 waits on.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace NScript {
class CQ_SunnyvaleMasterData {                          // Ego_r.pdb layout (offsets equal retail up to +0x100)
public:
    unsigned char _before[0xee];
    bool PrisonKeyStolenByHero;                         // +0xee
};
}
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
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
class CQ_PrisonEscapeRescueMotherScript {
public:
    class CCellDoor;
};
class CQ_PrisonEscapeRescueMotherScript::CCellDoor : public CEntityScriptBase {
public:
    CQ_PrisonEscapeRescueMotherScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18

private:
    virtual void Main();
};

void CQ_PrisonEscapeRescueMotherScript::CCellDoor::Main()
{
    Game->EntitySetTargetable(Me, false);
    while (!PMasterData->PrisonKeyStolenByHero) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetTargetable(Me, true);
    if (IsActiveThreadTerminating())
        return;
    do {
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->OpenDoor(Me);
            return;
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
}