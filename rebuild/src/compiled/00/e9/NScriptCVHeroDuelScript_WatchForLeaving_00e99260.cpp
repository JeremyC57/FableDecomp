// NScript::CV_HeroDuelScript::WatchForLeaving() -- Hero Duel watcher thread: once the duel has started, watch for the hero dying (record the lost duel: MissionFailed, MissionOver, master data LostToThunder) or the mission succeeding, then reset the Mayor's Invitation quest card objective and clean up the duel
// Leaf: ?WatchForLeaving@CV_HeroDuelScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E99260, 519 bytes, thiscall, plain `ret`.
// Identity (proven): the CSpawnedFunc<CV_HeroDuelScript> (vtable 0x012ED0E8) built in CV_HeroDuelScript::Main 0x00E987C0
//   stores this address as its member function (site 0x00E98DE1), right after its "WatchForLeaving" name literal 0x012D19EC
//   (site 0x00E98DB9); the row has no other reference. Ego_r.pdb name set ?WatchForLeaving@CV_HeroDuelScript@NScript@@QAEXXZ.
//   Catalog `sub_00e99260`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940,
//   CV_HeroDuelScript::CleanUpHeroDuel 0x00E98FB0 (landed); CGameScriptInterfaceBase slots 7 NewScriptFrame, 11 IsLevelLoaded,
//   70 GetHero, 296 SetQuestCardObjective; CScriptThing slot 75 IsAlive.
// Literals: "HeroDualArea" 0x012D2300, "HeroDualAreaCave" 0x012ED198, "" 0x0122D70E, "BowerstonePosh" 0x012C6C6C,
//   "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_09" 0x012ED16C, "V_MayorsInvitation" 0x012C5008.
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT); CV_HeroDuelScript members at Ego_r.pdb - 0x14 (retail CScriptBase
//   0x40): MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4A, DuelStarted +0x4C; CQ_SunnyvaleMasterData
//   LostToThunder +0x62 (Ego_r.pdb, no containers before it).
// Shape: every script statement opens with a termination check (the generated-script pattern of the landed quest rows). The
//   watch loop is the unrotated do { ... NewScriptFrame(); } while (!IsActiveThreadTerminating()) whose natural exit returns
//   through its own epilogue (GOTCHAS, 0x00DD8DE0), while the two exits that go on (hero dead, mission succeeded) jump to the
//   shared tail with heroDied in bl: the loop is followed by `return;` and the exits `goto finished`. `break` instead runs the
//   termination exit into the tail too (515v519, one `jmp` for the epilogue); for(;;) / while(true) / do-while(true) with the
//   termination return inside rotate the loop (542v519).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData {
public:
    unsigned char _before[0x62];                        // +0x00 .. +0x62 (opaque here)
    bool LostToThunder;                                 // +0x62
};
class CV_HeroDuelScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool MissionOver;                                   // +0x4a
    bool GotToExecutionTree;                            // +0x4b
    bool DuelStarted;                                   // +0x4c
    void WatchForLeaving();
    void CleanUpHeroDuel();                             // 0x00E98FB0
};

void CV_HeroDuelScript::WatchForLeaving()
{
    bool heroDied = false;
    while (!DuelStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsActiveThreadTerminating())
        return;
    do {
        if (!Game->GetHero().IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            heroDied = true;
            goto finished;
        }
        if (!Game->IsLevelLoaded("HeroDualArea") && !Game->IsLevelLoaded("HeroDualAreaCave")) {
            if (IsActiveThreadTerminating())
                return;
        }
        if (MissionSucceeded) {
            if (IsActiveThreadTerminating())
                return;
            goto finished;
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
    return;
finished:
    if (IsActiveThreadTerminating())
        return;
    if (heroDied) {
        if (IsActiveThreadTerminating())
            return;
        MissionFailed = true;
        MissionOver = true;
        MasterScriptData->LostToThunder = true;
    }
    Game->SetQuestCardObjective("V_MayorsInvitation", "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_09", "BowerstonePosh", "");
    CleanUpHeroDuel();
}
}   // namespace NScript