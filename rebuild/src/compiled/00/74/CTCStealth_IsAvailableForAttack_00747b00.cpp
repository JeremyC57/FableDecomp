// CTCStealth::IsAvailableForAttack() const -- the creature can be attacked when no attacker limit is exceeded
// Leaf: ?IsAvailableForAttack@CTCStealth@@QBE_NXZ
// NF48 (round 7, follower of 0x00747890): the callee 0x00747890 is CTCStealth::GetNumberOfAttackersOverMaximum(vector<const CThing*>*)
//   const (Ego_r.pdb, private; the landed row), called with a null list (`push 0`); the made-up stand-in GetAttackAvailability(bool) is
//   gone. The list type is STLport 4.6.2 std::vector<const CThing*> from the shared stlport/fable_containers.h.
// NF56 (round 7, queue q10431): Ego_r.pdb 0x0075A0C0 / FableWin.pdb 0x02592B00 ?IsAvailableForAttack@CTCStealth@@QBE_NXZ (const). The row
//   moved onto engine/CTCStealth.h, which declares the full Ego_r class (IsAvailableForAttack public const, GetNumberOfAttackersOverMaximum
//   private const); was a local CTCStealth model with a non-const IsAvailableForAttack.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCStealth.h"

bool CTCStealth::IsAvailableForAttack() const
{
    return GetNumberOfAttackersOverMaximum(0) == -1;
}
