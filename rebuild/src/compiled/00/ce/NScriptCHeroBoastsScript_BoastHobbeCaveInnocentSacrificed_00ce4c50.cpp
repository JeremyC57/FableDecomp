// NScript::CHeroBoastsScript::BoastHobbeCaveInnocentSacrificed(CCharString questName) -- boast thread for the Hobbe-cave sacrifice boast: while the innocent is not sacrificed (state non-zero), wait for the quest's end and mark boast 22 completed (state 2) or failed; once the state reads 0, show TEXT_QST_BOAST_STATE_FAILED_SACRIFICE (unless the quest already failed), wait until it is clicked past and mark the boast failed
// Leaf: ?BoastHobbeCaveInnocentSacrificed@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE4C50, 404 bytes, thiscall `ret 4` (the CCharString is passed by value and
//   destroyed here; the `push ecx` slot holds the game-info text temporary).
// Identity (proven, two sources): Ego_r.pdb `?BoastHobbeCaveInnocentSacrificed@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z`;
//   CHeroBoastsScript::ControlNewBoast stores this address (0x00CE253D, the row's only reference) in the
//   CSpawnedFunc1<CHeroBoastsScript, CCharString> it builds for boast 0x16 = 22, next to the
//   "BoastHobbeCaveInnocentSacrificed" thread-name literal 0x012C3A68; the state it watches is Ego_r
//   CQ_SunnyvaleMasterData::HobbeCaveInnocentSacrificed (long +0x110, no retail shift). It sits between the landed
//   BoastHobbeCavePerpertratorKilled 0x00CE4B70 and BoastBreakSiegeNoAssistance 0x00CE5050. The manifest label
//   "sub_00ce4c50" is a gapscan placeholder.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, CGameScriptInterfaceBase slots 7 NewScriptFrame (+0x1C), 28 MsgOnQuestCompletedBeforeScreenShown (+0x70),
//   29 MsgOnQuestFailedBeforeScreenShown (+0x74), 40 MsgIsGameInfoClickedPast (+0xA0), 115 DisplayGameInfo (+0x1CC),
//   305 SetBoastAsFailed (+0x4C4), 306 SetBoastAsCompleted (+0x4C8). Literal "TEXT_QST_BOAST_STATE_FAILED_SACRIFICE"
//   0x012C3D70.
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT: CLevelScriptBase<CQ_SunnyvaleMasterData> = CScriptBase 0x40, Game
//   +0x40, MasterScriptData +0x44); CQ_SunnyvaleMasterData has no shared header (local model, Ego_r offsets).
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
class CQ_SunnyvaleMasterData {                          // Ego_r.pdb (no shared header): only the state this thread reads
public:
    unsigned char _pad000[0x110];
    long          HobbeCaveInnocentSacrificed;          // +0x110
};
enum { BOAST_HOBBE_CAVE_INNOCENT_SACRIFICED = 22 };
class CHeroBoastsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void BoastHobbeCaveInnocentSacrificed(CCharString questName);
};
void CHeroBoastsScript::BoastHobbeCaveInnocentSacrificed(CCharString questName)
{
    while (!IsActiveThreadTerminating()) {
        if (!Game->MsgOnQuestFailedBeforeScreenShown(questName) && MasterScriptData->HobbeCaveInnocentSacrificed != 0) {
            if (Game->MsgOnQuestCompletedBeforeScreenShown(questName) && MasterScriptData->HobbeCaveInnocentSacrificed == 2) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetBoastAsCompleted(BOAST_HOBBE_CAVE_INNOCENT_SACRIFICED, questName);
                return;
            }
            if (Game->MsgOnQuestCompletedBeforeScreenShown(questName)) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetBoastAsFailed(BOAST_HOBBE_CAVE_INNOCENT_SACRIFICED, questName);
                return;
            }
            Game->NewScriptFrame();
            continue;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!Game->MsgOnQuestFailedBeforeScreenShown(questName)) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_BOAST_STATE_FAILED_SACRIFICE");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->SetBoastAsFailed(BOAST_HOBBE_CAVE_INNOCENT_SACRIFICED, questName);
        return;
    }
}
}   // namespace NScript