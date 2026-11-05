// NScript::CQ_SecretPassageScript::Main() -- the Secret Passage quest main: reset the mission flags, bind the ten entity scripts (SP_AmbushGuard .. DoorToMothersTortureChamber), log story entry 210, set the first Prison Escape objective, spawn WatchForTermination and DoMission, and cache the Prison Mother / Jack Captures music sets
// Leaf: ?Main@CQ_SecretPassageScript@NScript@@UAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DECAD0, 1479 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 2 (Main) of the RTTI-less quest-script vtable 0x012DDC30 (0x012DDC38), stored by the landed
//   CQ_SecretPassageScript ctor 0x00DEC7F0 (slots: 0 dtor 0x00DF10B0, 1 RegisterMain 0x00DECA10, 3 Init 0x00DEC8A0);
//   Ego_r.pdb / FableWin.pdb name set ?Main@CQ_SecretPassageScript@NScript@@UAEXXZ; every entity name literal is an SP_
//   script of this TU (SP_AmbushGuard 0x012DDD34 .. DoorToMothersTortureChamber 0x012DDC98) and the two spawned threads are
//   this class's WatchForTermination 0x00DED0C0 / DoMission 0x00DEFF60. The ledger name "CV_TravellingHeroesScript::Main"
//   was a propagated guess (that Main is the landed 0x00EDED10).
// Callees: InitialiseVariables 0x00DED0A0 (same TU; zeroes +0x48..+0x4D and +0x4F -- the manifest label
//   CQ_GuildTrainingScript::CMeleeApprentice::Init is propagated), operator new 0x00BFEA1A, CCharString(const char*, long)
//   0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012DDD2C),
//   CScriptBase::PostAddScriptedEntities 0x00CB8930, AddLogbookStoryEntry(long) 0x00CBE87F (__fastcall), CSpawnedFuncBase
//   0x00CDD450 (CSpawnedFunc<CQ_SecretPassageScript> vtable 0x012DDC54), CScriptBase::AddSpawnedFunction 0x00CB7E50;
//   binding allocators CSP_AmbushGuard 0x00DED450, CSP_InvestigatingGuard 0x00DEDA90, CSP_SecretPassageGuard 0x00DED720,
//   CSP_DeskGuard 0x00DEDE40, CSP_UndeadTrigger 0x00DEF620, CSP_NymphTrigger 0x00DED290, CSP_ScreamerTrigger 0x00DED1A0,
//   CMotherTorture 0x00DEE1A0, CTortureDoorCourtyard 0x00DEEF00, CDoorToMothersTortureChamber 0x00DEF0F0;
//   CGameScriptInterfaceBase slots 64 PostAddScriptedEntities (+0x100), 296 SetQuestCardObjective (+0x4a0),
//   655 GetActiveQuestName (+0xa3c, CCharString by value), 698 CacheMusicSet (+0xae8; 53 PRISON_MOTHER, 56 JACK_CAPTURES).
// Shape: the landed quest-Main model (CV_HeroDuelScript::Main 0x00E987C0, CQ_HeroSoulsMotherScript::Main 0x00D835D0): one
//   flag word (ebx, memory home zeroed at entry) guards each name temporary that exists only when its operator new succeeded;
//   the objective call builds its three literal temporaries right to left, then the GetActiveQuestName result, and destroys
//   them in reverse.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/NScript_CQ_SecretPassageScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/EMusicSetType.h"
class CEntityScriptBase;
void __fastcall AddLogbookStoryEntry(long entry);        // 0x00CBE87F
namespace NScript {
class CQ_SecretPassageScript::CSP_AmbushGuard { public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DED450
};
class CQ_SecretPassageScript::CSP_InvestigatingGuard { public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DEDA90
};
class CQ_SecretPassageScript::CSP_SecretPassageGuard { public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DED720
};
class CQ_SecretPassageScript::CSP_DeskGuard { public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DEDE40
};
class CQ_SecretPassageScript::CSP_UndeadTrigger { public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DEF620
};
class CQ_SecretPassageScript::CSP_NymphTrigger { public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DED290
};
class CQ_SecretPassageScript::CSP_ScreamerTrigger { public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DED1A0
};
class CQ_SecretPassageScript::CMotherTorture { public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DEE1A0
};
class CQ_SecretPassageScript::CTortureDoorCourtyard { public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DEEF00
};
class CQ_SecretPassageScript::CDoorToMothersTortureChamber { public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_SecretPassageScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DEF0F0
};
}
void NScript::CQ_SecretPassageScript::Main()
{
    InitialiseVariables();
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SecretPassageScript>("SP_AmbushGuard", this, &CSP_AmbushGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SecretPassageScript>("SP_InvestigatingGuard", this, &CSP_InvestigatingGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SecretPassageScript>("SP_SecretPassageGuard", this, &CSP_SecretPassageGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SecretPassageScript>("SP_DeskGuard", this, &CSP_DeskGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SecretPassageScript>("SP_UndeadTrigger", this, &CSP_UndeadTrigger::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SecretPassageScript>("SP_NymphTrigger", this, &CSP_NymphTrigger::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SecretPassageScript>("SP_ScreamerTrigger", this, &CSP_ScreamerTrigger::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SecretPassageScript>("MotherTorture", this, &CMotherTorture::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SecretPassageScript>("TortureDoorCourtyard", this, &CTortureDoorCourtyard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_SecretPassageScript>("DoorToMothersTortureChamber", this, &CDoorToMothersTortureChamber::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddLogbookStoryEntry(210);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_01", "", "");
    AddSpawnedFunction(new CSpawnedFunc<CQ_SecretPassageScript>("WatchForTermination", 0, this, &CQ_SecretPassageScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_SecretPassageScript>("DoMission", 0, this, &CQ_SecretPassageScript::DoMission), CCharString(""));
    Game->CacheMusicSet(MUSIC_SET_CUTSCENE_PRISON_MOTHER);
    Game->CacheMusicSet(MUSIC_SET_CUTSCENE_JACK_CAPTURES);
}