#ifndef FABLE_ENGINE_CQ_BOUNTYHUNTSCRIPT_H
#define FABLE_ENGINE_CQ_BOUNTYHUNTSCRIPT_H
// Original Ego_r TPI 0x1317c, field order and types; native base is 0x48.
// Layout: native Game+0x40; QuestFailed+0x48; BoastTime+0x80.
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
#include "engine/CCharString.h"
#include "stlport/fable_containers.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BountyHuntScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    struct TeleportData {
        CCharString defName; unsigned long appearanceSeed; float health;
        CCharString scriptName, teleportTo, teleportMkr;
    };
    CQ_BountyHuntScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual ~CQ_BountyHuntScript();
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    void SpawnBanditGangGrunts(CCharString spawnName);
    bool QuestFailed;
    bool QuestSucceeded;
    bool QuestOver;
    bool QuestStarted;
    bool FishermanDeactivated;
    bool VillagerKilled;
    bool VillagerIntroDone;
    bool LeaderKilled;
    bool Deputy1Killed;
    bool Deputy2Killed;
    bool Deputy1IntroComplete;
    bool Deputy2IntroComplete;
    bool Hostage1Killed;
    bool Hostage2Killed;
    bool Hostage1Safe;
    bool Hostage2Safe;
    bool Deputy1Escaped;
    bool LeaderEscaped;
    bool SpawnDeputy1Grunts;
    bool SpawnDeputy2Grunts;
    bool SpawnLeaderGrunts;
    bool AllDeputy1GruntsKilled;
    bool AllDeputy2GruntsKilled;
    bool AllLeaderGruntsKilled;
    bool AllLeaderChargeGruntsKilled;
    bool AllGruntsKilled;
    bool FishComplete;
    bool LeaderIntro1Complete;
    bool LeaderIntro2Complete;
    bool LeaderAttackingHostage;
    bool LeaderAttackingHero;
    bool LeaderRansomPaid;
    bool LeaderRansomDenied;
    bool LeaderLeftHostage;
    bool LeaderReachedMoney;
    bool LeaderAttackedAfterRansomPaid;
    bool LeaderChargeComplete;
    bool LeaderCutsceneComplete;
    bool LakeComplete;
    bool BanditCutsceneDone;
    bool BountyHuntIntroComplete;
    long BanditGruntTextCounter;
    bool BoastActiveTimeLimit;
    bool BoastActiveDecapitation;
    long BoastTimerID;
    CTimer BoastTime;
    bool BoastQuestInfoDisplayed;
    std::vector<TeleportData> TeleportInfo;
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BountyHuntScript)==0x94);
#endif
