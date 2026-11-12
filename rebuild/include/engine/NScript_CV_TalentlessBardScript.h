#ifndef FABLE_ENGINE_NSCRIPT_CV_TALENTLESSBARDSCRIPT_H
#define FABLE_ENGINE_NSCRIPT_CV_TALENTLESSBARDSCRIPT_H
// New header (Q18, row 0x00ED94D0 WatchForResetWanderCentre; no default path): NScript::CV_TalentlessBardScript, hand-written like
// engine/NScript_CV_ChickenKickingScript.h. Base CLevelScriptBase<CQ_SunnyvaleMasterData> (the nested CTB_Bard's Ego_r.pdb
// PMasterData is CQ_SunnyvaleMasterData*). Members: Ego_r.pdb CV_TalentlessBardScript (sizeof 260) in PDB order, each 0x14 lower
// in retail (base 0x48): bHurtBard +0x48, bBard_finished +0x49, lNumber_of_things_sung_about +0x4c, bRewardGiven +0x50, stBard +0x54,
// BardIntroComplete +0x60, AttackedByHero +0x61 (landed WatchForAttack 0x00ED9790), BardFollowing +0x62, BardWanderTime +0x64
// (0x00ED9566 `mov eax,[esi+64h]`), SongItems +0x68, QuestInfo[12] +0x74, SongThingAvailable +0xd4 (retail vector<bool> is 12
// bytes, Ego_r's 20) and RandomSpeechTimer +0xe0 (the landed Init 0x00ED83C0 layout; 0xe4 bytes). Methods: the Ego_r.pdb
// name set (public non-virtual QAE spellings; the overrides U). QuestData is Ego_r.pdb {CCharString name, tag} (8 bytes).
// Needs FABLE_RETAIL_NSCRIPT and FABLE_RETAIL_CLEVELSCRIPTBASE (+ _ORIGINAL_MEMBERS for the `Game.` reference spelling).
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CScriptThing.h"
#include "engine/CTimer.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_containers.h"
namespace NScript {
class CV_TalentlessBardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class QuestData {
    public:
        QuestData();
        ~QuestData();
        CCharString name;                                                // +0x00
        CCharString tag;                                                 // +0x04
    };
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    virtual void OnPersist(CPersistContext&);
    void WatchForCompletedQuests();
    long GetQuestIndex(CCharString);
    void WatchForResetWanderCentre(CScriptThing);                       // 0x00ED94D0
    void WatchForAttack(CScriptThing);                                  // 0x00ED9790
    bool IsHeroDressedInArmour();
    bool IsHeroDressedInWillRobe();
    bool IsHeroDressedAsApprentice();
    bool IsHeroDressedAsAWoman();
    void ProcessSongsQuestComplete();
    void ProcessSongsClothes();
    void ProcessSongsAge();
    void ProcessSongsMorality();
    void ProcessSongsRenown();
    void ProcessSongsMarriage();
    bool bHurtBard;                                                      // +0x48
    bool bBard_finished;                                                 // +0x49
    unsigned long lNumber_of_things_sung_about;                          // +0x4c
    bool bRewardGiven;                                                   // +0x50
    CScriptThing stBard;                                                 // +0x54
    bool BardIntroComplete;                                              // +0x60
    bool AttackedByHero;                                                 // +0x61
    bool BardFollowing;                                                  // +0x62
    CTimer BardWanderTime;                                               // +0x64
    std::vector<CCharString> SongItems;                                  // +0x68
    QuestData QuestInfo[12];                                             // +0x74
    std::vector<bool> SongThingAvailable;                                // +0xd4 (retail 12-byte byte vector: landed Init 0x00ED83C0)
    CTimer RandomSpeechTimer;                                            // +0xe0 (landed Init 0x00ED83C0)
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_TalentlessBardScript) == 0xe4);
#endif
