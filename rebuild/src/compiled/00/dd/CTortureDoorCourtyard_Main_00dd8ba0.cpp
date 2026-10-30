// NScript::CQ_PrisonEscapeRescueMotherScript::CTortureDoorCourtyard::Main() -- keep the courtyard torture door unlocked until the mother is rescued; then lock it and, for as long as the mother stays rescued, show the "escape via the secret passage" hint whenever the hero uses it
// Leaf: Main
// Identity (naming wave 11): ?Main@CTortureDoorCourtyard@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012DC4AC slot 1 is stored by the factory 0x00DD8AE0 registered as "TortureDoorCourtyard"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD8BA0, length 348 bytes (+4 int3). Entity-script slot 1 (Main) of the
//   CQ_PrisonEscapeRescueMotherScript nested CTortureDoorCourtyard (Ego_r.pdb has two CTortureDoorCourtyard classes; the
//   other, CQ_SecretPassageScript's, is landed at 0x00DEEFC0 and has no flag at +0x67).
// Callees: CGameScriptInterface slots 518 EntitySetAsLocked (+0x818), 7 NewScriptFrame (+0x1c), 115 DisplayGameInfo
//   (+0x1cc), 40 MsgIsGameInfoClickedPast (+0xa0); CScriptThing slot 20 MsgIsUsedBy (+0x50, 0x004AAB10); the entity
//   script's IsActiveThreadTerminating @ 0x00F35B30 (manifest label "CCreatureAction_TrollWhackGroundBase::Initialise" is
//   propagated); CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0.
// Strings: 0x0125D1C8 "SCRIPT_NAME_HERO", 0x012DC4C8 "TEXT_QST_055_HINT_ESCAPE_VIA_SECRET_PASSAGE".
// Layout: Ego_r.pdb CTortureDoorCourtyard (28 bytes, = retail): Game +0x04, Me +0x08, ParentClass +0x14, PMasterData +0x18;
//   the parent's mother-rescued flag at +0x67 as the landed sibling CTortureDoorPassage::Main (0x00DD8990) of the same TU.
// Shape: the landed CTortureDoorPassage::Main with the lock order reversed and a second (inverse) flag loop; the trailing
//   `if (IsActiveThreadTerminating()) return;` leaves a bare call before the epilogue.
namespace NScript { class CPredicateBase; }
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_PrisonEscapeRescueMotherScript {
public:
    unsigned char _pad00[0x67];
    bool          MotherRescued;                         // +0x67
public:
    class CTortureDoorCourtyard;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_PrisonEscapeRescueMotherScript;
class CQ_SunnyvaleMasterData;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_PrisonEscapeRescueMotherScript::CTortureDoorCourtyard : public CEntityScriptBase {
public:
        CQ_PrisonEscapeRescueMotherScript* ParentClass;      // +0x14
    CQ_SunnyvaleMasterData*            PMasterData;      // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_PrisonEscapeRescueMotherScript::CTortureDoorCourtyard::Main()
{
    Game->EntitySetAsLocked(Me, false);
    while (!ParentClass->MotherRescued) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsLocked(Me, true);
    while (ParentClass->MotherRescued) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_055_HINT_ESCAPE_VIA_SECRET_PASSAGE");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}