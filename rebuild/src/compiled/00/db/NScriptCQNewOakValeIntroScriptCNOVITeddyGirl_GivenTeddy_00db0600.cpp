// NScript::CQ_NewOakValeIntroScript::CNOVI_TeddyGirl::GivenTeddy() -- the hero hands over the ungiveable teddy: the game takes
//   OBJECT_TEDDY_BEAR_UNGIVEABLE from the hero (slot 125), FoundTeddy is set, the quest counts a good deed, the girl's information
//   flag is cleared (slot 361) and the master data's TeddySolution becomes "B".
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DB0600, length 96 bytes.
// Leaf: ?GivenTeddy@CNOVI_TeddyGirl@CQ_NewOakValeIntroScript@NScript@@AAEXXZ
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?GivenTeddy@CNOVI_TeddyGirl@CQ_NewOakValeIntroScript@NScript@@AAEXXZ
//   (private, nested); engine/CQ_NewOakValeIntroScript.h already declares it so (FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES2), and the
//   landed Main 0x00DAF080 calls it twice. Was spelled through the stand-ins CNOVI_TeddyGirl (global) / CGameCalls_00db0600 /
//   SQuestRecord_00db0600.
// Callees: CGameScriptInterfaceBase slot 125 (+0x1f4) TakeObjectFromHero(const CCharString&) const and slot 361 (+0x5a4)
//   ClearThingHasInformation(const CScriptThing&) const (engine/CGameScriptInterfaceBase.h slot comments), CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0 / operator=(const char*) 0x0099EFE0, CQ_NewOakValeIntroScript::AddGoodDeed 0x00DB0660.
// Layout: CEntityScriptBase Game +0x04, Me +0x08; CNOVI_TeddyGirl ParentClass +0x14, PMasterData +0x18, FoundTeddy +0x1d (Ego_r);
//   CQ_SunnyvaleMasterData TeddySolution +0x54.
// Shape: a 4-byte frame for the string temporary, `this` in esi, `ret`.
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES2
#include "engine/C3DVector.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"

void NScript::CQ_NewOakValeIntroScript::CNOVI_TeddyGirl::GivenTeddy()
{
    Game.TakeObjectFromHero("OBJECT_TEDDY_BEAR_UNGIVEABLE");
    FoundTeddy = true;
    ParentClass.AddGoodDeed();
    Game.ClearThingHasInformation(Me);
    PMasterData->TeddySolution = "B";
}
