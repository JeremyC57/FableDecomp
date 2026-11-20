// NScript::CQ_NewOakValeIntroScript::CNOVI_Bully::GivenTeddy() -- the hero gave the teddy to the bully: the game gives the hero
// one gold (slot 126) and takes the OBJECT_TEDDY_BEAR_UNGIVEABLE back (slot 125), the quest's SpokeAboutFindingTeddy (+0x70) and
// TeddyRuined (+0x91) are set, then a bad deed of kind 3 on the quest (AddBadDeed 0x00DAEA70)
// Leaf: ?GivenTeddy@CNOVI_Bully@CQ_NewOakValeIntroScript@NScript@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DBCD00, length 96 bytes.
// Name (agentNF163, 2026-11-20): Ego_r.pdb 0x00DDC440 + FableWin.pdb 0x0381A9A0
//   ?GivenTeddy@CNOVI_Bully@CQ_NewOakValeIntroScript@NScript@@AAEXXZ (private); the landed caller CNOVI_Bully::Main 0x00DBB310 binds
//   this spelling (reloc_bindings). Was the July stand-in `CNOVI_Bully::GivenTeddy` (global struct, local CCharString / game-call /
//   quest models, AddBadDeed bound as ?AddBadDeed@CQ_NewOakValeIntroScript@@QAEXJ@Z); ported onto engine/CQ_NewOakValeIntroScript.h
//   (FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BULLY), so AddBadDeed binds the PDB ?AddBadDeed@CQ_NewOakValeIntroScript@NScript@@QAEXW4EBadDeeds@12@@Z.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CQ_NewOakValeIntroScript::AddBadDeed
//   @ 0x00DAEA70; the game script interface (CEntityScriptBase::Game +0x04) slots 126 GiveHeroGold(long), 125
//   TakeObjectFromHero(const CCharString&). Shape: a 4-byte frame for the string temporary, `this` in esi, `ret`.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BULLY
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CGameScriptInterfaceBase.h"

void NScript::CQ_NewOakValeIntroScript::CNOVI_Bully::GivenTeddy()
{
    Game->GiveHeroGold(1);
    Game->TakeObjectFromHero("OBJECT_TEDDY_BEAR_UNGIVEABLE");
    ParentClass.SpokeAboutFindingTeddy = true;
    ParentClass.TeddyRuined = true;
    ParentClass.AddBadDeed((EBadDeeds)3);
}
