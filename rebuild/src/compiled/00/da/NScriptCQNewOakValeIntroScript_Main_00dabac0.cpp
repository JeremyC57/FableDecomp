// CQ_NewOakValeIntroScript::Main() -- the new Oakvale intro quest's main: bind the sixteen NOVI_* / OVI_DeadFather entity scripts, finish adding scripted entities, close the post-attack quest if the attack is already over, set the active quest's first objective, spawn StartBarrelTimer and run DoMission
// Leaf: ?Main@CQ_NewOakValeIntroScript@NScript@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DABAC0, length 2018 bytes. Catalog label "CPersonalScript_GlobalThingsScript::Main"
//   is propagated: the vtable 0x012D7A30 slot holding this body and the NOVI_ strings / DoMission 0x00DBDE40 call are
//   CQ_NewOakValeIntroScript's.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddEntityScriptBinding 0x00CB8230 /
//   PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase ctor 0x00CDD450 (CSpawnedFunc vtable
//   0x012D7A3C), StartBarrelTimer 0x00DB4F70 (catalog label "Script_Guard_Duty" is propagated), DoMission 0x00DBDE40; the
//   binding allocators 0x00DAC2C0 .. 0x00DB81B0 (landed CreateCVtable bodies; binding vtable 0x012D8370); the game script
//   interface (+0x40) slots 64, 280, 296, 655.
// Layout (retail = Ego_r.pdb - 0x14): AttackOver +0x50.
// Shape: bindings / spawned function as the landed CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
// Naming (agentNF163, 2026-11-20): the sixteen entity scripts are CQ_NewOakValeIntroScript::CNOVI_<Name> / COVI_DeadFather (Ego_r.pdb +
//   FableWin.pdb ?Alloc@CNOVI_LiveFather@CQ_NewOakValeIntroScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
//   ... ?Alloc@COVI_DeadFather@...; the landed Alloc rows 0x00DAC2C0 .. 0x00DB81B0 define exactly those leaves). Ported off the local
//   CCharString / CEntityScriptBinding / CQ_NewOakValeIntroScript (ALLOC(CLiveFather) .. stand-in nested structs) models onto
//   fable_string.h, engine/CEntityScriptBinding.h and engine/CQ_NewOakValeIntroScript.h (entity opt-ins, ENTITIES3 for the last
//   five), as the landed CV_ChapelOfEvilScript::Main 0x00E693F0; every binding now names the PDB Alloc.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES2
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES3
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_VICTIM_LIVEFATHER
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_GUARD
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BARRELMAN_THERESA
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BULLY
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/C3DVector.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

void NScript::CQ_NewOakValeIntroScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_LiveFather", this, &CNOVI_LiveFather::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Theresa", this, &CNOVI_Theresa::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Guard", this, &CNOVI_Guard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Villager", this, &CNOVI_Villager::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Bully", this, &CNOVI_Bully::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Victim", this, &CNOVI_Victim::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_TeddyGirl", this, &CNOVI_TeddyGirl::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_AffairMan", this, &CNOVI_AffairMan::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_AffairWoman", this, &CNOVI_AffairWoman::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_AffairWife", this, &CNOVI_AffairWife::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_BookTrader", this, &CNOVI_BookTrader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_BarrelMan", this, &CNOVI_BarrelMan::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_BarrelThug", this, &CNOVI_BarrelThug::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_Barrel", this, &CNOVI_Barrel::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("NOVI_CreatedBeetle", this, &CNOVI_CreatedBeetle::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_NewOakValeIntroScript>("OVI_DeadFather", this, &COVI_DeadFather::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (AttackOver) {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuest("Q__OakValeIntro_PostAttack", 0);
    }
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01", "", "");
    AddSpawnedFunction(new CSpawnedFunc<CQ_NewOakValeIntroScript>("StartBarrelTimer", 0, this, &CQ_NewOakValeIntroScript::StartBarrelTimer), CCharString(""));
    DoMission();
}
