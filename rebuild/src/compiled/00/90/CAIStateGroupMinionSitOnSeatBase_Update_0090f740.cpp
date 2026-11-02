// CAIStateGroup_MinionSitOnSeatBase::Update() -- walk to the seat and sit down; while sitting, idle as long as the state group stays valid; then Terminate()
// Leaf: ?Update@CAIStateGroup_MinionSitOnSeatBase@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F740, length 47 bytes, tail jump (virtual, no arguments).
// Identity: slot 12 of vtable 0x0128B584 (RTTI CAIStateGroup_MinionSitOnSeatBase : CAIStateGroup_UsesSeat : CAIStateGroupBase :
//   CBaseClass); the other five references are the same slot of the derived MinionSitAt*/SitOnSeat vtables, which inherit it.
//   Ego_r + FableWin ?Update@CAIStateGroup_MinionSitOnSeatBase@@MAEXXZ (protected virtual); the same vtable's slots 9 / 11 / 14 / 17 /
//   18 are Cleanup / IsRunnable / IsStateGroupStillValid / IsOwnerSubjectToRegulation / GetSeat (manifest, PDB name sets).
// Callees: CAIStateGroup_UsesSeat::MoveToSeatAndSitDown(EMovementType) @ 0x009387C0 (Ego_r + FableWin protected `IAE_NW4EMovementType@@`),
//   CAIStateGroupBase::AIAction_DoNothing() @ 0x0097E200 (both PDBs, protected, EAIActionReturn), the virtual
//   IsStateGroupStillValid() const (slot 14, +0x38; both PDBs `MBE_NXZ`), CAIStateGroupBase::Terminate() @ 0x008FCF40 (tail jump).
// Shape: the movement-type argument is pushed before `this` moves to esi; the while condition is rotated (one test before the
//   loop, the DoNothing == 4 test and the virtual at the loop head, 16-aligned with `mov edi,edi`).
#include "engine/CBaseClass.h"

enum EAIActionReturn { };
enum EMovementType { };

class CAIStateGroupBase : public CBaseClass { public:        // retail 0x14 bytes (engine/CAIStateGroupBase.h: PBrain +4 .. Enabled +0x12)
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
protected:
    virtual void Update();                                   // slot 12 (+0x30)
    virtual void v13();
    virtual bool IsStateGroupStillValid() const;             // slot 14 (+0x38)
public:
    void Terminate();                                        // 0x008FCF40
protected:
    EAIActionReturn AIAction_DoNothing();                    // 0x0097E200
    unsigned char _body[0x14 - 4];
};

class CAIStateGroup_UsesSeat : public CAIStateGroupBase { public:
protected:
    bool MoveToSeatAndSitDown(EMovementType movementType);   // 0x009387C0
};

class CAIStateGroup_MinionSitOnSeatBase : public CAIStateGroup_UsesSeat { public:   // vtable 0x0128B584
protected:
    virtual void Update();
    virtual bool IsStateGroupStillValid() const;
};

void CAIStateGroup_MinionSitOnSeatBase::Update()
{
    if (MoveToSeatAndSitDown((EMovementType)3)) {
        while (AIAction_DoNothing() == 4 && IsStateGroupStillValid())
            ;
    }
    Terminate();
}