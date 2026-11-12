// CThing::SetCurrentMapNumber(long) -- store the map number (truncated to the short CurrentMapNumber)
// Leaf: ?SetCurrentMapNumber@CThing@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C7580.
// Naming (agentNF31, 2026-11-12): was the __fastcall stand-in CThing_SetCurrentMapNumber(CThing*, int, short) over a local CThing
//   model. Ego_r.pdb 0x004C9140 (egor_signatures.tsv) + FableWin.pdb 0x01CF8A20 ?SetCurrentMapNumber@CThing@@QAEXJ@Z; engine/CThing.h
//   declares it at 0x004C7580 (FABLE_RETAIL_CTHING_STATEACCESS) with CurrentMapNumber (short) in the layout; the landed caller
//   0x006B0C10 binds this symbol.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "fable_string.h"
#include "engine/CThing.h"

void CThing::SetCurrentMapNumber(long mapNumber)
{
    CurrentMapNumber = (short)mapNumber;
}
