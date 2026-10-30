// NScript::CQ_PrisonEscapeRescueMotherScript::CCellDoorHero::Main() -- the hero's cell door: untargetable until the hero has stolen the prison key, then targetable; once the hero uses it, mark HeroEscapedCell, open the door and activate Q_PrisonEscapeFromCell
// Leaf: ?Main@CCellDoorHero@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD8DE0, 259 bytes, thiscall, plain `ret` (two exits).
// Identity (proven): the row is referenced once, from slot 1 (Main) of the entity-script vtable 0x012DC4F4, which the landed
//   NScript::CQ_PrisonEscapeRescueMotherScript::CCellDoorHero::Alloc 0x00DD8D20 stores; Ego_r.pdb
//   ?Main@CCellDoorHero@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ (0x00E10120, right before its Alloc 0x00E10290).
//   Catalog label sub_00dd8de0. Sibling: the landed CPrisonKeyDoor::Main 0x00DD9180 (same quest, same PrisonKeyStolenByHero wait).
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30 (Ego_r `QBE`), CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0; CScriptThing slot 20 MsgIsUsedBy; CGameScriptInterface slots 7 NewScriptFrame,
//   276 ActivateQuest, 425 OpenDoor, 469 EntitySetTargetable.
// Literals: "SCRIPT_NAME_HERO" 0x0125D1C8, "Q_PrisonEscapeFromCell" 0x012DC510.
// Layout: Ego_r CCellDoorHero (0x1c): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18.
//   CQ_SunnyvaleMasterData (no shift): PrisonKeyStolenByHero +0xee. CQ_PrisonEscapeRescueMotherScript at Ego_r - 0x14:
//   HeroEscapedCell +0x78 (Ego_r 0x8c).
// Shape: a rotated wait on the key flag; the used-by wait is a do-while whose top tests MsgIsUsedBy (the CCharString
//   temporary's result kept raw in bl across its destructor) and whose then-arm (the success tail, which returns) VC7.1
//   moves out of the loop; the `while (!IsActiveThreadTerminating())` exit gets its own `pop ebx ... ret` copy.
//   `while (!used)` and `for (;;) { if (used) break; ... }` both rotate the loop (the test is duplicated, 298v259).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C (slot 0 = the CBaseClass dtor)
public:
    virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015();
    virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019();
    virtual bool MsgIsUsedBy(const CCharString& name) const;      // 0x004AAB10 (Ego_r ?MsgIsUsedBy@CScriptThing@@UBE_NABVCCharString@@@Z)   // slot 20 (+0x50)
    CCountedPointer<CScriptThing> PImp;                 // +0x04
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CEntityScriptBase {                               // retail 0x14 bytes
public:
    virtual ~CEntityScriptBase();
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData* MasterScriptData;                            // +0x44
};
namespace NScript {
class CQ_SunnyvaleMasterData {                          // Ego_r layout, no shift up to here
public:
    unsigned char _before[0xee];
    bool PrisonKeyStolenByHero;                         // +0xee
};
class CQ_PrisonEscapeRescueMotherScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long CurrentTask;                                   // +0x48
    long TasksDone[3];                                  // +0x4c
    long TargetNumber[3];                               // +0x58
    bool HeroAgreesToTask;                              // +0x64
    bool HeroHasFoundWeapons;                           // +0x65
    bool MotherCellDoorOpen;                            // +0x66
    bool MotherFollowing;                               // +0x67
    bool MotherDied;                                    // +0x68
    bool WatchingHero;                                  // +0x69
    bool PrisonCutsceneFinished;                        // +0x6a
    bool RaceStarting;                                  // +0x6b
    long YearsPassedInCell;                             // +0x6c
    long PrisonerConversationIndex;                     // +0x70
    long ConversationIndex;                             // +0x74
    bool HeroEscapedCell;                               // +0x78
    class CCellDoorHero : public CEntityScriptBase {
    public:
        CQ_PrisonEscapeRescueMotherScript* ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    private:
        virtual void Main();                            // slot 1
    };
};

void CQ_PrisonEscapeRescueMotherScript::CCellDoorHero::Main()
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
            ParentClass->HeroEscapedCell = true;
            Game->OpenDoor(Me);
            Game->ActivateQuest("Q_PrisonEscapeFromCell");
            return;
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
}   // namespace NScript