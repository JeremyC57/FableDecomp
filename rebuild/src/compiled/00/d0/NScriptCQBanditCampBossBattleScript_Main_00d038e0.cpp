// NScript::CQ_BanditCampBossBattleScript::Main() -- the bandit camp boss battle quest main: bind the fourteen entity scripts (Gate2Guard1 .. CROWDBANDITS), post the scripted entities, spawn the seven quest threads (BanditKingMissionProcess .. CheckForSecondAreaDoorHelp) and cache the Twinblade / Theresa cutscene music sets
// Leaf: ?Main@CQ_BanditCampBossBattleScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D038E0, 2419 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00D04252.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012C88E0 (the vtable the landed RegisterMain 0x00D03820 / Allocs
//   use); Ego_r.pdb name set ?Main@CQ_BanditCampBossBattleScript@NScript@@UAEXXZ. Every bound Alloc is a Ego_r.pdb
//   ?Alloc@<Class>@CQ_BanditCampBossBattleScript@NScript@@SI... name whose binding-name literal is the class name less its `C`
//   (CGate2Guard1 0x00D071F0 .. CBanditKing 0x00D0A700 landed; CCROWDBANDITS 0x00D0B2D0 not landed yet), and the seven threads are
//   this class's PDB methods named by their thread-name literals: BanditKingMissionProcess 0x00D109F0, WatchForBanditCampGates
//   0x00D04770, ProcessHostageCutscenes 0x00D0FE80, WatchForTermination 0x00D04260 (landed), CheckForFirstAreaMassacre 0x00D0EE70,
//   CheckForSecondAreaMassacre 0x00D0F640, CheckForSecondAreaDoorHelp 0x00D043A0.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012C8A6C), CScriptBase::PostAddScriptedEntities 0x00CB8930,
//   CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_BanditCampBossBattleScript> vtable 0x012C88F4), CScriptBase::AddSpawnedFunction
//   0x00CB7E50; CGameScriptInterfaceBase slots 64 PostAddScriptedEntities (+0x100), 698 CacheMusicSet (+0xae8).
// Shape: the landed quest-Main model (CQ_PrisonEscapeRescueMotherScript::Main 0x00DD2CE0): one flag word (ebx, memory home zeroed at
//   entry) guards each name temporary that exists only when its operator new succeeded; binding flags 1 / 0 / 1 x12 as retail stores them.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/EMusicSetType.h"

namespace NScript {
#define FABLE_BANDITCAMP_ENTITY(Name) \
    class CQ_BanditCampBossBattleScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BanditCampBossBattleScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_BANDITCAMP_ENTITY(CGate2Guard1)        // Alloc 0x00D071F0
FABLE_BANDITCAMP_ENTITY(CBCGameMaster)       // Alloc 0x00D066B0
FABLE_BANDITCAMP_ENTITY(CAssassinMarker)     // Alloc 0x00D048C0
FABLE_BANDITCAMP_ENTITY(CAssassin1)          // Alloc 0x00D04990
FABLE_BANDITCAMP_ENTITY(CAssassin2)          // Alloc 0x00D05910
FABLE_BANDITCAMP_ENTITY(CAssassin3)          // Alloc 0x00D05FE0
FABLE_BANDITCAMP_ENTITY(CCampHostage)        // Alloc 0x00D07D20
FABLE_BANDITCAMP_ENTITY(CCampHostage2)       // Alloc 0x00D08670
FABLE_BANDITCAMP_ENTITY(CCampHostageDoor)    // Alloc 0x00D08A70
FABLE_BANDITCAMP_ENTITY(CCampHostageGuard)   // Alloc 0x00D08E50
FABLE_BANDITCAMP_ENTITY(CBanditForger)       // Alloc 0x00D0C2C0
FABLE_BANDITCAMP_ENTITY(CBanditKing)         // Alloc 0x00D0A700
FABLE_BANDITCAMP_ENTITY(CCROWDBANDITS)       // Alloc 0x00D0B2D0
#undef FABLE_BANDITCAMP_ENTITY
}

void NScript::CQ_BanditCampBossBattleScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("Gate2Guard1", this, &CGate2Guard1::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("BCGameMaster", this, &CBCGameMaster::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("AssassinMarker", this, &CAssassinMarker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("Assassin1", this, &CAssassin1::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("Assassin2", this, &CAssassin2::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("Assassin3", this, &CAssassin3::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("Gate3Guard", this, &CGate3Guard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("CampHostage", this, &CCampHostage::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("CampHostage2", this, &CCampHostage2::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("CampHostageDoor", this, &CCampHostageDoor::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("CampHostageGuard", this, &CCampHostageGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("BanditForger", this, &CBanditForger::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("BanditKing", this, &CBanditKing::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BanditCampBossBattleScript>("CROWDBANDITS", this, &CCROWDBANDITS::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_BanditCampBossBattleScript>("BanditKingMissionProcess", 0, this, &CQ_BanditCampBossBattleScript::BanditKingMissionProcess), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_BanditCampBossBattleScript>("WatchForBanditCampGates", 0, this, &CQ_BanditCampBossBattleScript::WatchForBanditCampGates), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_BanditCampBossBattleScript>("ProcessHostageCutscenes", 0, this, &CQ_BanditCampBossBattleScript::ProcessHostageCutscenes), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_BanditCampBossBattleScript>("WatchForTermination", 0, this, &CQ_BanditCampBossBattleScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_BanditCampBossBattleScript>("CheckForFirstAreaMassacre", 0, this, &CQ_BanditCampBossBattleScript::CheckForFirstAreaMassacre), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_BanditCampBossBattleScript>("CheckForSecondAreaMassacre", 0, this, &CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_BanditCampBossBattleScript>("CheckForSecondAreaDoorHelp", 0, this, &CQ_BanditCampBossBattleScript::CheckForSecondAreaDoorHelp), CCharString(""));
    Game->CacheMusicSet(MUSIC_SET_CUTSCENE_TWINBLADE);
    Game->CacheMusicSet(MUSIC_SET_CUTSCENE_THERESA_01);
    Game->CacheMusicSet(MUSIC_SET_CUTSCENE_THERESA_02);
}