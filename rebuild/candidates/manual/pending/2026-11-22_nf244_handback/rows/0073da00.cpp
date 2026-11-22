// CModeStandBase::IsAllowingPlayerSpecialMovementMoves() const -- the stand / walk modes allow the player's special movement moves: true.
// Leaf: ?IsAllowingPlayerSpecialMovementMoves@CModeStandBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073DA00, 3 bytes (`mov al,1 ; ret`); CModeBase slot 9 of every CModeStandBase-derived vtable
//   (0x012857A8 0x01285860 0x012681EC 0x01267D10 0x01278338 0x01285900 0x01268774, NF242 q12677).
// NF244 (q12681): the self row moves off its free `__fastcall` stand-in onto engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE
//   declares the override): Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?IsAllowingPlayerSpecialMovementMoves@CModeStandBase@@UBE_NXZ, the
//   spelling every landed vtable emitter already binds at this address.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"

bool CModeStandBase::IsAllowingPlayerSpecialMovementMoves() const
{
    return true;
}
