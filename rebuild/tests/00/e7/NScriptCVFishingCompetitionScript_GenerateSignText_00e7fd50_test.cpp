#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
typedef std::string CCharString;
typedef std::string CWideString;
typedef std::string CScriptThing;
static void ck(bool c){if(!c)exit(2);}
static int logged[20],count;
static void evt(int n){ck(count<20);logged[count++]=n;}
static CWideString GFFloatToWideString(double n){evt(4);char text[48];sprintf(text,"%.4f",n);return text;}
#include "engine/GFPackFloat.h"
static float roundForSign(float n,long m) { ck(m==2);evt(3);return GFRoundFloatNear(n,m); }
struct Config{float FishPrizeWeights[8];};static Config config;static Config* pGlobalScriptDef=&config;
enum EHeroTitle { TITLE_NONE=0, TITLE_REAPER=1, TITLE_SHADOWHUNTER=2, TITLE_MALEFICUS=3, TITLE_DEATHBRINGER=4, TITLE_ASSASSIN=5, TITLE_NECROMANCER=6, TITLE_AVATAR=7, TITLE_PILGRIM=8, TITLE_LIBERATOR=9, TITLE_PALADIN=10, TITLE_DRUID=11, TITLE_RANGER=12, TITLE_RUNEMASTER=13, TITLE_HOOD=14, TITLE_GLADIATOR=15, TITLE_SABRE=16, TITLE_ARROWDODGER=17, TITLE_PIEMASTER=18, TITLE_CHICKEN_CHASER=19, TITLE_ARSEFACE=20, TITLE_JACK=21, TITLE_MAZE=22, TITLE_SCARLET_ROBE=23, TITLE_SCYTHE=24, TITLE_THUNDER=25, TITLE_WHISPER=26, TITLE_TWINBLADE=27, TITLE_BRIAR_ROSE=28, TITLE_LADY_GREY=29, TITLE_GUILDMASTER=30, TITLE_SCORPION_SLAYER=31, TITLE_DEATH_BRINGER=32 };
struct GameFixture{
 EHeroTitle title;std::string result;
 CScriptThing GetThingWithScriptName(const CCharString& s){ck(s=="FishCompSign");evt(0);return "sign-handle";}
 CWideString GetTextString(const CCharString& key){evt(1);return "<"+key+">";}
 EHeroTitle GetHeroTitle(){evt(2);return title;}
 CWideString GetFormattedString(const CCharString& k,const std::vector<CWideString>& v){evt(5);CWideString r=k;for(size_t i=0;i<v.size();++i)r+="|"+v[i];return r;}
 void SetReadableObjectText(const CScriptThing& sign,const CWideString& v){ck(sign=="sign-handle");evt(6);result=v;}
};
namespace NScript {
struct CV_FishingCompetitionScript{
 GameFixture& Game;explicit CV_FishingCompetitionScript(GameFixture& g):Game(g){}
 long PrizeNumber;bool CompetitionWon;float BiggestCompetitorFish;CCharString Winner;
 CCharString GetHeroTitleTextTag();float GetNextPrizeCategoryWeight();void GenerateSignText();
};
}
CCharString NScript::CV_FishingCompetitionScript::GetHeroTitleTextTag()
{
    CCharString tag = "";
    switch (Game.GetHeroTitle())
    {
    case TITLE_ARROWDODGER:    tag = "TEXT_GUI_HERO_TITLE_ARROWDODGER"; break;
    case TITLE_ARSEFACE:       tag = "TEXT_GUI_HERO_TITLE_ARSEFACE"; break;
    case TITLE_ASSASSIN:       tag = "TEXT_GUI_HERO_TITLE_ASSASSIN"; break;
    case TITLE_AVATAR:         tag = "TEXT_GUI_HERO_TITLE_AVATAR"; break;
    case TITLE_CHICKEN_CHASER: tag = "TEXT_GUI_HERO_TITLE_CHICKEN_CHASER"; break;
    case TITLE_DEATHBRINGER:   tag = "TEXT_GUI_HERO_TITLE_DEATHBRINGER"; break;
    case TITLE_DRUID:          tag = "TEXT_GUI_HERO_TITLE_DRUID"; break;
    case TITLE_GLADIATOR:      tag = "TEXT_GUI_HERO_TITLE_GLADIATOR"; break;
    case TITLE_HOOD:           tag = "TEXT_GUI_HERO_TITLE_HOOD"; break;
    case TITLE_LIBERATOR:      tag = "TEXT_GUI_HERO_TITLE_LIBERATOR"; break;
    case TITLE_MALEFICUS:      tag = "TEXT_GUI_HERO_TITLE_MALEFICUS"; break;
    case TITLE_NECROMANCER:    tag = "TEXT_GUI_HERO_TITLE_NECROMANCER"; break;
    case TITLE_PALADIN:        tag = "TEXT_GUI_HERO_TITLE_PALADIN"; break;
    case TITLE_PIEMASTER:      tag = "TEXT_GUI_HERO_TITLE_PIEMASTER"; break;
    case TITLE_PILGRIM:        tag = "TEXT_GUI_HERO_TITLE_PILGRIM"; break;
    case TITLE_RANGER:         tag = "TEXT_GUI_HERO_TITLE_RANGER"; break;
    case TITLE_REAPER:         tag = "TEXT_GUI_HERO_TITLE_REAPER"; break;
    case TITLE_RUNEMASTER:     tag = "TEXT_GUI_HERO_TITLE_RUNEMASTER"; break;
    case TITLE_SABRE:          tag = "TEXT_GUI_HERO_TITLE_SABRE"; break;
    case TITLE_SHADOWHUNTER:   tag = "TEXT_GUI_HERO_TITLE_SHADOWHUNTER"; break;
    }
    return tag;
}
namespace NScript {
float CV_FishingCompetitionScript::GetNextPrizeCategoryWeight()
{
    long next = PrizeNumber + 1;
    if (PrizeNumber == -1)
        ++next;
    return pGlobalScriptDef->FishPrizeWeights[next];
}
void CV_FishingCompetitionScript::GenerateSignText()
{
    CScriptThing Sign = Game.GetThingWithScriptName("FishCompSign");
    CWideString WinnerTitle = Game.GetTextString(Winner);
    if (Winner == "HERO")
        WinnerTitle = Game.GetTextString(GetHeroTitleTextTag());
    float NextPrizeCategory = GetNextPrizeCategoryWeight();
    std::vector<CWideString> values;
    values.push_back(GFFloatToWideString(roundForSign(BiggestCompetitorFish, 2)));
    values.push_back(WinnerTitle);
    CWideString CompetitionText;
    if (CompetitionWon)
        CompetitionText = Game.GetFormattedString("TEXT_QST_B15_FISH_SIGN_INFO_HEROWON", values);
    else {
        values.push_back(GFFloatToWideString(NextPrizeCategory));
        CompetitionText = Game.GetFormattedString("TEXT_QST_B15_FISH_SIGN_INFO_NORMAL", values);
    }
    Game.SetReadableObjectText(Sign, CompetitionText);
}
}
int main(){
 const char* titles[]={"","REAPER","SHADOWHUNTER","MALEFICUS","DEATHBRINGER","ASSASSIN","NECROMANCER","AVATAR","PILGRIM","LIBERATOR","PALADIN","DRUID","RANGER","RUNEMASTER","HOOD","GLADIATOR","SABRE","ARROWDODGER","PIEMASTER","CHICKEN_CHASER","ARSEFACE"};
 const float fish[]={-2.125f,-0.375f,0,0.125f,0.375f,2.3515625f,3.140625f,12.25f};
 const char* sizes[]={"-2.1200","-0.3800","0.0000","0.1200","0.3800","2.3500","3.1400","12.2500"};
 GameFixture g;NScript::CV_FishingCompetitionScript s(g);
 for(int i=0;i!=8;++i)config.FishPrizeWeights[i]=i+0.25f;
 for(int hero=0;hero!=3;++hero)for(int won=0;won!=2;++won)for(int t=0;t<33;++t)for(int prize=-1;prize<6;++prize)for(int j=0;j!=8;++j){
  count=0;g.title=(EHeroTitle)t;s.PrizeNumber=prize;s.CompetitionWon=won!=0;s.Winner=hero==1?"HERO":hero==2?"":"FISHER";s.BiggestCompetitorFish=fish[j];
  s.GenerateSignText();
  CCharString title=hero==1?(t&&t<=20?CCharString("TEXT_GUI_HERO_TITLE_")+titles[t]:""):s.Winner;
  CCharString expected=won?"TEXT_QST_B15_FISH_SIGN_INFO_HEROWON":"TEXT_QST_B15_FISH_SIGN_INFO_NORMAL";
  expected+="|";expected+=sizes[j];expected+="|<"+title+">";
  if(!won){char v[32];sprintf(v,"|%.4f",(prize==-1?1:prize+1)+0.25);expected+=v;}
  ck(g.result==expected);int e=0;ck(logged[e++]==0);ck(logged[e++]==1);
  if(hero==1){ck(logged[e++]==2);ck(logged[e++]==1);}
  ck(logged[e++]==3);ck(logged[e++]==4);if(!won)ck(logged[e++]==4);ck(logged[e++]==5);ck(logged[e++]==6);ck(e==count);
 }
 puts("00e7fd50_TEST PASS");return 0;
}