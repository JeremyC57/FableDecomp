// CTyler::OnPredicateFail() -- when Tyler's thing still answers its slot-75 query, hands the hero to the script interface's slot-451 fight call; then either runs CleanupAfterFight (a fight is in progress in the parent club script) or releases the club's opponent through slot 338
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Leaf: ?OnPredicateFail@CTyler@CV_FisticuffsClubScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E83620, length 87 bytes.
// Callees: Tyler's script thing at +8, virtual slot 75 (+0x12c, bool); the script interface at +4, virtual slots
// 70 (+0x118, the hero handle), 451 (+0x70c, thing + handle) and 338 (+0x548, the opponent long at +0x7c of the
// parent script); CTyler::CleanupAfterFight @ 0x00E83680 (tail jump).
// Layout: PDB CTyler (44 bytes): a 20-byte script base (vptr, the script interface +4, the CScriptThing +8),
// ParentClass (CV_FisticuffsClubScript*) +0x14; the club script's flag at +0x53 and opponent at +0x7c are
// modelled locally (0x00E8xxxx quest-script module). Slot names are descriptive.
// Shape: the nested `m_pGSI->SetFightTarget(Me, m_pGSI->GetHero())` fetches the outer vtable into ebx before
// the inner call (GOTCHAS wave 24n) and re-reads the interface pointer after it; the flag test chooses a
// tail jump to CleanupAfterFight or the release call.
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

void NScript::CV_FisticuffsClubScript::CTyler::OnPredicateFail()
{
    if (Me.IsAlive())
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (ParentClass.HeroInFisticuffsClub)
        CleanupAfterFight();
    else
        Game->RemoveQuestInfoElement(ParentClass.HitID);
}
