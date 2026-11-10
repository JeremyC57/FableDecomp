#ifndef FABLE_NSCRIPT_CV_FISHINGCOMPETITIONSCRIPT_H
#define FABLE_NSCRIPT_CV_FISHINGCOMPETITIONSCRIPT_H
// Original complete PDB data suffix; retail base/container sizes.
#include "engine/CLevelScriptBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishingCompetitionScript:public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
 virtual void Init();
 bool BountyHuntActive,OrganiserRanAway,OrganiserKilled,CompetitionRunning,CompetitionWon,RodOfChampsGiven,CommentIRanAway,CompetitorsAttacked,CompetitorsJustGotAttacked;
 long AttackCounter,AttackDayCount,HitCount;bool DoneIntro,RodHintGiven;long PrizeNumber;
 CCharString Winner;float BiggestFish,BiggestCompetitorFish;std::vector<long> SizeComment;
};
FABLE_STATIC_ASSERT(sizeof(CV_FishingCompetitionScript)==0x80);
}
#endif
