// NScript::CV_ArcheryCompetitionScript::CAC_Owner::Init() -- set up the ambush convoy spy: reset the parent's speech timer, mark the spy as informed and allied with the hero
// Leaf: ?Init@CAC_Owner@CV_ArcheryCompetitionScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E33720, length 148 bytes.
// Callees: NScript::CV_ArcheryCompetitionScript::ResetRandomSpeechTime @ 0x00E337C0 (Ego_r/FableWin; manifest module "CV_AmbushScamScript" is
// propagated), and the game script interface's virtual slots (rtti-vtables.tsv, CGameScriptInterface
// 0x01260F0C): 360 SetThingHasInformation(const CScriptThing&, bool, bool, bool) @ 0x0088E440, 70 GetHero()
// @ 0x00891CA0, 599 EntitySetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) @ 0x008A7EC0, 844
// SetIsPushableByHero(CScriptThing, bool) @ 0x008A6DD0.
//
// Shape: the entity-script layout of the landed 0x00D72160 (GSI +0x04, the spy's CScriptThing +0x08,
// ParentClass +0x14, PMasterData +0x18) plus two counters at +0x1c/+0x20 set to 10 (stored from one register
// among the first call's pushes). The parent's flag byte +0x4f is cleared before its speech timer reset; the
// thing's address stays in edi. The ally calls go both ways -- (spy, hero) evaluates GetHero before both
// pushes, (hero, spy) pushes the spy first -- and the GSI vtable stays in ebx across each GetHero/ally pair.
// The last call takes the spy BY VALUE: a CScriptThing copy built in the argument area by the inline copy
// constructor (root vtable store, CScriptThing vtable store, then the PImp counted pointer through ITS OWN
// inline copy constructor: re-addressed from esi, info in ecx; a flat Data/Info copy reuses edi).
// NF53: moved onto engine/CV_ArcheryCompetitionScript.h (CAC_Owner: ParentClass reference +0x14, PMasterData +0x18,
//   lastRandomSpeechIdx1/2 +0x1c/+0x20; parent AttackedByHero +0x4f) and engine/CEntityScriptBase.h (Game +0x04, Me +0x08).
// Naming wave 35: identity reg 00e325a3 "AC_Owner" in 00e32540; Alloc 00e335b0; vtable 012e30f8 slot 2; PDB ?Init@CAC_Owner@CV_ArcheryCompetitionScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); replaces CAT_ConvoySpy::Init: current name also held by 00f04410; CAT_ConvoySpy::Init is at 00f04410 (its own vtable slot) (wave3/labels_alloc.tsv); PDB ?Init@CAC_Owner@CV_ArcheryCompetitionScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CAT_ConvoySpy@@QAEXXZ`; this source now defines the PDB member (class CAT_ConvoySpy renamed CAC_Owner); same bytes.

#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_OWNERMAIN
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_ArcheryCompetitionScript.h"
#include "engine/CGameScriptInterfaceBase.h"

void NScript::CV_ArcheryCompetitionScript::CAC_Owner::Init()
{
    ParentClass.AttackedByHero = false;
    ParentClass.ResetRandomSpeechTime();
    lastRandomSpeechIdx1 = 10;
    lastRandomSpeechIdx2 = 10;
    Game->SetThingHasInformation(Me, false, true, false);
    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
    Game->SetIsPushableByHero(Me, false);
}
