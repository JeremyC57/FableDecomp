// NThingShot::GetCreatureFirePosAndOrientation(const CThing& creature, C3DVector& pos, CRightHandedSet& orientation) -- where a creature's shot starts and which way it faces: on the carried projectile weapon (or the creature) the current action's strike-event helper point, else the SHOT_CREATION_DUMMY dummy object, else the SHOT_CREATION_POINT helper (with the thing's physics orientation); failing all three, a point on the creature's facing at its physics radius, at head height (0.2 above it)
// Leaf: ?GetCreatureFirePosAndOrientation@NThingShot@@YIXABVCThing@@AAVC3DVector@@AAVCRightHandedSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BF000, 679 bytes (+9 int3 to 0x007BF2B0), __fastcall (creature in ecx, pos in edx,
//   orientation on the stack), `ret 4`.
// Identity (proven, two sources): Ego_r.pdb 0x007BDEF0 / FableWin.pdb
//   ?GetCreatureFirePosAndOrientation@NThingShot@@YIXABVCThing@@AAVC3DVector@@AAVCRightHandedSet@@@Z (the only NThingShot function with
//   a C3DVector& and a CRightHandedSet& out pair; 2 registers + 1 stack word = `ret 4`); callers 0x007C0B56 (NThingShot::CreateShot
//   0x007C0AD0, which passes the position and set it then hands to the shot), 0x00844585, 0x0090583B.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const 0x004365B0 (PeekTC of TCI_CARRYING 70 / TCI_WEAPON 72),
//   CTCCarrying::GetThingInPrimarySlot 0x006A1440, CTCWeapon::IsProjectileWeapon 0x005DAA40, the current action's virtual slots 22
//   IsActionWithStrikeEventHelperName / 23 GetStrikeEventHelperName (CCharString by hidden pointer), ~CCharString 0x0099EAE0,
//   CTCGraphicAppearance::GetHelperPoint 0x004C0560 / GetDummyObject 0x004C08C0, the physics TC's slots 72 GetRHSet (+0x120) / 70
//   GetFacingVector (+0x118), CThingCreatureBase::GetHeadPos 0x00661B40, CThing::GetPos 0x004C73D0.
// Data: SHOT_CREATION_DUMMY 0x013BACB4 ("SHOT_CREATION_DUMMY", initializer 0x0121EA20) and SHOT_CREATION_POINT 0x013BACB8
//   ("SHOT_CREATION_POINT", 0x0121EA40): both PDBs list `dynamic initializer for 'SHOT_CREATION_DUMMY'` / `'SHOT_CREATION_POINT'`;
//   0.2f 0x012305A0. The creature test is ThingTypeFlags bit 3 (the CThingCreatureBase bit: the same test guards GetHeadPos).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEADPOS
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

extern CCharString SHOT_CREATION_DUMMY;                       // 0x013BACB4
extern CCharString SHOT_CREATION_POINT;                       // 0x013BACB8

namespace NThingShot {
void __fastcall GetCreatureFirePosAndOrientation(const CThing& creature, C3DVector& pos, CRightHandedSet& orientation)
{
    const CThing* source = &creature;
    const CTCCarrying* carrying;
    if (creature.PeekTC(&carrying)) {
        const CThing* weapon = carrying->GetThingInPrimarySlot();
        const CTCWeapon* weaponTC;
        if (weapon && weapon->PeekTC(&weaponTC) && weaponTC->IsProjectileWeapon())
            source = weapon;
    }
    const NGraphicAppearance::CTCGraphicAppearance* appearance = source->GetTCGraphicAppearance();
    if (source->GetTypeFlags() & 8) {
        const CCreatureActionBase* action = static_cast<const CThingCreatureBase*>(source)->PeekPCurrentAction();
        if (action && action->IsActionWithStrikeEventHelperName()) {
            bool found = appearance->GetHelperPoint(action->GetStrikeEventHelperName(), 0, 0, pos, 1.0f);
            if (found) {
                orientation = source->GetPhysicsTC()->GetRHSet();
                return;
            }
        }
    }
    if (appearance->GetDummyObject(SHOT_CREATION_DUMMY, 0, 0, pos, orientation, 1.0f))
        return;
    if (appearance->GetHelperPoint(SHOT_CREATION_POINT, 0, 0, pos, 1.0f)) {
        orientation = source->GetPhysicsTC()->GetRHSet();
        return;
    }
    float height = 0.2f;
    if (creature.GetTypeFlags() & 8)
        height = static_cast<const CThingCreatureBase&>(creature).GetHeadPos(false).Z;
    pos = creature.GetPos();
    pos.Z = height;                                       // stored first, then raised (0x007BF1EB / 0x007BF1F3)
    if (pos.Z < height + 0.2f)
        pos.Z = height + 0.2f;
    C3DVector facing = creature.GetPhysicsTC()->GetFacingVector();
    orientation = CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), facing);
    pos += facing * creature.GetPhysicsTC()->GetRadius();
}
}