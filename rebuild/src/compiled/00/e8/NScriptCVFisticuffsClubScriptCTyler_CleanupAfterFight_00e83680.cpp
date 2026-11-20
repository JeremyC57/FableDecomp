// CTyler::CleanupAfterFight() -- releases the club's opponent through the script interface, clears the club script's fight flag, re-enables Tyler's thing (slot 67 with true) and ends the fight for it (interface slot 452)
// Leaf: ?CleanupAfterFight@CTyler@CV_FisticuffsClubScript@NScript@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E83680, length 59 bytes.
// Callees: the script interface at +4, virtual slots 338 (+0x548, the opponent long at +0x7c of the parent
// script) and 452 (+0x710, Tyler's thing); Tyler's script thing at +8, virtual slot 67 (+0x10c, one bool).
// Layout: PDB CTyler (44 bytes): a 20-byte script base (vptr, the script interface +4, the CScriptThing +8),
// ParentClass (CV_FisticuffsClubScript*) +0x14; the club script's flag at +0x53 and opponent at +0x7c are
// modelled locally (0x00E8xxxx quest-script module). Slot names are descriptive.
// Shape: the parent pointer is re-read after the first call for the flag store; the thing's address is
// formed once (`lea edi,[esi+8]`) and reused as `this` of its own call and as the last call's argument.
// NF200 (2026-11-20, q6530): CleanupAfterFight (0x00E83680) and its caller OnPredicateFail (0x00E83620) moved together onto the shared
//   engine/CV_FisticuffsClubScript.h (FABLE_RETAIL_NSCRIPT; the nested NScript::CV_FisticuffsClubScript::CTyler of
//   NScript_CV_FisticuffsClubScript.h, as the landed sibling 0x00E87030 CTyler::Main). Own names: Ego_r.pdb 0x00EE46B0 / 0x00EE49F0 +
//   FableWin.pdb 0x039F68B0 / 0x039F67F0 ?CleanupAfterFight@CTyler@CV_FisticuffsClubScript@NScript@@AAEXXZ (private) and
//   ?OnPredicateFail@CTyler@CV_FisticuffsClubScript@NScript@@EAEXXZ (private virtual); 0x00E87030 already binds the former. The
//   descriptive slot names are the header's PDB virtuals: game interface 70 GetHero, 338 RemoveQuestInfoElement(long), 451
//   GiveThingBestEnemyTarget, 452 ClearThingBestEnemyTarget; CScriptThing 67 SetFriendsWithEverythingFlag, 75 IsAlive. The club
//   script's +0x53 / +0x7C are HeroInFisticuffsClub / HitID (Ego_r.pdb CV_FisticuffsClubScript +0x67 / +0x90, native = donor - 0x14 as
//   the header notes). The row-local CScriptThingCalls / CGameScriptInterface / CV_FisticuffsClubScript / CTyler models are gone.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CV_FisticuffsClubScript.h"
#include "engine/CGameScriptInterfaceBase.h"

void NScript::CV_FisticuffsClubScript::CTyler::CleanupAfterFight()
{
    Game->RemoveQuestInfoElement(ParentClass.HitID);
    ParentClass.HeroInFisticuffsClub = false;
    Me.SetFriendsWithEverythingFlag(true);
    Game->ClearThingBestEnemyTarget(Me);
}
