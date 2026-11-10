#include <stdio.h>
#include <string>
#include <vector>
static int rolls[2],roll_count,format_count,join_count;
static int fixture_rand(){return rolls[roll_count++];}
#define rand fixture_rand
class CCharString {public:std::string text;CCharString(){}CCharString(const char* p):text(p){}const CCharString& operator=(const CCharString& s){text=s.text;return *this;}};
static CCharString GFIntToCharString(long n){++format_count;char b[32];sprintf(b,"%ld",n);return CCharString(b);}
static CCharString operator+(const char* a,const CCharString& b){++join_count;CCharString result;result.text=std::string(a)+b.text;return result;}
class CScriptDef {public:std::vector<float> FishPrizeWeights;};
static const CScriptDef* pGlobalScriptDef;
namespace NScript {
class CV_FishingCompetitionScript {public:
 bool BountyHuntActive,OrganiserRanAway,OrganiserKilled,CompetitionRunning,CompetitionWon,RodOfChampsGiven,CommentIRanAway,CompetitorsAttacked,CompetitorsJustGotAttacked;
 long AttackCounter,AttackDayCount,HitCount;bool DoneIntro,RodHintGiven;long PrizeNumber;CCharString Winner;float BiggestFish,BiggestCompetitorFish;std::vector<long> SizeComment;
 void Init();
};
}
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
int main(){
 const float bases[]={0,4,20,100};const int sizes[]={0,1,3,5,8};
 for(int b=0;b<4;++b)for(int name=0;name<3;++name)for(int weight=0;weight<50;++weight)for(int len=0;len<5;++len){
  CScriptDef def;def.FishPrizeWeights.push_back(-999);def.FishPrizeWeights.push_back(bases[b]);pGlobalScriptDef=&def;
  NScript::CV_FishingCompetitionScript row;row.BountyHuntActive=row.OrganiserRanAway=row.OrganiserKilled=row.CompetitionRunning=row.CompetitionWon=row.RodOfChampsGiven=row.CommentIRanAway=row.CompetitorsAttacked=row.CompetitorsJustGotAttacked=row.DoneIntro=row.RodHintGiven=true;
  row.AttackCounter=row.HitCount=7;row.AttackDayCount=123;row.PrizeNumber=5;row.BiggestFish=row.BiggestCompetitorFish=999;row.Winner.text="old";row.SizeComment.resize(sizes[len],456);
  rolls[0]=name+300;rolls[1]=weight+500;roll_count=format_count=join_count=0;row.Init();
  char wanted[80];sprintf(wanted,"TEXT_QST_B15_FISHER_NAME_%d",name);
  float expected=bases[b]*0.25f+weight*0.1f;float error=row.BiggestCompetitorFish-expected;if(error<0)error=-error;
  if(roll_count!=2||format_count!=1||join_count!=1||row.Winner.text!=wanted||error>0.00001f||row.BiggestFish!=0||row.PrizeNumber!=-1||row.SizeComment.size()!=3||row.SizeComment[0]!=-1||row.SizeComment[1]!=-1||row.SizeComment[2]!=-1)return 1;
  if(row.BountyHuntActive||row.OrganiserRanAway||row.OrganiserKilled||!row.CompetitionRunning||row.CompetitionWon||row.RodOfChampsGiven||row.CommentIRanAway||row.CompetitorsAttacked||row.CompetitorsJustGotAttacked||row.DoneIntro||row.RodHintGiven||row.AttackCounter||row.HitCount||row.AttackDayCount!=123)return 1;
 }
 puts("00e7f330_TEST PASS");return 0;
}