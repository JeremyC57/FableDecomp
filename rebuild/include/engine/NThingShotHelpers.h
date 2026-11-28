#ifndef FABLE_ENGINE_NTHINGSHOTHELPERS_H
#define FABLE_ENGINE_NTHINGSHOTHELPERS_H
#include "engine/C3DVector.h"
class CThing;
// Both original PDBs; actual donor026fb6e0 and native007bedf0.
namespace NThingShot {C3DVector FABLE_FASTCALL GetCreatureFireAtPos(const CThing& creature);}
#ifdef FABLE_RETAIL_ACTION_EVENT_SHOT_PASS30
class CRightHandedSet;
class CMeleeCombatAbilityDef;
template<class T> class CDefPointer;
namespace NThingShot {
// The two PDBs give the existing-thing and global-index overloads, 0x007C0E80 and 0x007C0AD0.
CThing* FABLE_FASTCALL CreateShot(CThing&, CThing*, CThing*, const CThing*, float, float, float,
    const CDefPointer<const CMeleeCombatAbilityDef>&, long);
CThing* FABLE_FASTCALL CreateShot(long, CThing*, CThing*, const CThing*, float, float, float,
    const CDefPointer<const CMeleeCombatAbilityDef>&, const C3DVector*, const CRightHandedSet*);
}
#endif
#ifdef FABLE_RETAIL_DRAGON_FIREBALL_MOVEMENT_PASS30
namespace NThingShot {
C3DVector FABLE_FASTCALL RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed);
}
#endif
#endif
