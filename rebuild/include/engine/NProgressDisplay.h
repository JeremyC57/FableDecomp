#ifndef FABLE_ENGINE_NPROGRESSDISPLAY_H
#define FABLE_ENGINE_NPROGRESSDISPLAY_H
// NEW (agentNF11, row 0x006A9DD0 CThingPlayerCreature::ConstructFromParams): the loading-progress namespace functions.
// Ego_r.pdb / FableWin.pdb ?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z (__fastcall: the text in ecx, the first bool in dl,
// progress and the second bool on the stack), out of line in retail at 0x009E9F40 (the landed 0x004350D0 declares the same spelling
// locally). Declaration only.
#include "rebuild_abi.h"
class CCharString;
namespace NProgressDisplay {
void FABLE_FASTCALL DisplayProgress(const CCharString& text, float progress, bool flag1, bool flag2);   // 0x009E9F40
#ifdef FABLE_RETAIL_NPROGRESSDISPLAY_STARTPROGRESS
// Opt-in (chunk 2052, row 0x004A21F0): Ego_r.pdb / FableWin.pdb ?StartProgress@NProgressDisplay@@YIX_N00000@Z (__fastcall: the first two
// bools in cl / dl, four pushed), out of line at 0x009E9EF0. Declaration only.
void FABLE_FASTCALL StartProgress(bool b1, bool b2, bool b3, bool b4, bool b5, bool b6);   // 0x009E9EF0
#endif
}
#endif
