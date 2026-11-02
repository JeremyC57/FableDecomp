// CTCBase::OnDie() @ 0x006868E0 -- the empty base slot (a bare `ret`)
// Leaf: ?OnDie@CTCBase@@UAEXXZ
// Naming wave 31 (family tcb): the local one-slot CTCBase is engine/CTCBase.h's retail branch (FABLE_RETAIL_CTCBASE; DECISIONS_W30 D5)
// Naming wave 30 (family c699): Ego_r.pdb / FableWin.pdb ?OnDie@CTCBase@@UAEXXZ, CTCBase vtable 0x0125BE0C slot 13 (OnDie in the CTCBase
//   virtual order: OnDie 13, OnKill 14 (chunk 695); every derived slot-13 body of the hierarchy is an OnDie and tails here: CTCInventoryItem
//   0x005D8EC0, CTCScriptedControl 0x00713740, CTCKrakenTentacle 0x007F0890). Was the placeholder struct C006868e0 (?OnDie@C006868e0@@QAEXXZ).
// Layout: engine/CTCBase.h's retail branch (CTCBase : CBaseClassNonCopyable, vtable 0x0125BE0C, OnDie slot 13).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                              // : CBaseClassNonCopyable, vtable 0x0125BE0C; slot 13 OnDie 0x006868E0

void CTCBase::OnDie()
{
}