// NScript::CHeroBoastsScript::BoastBreakSiegeNoAssistance(CCharString questName) -- boast thread for "break the siege without hiring guards": while the no-assistance flag holds, wait for the quest's end and mark boast 25 failed or completed; once the hero hires guards (flag cleared), show TEXT_QST_BOAST_STATE_FAILED_HIRENOGUARDS, wait until it is clicked past and mark the boast failed
// Leaf: ?BoastBreakSiegeNoAssistance@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE5050, 296 bytes, thiscall `ret 4` (the CCharString is passed by value and
//   destroyed here; the `push ecx` slot holds the game-info text temporary).
// Identity (proven, two sources): Ego_r.pdb `?BoastBreakSiegeNoAssistance@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z`;
//   CHeroBoastsScript::ControlNewBoast stores this address (0x00CE2861, the row's only reference) in the
//   CSpawnedFunc1<CHeroBoastsScript, CCharString> it builds for boast 0x19 = 25, next to the "BoastBreakSiegeNoAssistance"
//   thread-name literal 0x012C39F0; the flag it watches is Ego_r CQ_SunnyvaleMasterData::BreakSiegeNoAssistance (+0x116,
//   no container before it, so no retail shift); the sibling BoastBreakSiegeKilledLeader 0x00CE5180 (boast 26, flag
//   +0x117) follows it. The manifest label "sub_00ce5050" is a gapscan placeholder.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, CGameScriptInterfaceBase slots 7 NewScriptFrame (+0x1C), 28 MsgOnQuestCompletedBeforeScreenShown (+0x70),
//   29 MsgOnQuestFailedBeforeScreenShown (+0x74), 40 MsgIsGameInfoClickedPast (+0xA0), 115 DisplayGameInfo (+0x1CC),
//   305 SetBoastAsFailed (+0x4C4), 306 SetBoastAsCompleted (+0x4C8). Literal "TEXT_QST_BOAST_STATE_FAILED_HIRENOGUARDS"
//   0x012C3DEC.
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT: CLevelScriptBase<CQ_SunnyvaleMasterData> = CScriptBase 0x40, Game
//   +0x40, MasterScriptData +0x44); CQ_SunnyvaleMasterData has no shared header (local model, Ego_r offsets).
// Shape: a rotated watch loop whose normal exit keeps its own destructor epilogue; the flag-cleared arm is laid out after
//   it; the two failure paths share one failure block and the completed path its own; the game-info text converts
//   implicitly (the temporary's address re-taken with `lea` for the push).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData {                          // Ego_r.pdb (no shared header): only the flag this thread reads
public:
    unsigned char _pad000[0x116];
    bool          BreakSiegeNoAssistance;               // +0x116
};

enum { BOAST_BREAK_SIEGE_NO_ASSISTANCE = 25 };

class CHeroBoastsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void BoastBreakSiegeNoAssistance(CCharString questName);
};

void CHeroBoastsScript::BoastBreakSiegeNoAssistance(CCharString questName)
{
    while (!IsActiveThreadTerminating()) {
        if (!MasterScriptData->BreakSiegeNoAssistance) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_BOAST_STATE_FAILED_HIRENOGUARDS");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->SetBoastAsFailed(BOAST_BREAK_SIEGE_NO_ASSISTANCE, questName);
            return;
        }
        if (Game->MsgOnQuestFailedBeforeScreenShown(questName)) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetBoastAsFailed(BOAST_BREAK_SIEGE_NO_ASSISTANCE, questName);
            return;
        }
        if (Game->MsgOnQuestCompletedBeforeScreenShown(questName)) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetBoastAsCompleted(BOAST_BREAK_SIEGE_NO_ASSISTANCE, questName);
            return;
        }
        Game->NewScriptFrame();
    }
}
}   // namespace NScript