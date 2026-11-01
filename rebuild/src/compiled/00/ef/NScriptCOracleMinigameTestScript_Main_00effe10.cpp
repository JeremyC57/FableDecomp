// NScript::COracleMinigameTestScript::Main() -- the oracle minigame test quest's main: bind the OracleGamesmaster, GamesmasterOracleMarker and OracleSpectator entity scripts, finish adding scripted entities, wait for the mission to succeed, then complete and deactivate the active quest
// Leaf: ?Main@COracleMinigameTestScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFFE10, 480 bytes, thiscall, plain `ret`. Slot 2 (Main) of the RTTI-less script vtable
//   0x012F70A0 (word 0x012F70A8), whose slot 1 is the landed RegisterMain 0x00EFFD70.
// Identity (proven, two sources): (1) the vtable slot (CScriptBase slots 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist;
//   the landed RegisterMain / ctor 0x00EFFD30 rows prove the vtable is COracleMinigameTestScript's); (2) the three binding
//   allocators are the landed COracleMinigameTestScript::COracleGamesmaster::Alloc 0x00F00000, CGamesmasterOracleMarker::Alloc
//   0x00F00A10 and COracleSpectator::Alloc 0x00F00BB0, and Ego_r.pdb holds ?Main@COracleMinigameTestScript@NScript@@UAEXXZ right
//   after RegisterMain. Catalog row "_gapscan::sub_00effe10" carried no name.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 /
//   IsActiveThreadTerminating 0x00CB7940; the game script interface (+0x40) slots 7 NewScriptFrame, 64 PostAddScriptedEntities,
//   281 DeactivateQuestLater, 288 SetQuestAsCompleted, 655 GetActiveQuestName (ghidra_out/cgsi_slots_aligned.tsv).
// Data: binding vtable 0x012F70F8 (CEntityScriptBinding<COracleMinigameTestScript>), "OracleGamesmaster" 0x01270714,
//   "GamesmasterOracleMarker" 0x012F70E0, "OracleSpectator" 0x012F70D0. Layout: Ego_r.pdb COracleMinigameTestScript - 0x14:
//   MissionSucceeded +0x48.
// Shape: the landed CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20 model (one flag word in ebx guarding the three name
//   temporaries, bits 1/2/4; every binding's Flags is 0); the wait loop tests the member before entering (`jne` over the
//   aligned loop head); SetQuestAsCompleted's bool arguments are pushed before its GetActiveQuestName temporary.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CEntityScriptBinding.h"                // CEntityScriptBinding<T>: 0x1c bytes, inline ctor, slot 0 GetEntityScriptName
#include "engine/CLevelScriptBase.h"                    // CScriptBase 0x40 + Game +0x40, MasterScriptData +0x44
#include "engine/CGameScriptInterfaceBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class COracleMinigameTestScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class COracleGamesmaster { public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(COracleMinigameTestScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00F00000
    };
    class CGamesmasterOracleMarker { public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(COracleMinigameTestScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00F00A10
    };
    class COracleSpectator { public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(COracleMinigameTestScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00F00BB0
    };
    bool MissionSucceeded;                              // +0x48
    virtual void Main();                                // slot 2
};
void COracleMinigameTestScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<COracleMinigameTestScript>("OracleGamesmaster", this, &COracleGamesmaster::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<COracleMinigameTestScript>("GamesmasterOracleMarker", this, &CGamesmasterOracleMarker::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<COracleMinigameTestScript>("OracleSpectator", this, &COracleSpectator::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}