// CTCTrapBase::IsCurrentTrapToCauseKnockdown() const -- every trap knocks down: `mov al,1 ; ret`.
// Leaf: ?IsCurrentTrapToCauseKnockdown@CTCTrapBase@@QBE_NXZ
// NF37 (2026-11-16): moved off the free `fn_operator` stand-in onto the shared engine/CTCTrapBase.h (Ego_r.pdb 0x0078EF7D / FableWin.pdb
// 0x0266AF4F ?IsCurrentTrapToCauseKnockdown@CTCTrapBase@@QBE_NXZ; the PDB copy is the same 3-byte body inside the CTCTrapBase range).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCTRAPBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCTrapBase.h"

bool CTCTrapBase::IsCurrentTrapToCauseKnockdown() const
{
    return true;
}
