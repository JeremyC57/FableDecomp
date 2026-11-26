// CInputProcessParalysed::Init() -- clear the button-mash count when the process is added
// Leaf: ?Init@CInputProcessParalysed@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00690D20; CInputProcessParalysed vftable slot 2 (RTTI; NF477 audit: one `.rdata` dword, no call).
// Identity: Ego_r.pdb + FableWin.pdb ?Init@CInputProcessParalysed@@UAEXXZ (public virtual, the CAInputProcess slot 2 override).
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the stand-in ?Init@CInputProcessParalysed_Methods@@QAEXXZ (owner rule 2026-11-05 stand-in
//   class named after the real class); respelt onto engine/CInputProcessParalysed.h FABLE_RETAIL_INPUTPROCESS_VTABLE. No body change.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessParalysed.h"

void CInputProcessParalysed::Init()
{
    ButtonsMashed = 0;
}
