// CTCCarrying::RemoveThingInCarrySlot(long slot, bool immediately) -- take the thing out of a carry slot: drop null entries from the primary-slot list, clear the slot, and for a living thing unhook its carryable, drop it from the primary-slot list, end the carrier's weapon-stand mode when no weapon is left, and when the carrier was carrying a generic box stand the thing upright again and make it not solid to the carrier; returns the removed thing
// Leaf: ?RemoveThingInCarrySlot@CTCCarrying@@QAEPAVCThing@@J_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A16A0, 770 bytes (+ int3 padding), thiscall `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?RemoveThingInCarrySlot@CTCCarrying@@QAEPAVCThing@@J_N@Z (callers: the landed RemoveThing 0x006A19B0,
//   CreateWeaponForThingFromDef 0x00663990, GiveThingItemInSlot 0x00895190). The bool is unused in the retail body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCARRYING_MEMBERS
#define FABLE_RETAIL_CTCCARRYING_SLOTACCESS
#define FABLE_RETAIL_CTCCARRYABLE_SETASCARRIEDBY
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(acos)
#endif
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CVectorMap.h"
#include "stlport/fable_containers.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/NCreatureMode.h"

CThing* CTCCarrying::RemoveThingInCarrySlot(long slot, bool)
{
    ThingsInPrimarySlots.remove(0);
    CThing* thing = 0;
    CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<long, CIntelligentPointer<CThing> > >::iterator it = CarrySlots.Find(slot);
    if (it != CarrySlots.end()) {
        CIntelligentPointer<CThing>& carried = it->second;
        thing = carried.Get();
        if (thing && !thing->IsDead()) {
            CTCCarryable* carryable;
            thing->GetTC(&carryable);
            carryable->SetAsCarriedBy(0);
        }
        carried = 0;
        if (thing && !thing->IsDead()) {
            ThingsInPrimarySlots.remove(thing);
            CThing& parent = GetParentThing();
            CTCCreatureModeManager* modes;
            if (parent.GetTC(&modes)) {
                if (!IsThingCarryingWeapon(parent))
                    modes->RemoveMode(NCreatureMode::MODE_CARRYING_WEAPON_STAND);
                if (modes->GetCurrentMode().GetType() == NCreatureMode::MODE_CARRYING_GENERIC_BOX) {
                    modes->RemoveMode(NCreatureMode::MODE_CARRYING_GENERIC_BOX);
                    C3DVector up(0.0f, 0.0f, 1.0f);
                    CRightHandedSet rhSet = thing->GetPhysicsTC()->GetRHSet(1.0f);
                    C3DVector axis(rhSet.GetUp().Y, -rhSet.GetUp().X, 0.0f);
                    float angle = -((float)acos(rhSet.GetUp().Dot(up)) * 0.15915494f);
                    rhSet.Rotate(axis, angle);
                    rhSet.SetUp(up);
                    rhSet.SetForward(C3DVector(rhSet.GetForward().X, rhSet.GetForward().Y, 0.0f));
                    rhSet.MakePerpendicular();
                    thing->GetPhysicsTC()->SetRHSet(rhSet);
                    if (!thing->IsDying()) {
                        CTCSolidToThing* solid;
                        if (thing->HasTC(TCI_SOLID_TO_THING))
                            thing->GetTC(&solid);
                        else
                            solid = (CTCSolidToThing*)thing->AddTC("CTCSolidToThing", false, 0);
                        solid->AddThingNotSolidTo(&GetParentThing());
                    }
                }
            }
        }
    }
    return thing;
}