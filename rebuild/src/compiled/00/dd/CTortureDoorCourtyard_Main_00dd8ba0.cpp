// CTortureDoorCourtyard::Main() -- keep the courtyard torture door unlocked until the mother is rescued; then lock it and, for as long as the mother stays rescued, show the "escape via the secret passage" hint whenever the hero uses it
// Leaf: Main
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
class CScriptThing { public:                                    // 12 bytes, vtable 0x01238C8C
    virtual void s000() const; virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const;
    virtual void s006() const; virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const;
    virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const; virtual void s017() const;
    virtual void s018() const; virtual void s019() const;
    virtual bool MsgIsUsedBy(const CCharString& user) const;   // slot 20 (+0x50) 0x004AAB10
    void* PImpData;                                      // +0x04
    void* PImpInfo;                                      // +0x08
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CQ_PrisonEscapeRescueMotherScript {
public:
    unsigned char _pad00[0x67];
    bool          MotherRescued;                         // +0x67
};
class CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CTortureDoorCourtyard : public CEntityScriptBase {
public:
    void*                              __vftable;        // +0x00
    CGameScriptInterface*              Game;             // +0x04
    CScriptThing                       Me;               // +0x08
    CQ_PrisonEscapeRescueMotherScript* ParentClass;      // +0x14
    CQ_SunnyvaleMasterData*            PMasterData;      // +0x18
    void Main();
};

void CTortureDoorCourtyard::Main()
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