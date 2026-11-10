// NScript::CV_FishingCompetitionScript::Init -- reset competition and competitor catch.
// Leaf: ?Init@CV_FishingCompetitionScript@NScript@@UAEXXZ
// Retail: Fable.exe 0x00E7F330,218 bytes.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_text.h"
#include "stlport/fable_containers.h"
#include "engine/fable_script_def_global.h"
#include "engine/NScript_CV_FishingCompetitionScript.h"
#include <stdlib.h>
void NScript::CV_FishingCompetitionScript::Init()
{
    CompetitorsAttacked=false;
    CompetitorsJustGotAttacked=false;
    OrganiserKilled=false;
    OrganiserRanAway=false;
    CompetitionRunning=true;
    BountyHuntActive=false;
    CompetitionWon=false;
    RodOfChampsGiven=false;
    CommentIRanAway=false;
    AttackCounter=0;
    HitCount=0;
    Winner="TEXT_QST_B15_FISHER_NAME_"+GFIntToCharString(rand()%3);
    BiggestCompetitorFish=((pGlobalScriptDef->FishPrizeWeights[1])/4.0f)+((rand()%50)/10.0f);
    DoneIntro=false;
    RodHintGiven=false;
    BiggestFish=0;
    PrizeNumber=-1;
    SizeComment.resize(3);
    SizeComment[0]=-1;
    SizeComment[1]=-1;
    SizeComment[2]=-1;
}