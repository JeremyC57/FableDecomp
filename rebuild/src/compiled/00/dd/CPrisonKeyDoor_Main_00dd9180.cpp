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

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0xee];
    bool PrisonKeyStolenByHero;                        // +0xee
};

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

class CPrisonKeyDoor : public CEntityScriptBase {
public:
        void*                   ParentClass;               // +0x14 (CQ_PrisonEscapeRescueMotherScript*)
    CQ_SunnyvaleMasterData* PMasterData;               // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
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