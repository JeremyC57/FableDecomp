// CPrisonKeyDoor::Main() -- entity script: lock the warden's door until the hero has stolen the prison key; whenever the hero uses it meanwhile show the "door locked" game info, wait for it to be clicked past and pause five seconds; then unlock it
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD9180, length 302 bytes, plain `ret` (entity-script vtable slot at
// 0x012DC548).
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30 (on `this`), CScriptThing::MsgIsUsedBy(const
// CCharString&) (virtual slot 20, +0x50, 0x004AAB10), CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0; through Game (+4): EntitySetAsLocked(const CScriptThing&, bool) (slot 518, +0x818, 0x0089DC10),
// NewScriptFrame (slot 7, +0x1c), DisplayGameInfo(const CCharString&) (slot 115, +0x1cc, 0x008929D0),
// MsgIsGameInfoClickedPast (slot 40, +0xa0, 0x00894370), Pause(float) (slot 376, +0x5e0).
// Literals "SCRIPT_NAME_HERO" 0x0125D1C8, "TEXT_QST_055_WARDENS_DOOR_LOCKED" 0x012DC560.
// Layout = Ego_r.pdb CPrisonKeyDoor (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData
// +0x18 (CQ_SunnyvaleMasterData: PrisonKeyStolenByHero +0xee, PDB offset).
// Shape: three rotated while loops (outer key test, the click-past wait); the used-by result survives the temporary's
// destructor in bl.
namespace NScript { class CPredicateBase; }
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual ~CScriptThing();                           // slot 0
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const;
    virtual bool MsgIsUsedBy(const CCharString& name) const;   // slot 20 (+0x50), 0x004AAB10
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0xee];
    bool PrisonKeyStolenByHero;                        // +0xee
};

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CPrisonKeyDoor : public CEntityScriptBase {
public:
    void*                   __vftable;                 // +0x00
    CGameScriptInterface*   Game;                      // +0x04
    CScriptThing            Me;                        // +0x08
    void*                   ParentClass;               // +0x14 (CQ_PrisonEscapeRescueMotherScript*)
    CQ_SunnyvaleMasterData* PMasterData;               // +0x18
    void Main();
};

void CPrisonKeyDoor::Main()
{
    Game->EntitySetAsLocked(Me, true);
    while (!PMasterData->PrisonKeyStolenByHero) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_055_WARDENS_DOOR_LOCKED");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->Pause(5.0f);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsLocked(Me, false);
}