// CAIStateGroupBase::Terminate() -- mark the state group terminated (Terminated +0x10 = true)
// Leaf: ?Terminate@CAIStateGroupBase@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FCF40, 5 bytes: `mov byte ptr [ecx+10h],1 ; ret`.
// Identity (two sources): Ego_r.pdb and FableWin.pdb ?Terminate@CAIStateGroupBase@@QAEXXZ (public, non-virtual); engine/CAIStateGroupBase.h
//   names it at 0x008FCF40 and Terminated at +0x10 (Ego_r member, the IsStopping 0x008FD010 sibling reads the same byte).
// Name fix (agentNF5 2026-11-10): was the stand-in `C008fcf40::M` (?M@C008fcf40@@QAEXXZ) over a padded local struct; now the shared
//   header's retail class (FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"
void CAIStateGroupBase::Terminate()
{
    Terminated = true;
}
