// CV_BordelloScript::GetHeroStatusTextTag() -- the text-tag suffix for the hero's status in the bordello: "_HEROWHORE" while tricking in a dress, otherwise "_HEROLADY" in a dress and "_HEROMAN" without, returned by value
// Leaf: GetHeroStatusTextTag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E3E6B0, length 100 bytes (padded to 112), `ret 4` (hidden result).
// Callees: CV_BordelloScript::IsHeroDressedAsLady @ 0x00E3E320 (it tests the hero for
// "OBJECT_HERO_TROUSERS_DRESS"), CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literals "_HEROWHORE"
// 0x012E4208, "_HEROLADY" 0x012E41FC, "_HEROMAN" 0x012E41F0).
// Layout: HeroTricking at retail +0x49 (Ego_r.pdb CV_BordelloScript 0x5d, quest scripts sit 0x14 below the PDB).
// Shape: three returns of a constructed result, each its own tail.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// Naming (NF162, 2026-11-20, q4636): the callee 0x00E3E320 was the descriptive IsHeroWearingDress on a row-local
//   NScript::CV_BordelloScript model; the row is now on the shared engine/CV_BordelloScript.h, whose
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ (Ego_r.pdb + FableWin.pdb name sets) the landed self row 0x00E3E320
//   and its other five landed callers already bind. HeroTricking is the header's bool at retail +0x49.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_BordelloScript.h"

CCharString NScript::CV_BordelloScript::GetHeroStatusTextTag()
{
    if (HeroTricking && IsHeroDressedAsLady())
        return CCharString("_HEROWHORE");
    if (IsHeroDressedAsLady())
        return CCharString("_HEROLADY");
    return CCharString("_HEROMAN");
}
