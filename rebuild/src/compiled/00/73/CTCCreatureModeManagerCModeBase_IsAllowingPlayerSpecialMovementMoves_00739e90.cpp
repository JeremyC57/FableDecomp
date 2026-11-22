// CTCCreatureModeManager::CModeBase::IsAllowingPlayerSpecialMovementMoves() const -- the base mode allows none: false
// Leaf: ?IsAllowingPlayerSpecialMovementMoves@CModeBase@CTCCreatureModeManager@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00739E90, 3 bytes: `xor al, al` ; `ret`.
// NF240 (q12674): the CModeBase vtable slot 9 (0x012673CC; 29 vtable references, no code reference). Was the global stand-in
//   ?IsAllowingPlayerSpecialMovementMoves@CModeBase@@QAE_NXZ; now the PDB member on engine/CTCCreatureModeManager.h (CMODEBASE_VIRTUALS):
//   Ego_r.pdb / FableWin.pdb 0x01DD67A0 / Ego_d.pdb ?IsAllowingPlayerSpecialMovementMoves@CModeBase@CTCCreatureModeManager@@UBE_NXZ (public
//   virtual const), the spelling the sit-bench vtable 0x01267BFC (row 0x00740040) binds at slot 9.
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

bool CTCCreatureModeManager::CModeBase::IsAllowingPlayerSpecialMovementMoves() const
{
    return false;
}
