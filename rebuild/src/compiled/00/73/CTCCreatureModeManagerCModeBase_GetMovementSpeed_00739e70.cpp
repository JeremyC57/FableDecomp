// CTCCreatureModeManager::CModeBase::GetMovementSpeed(const C3DVector&, const C3DVector&, EMovementType) const -- the base mode does not move: 0.0f
// Leaf: ?GetMovementSpeed@CModeBase@CTCCreatureModeManager@@UBEMABVC3DVector@@0W4EMovementType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00739E70, 9 bytes: `fld dword ptr [0x0122DEDC]` (0.0f) ; `ret 0Ch`.
// NF240 (q12674): the CModeBase vtable slot 7 (0x012673CC; 25 vtable references, no code reference). Was the placeholder
//   ?vfunc_7@CClass_00739e70@@QAEMJJJ@Z; now the PDB member on engine/CTCCreatureModeManager.h (CMODEBASE_VIRTUALS): Ego_r.pdb
//   0x0051C5F0 / FableWin.pdb 0x01DD6780 / Ego_d.pdb ?GetMovementSpeed@CModeBase@CTCCreatureModeManager@@UBEMABVC3DVector@@0W4EMovementType@@@Z,
//   the spelling the sit-bench vtable 0x01267BFC (row 0x00740040) binds at slot 7.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"

float CTCCreatureModeManager::CModeBase::GetMovementSpeed(const C3DVector&, const C3DVector&, EMovementType) const
{
    return 0.0f;
}
