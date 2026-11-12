// CTCTavernGameCardBase::GetRandomCard(long&, long&, long, long) -- does nothing
// Leaf: ?GetRandomCard@CTCTavernGameCardBase@@UAEXAAJ0JJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F09B0, thiscall, `ret 16`.
// Identity: slot 58 of CTCTavernGameCardBase's vtable 0x01288294 (defined here: the slot holds this body in 3 vtable(s), all of CTCTavernGameCardBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 36 TLC-named slots of that vtable puts GetRandomCard(long &, long &, long, long) at this slot; Ego_r/FableWin name set: ?GetRandomCard@CTCTavernGameCardBase@@UAEXAAJ0JJ@Z; `ret` matches the signature.
// Header: engine/card_sorting_types.h (declares the virtual; opt-ins as in the landed row 0x007D5070).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_game.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
void CTCTavernGameCardBase::GetRandomCard(long&, long&, long, long)
{
}
