// NScript::CPersonalScript_EmptyScript::Main() -- the empty personal (test) script main: clear ReachedGoalInStealthMission, bind the thirty test entity scripts (TestPerson .. CrateDeathMarker; TriggerCrate and ArcherMan with flag 1), post the scripted entities, spawn CheckForHeroActions, activate TestQuestCard1, activate the NewGuildTeleport teleporter if it is alive, then report the hero's death and his sleeps (day / night) as screen messages every frame
// Inline-split: three single-site `do { if (IsActiveThreadTerminating()) return; } while (0);` statements (the loop head and the first check in each message arm); retail's construct there is unproven (docs/pipeline/INLINE_DEPTH_REVISIT.md): they make c2 expand the teleporter's ~CScriptThing at those three exits and call 0x004AA840 at the others, as retail does.
// Leaf: ?Main@CPersonalScript_EmptyScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF0090, 4002 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 2 (Main) of the CPersonalScript_EmptyScript vtable (RegisterMain 0x00EEFFD0 spawns it as "Main"); Ego_r.pdb name
//   set ?Main@CPersonalScript_EmptyScript@NScript@@UAEXXZ. Every bound Alloc is a landed CPersonalScript_EmptyScript::C<Name>::Alloc row
//   (binding-name literal = class name less its C); the thread is the landed CheckForHeroActions 0x00EF5200 ("CheckForHeroActions").
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CScriptBase::
//   AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50 / IsActiveThreadTerminating
//   0x00CB7940, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CPersonalScript_EmptyScript> vtable 0x012F5D90, binding vtable 0x012F5F7C),
//   ~CScriptThing 0x004AA840, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; CGameScriptInterfaceBase slots 7 NewScriptFrame,
//   50 MsgOnHeroSlept(bool&), 64 PostAddScriptedEntities, 70 GetHero, 72 GetThingWithScriptName (by value), 112 AddScreenMessage,
//   276 ActivateQuest, 563 SetTeleporterAsActive; CScriptThing slot 18 MsgIsKilledBy and slot 75 IsAlive (through PImp).
// Layout: engine/CPersonalScript_EmptyScript.h (ReachedGoalInStealthMission +0x48 = Ego_r 0x5c - 0x14), with the chunk 799 opt-in
//   FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES for the 27 entity classes the header did not declare.
// Shape: the landed quest-Main binding model (CQ_RansomVictimScript::Main 0x00DE0850): each new-expression's CCharString temporary is
//   conditional, so VC7.1 keeps one flag bit per binding in ebx. ReachedGoalInStealthMission = false is InitialiseVariables' store.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CPersonalScript_EmptyScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline CScriptThing::~CScriptThing() { }

void NScript::CPersonalScript_EmptyScript::Main()
{
    ReachedGoalInStealthMission = false;
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("TestPerson", this, &CTestPerson::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("MeanBandit", this, &CMeanBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("Leader", this, &CLeader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("Follower", this, &CFollower::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("TestPos", this, &CTestPos::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("OpinionMan", this, &COpinionMan::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("Walker", this, &CWalker::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("UndeadDude", this, &CUndeadDude::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("Beggar", this, &CBeggar::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("ScrumpyCrate", this, &CScrumpyCrate::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("Villager", this, &CVillager::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("GhostDude", this, &CGhostDude::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("Door", this, &CDoor::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("Chatter1", this, &CChatter1::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("Chatter2", this, &CChatter2::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("UndeadFellow", this, &CUndeadFellow::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("StealthGuard", this, &CStealthGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("GoalChest", this, &CGoalChest::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("TriggerCrate", this, &CTriggerCrate::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("ArcherMan", this, &CArcherMan::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("KillMan", this, &CKillMan::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("CinemaMan", this, &CCinemaMan::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("SwordChest", this, &CSwordChest::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("BanditWalker", this, &CBanditWalker::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("LookAtMe1", this, &CLookAtMe1::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("TrappedMan", this, &CTrappedMan::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("Whispie", this, &CWhispie::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("Crate1", this, &CCrate1::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("Crate2", this, &CCrate2::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CPersonalScript_EmptyScript>("CrateDeathMarker", this, &CCrateDeathMarker::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CPersonalScript_EmptyScript>("CheckForHeroActions", 0, this, &CPersonalScript_EmptyScript::CheckForHeroActions), CCharString(""));
    Game->ActivateQuest("TestQuestCard1");
    CScriptThing teleporter = Game->GetThingWithScriptName("NewGuildTeleport");
    if (teleporter.IsAlive())
        Game->SetTeleporterAsActive(teleporter, true);
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (Game->GetHero().MsgIsKilledBy("")) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            Game->AddScreenMessage("Ooh, the hero died.", (ETextGroupSelectionMethod)0);
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        bool daytime;
        if (Game->MsgOnHeroSlept(daytime)) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            if (daytime) {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddScreenMessage("You slept until daytime.", (ETextGroupSelectionMethod)0);
                Game->NewScriptFrame();
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->AddScreenMessage("You slept until night time.", (ETextGroupSelectionMethod)0);
                Game->NewScriptFrame();
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}