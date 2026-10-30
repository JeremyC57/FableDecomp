// NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::Main() -- the second guardian-sister info quest main: bind the MazeAtTavern entity script, post-add the scripted entities, set the quest card objective (Oakvale), wait until the guardian has spoken to the hero, then wait while Maze is alive and finally complete and deactivate the active quest
// Leaf: ?Main@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E268C0, 534 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012E1F08 [dtor 0x00E27820, RegisterMain 0x00E26820 (landed
//   NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::RegisterMain), Main 0x00E268C0, Init 0x00E26810], stored by the
//   landed ctor 0x00E267E0 (site 0x00E267F8); the row is referenced only from that slot (0x012E1F10). Ego_r.pdb / FableWin.pdb
//   name set ?Main@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@UAEXXZ. The binding factory 0x00E26AF0 is the landed
//   CMazeAtTavern::Alloc, paired with its "MazeAtTavern" literal 0x012E1E18. Catalog `sub_00e268c0`.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / IsActiveThreadTerminating 0x00CB7940,
//   ~CScriptThing 0x004AA840; vtable CEntityScriptBinding<..> 0x012E1F6C; CGameScriptInterfaceBase slots 7 NewScriptFrame,
//   64 PostAddScriptedEntities, 72 GetThingWithScriptName, 281 DeactivateQuestLater, 288 SetQuestAsCompleted,
//   296 SetQuestCardObjective, 655 GetActiveQuestName; CScriptThing slot 75 IsAlive.
// Literals: "MazeAtTavern" 0x012E1E18 (twice), "OakBay" 0x012D31E4, "" 0x0122D70E,
//   "TEXT_QUEST_GUARDIAN_SISTER_INFO_SECOND_OBJECTIVE_01" 0x012E1F38.
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT); GuardianSpokeToHero +0x48 (Ego_r.pdb +0x5C - 0x14, retail CScriptBase
//   0x40). Master data CQ_SunnyvaleMasterData: the Ego_r.pdb type of CMazeAtTavern::PMasterData.
// Model: the landed CV_TalentlessBardScript::Main 0x00ED7830 (binding, rotated wait loops); one function-scope CScriptThing
//   whose out-of-line destructor serves every later exit.
// Shape: retail calls IsAlive through Maze's vtable (`mov edx,[esp+1Ch] ; call [edx+12Ch]`) and hoists `lea ecx,[esp+1Ch]`
//   above the termination branch, shared by the exit's ~CScriptThing and the next IsAlive. A plain `maze.IsAlive()` is
//   devirtualised (direct call); a reference or a named pointer local keeps the vcall but not the hoisted `lea` (2 lines
//   differ); the call through the address expression `((const CScriptThing*)&maze)->IsAlive()` gives both, as the landed
//   CQ_TraderEscortScript::WatchForSurprisingBalverines 0x00E06050 needed for the same wait loop.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CGameScriptInterfaceBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_GuardianSisterInfo2_SisterInBanditCampScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CMazeAtTavern {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQS_GuardianSisterInfo2_SisterInBanditCampScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E26AF0
    };
    bool GuardianSpokeToHero;                           // +0x48
    virtual void Main();
};

void CQS_GuardianSisterInfo2_SisterInBanditCampScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQS_GuardianSisterInfo2_SisterInBanditCampScript>("MazeAtTavern", this, &CMazeAtTavern::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GUARDIAN_SISTER_INFO_SECOND_OBJECTIVE_01", "", "OakBay");
    while (!GuardianSpokeToHero) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing maze = Game->GetThingWithScriptName("MazeAtTavern");
    while (((const CScriptThing*)&maze)->IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}   // namespace NScript