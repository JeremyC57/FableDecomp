// CAIStateGroup_PrepareMeal::IsRunnable() -- forget the cooking pot and action point, then (for a creature with a home, at meal-preparation time) pick a free cooking pot and claim a free action point on it; runnable while the claimed pot is alive
// Leaf: ?IsRunnable@CAIStateGroup_PrepareMeal@@MAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00946400, 284 bytes; vtable slot 11 (Ego_r.pdb ?IsRunnable@CAIStateGroup_PrepareMeal@@MAE_NXZ).
// Callees: CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10 (the clear through an
//   inline CIntelligentPointer<CThing> temporary, vtable 0x01238C6C), CThingAICreature::GetHomeBuilding() const (inline, HomeBuilding
//   +0x168), IsTimeToPrepareMeal() const 0x009462A0 and GetFreeCookingPot() const 0x00946380 (Ego_r.pdb, private), CVectorMap
//   LowerBound 0x0040F020 (GetTC(TCI_SINGLE_USER_ACTION_POINTS = 45), bit 13 of +0x24), CTCUserActionPointBase::GetFreeActionPoint
//   (CThing&, C3DVector&, CRightHandedSet&, long&) 0x007FEDE0 (Ego_r.pdb), and the claim `PCookingPot = pot;`, whose converting
//   temporary CIntelligentPointer<CThing>(CThing*) 0x004A96F0 and operator= 0x004A9760 VC7.1 keeps out of line at this site.
// Layout: Ego_r.pdb CAIStateGroup_PrepareMeal (0x44): PCookingPot +0x14, ActionPointIndex +0x1c, ActionPointPos +0x20,
//   ActionOrientation (CRightHandedSet) +0x2c.
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
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThingBuilding.h"
// FableWin.pdb ?GetHomeBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ, inline in retail (declared in engine/CThingAICreature.h).
inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }

class CTCUserActionPointBase : public CTCBase {         // no shared header
public:
    bool GetFreeActionPoint(CThing& user, C3DVector& pos, CRightHandedSet& orientation, long& index);   // 0x007FEDE0
};
class CAIStateGroup_PrepareMeal : public CAIStateGroupBase {   // engine/CAIStateGroup_PrepareMeal.h is the flat Ego_r layout
protected:
    virtual bool IsRunnable();                          // slot 11
private:
    bool IsTimeToPrepareMeal() const;                   // 0x009462A0
    CThing* GetFreeCookingPot() const;                  // 0x00946380
    CIntelligentPointer<CThing> PCookingPot;            // +0x14
    long                        ActionPointIndex;       // +0x1c
    C3DVector                   ActionPointPos;         // +0x20
    CRightHandedSet             ActionOrientation;      // +0x2c
};

bool CAIStateGroup_PrepareMeal::IsRunnable()
{
    PCookingPot = 0;
    ActionPointIndex = -1;
    if (PBrain->Creature->GetHomeBuilding() && IsTimeToPrepareMeal()) {
        CThing* pot = GetFreeCookingPot();
        if (pot && pot->HasTC(TCI_SINGLE_USER_ACTION_POINTS)) {
            CTCUserActionPointBase& points = static_cast<CTCUserActionPointBase&>(pot->GetTC(TCI_SINGLE_USER_ACTION_POINTS));
            CThingAICreature& creature = *PBrain->Creature;
            if (points.GetFreeActionPoint(creature, ActionPointPos, ActionOrientation, ActionPointIndex))
                PCookingPot = pot;
        }
    }
    if (PCookingPot.Get() && !PCookingPot->IsDead())
        return true;
    return false;
}