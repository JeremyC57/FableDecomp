// NScript::CQ_WizardBattleScript::~CQ_WizardBattleScript() -- implicit quest-script destructor: destroy MiddleOfLighthouse, TopOfLighthouse and Maze (CScriptThing) in reverse order, then ~CScriptBase
// Leaf: ??1CQ_WizardBattleScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1DA40, 185 bytes, tail jump to ~CScriptBase 0x00CBD510; sole caller the scalar
//   deleting dtor 0x00E1DA20 (slot 0 of the RTTI-less quest vtable 0x012E0F54).
// Identity (proven): vtable 0x012E0F54 is stored only by the ctor 0x00E1A800 (manifest label CQ_WizardBattleScript), whose only
//   caller is the Alloc 0x00E1D9F0 (`new` 0x80 = Ego_r sizeof 0x94 - 0x14); CScriptInfoManager::RegisterScripts stores that Alloc
//   right after `push "S_QWZB"` (0x00CD7ADD; refs/script_recovery/conversion_queue.tsv: Q_WizardBattle / S_QWZB / 0x00E1D9F0).
//   Ego_r.pdb / FableWin.pdb carry ??0CQ_WizardBattleScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z;
//   no vtable store in the body = compiler-generated dtor. The catalog label (CHTG_Audience::~CHTG_Audience) is propagated from
//   the masked-identical 0x00D728F0 and wrong.
// Layout: Ego_r.pdb CQ_WizardBattleScript members at retail offsets (Ego_r - 0x14: retail CScriptBase is 0x40, Ego_r 0x54):
//   eight bools +0x48..+0x4f, MazePhase +0x50, NumberOfScreamersAlive +0x54, MazeConversation +0x58, Maze +0x5c,
//   TopOfLighthouse +0x68, MiddleOfLighthouse +0x74 (0x80 = the Alloc size).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunk 665:
//   a quest's implicit dtor tail-jumps straight to ~CScriptBase), so CLevelScriptBase<T> is a local copy of the header without
//   the dtor declaration (NOTES.md "Header debt").
// Callees: ~CScriptThing inline (vtable 0x01238C8C store, CCountedPointer<CScriptThing> release: DeleteFunc fastcall + operator
//   delete 0x00BFE9BC, then ~CBaseClass 0x0099A2E0), ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00E1A800), which emits the vtable, ??_G and this implicit dtor.
void operator delete(void* p);                          // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() {}
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_WizardBattleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_WizardBattleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool         VillagersTurnOnHero;                   // +0x48
    bool         VillagersChased;                       // +0x49
    bool         FightersStart;                         // +0x4a
    bool         MissionSucceeded;                      // +0x4b
    bool         MissionFailed;                         // +0x4c
    bool         MissionOver;                           // +0x4d
    bool         QuestStartScreenShown;                 // +0x4e
    bool         MazeDefeated;                          // +0x4f
    long         MazePhase;                             // +0x50
    long         NumberOfScreamersAlive;                // +0x54
    long         MazeConversation;                      // +0x58
    CScriptThing Maze;                                  // +0x5c
    CScriptThing TopOfLighthouse;                       // +0x68
    CScriptThing MiddleOfLighthouse;                    // +0x74
};
CQ_WizardBattleScript::CQ_WizardBattleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00E1A800
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}