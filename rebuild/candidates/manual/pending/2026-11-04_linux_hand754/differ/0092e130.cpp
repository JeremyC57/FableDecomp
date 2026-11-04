// CAIStateGroup_MinionSleep::IsRunnable() -- runnable straight away on the sleep component's fast track; otherwise only when it is ready to sleep, the go-to-bed group is runnable and the creature already stands on the bed position facing the bed's direction
// Grade: DIFFER(229v229) -- this/CTCSleep register swap (retail this esi, map+sleep edi); retail headers + PDB names (GetTC<CTCSleep>, IsReadyToSleep, GoToBed BedPos/Rhset, PhysicsTC GetRHSet slot 72) and 9 control/local forms: unchanged (best b)
// Leaf: ?IsRunnable@CAIStateGroup_MinionSleep@@UAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092E130, 229 bytes; Ego_r.pdb ?IsRunnable@CAIStateGroup_MinionSleep@@UAE_NXZ (slot 11).
// Callees: CVectorMap LowerBound 0x0040F020 (inside CThing::GetTC<CTCSleep>, interface 0xD4), CTCSleep::GetFastTrack 0x0092C3B0
//   (Ego_r.pdb ?GetFastTrack@CTCSleep@@QAE_NXZ), CAIStateGroup_GoToBed::IsRunnable 0x0092D6F0 (qualified), CThing::GetPos const
//   0x004C73D0, CTCPhysicsBase::GetRHSet() const (virtual slot 72, +0x120: the reversed GetRHSet overload pair after
//   GetFacingVector 69/70).
// Layout: Ego_r.pdb CTCSleep IsReadyToSleep +0x0C; CAIStateGroup_GoToBed (0x60) BedPos +0x3C, Rhset +0x48 (Forward +0x54).
// Shape: each distance is `C2DVector(a.X - b.X, a.Y - b.Y).GetMagnitude()` against 0.01 / 0.1.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C2DVECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
class CTCSleep : public CTCBase {                       // no shared header
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0xd4; }
    bool GetFastTrack();                                // 0x0092C3B0
    bool IsReadyToSleep;                                // +0x0c
    bool FastTrack;                                     // +0x0d
};
class CAIStateGroup_GoToBed : public CAIStateGroupBase {
public:
    virtual bool IsRunnable();                          // 0x0092D6F0
protected:
    CIntelligentPointer<CThing> PBed;                   // +0x14
    bool IsBedSleepPointMarker;                         // +0x1c
    long SleepStartTimeVariation;                       // +0x20
    long SleepEndTimeVariation;                         // +0x24
    bool BedToRightOfCreature;                          // +0x28
    long ActionPointIndex;                              // +0x2c
    CCharString BedSide;                                // +0x30
    bool PlayEmergencyGetOutOfBed;                      // +0x34
    long BedGetOutOfBedFastAnim;                        // +0x38
    C3DVector BedPos;                                   // +0x3c
    CRightHandedSet Rhset;                              // +0x48
};
class CAIStateGroup_MinionSleep : public CAIStateGroup_GoToBed {
public:
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionSleep::IsRunnable()
{
    CTCSleep* sleep;
    if (!PBrain->Creature->GetTC(&sleep))
        return false;
    if (!sleep->GetFastTrack()) {
        if (!sleep->IsReadyToSleep || !CAIStateGroup_GoToBed::IsRunnable())
            return false;
        const C3DVector& pos = PBrain->Creature->GetPos();
        if (C2DVector(BedPos.X - pos.X, BedPos.Y - pos.Y).GetMagnitude() > 0.01f)
            return false;
        const C3DVector& facing = PBrain->Creature->GetPhysicsTC()->GetRHSet().GetForward();
        if (C2DVector(Rhset.GetForward().X - facing.X, Rhset.GetForward().Y - facing.Y).GetMagnitude() > 0.1f)
            return false;
    }
    return true;
}
