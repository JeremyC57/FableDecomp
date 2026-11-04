// CAIStateGroup_SitForMeal::Update() -- while the creature can eat, take food from a live cooking pot and either sit on a free chair (eating while the do-nothing action keeps running and the state group stays valid) or stand around eating; then terminate
// Leaf: ?Update@CAIStateGroup_SitForMeal@@MAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945BD0, 219 bytes; vtable 0x0128BD34 slot 12 (Ego_r.pdb ?Update@CAIStateGroup_SitForMeal@@MAEXXZ).
// Callees: CAIStateGroupBase::Terminate 0x008FCF40 (both exits), CAIStateGroup_SitForMeal::GetPCookingPot const 0x00945A90,
//   CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / ~CBaseIntelligentPointer 0x00A01C10 (the
//   converting CIntelligentPointer<CThing> temporary and its inline operator=), GetFoodFromCookingPot 0x009456A0, GetPFreeChair const
//   0x00945B10, ReleaseControlOfPot 0x00945530, CAIStateGroup_UsesSeat::SetSeat 0x009106E0 / MoveToSeatAndSitDown(EMovementType)
//   0x009387C0, CAIStateGroupBase::AIAction_DoNothing 0x0097E200, virtual IsStateGroupStillValid const (slot 14, +0x38: 0x00945610
//   tests IsMealTime), StandAroundAndEat 0x009454C0 -- all Ego_r.pdb spellings.
// Layout: CAIStateGroup_UsesSeat (0x34: CAIStateGroupBase 0x14 + its seat members), PCookingPot +0x34; CThing bit 0x46 of the
//   interface bitset (`test byte [creature+28h],40h`).
// Shape: the eat loop is `do { if (AIAction_DoNothing() != 4) break; } while (IsStateGroupStillValid());`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
class CThingObject;
enum EMovementType;                                     // MS forward enum
class CAIStateGroup_UsesSeat : public CAIStateGroupBase {
protected:
    void SetSeat(CThingObject* seat);                   // 0x009106E0
    bool MoveToSeatAndSitDown(EMovementType movement);  // 0x009387C0
    unsigned char _pad_0x14[0x34 - 0x14];               // +0x14 the seat members (not modelled)
};
class CAIStateGroup_SitForMeal : public CAIStateGroup_UsesSeat {
protected:
    virtual void Update();                              // slot 12
    virtual bool IsStateGroupStillValid() const;        // slot 14 (+0x38)
private:
    bool StandAroundAndEat();                           // 0x009454C0
    void ReleaseControlOfPot();                         // 0x00945530
    bool GetFoodFromCookingPot();                       // 0x009456A0
    CThing* GetPCookingPot() const;                     // 0x00945A90
    CThingObject* GetPFreeChair() const;                // 0x00945B10
    CIntelligentPointer<CThing> PCookingPot;            // +0x34
};

void CAIStateGroup_SitForMeal::Update()
{
    if (!PBrain->Creature->HasTC((ETCInterfaceType)0x46)) {
        Terminate();
        return;
    }
    CThing* pot = GetPCookingPot();
    if (pot != 0 && !pot->IsDead()) {
        PCookingPot = pot;
        if (!GetFoodFromCookingPot()) {
            Terminate();
            return;
        }
        CThingObject* chair = GetPFreeChair();
        if (chair != 0) {
            ReleaseControlOfPot();
            SetSeat(chair);
            if (MoveToSeatAndSitDown((EMovementType)3)) {
                do {
                    if (AIAction_DoNothing() != 4)
                        break;
                } while (IsStateGroupStillValid());
            }
        } else {
            StandAroundAndEat();
        }
    }
    Terminate();
}